/*
  ==============================================================================

    MidiHandler.h
    Created: 26 Jul 2024 4:53:20pm
    Author:  Solomon Moulang Lewis

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MidiHandler
{
public:
    MidiHandler()
    {
        // check if any midi devices
        if (juce::MidiOutput::getAvailableDevices().isEmpty())
        {
            std::cout << "no midi devices found" << std::endl;
            
            // exit early if no midi devices were found
            return;
        }
        
        // store available midi output devices
        auto midiOutputs = juce::MidiOutput::getAvailableDevices();
        if (!midiOutputs.isEmpty())
        {
            // output names of avaiable midi devices
            for (const auto& midiOutput : midiOutputs)
            {
                std::cout << "MIDI Output Device: " << midiOutput.name << " (" << midiOutput.identifier << ")" << std::endl;
            }
            // fetch first available midiOutput device
            auto midiOutput = midiOutputs[0];
            midiOutputDevice = juce::MidiOutput::openDevice(midiOutput.identifier);
            
            // console out the name of the first open midi device
            if (midiOutputDevice != nullptr)
            {
                DBG("MIDI output device opened: " + midiOutput.name);
            }
            else
            {
                DBG("Failed to open MIDI output device: " + midiOutput.name);
            }
        }
    }
    ~MidiHandler() {}
    
    void outputMidiMessage (const int midi_ch, const int midi_cc, const int midi_cc_value)
    {
        juce::MidiMessage midi_cc_message = juce::MidiMessage::controllerEvent (midi_ch, midi_cc, midi_cc_value);
        midiOutputDevice->sendMessageNow (midi_cc_message);
    }
    
private:
    std::unique_ptr<juce::MidiOutput> midiOutputDevice;
};
