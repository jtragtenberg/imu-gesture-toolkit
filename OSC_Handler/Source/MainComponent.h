#pragma once

#include <JuceHeader.h>

#include "OSCDemo.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

class MainComponent
:
public juce::Component,
private juce::OSCReceiver,
private juce::OSCReceiver::ListenerWithOSCAddress<juce::OSCReceiver::RealtimeCallback>
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void oscBundleReceived (const juce::OSCBundle& bundle);
    void oscMessageReceived (const juce::OSCMessage& message) override;

private:
    //==============================================================================
    // Your private member variables go here...

    juce::Slider rotaryKnob;
    
    juce::OSCReceiver osc_; // temp
    
    OSCDemo oscDemo_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
