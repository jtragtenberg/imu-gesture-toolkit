#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (700, 400);
    // specify here on which UDP port number to receive incoming OSC messages
    
    rotaryKnob.setRange (-1.0, 1.0);
    rotaryKnob.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    rotaryKnob.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 150, 25);
    rotaryKnob.setBounds (10, 10, 180, 180);
    rotaryKnob.setInterceptsMouseClicks (false, false);
//    addAndMakeVisible (rotaryKnob);
    
    // Adding the OSC demo component (contains the auto-connect logic)
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
