/*
  ==============================================================================

    OSCHandler.h
    Created: 26 Jul 2024 4:53:04pm
    Author:  Solomon Moulang Lewis

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

// TODO: in future update OSCHandler to have two sub classes: Receiver, Sender
class OSCHandler
:
private juce::OSCReceiver,
private juce::OSCReceiver::Listener<juce::OSCReceiver::MessageLoopCallback>
{
public:
    OSCHandler()
    {
        port_number_ = 1333; // temp - initial port number
        
        if (connectOSC (port_number_))
        {
            std::cout << "connected" << std::endl;
        }
        else
        {
            std::cout << "didnt connect" << std::endl;
        }
        
        for (int i = 0; i < 3; i++)
        {
            osc_messages_[i] = 0.0f;
        }
        
        addListener (this); // note: super important
    }
    
    ~OSCHandler() {}
    
    std::function<void(std::string, float*)> oscMessageCallback;
    
    // TODO: make work with messages too
    void oscMessageReceived (const juce::OSCMessage& message) override
    {
        std::cout << message.getAddressPattern().toString() << std::endl;
        std::cout << "osc should be bundled" << std::endl;
//        jassert (false);
        
        handleOscMessage (message);
        
    }
    
    void oscBundleReceived (const juce::OSCBundle& bundle) override
    {
        time_tag_ = bundle.getTimeTag(); /// NOTE: time_tag_.toTime().toString(true, true, true, true); - to get time string
        
        for (auto& element : bundle)
        {
            if (element.isMessage())
            {
                handleOscMessage (element.getMessage());
            }
        }
        
    } /// NOTE: only handling OSC bundles from giromin
    
    // TODO: UI should handle bad connection error message - update this to return an enum error message for if 1. invalid port number 2. failed to connect to port number
    bool connectOSC (int port)
    {
        if (!isValidOscPort (port)) return false; // validate correct port number
        
        port_number_ = port;
        
        if (!connect (port_number_)) return false; // validate connection to port - NOTE: checks and connects in same line
        
        return true; // assume connection if not returned previously
    }
    
    // TODO: UI should handle disconnect error message
    bool disconnectOSC()
    {
        if (!disconnect()) return false;
        
        port_number_ = -1; // set to out of range port number
        
        return true;
    }
    
private:
    void handleOscMessage (const juce::OSCMessage& message)
    {
        // assert all messages are float
        for (int i = 0; i < message.size(); i++)
        {
            // TODO: add other conditions, still output the message
            if (!message[i].isFloat32())
            {
                std::cout << "message is not FLOAT!!!!" << std::endl;
                return;
            }
        }
        
        // store messages
        if (!message.isEmpty())
        {
            for (int i = 0; i < message.size(); i++)
            {
                osc_messages_[i] = message[i].getFloat32();
            }
        }
        
        std::string address = message.getAddressPattern().toString().toStdString();
        
        oscMessageCallback (address, osc_messages_);
    }
    
    bool isValidOscPort (int port) const { return port > 0 && port < 65536; }
    
    juce::OSCTimeTag time_tag_;
    
    int port_number_;
    
    float osc_messages_[4];
};
