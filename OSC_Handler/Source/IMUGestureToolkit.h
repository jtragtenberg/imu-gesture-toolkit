/*
  ==============================================================================

    IMUGestureToolkit.h
    Created: 26 Jul 2024 4:53:55pm
    Author:  Solomon Moulang Lewis

  ==============================================================================
*/
    
/**
 NOTE: because these methods are 'static', they can be used ANYWHERE in the codebase.
    
 To USE: GirominMathUtils::'method_name' (p1, ... pN);
 
 e.g. float filtered = GirominMathUtils::(in, filtered_val, filtering_amount);
 */

#pragma once

class IMUGestureToolkit
{
public:

    // Constructor to initialize default values
    IMUGestureToolkit()
        : toggle_state_(0.0f),
          filtered_value_(0.0f),
          last_raw_value_(0.0f)
    {
    }
    
    enum class ButtonAction {
        PUSH,           // Output the raw value
        INVERTED_PUSH,  // Output the opposite value
        TOGGLE          // Output the toggled value
    };
    
    enum class GyroAxis {
        X,              // Gyroscope X-axis
        Y,              // Gyroscope Y-axis
        Z,              // Gyroscope Z-axis
        MAGNITUDE       // Quadratic sum of all axes
    };

    enum class GyroDirection {
        POSITIVE,       // Only positive values
        NEGATIVE,       // Only negative values
        BOTH,           // Both positive and negative values
        ABSOLUTE        // Absolute values
    };
    
    float processButtonSignal(float input, ButtonAction action) {
        switch (action) {
            case ButtonAction::PUSH:
                return input;
            case ButtonAction::INVERTED_PUSH:
                return (input == 0.0f) ? 1.0f : 0.0f;
            case ButtonAction::TOGGLE:
                
                
                
                if (input == 1.f)
                {
                    toggle_state_ = (toggle_state_ == 0.0f) ? 1.0f : 0.0f;
                }
                return toggle_state_;
            default:
                throw std::invalid_argument("Unknown ButtonAction");
        }
    }
    
    float processRotationRate (const std::array<float, 3>& gyroData,
                               GyroAxis axis,
                               GyroDirection direction,
                               float gain = 1.0f,
                               float riseFilteringAmount = 0.0f,
                               float fallFilteringAmount = 0.0f)
    {
        float value;

        // Select axis or magnitude
        switch (axis) {
            case GyroAxis::X:
                value = gyroData[0];
                break;
            case GyroAxis::Y:
                value = gyroData[1];
                break;
            case GyroAxis::Z:
                value = gyroData[2];
                break;
            case GyroAxis::MAGNITUDE:
                value = std::sqrt(gyroData[0] * gyroData[0] +
                                  gyroData[1] * gyroData[1] +
                                  gyroData[2] * gyroData[2]);
                break;
            default:
                throw std::invalid_argument("Unknown GyroAxis");
        }

        // Apply direction filtering
        switch (direction) {
            case GyroDirection::POSITIVE:
                value = (value > 0) ? value : 0;
                break;
            case GyroDirection::NEGATIVE:
                value = (value < 0) ? value : 0;
                break;
            case GyroDirection::BOTH:
                // No change needed
                break;
            case GyroDirection::ABSOLUTE:
                value = std::fabs(value);
                break;
            default:
                throw std::invalid_argument("Unknown GyroDirection");
        }

        // Apply gain
        value *= gain;

        // Apply EMA filter if any filteringAmount is set
        if (riseFilteringAmount > 0.0f || fallFilteringAmount > 0.0f)
        {
//            filtered_value_ = filterEMA(value, filtered_value_, fallFilteringAmount);
            filtered_value_ = filterEMATwoWays (value, filtered_value_, riseFilteringAmount, fallFilteringAmount);
        }

        return filtered_value_;
    }
    
    //Untested, started implemented only
    std::array<float, 3> convertQuaternionToEuler(float q0, float q1, float q2, float q3) {
            // Inverter a ordem dos quaternions
            float q0_ = q0;
            float q1_ = q2;
            float q2_ = q3;
            float q3_ = q1; // z-yaw, y-pitch, x-roll

            // Basicamente, (0, 0, 0, 1) é um quaternion neutro
            float neutral_q0 = 0.0f;
            float neutral_q1 = 0.0f;
            float neutral_q2 = 0.0f;
            float neutral_q3 = 1.0f;

            // Multiplicar o quaternion de entrada pelo quaternion neutro
            std::array<float, 4> resultQuat = multiplyQuaternions({q0_, q1_, q2_, q3_}, {neutral_q0, neutral_q1, neutral_q2, neutral_q3});

            // Converter o resultado para ângulos de Euler na ordem YZK (Yaw, Pitch, Roll)
            float yaw = std::atan2(2.f * (resultQuat[0] * resultQuat[3] + resultQuat[1] * resultQuat[2]),
                                   1.f - 2.f * (resultQuat[2] * resultQuat[2] + resultQuat[3] * resultQuat[3]));

            float pitch = std::asin(2.f * (resultQuat[0] * resultQuat[2] - resultQuat[3] * resultQuat[1]));

            float roll = std::atan2(2.f * (resultQuat[0] * resultQuat[1] + resultQuat[2] * resultQuat[3]),
                                   1.f - 2.f * (resultQuat[1] * resultQuat[1] + resultQuat[2] * resultQuat[2]));

            return {yaw, pitch, roll};
        }
    
    // EMA Filter
    static float filterEMA (float inputValue, float filteredValue, float filteringAmount)
    {
        return ((1 - filteringAmount) * inputValue) + (filteringAmount * filteredValue);
    }
    
    
    float filterEMATwoWays(float inputValue, float filteredValue, float riseFilteringAmount, float fallFilteringAmount)
    {
        float filteringAmount = 0.f;
        if (inputValue > last_filtered_value_)
        {
            filteringAmount = riseFilteringAmount;
        }
        else
        {
            filteringAmount = fallFilteringAmount;
        }
        
        std::cout << "filteringAmount: " << filteringAmount << std::endl;
        
        filteredValue = filterEMA (inputValue, filteredValue, filteringAmount);
        last_raw_value_ = inputValue;
        last_filtered_value_ = filteredValue;
        return filteredValue;
    }
    
    // SCALE
    static float scale (float value, float inMin, float inMax, float outMin, float outMax)
    {
        return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
    }
    
    // SCALE & CLAMP
    static float scaleAndClamp (float value, float inMin, float inMax, float outMin, float outMax)
    {
        if (value < inMin) value = inMin;
        if (value > inMax) value = inMax;
        return scale (value, inMin, inMax, outMin, outMax);
    }

    std::array<float, 4> multiplyQuaternions (const std::array<float, 4>& q1, const std::array<float, 4>& q2)
    {
        float w = q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2] - q1[3] * q2[3];
        float x = q1[0] * q2[1] + q1[1] * q2[0] + q1[2] * q2[3] - q1[3] * q2[2];
        float y = q1[0] * q2[2] - q1[1] * q2[3] + q1[2] * q2[0] + q1[3] * q2[1];
        float z = q1[0] * q2[3] + q1[1] * q2[2] - q1[2] * q2[1] + q1[3] * q2[0];

        return {w, x, y, z};
    }
    
    //On Change Method
    bool changed (int input_value)
    {
        if (input_value != previous_input_value_)
        {
            previous_input_value_ = input_value;
            return (true);
        }
        else
        {
            return (false);
        }
    }
    
private:
    float toggle_state_;
    float filtered_value_;
    float last_raw_value_;
    float last_filtered_value_;
    int previous_input_value_;


};
