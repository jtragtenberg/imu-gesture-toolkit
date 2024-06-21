#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

class MainComponent
:
public juce::Component,
public juce::OSCReceiver,
public juce::OSCReceiver::ListenerWithOSCAddress<juce::OSCReceiver::MessageLoopCallback>
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void oscMessageReceived (const juce::OSCMessage& message) override;

private:
    //==============================================================================
    // Your private member variables go here...

    juce::Slider rotaryKnob;
    
    juce::OSCReceiver osc_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
