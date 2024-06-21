#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (600, 400);
    // specify here on which UDP port number to receive incoming OSC messages
    int port_number = 1333;
    
    rotaryKnob.setRange (0.0, 1.0);
    rotaryKnob.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    rotaryKnob.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 150, 25);
    rotaryKnob.setBounds (10, 10, 180, 180);
    rotaryKnob.setInterceptsMouseClicks (false, false);
    addAndMakeVisible (rotaryKnob);
    
    connect (port_number);
    addListener (this, "/giromin/25/a");
    
    std::cout << "sdlkfj" << std::endl;
    
    DBG ("a;kdflkj");
    
    bool something = 1;
    printf(something);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
}

void MainComponent::resized()
{
    rotaryKnob.setBounds(getLocalBounds());
}

void MainComponent::oscMessageReceived (const juce::OSCMessage& message)
{
    
//    if (message[0].isFloat32())
//    {
//        rotaryKnob.setValue(message[0].getFloat32());
//    }
}
