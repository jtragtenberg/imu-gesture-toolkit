/*
  ==============================================================================

    GirominController.h
    Created: 26 Jul 2024 4:52:51pm
    Author:  Solomon Moulang Lewis

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "GirominData.h"
#include "OSCHandler.h"
#include "MidiHandler.h"
#include "IMUGestureToolkit.h"

#define MAX_GIROMINS 6

#define A_NORMALISATION_CONSTANT 0.006535947712     // 1/15.3   - accelerometer in 16g resolution
#define G_NORMALISATION_CONSTANT 0.02895193978      // 1/34.54  - gyroscope in 500dps resolution

class GirominController
{
public:
    
    std::function<void(float)> update_UI;
    
    std::function<void(std::array<float, 3>)> update_euler_UI;
    
    GirominController()
    {
//        GirominData g1;
        
        for (int i = 0; i < MAX_GIROMINS; i++)
        {
            giromins_.emplace_back (GirominData());
        }
        giromins_[0].setID (23);
        
        //GirominData view_data;
        
        osc_handler_.oscMessageCallback = [this](std::string addr, float* values)
        {
            UpdateGiromin (addr, values);
            
            /*
            for (int i=0; i<giromins_.size(); i++){
                //TODO: Go through the recorded mappings table
            }
            */
            
            //===============================================================================================
            // ROTATION RATE
            //===============================================================================================
            std::array<float, 3> gyro_data = {getGiromin(0)->getGX(),
                                                getGiromin(0)->getGY(),
                                                getGiromin(0)->getGZ()};
            
            float rotation_rate =  gesture2.processRotationRate (gyro_data,
                                                                 IMUGestureToolkit::GyroAxis::MAGNITUDE,
                                                                 IMUGestureToolkit::GyroDirection::ABSOLUTE,
                                                                 1.f,
                                                                 0.1f,
                                                                 0.999f);
            
//            std::cout << rotation_rate << std::endl;
            
            update_UI (rotation_rate);
            
            //===============================================================================================
            // QUATERNIONS to EULER
            //===============================================================================================
            
            std::array<float, 4> quaternions = {getGiromin(0)->getQ1(),
                                                getGiromin(0)->getQ2(),
                                                getGiromin(0)->getQ3(),
                                                getGiromin(0)->getQ4()};
            
            std::array<float, 3> euler_angels = gesture3.convertQuaternionToEuler (quaternions[0],
                                                                                   quaternions[1],
                                                                                   quaternions[2],
                                                                                   quaternions[3]);
            
            std::cout << "euler_angels[0]: " << euler_angels[0] << std::endl;
            std::cout << "euler_angels[1]: " << euler_angels[1] << std::endl;
            std::cout << "euler_angels[2]: " << euler_angels[2] << std::endl;
            
            update_euler_UI (euler_angels);
            
            //===============================================================================================
            // BUTTON ACTIONS SETUP
            //===============================================================================================
            float giromin_data_value = getGiromin(0)->getB1();
            
            if (giromin_data_value != previous_giromin_data_value_)
            {
                float output_value = gesture1.processButtonSignal (giromin_data_value,
                                                                   IMUGestureToolkit::ButtonAction::TOGGLE);
                
                if (gesture1.changed (static_cast<int>(output_value)))
                {
                    std::cout << "giromin_data_value: " << output_value << std::endl;
                    
                    // OUTPUT MIDI
                    midi_handler_.outputMidiMessage (1, 10, (int)output_value);
                }
                
                previous_giromin_data_value_ = giromin_data_value;
            }
            
            //===============================================================================================
        };
    }
    
    GirominData* getGiromin (int index)
    {
        //TODO: use ID instead
        
        // assert (index > 0 && index < MAX_NUM_GIROMINS)
        return &giromins_[index];
    }
    
private:
    // TODO: break out into private methods getID, getParamGroupID, getParam methods
    // /giromin/25/a/x
    void UpdateGiromin (std::string addr, float *values)
    {
        // TODO: add a condition to check for 'giromin' - could be a different address.
        const char* start = strchr (addr.c_str() + 1, '/');
        const char* next = strchr (start + 1, '/');
        int address_id = std::atoi (std::string (start + 1, next).c_str());
        
        for (int i = 0; i < MAX_GIROMINS; i ++)
        {
            if (giromins_[i].getId() == address_id)
            {
                start = next;
                next = strchr (start + 1, '/');
                std::string param_group_id (start + 1, next ? next - start - 1 : strlen (start + 1));
                std::string param;
                
                if (next) { param = next + 1; }
                
                switch (param_group_id[0])
                {
                    case 'a':
                        if (param == "x")
                        {
                            giromins_[i].setAX (values[0] * A_NORMALISATION_CONSTANT);
                        }
                        else if (param == "y")
                        {
                            giromins_[i].setAY (values[0] * A_NORMALISATION_CONSTANT);
                        }
                        else if (param == "z")
                        {
                            giromins_[i].setAZ (values[0] * A_NORMALISATION_CONSTANT);
                        }
                        break;
                    case 'g':
                        if (param == "x")
                        {
                            giromins_[i].setGX (values[0] * G_NORMALISATION_CONSTANT);
                        }
                        else if (param == "y")
                        {
                            giromins_[i].setGY (values[0] * G_NORMALISATION_CONSTANT);
                        }
                        else if (param == "z")
                        {
                            giromins_[i].setGZ (values[0] * G_NORMALISATION_CONSTANT);
                        }
                        break;
                    case 'q':
                        giromins_[i].setQ1 (values[0]);
                        giromins_[i].setQ2 (values[1]);
                        giromins_[i].setQ3 (values[2]);
                        giromins_[i].setQ4 (values[3]);
                        break;
                    case 'b':
                        if (param_group_id == "b1")
                        {
                            giromins_[i].setB1 (values[0]);
                        }
                        else if (param_group_id == "b2")
                        {
                            giromins_[i].setB2 (values[0]);
                        }
                        break;
                    default:
                        std::cout << "unknown param_group_id: " << param_group_id << "\n";
                        break;
                }
            }
        }
    }
    
    IMUGestureToolkit gesture1, gesture2, gesture3;
    float previous_giromin_data_value_ = 0;
    float previous_giromin_output_value_ = 0;
    
    std::vector<GirominData> giromins_;
    OSCHandler osc_handler_;
    MidiHandler midi_handler_;
};
