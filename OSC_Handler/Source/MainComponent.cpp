#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (900, 400);
    // specify here on which UDP port number to receive incoming OSC messages
    
    rotaryKnob.setRange (0, 1.0);
    rotaryKnob.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    rotaryKnob.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 150, 25);
    rotaryKnob.setBounds (10, 10, 180, 180);
    rotaryKnob.setInterceptsMouseClicks (false, false);
//    addAndMakeVisible (rotaryKnob);
    
    for (int i = 0; i < eulerKnobs.size(); i++)
    {
        eulerKnobs[i].setRange (-(M_PI), M_PI);
        eulerKnobs[i].setSliderStyle (juce::Slider::RotaryVerticalDrag);
        eulerKnobs[i].setTextBoxStyle (juce::Slider::TextBoxBelow, true, 150, 25);
        eulerKnobs[i].setBounds (200 * (i + 1), 10, 180, 180);
        eulerKnobs[i].setInterceptsMouseClicks (false, false);
        addAndMakeVisible (eulerKnobs[i]);
    }
    
    eulerKnobs[1].setRange(-(M_PI / 2), (M_PI / 2));
    
    giromin_controller_.update_UI = [&](float data)
    {
        rotaryKnob.setValue (static_cast<double>(data));
    };
    
    giromin_controller_.update_euler_UI = [&](std::array<float, 3> angels)
    {
        for (int i = 0; i < angels.size(); i++)
        {
            eulerKnobs[i].setValue (static_cast<double>(angels[i]));
        }
    };
    
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
