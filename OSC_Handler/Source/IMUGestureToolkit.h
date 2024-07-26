/*
  ==============================================================================

    IMUGestureToolkit.h
    Created: 26 Jul 2024 4:53:55pm
    Author:  Solomon Moulang Lewis

  ==============================================================================
*/

#pragma once

class IMUGestureToolkit
{
public:
    
    /**
        NOTE: because these methods are 'static', they can be used ANYWHERE in the codebase.
            
        To USE: GirominMathUtils::'method_name' (p1, ... pN);
     
        e.g. float filtered = GirominMathUtils::(in, filtered_val, filtering_amount);
    */
    
    enum class ButtonAction {
        PUSH,           // Output the raw value
        INVERTED_PUSH,  // Output the opposite value
        TOGGLE          // Output the toggled value
    };
    
    float processButtonSignal(float input, ButtonAction action) {
            switch (action) {
                case ButtonAction::PUSH:
                    return input;
                case ButtonAction::INVERTED_PUSH:
                    return (input == 0.0f) ? 1.0f : 0.0f;
                case ButtonAction::TOGGLE:
                    toggle_state_ = (toggle_state_ == 0.0f) ? 1.0f : 0.0f;
                    return toggle_state_;
                default:
                    throw std::invalid_argument("Unknown ButtonAction");
            }
        }
    
    // EMA Filter
    static float filterEMA (float inputValue, float filteredValue, float filteringAmount)
    {
        return ((1 - filteringAmount) * inputValue) + (filteringAmount * filteredValue);
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
    
private:
    float toggle_state_;
};
