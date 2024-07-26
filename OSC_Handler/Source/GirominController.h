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

class GirominController
{
public:
    GirominController()
    {
        GirominData g_data;
        g_data.setID (25);
        
        giromins_.push_back (g_data);
        
        osc_handler_.oscMessageCallback = [this](std::string addr, float* values)
        {
            UpdateGiromin (addr, values);
            
            // TODO: have a map where a giromin value is with a boolean to check if its active or not. Use this to set what midi not to send
            
            float giromin_data_value = getGiromin(0)->getAX();
            
            midi_handler_.outputMidiMessage (1, 10, giromin_data_value);
            
            std::cout << "should update midi" << std::endl;
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
        int id = std::atoi (std::string (start + 1, next).c_str());
        
        // TODO: use g_data.getID(); instead and compare
        int id_index = 0; // TODO: THIS IS TEMP - REMOVE and use giromin ID
        
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
                    giromins_[id_index].setAX (values[0]);
                    std::cout << addr << std::endl;
                    std::cout << values[0] << std::endl;
                }
                else if (param == "y")
                {
                    giromins_[id_index].setAY (values[0]);
                }
                else if (param == "z")
                {
                    giromins_[id_index].setAZ (values[0]);
                }
                break;
            case 'g':
                if (param == "x")
                {
                    giromins_[id_index].setGX (values[0]);
                }
                else if (param == "y")
                {
                    giromins_[id_index].setGY (values[0]);
                }
                else if (param == "z")
                {
                    giromins_[id_index].setGZ (values[0]);
                }
                break;
            case 'q':
                giromins_[id_index].setQ1 (values[0]);
                giromins_[id_index].setQ2 (values[1]);
                giromins_[id_index].setQ3 (values[2]);
                giromins_[id_index].setQ4 (values[3]);
                break;
            case 'b':
                if (param_group_id == "b1")
                {
                    giromins_[id_index].setB1 (values[0]);
                }
                else if (param_group_id == "b2")
                {
                    giromins_[id_index].setB2 (values[0]);
                }
                break;
            default:
                std::cout << "unknown param_group_id: " << param_group_id << "\n";
                break;
        }
    }
    
    std::vector<GirominData> giromins_;
    OSCHandler osc_handler_;
    MidiHandler midi_handler_;
};
