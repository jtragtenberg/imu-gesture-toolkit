/*
  ==============================================================================

   This file is part of the JUCE framework examples.
   Copyright (c) Raw Material Software Limited

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   to use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
   REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
   AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
   INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
   LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
   OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
   PERFORMANCE OF THIS SOFTWARE.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             OSCDemo
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Application using the OSC protocol.

 dependencies:     juce_core, juce_data_structures, juce_events, juce_graphics,
                   juce_gui_basics, juce_osc
 exporters:        xcode_mac, vs2022, linux_make

 moduleFlags:      JUCE_STRICT_REFCOUNTEDPOINTER=1

 type:             Component
 mainClass:        OSCDemo

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/

#pragma once

#include <JuceHeader.h>

using namespace juce;

class GirominData
{
public:
    GirominData()
    {
        id_ = 0;
        is_active_ = false;
        addr_ = "";
        
        for (int i = 0; i < 4; i++)
        {
            q_[i] = 0.0f;
        }
        
        for (int i = 0; i < 3; i++)
        {
            a_[i] = 0.0f;
            g_[i] = 0.0f;
        }
        
        for (int i = 0; i < 2; i++)
            b_[0] = 0.0f;
    }
    
    ~GirominData() {}
    
    // SETTERS
    void setID (int id) { id_ = id; }
    
    void setIsActive (bool is_active) { is_active_ = is_active; }
    
    void setAX (float value) { a_[0] = value; }
    void setAY (float value) { a_[1] = value; }
    void setAZ (float value) { a_[2] = value; }
    
    void setGX (float value) { g_[0] = value; }
    void setGY (float value) { g_[1] = value; }
    void setGZ (float value) { g_[2] = value; }
    
    void setQ1 (float value) { q_[0] = value; }
    void setQ2 (float value) { q_[1] = value; }
    void setQ3 (float value) { q_[2] = value; }
    void setQ4 (float value) { q_[3] = value; }
    
    void setB1 (float value) { b_[0] = value; }
    void setB2 (float value) { b_[1] = value; }
    
    // GETTERS
    float getAX() { return a_[0]; }
    float getAY() { return a_[1]; }
    float getAZ() { return a_[2]; }
    
    float getGX() { return g_[0]; }
    float getGY() { return g_[1]; }
    float getGZ() { return g_[2]; }
    
    float getQ1() { return q_[0]; }
    float getQ2() { return q_[1]; }
    float getQ3() { return q_[2]; }
    float getQ4() { return q_[3]; }
    
    float getB1() { return b_[0]; }
    float getB2() { return b_[1]; }
    
private:
    int id_;
    bool is_active_;
    std::string addr_;
    float a_[3];
    float g_[3];
    float q_[4];
    float b_[2];
};

struct GirominController
{
    std::vector<GirominData> giromins_;
    
    GirominController()
    {
        // TODO: add all giromins
        
        // TEMP: add one giromin
        GirominData g_data;
        g_data.setID (28);
        
        giromins_.push_back (g_data);
    }
    
    // TODO: break out into private methods getID, getParamGroupID, getParam methods
    void UpdateGiromin (std::string addr, float *values)
    {
        size_t start = addr.find ("/", 1);
        size_t next = addr.find ("/", start + 1);
        int id = std::stoi (addr.substr (start + 1, next - start - 1)); // ignore warning, read comment below
        int id_index = 0; // TODO: THIS IS TEMP - find out how many giromins, then: 'id - 25' instead
        
        start = next;
        next = addr.find ("/", start + 1);
        std::string param_group_id = addr.substr (start + 1, next - start - 1);
        
        std::string param;
        if (next != std::string::npos)
        {
            param = addr.substr (next + 1);
        }
        
        switch (param_group_id[0])
        {
            case 'a':
                if (param == "x")
                    giromins_[id_index].setAX (values[0]);
                else if (param == "y")
                    giromins_[id_index].setAY (values[0]);
                else if (param == "z")
                    giromins_[id_index].setAZ (values[0]);
                break;
            case 'g':
                if (param == "x")
                    giromins_[id_index].setGX (values[0]);
                else if (param == "y")
                    giromins_[id_index].setGY (values[0]);
                else if (param == "z")
                    giromins_[id_index].setGZ (values[0]);
                break;
            case 'q':
                giromins_[id_index].setQ1 (values[0]);
                giromins_[id_index].setQ2 (values[1]);
                giromins_[id_index].setQ3 (values[2]);
                giromins_[id_index].setQ4 (values[3]);
                break;
            case 'b':
                if (param_group_id == "b1")
                    giromins_[id_index].setB1 (values[0]);
                else if (param_group_id == "b2")
                    giromins_[id_index].setB2 (values[0]);
                break;
            default:
                std::cout << "Unknown param_group_id: " << param_group_id << "\n";
                break;
        }
        
        std::cout << "updated giromin" << std::endl;
    }
};

// TODO: in future update OSCHandler to have two sub classes: Receiver, Sender
class OSCHandler
:
private OSCReceiver,
private OSCReceiver::Listener<OSCReceiver::MessageLoopCallback>
{
public:
    OSCHandler (GirominController &giromin_controller_p)
    {
        giromin_controller_p_ = &giromin_controller_p;
        
        port_number_ = 1333; // temp - initial port number
        
        if (!(connectOSC (port_number_))) // temp - automatic connection
        {
            std::cout << "somethings wrong" << std::endl;
        }
        else
        {
            std::cout << "connected" << std::endl;
        }
        
        for (int i = 0; i < 3; i++)
        {
            osc_messages_[i] = 0.0f;
        }
        
        addListener (this); // note: super important
    }
    
    ~OSCHandler() {}
    
    void oscMessageReceived (const juce::OSCMessage& message) override {} // pure virtual member must be implemented - not in use
    
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
            assert (message[i].isFloat32());
        }
        
        // store messages
        if (!message.isEmpty())
        {
            for (int i = 0; i < message.size(); i++)
            {
                osc_messages_[i] = message[i].getFloat32();
            }
        }
        
        // update giromin
        giromin_controller_p_->UpdateGiromin (message.getAddressPattern().toString().toStdString(),
                                              osc_messages_);
    }
    
    bool isValidOscPort (int port) const { return port > 0 && port < 65536; }
    
    juce::OSCTimeTag time_tag_;
    
    int port_number_;
    
    float osc_messages_[4];
    
    GirominController *giromin_controller_p_;
};

class MidiHandler
{
public:
    MidiHandler()
    {
        
    }
    
private:
    std::unique_ptr<juce::MidiOutput> midiOutputDevice;
};

//==============================================================================
class OSCLogListBox final : public ListBox,
                            private ListBoxModel,
                            private AsyncUpdater
{
public:
    OSCLogListBox()
    {
        setModel (this);
        
        osc_messages_.resize (4);
    }

    ~OSCLogListBox() override = default;
    
    // data callback function
    std::function<void(juce::String, float*)> dataCallback;

    //==============================================================================
    int getNumRows() override
    {
        return oscLogList.size();
    }

    //==============================================================================
    void paintListBoxItem (int row, Graphics& g, int width, int height, bool rowIsSelected) override
    {
        ignoreUnused (rowIsSelected);

        if (isPositiveAndBelow (row, oscLogList.size()))
        {
            g.setColour (Colours::white);

            g.drawText (oscLogList[row],
                        Rectangle<int> (width, height).reduced (4, 0),
                        Justification::centredLeft, true);
        }
    }

    //==============================================================================
    // 
    void addOSCMessage (const OSCMessage& message, int level = 0)
    {
        String messageBuffer = {};
        for (int i = 0; i < message.size(); ++i){
            if (message[i].isFloat32()) {
                messageBuffer += String(message[i].getFloat32());
                messageBuffer += " ";
            }
            else if (message[i].isInt32()) {
                messageBuffer += String(message[i].getInt32());
                messageBuffer += " ";
            }
            else if (message[i].isString()) {
                messageBuffer += String(message[i].getString().toStdString());
                messageBuffer += " ";
            }
            else {
                messageBuffer += "unknown type";
            }

        }
        oscLogList.add (getIndentationString (level)
                        + message.getAddressPattern().toString()
                        + ": "
                        + messageBuffer
                        );
                        
        // address
        juce::String address = message.getAddressPattern().toString();
        
//        std::cout << address << std::endl;

        if (!message.isEmpty())
        {
            for (int i = 0; i < message.size(); i++)
            {
                osc_messages_[i] = message[i].getFloat32();
            }
        }

        triggerAsyncUpdate();
        
        // callback data function
        dataCallback (address, osc_messages_.data());
    }

    //==============================================================================
    void addOSCBundle (const OSCBundle& bundle, int level = 0)
    {
        OSCTimeTag timeTag = bundle.getTimeTag();

        oscLogList.add (getIndentationString (level)
                        + "- osc bundle, time tag = "
                        + timeTag.toTime().toString (true, true, true, true));

        for (auto& element : bundle)
        {
            if (element.isMessage())
            {
                addOSCMessage (element.getMessage(), level + 1);
            }
            else if (element.isBundle())
            {
                addOSCBundle (element.getBundle(), level + 1);
            }
        }

        triggerAsyncUpdate();
    }

    //==============================================================================
    String addOSCMessageArgument (const OSCArgument& arg, int level)
    {
        String typeAsString;
        String valueAsString;

        if (arg.isFloat32())
        {
            typeAsString = "float32";
            valueAsString = String (arg.getFloat32());
        }
        else if (arg.isInt32())
        {
            typeAsString = "int32";
            valueAsString = String (arg.getInt32());
        }
        else if (arg.isString())
        {
            typeAsString = "string";
            valueAsString = arg.getString();
        }
        else if (arg.isBlob())
        {
            typeAsString = "blob";
            auto& blob = arg.getBlob();
            valueAsString = String::fromUTF8 ((const char*) blob.getData(), (int) blob.getSize());
        }
        else
        {
            typeAsString = "(unknown)";
        }

        oscLogList.add (getIndentationString (level + 1) + "- " + typeAsString.paddedRight (' ', 12) + valueAsString);
        
        return valueAsString;
    }

    //==============================================================================
    void addInvalidOSCPacket (const char* /* data */, int dataSize)
    {
        oscLogList.add ("- (" + String (dataSize) + "bytes with invalid format)");
    }

    //==============================================================================
    void clear()
    {
        oscLogList.clear();
        triggerAsyncUpdate();
    }

    //==============================================================================
    void handleAsyncUpdate() override
    {
        updateContent();
        scrollToEnsureRowIsOnscreen (oscLogList.size() - 1);
        repaint();
    }

private:
    static String getIndentationString (int level)
    {
        return String().paddedRight (' ', 2 * level);
    }

    //==============================================================================
    StringArray oscLogList;
    
    std::vector<float> osc_messages_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCLogListBox)
};

//==============================================================================
//class OSCSenderDemo final : public Component
//{
//public:
//    OSCSenderDemo()
//    {
//        addAndMakeVisible (senderLabel);
//        senderLabel.attachToComponent (&rotaryKnob, false);
//
//        rotaryKnob.setRange (0.0, 1.0);
//        rotaryKnob.setSliderStyle (Slider::RotaryVerticalDrag);
//        rotaryKnob.setTextBoxStyle (Slider::TextBoxBelow, true, 150, 25);
//        rotaryKnob.setBounds (50, 50, 180, 180);
//        addAndMakeVisible (rotaryKnob);
//        rotaryKnob.onValueChange = [this]
//        {
//            // create and send an OSC message with an address and a float value:
//            if (! sender1.send ("/juce/rotaryknob", (float) rotaryKnob.getValue()))
//                showConnectionErrorMessage ("Error: could not send OSC message.");
//            if (! sender2.send ("/juce/rotaryknob", (float) rotaryKnob.getValue()))
//                showConnectionErrorMessage ("Error: could not send OSC message.");
//        };
//
//        // specify here where to send OSC messages to: host URL and UDP port number
//        if (! sender1.connect ("127.0.0.1", 9001))
//            showConnectionErrorMessage ("Error: could not connect to UDP port 9001.");
//        if (! sender2.connect ("127.0.0.1", 9002))
//            showConnectionErrorMessage ("Error: could not connect to UDP port 9002.");
//    }
//
//private:
//    //==============================================================================
//    void showConnectionErrorMessage (const String& messageText)
//    {
//        auto options = MessageBoxOptions::makeOptionsOk (MessageBoxIconType::WarningIcon,
//                                                         "Connection error",
//                                                         messageText);
//        messageBox = AlertWindow::showScopedAsync (options, nullptr);
//    }
//
//    //==============================================================================
//    Slider rotaryKnob;
//    OSCSender sender1, sender2;
//    Label senderLabel { {}, "Sender" };
//    ScopedMessageBox messageBox;
//
//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCSenderDemo)
//};
//
////OSCSender {
////
////}
////
////OSCReceiever {
////    some function
////}
////
////OscHandler {
////    OSCReceiever {passing that function}
////    OSCSender
////}
////
////Calibration {
////
////}
////
////GirominController {
////    OscHandler {when that function happens = do something with that data}
////    Calibration
////}
//
//
//
////==============================================================================
class OSCReceiverDemo final : public Component,
                              private OSCReceiver,
                              private OSCReceiver::ListenerWithOSCAddress<OSCReceiver::MessageLoopCallback>
{
public:
    //==============================================================================
    OSCReceiverDemo()
    {
        addAndMakeVisible (receiverLabel);
        receiverLabel.attachToComponent (&rotaryKnob, false);

        rotaryKnob.setRange (0.0, 1.0);
        rotaryKnob.setSliderStyle (Slider::RotaryVerticalDrag);
        rotaryKnob.setTextBoxStyle (Slider::TextBoxBelow, true, 150, 25);
        rotaryKnob.setBounds (50, 50, 180, 180);
        rotaryKnob.setInterceptsMouseClicks (false, false);
        addAndMakeVisible (rotaryKnob);

        // specify here on which UDP port number to receive incoming OSC messages
        if (! connect (1333))
            showConnectionErrorMessage ("Error: could not connect to UDP port 9001.");

        // tell the component to listen for OSC messages matching this address:
        addListener (this, "/giromin/26/a/x");
        
    }

private:
    //==============================================================================
    void oscMessageReceived (const OSCMessage& message) override
    {
        if (message.size() == 1 && message[0].isFloat32())
            rotaryKnob.setValue (jlimit (0.0f, 10.0f, message[0].getFloat32()));
    }

    void showConnectionErrorMessage (const String& messageText)
    {
        auto options = MessageBoxOptions::makeOptionsOk (MessageBoxIconType::WarningIcon,
                                                         "Connection error",
                                                         messageText);
        messageBox = AlertWindow::showScopedAsync (options, nullptr);
    }

    //==============================================================================
    Slider rotaryKnob;
    Label receiverLabel { {}, "Receiver" };
    ScopedMessageBox messageBox;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCReceiverDemo)
};

//==============================================================================
class OSCMonitorDemo final : public Component,
                             private OSCReceiver::Listener<OSCReceiver::MessageLoopCallback>
{
public:
    //==============================================================================
    
    struct GirominOSCData
    {
        std::string address_ = "";
        float osc_message_list_[4];
    };
    std::array<GirominOSCData, 9> giromin_data_;
    
    std::unique_ptr<juce::MidiOutput> midiOutputDevice;
    
    OSCMonitorDemo()
    {
        //GUI
        portNumberLabel.setBounds (10, 18, 130, 25);
        addAndMakeVisible (portNumberLabel);

        portNumberField.setEditable (true, true, true);
        portNumberField.setBounds (140, 18, 50, 25);
        addAndMakeVisible (portNumberField);

        connectButton.setBounds (210, 18, 100, 25);
        addAndMakeVisible (connectButton);
        connectButton.onClick = [this] { connectButtonClicked(); };

        clearButton.setBounds (320, 18, 60, 25);
        addAndMakeVisible (clearButton);
        clearButton.onClick = [this] { clearButtonClicked(); };

        connectionStatusLabel.setBounds (450, 18, 240, 25);
        updateConnectionStatusLabel();
        addAndMakeVisible (connectionStatusLabel);

        oscLogListBox.setBounds (0, 60, 700, 340);
        addAndMakeVisible (oscLogListBox);
        
        addAndMakeVisible (receiverLabel);
        receiverLabel.attachToComponent (&rotaryKnob, false);

        rotaryKnob.setRange (-1.0, 1.0);
        rotaryKnob.setSliderStyle (Slider::RotaryVerticalDrag);
        rotaryKnob.setTextBoxStyle (Slider::TextBoxBelow, true, 150, 25);
        rotaryKnob.setBounds (500, 60, 180, 180);
        rotaryKnob.setInterceptsMouseClicks (false, false);
        addAndMakeVisible (rotaryKnob);
        
        //OSC IN Setup()
            //Connect to OSC Server
        if (oscReceiver.connect (currentPortNumber))
               {
//                   currentPortNumber = 1333;
                   connectButton.setButtonText ("Disconnect");
               }

        oscReceiver.addListener (this);
        oscReceiver.registerFormatErrorHandler ([this] (const char* data, int dataSize)
                                                {
                                                    oscLogListBox.addInvalidOSCPacket (data, dataSize);
                                                });
        
        //Giromin Controller Setup()
        giromin_data_[0].address_ = "/giromin/26/a/x";
        giromin_data_[1].address_ = "/giromin/26/a/y";
        giromin_data_[2].address_ = "/giromin/26/a/z";
        giromin_data_[3].address_ = "/giromin/26/g/x";
        giromin_data_[4].address_ = "/giromin/26/g/y";
        giromin_data_[5].address_ = "/giromin/26/g/z";
        giromin_data_[6].address_ = "/giromin/26/q";
        giromin_data_[7].address_ = "/giromin/26/b1";
        giromin_data_[8].address_ = "/giromin/26/b2";
        
        //Midi Out Setup()
        auto midiOutputs = juce::MidiOutput::getAvailableDevices();
        if (!midiOutputs.isEmpty())
        {
            // Print details of each available MIDI output device
            for (const auto& midiOutput : midiOutputs)
            {
                std::cout << "MIDI Output Device: " << midiOutput.name << " (" << midiOutput.identifier << ")" << std::endl;
            }
            auto midiOutput = midiOutputs[0];
            midiOutputDevice = juce::MidiOutput::openDevice(midiOutput.identifier);
            if (midiOutputDevice != nullptr)
            {
                DBG("MIDI output device opened: " + midiOutput.name);
            }
            else
            {
                DBG("Failed to open MIDI output device: " + midiOutput.name);
            }
        }
        else
        {
            DBG("No MIDI output devices available.");
        }
        
        
        //Gesture Processing Setup
        float filtered = 0;
        
        oscLogListBox.dataCallback = [&] (juce::String address, float* osc_messages)
        {
            //OSC In Process()
            
//            std::cout << address << std::endl;
            for (int i = 0; i < giromin_data_.size(); i++)
            {
                if (giromin_data_[i].address_ == address)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        giromin_data_[i].osc_message_list_[j] = osc_messages[j];
//                        std::cout << osc_messages[j] << std::endl;
                    }
                }
            }
            
//            juce::MidiMessage midiMessage = juce::MidiMessage::noteOn (1, giromin_data_[0].osc_message_list_[0], (juce::uint8) 127);
            
            //Giromin Controller Process
            const float accelNormalizationValue = 15.3;
            
            float accelData[3] = {
                giromin_data_[0].osc_message_list_[0],
                giromin_data_[1].osc_message_list_[0],
                giromin_data_[2].osc_message_list_[0]
            };
            
            float nomalizedAccelData = accelData[0] / accelNormalizationValue;
            
            //Gesture Processing
            filtered = EMAFilter(nomalizedAccelData, filtered, 0.9);
            float gestureVal = scaleAndClamp(filtered, -1.0, 1.0, 0., 1.);
            gestureVal = accelData[0];
            
            //Mapping
            const int MidiChan = 1;
            const int MidiCCNumber = 1;
            int MidiCCValue = static_cast<int>(scaleAndClamp(gestureVal, 0., 1.0, 0, 127));
//          std::cout << "Input Value: " << accelData[0] << ", Midi Output: " << midi_msg << std::endl;
            
            //MIDI Out
            juce::MidiMessage midiMessage = juce::MidiMessage::controllerEvent (MidiChan, MidiCCNumber, MidiCCValue);
            
            midiOutputDevice->sendMessageNow (midiMessage);
        };
    }

private:
    //==============================================================================
    
    
    Label portNumberLabel    { {}, "UDP Port Number: " };
    Label portNumberField    { {}, "1333" };
    TextButton connectButton { "Connect" };
    TextButton clearButton   { "Clear" };
    Label connectionStatusLabel;

    int currentPortNumber = 1333;
    OSCLogListBox oscLogListBox;
    OSCReceiver oscReceiver;
    
    //==============================================================================


    //==============================================================================
    Slider rotaryKnob;
    Label receiverLabel { {}, "Receiver" };

    //==============================================================================
    float EMAFilter(float inputValue, float filteredValue, float filteringAmount) {
      return ((1 - filteringAmount) * inputValue) + (filteringAmount * filteredValue);
    }
    
    //==============================================================================
    float scale(float value, float inMin, float inMax, float outMin, float outMax)
    {
        return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
    }
    
    //==============================================================================
    float scaleAndClamp(float value, float inMin, float inMax, float outMin, float outMax)
    {
        // Clamp value to be within the input range
        if (value < inMin) value = inMin;
        if (value > inMax) value = inMax;

        return scale(value, inMin, inMax, outMin, outMax);
    }
    
    //==============================================================================
    void connectButtonClicked()
    {
        if (! isConnected())
            connect();
        else
            disconnect();

        updateConnectionStatusLabel();
    }

    //==============================================================================
    void clearButtonClicked()
    {
        oscLogListBox.clear();
    }

    //==============================================================================
    void oscMessageReceived (const OSCMessage& message) override
    {
        oscLogListBox.addOSCMessage (message);
        
        // Define the specific tag you are interested in
        const juce::String oscTagToDisplay = "/giromin/26/a/x";
        
        // Check if the message address matches the specific tag
        if (message.getAddressPattern().toString() == oscTagToDisplay)
        {
            if (message[0].isFloat32())
            {
                float value = message[0].getFloat32();
                rotaryKnob.setValue (jlimit (-1.0f, 1.0f, value));
            }
        }
        
        std::cout << "osc message" << std::endl;
    }
    
    void oscBundleReceived (const OSCBundle& bundle) override
    {
        oscLogListBox.addOSCBundle (bundle);
    }

    //==============================================================================
    void connect()
    {
        auto portToConnect = portNumberField.getText().getIntValue();

        if (! isValidOscPort (portToConnect))
        {
            handleInvalidPortNumberEntered();
            return;
        }

        if (oscReceiver.connect (portToConnect))
        {
            currentPortNumber = portToConnect;
            connectButton.setButtonText ("Disconnect");
        }
        else
        {
            handleConnectError (portToConnect);
        }
    }

    //==============================================================================
    void disconnect()
    {
        if (oscReceiver.disconnect())
        {
            currentPortNumber = -1;
            connectButton.setButtonText ("Connect");
        }
        else
        {
            handleDisconnectError();
        }
    }

    //==============================================================================
    void handleConnectError (int failedPort)
    {
        auto options = MessageBoxOptions::makeOptionsOk (MessageBoxIconType::WarningIcon,
                                                         "OSC Connection error",
                                                         "Error: could not connect to port " + String (failedPort));
        messageBox = AlertWindow::showScopedAsync (options, nullptr);
    }

    //==============================================================================
    void handleDisconnectError()
    {
        auto options = MessageBoxOptions::makeOptionsOk (MessageBoxIconType::WarningIcon,
                                                         "Unknown error",
                                                         "An unknown error occurred while trying to disconnect from UDP port.");
        messageBox = AlertWindow::showScopedAsync (options, nullptr);
    }

    //==============================================================================
    void handleInvalidPortNumberEntered()
    {
        auto options = MessageBoxOptions::makeOptionsOk (MessageBoxIconType::WarningIcon,
                                                         "Invalid port number",
                                                         "Error: you have entered an invalid UDP port number.");
        messageBox = AlertWindow::showScopedAsync (options, nullptr);
    }

    //==============================================================================
    bool isConnected() const
    {
        return currentPortNumber != -1;
    }

    //==============================================================================
    bool isValidOscPort (int port) const
    {
        return port > 0 && port < 65536;
    }

    //==============================================================================
    void updateConnectionStatusLabel()
    {
        String text = "Status: ";

        if (isConnected())
            text += "Connected to UDP port " + String (currentPortNumber);
        else
            text += "Disconnected";

        auto textColour = isConnected() ? Colours::green : Colours::red;

        connectionStatusLabel.setText (text, dontSendNotification);
        connectionStatusLabel.setFont (FontOptions (15.00f, Font::bold));
        connectionStatusLabel.setColour (Label::textColourId, textColour);
        connectionStatusLabel.setJustificationType (Justification::centredRight);
    }

    ScopedMessageBox messageBox;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCMonitorDemo)
};

//==============================================================================
class OSCDemo final : public Component
{
public:
    OSCDemo() : osc_handler_ (giromin_controller_)
    {
//        addAndMakeVisible (monitor);
//        addAndMakeVisible (receiver);
//        addAndMakeVisible (sender);

        setSize (700, 400);
    }

    void resized() override
    {
//        auto bounds = getLocalBounds();
//
//        auto lowerBounds = bounds.removeFromBottom (getHeight() / 2);
//        auto halfBounds  = bounds.removeFromRight  (getWidth()  / 2);

//        sender  .setBounds (bounds);
//        receiver.setBounds (halfBounds);
//        monitor.setBounds (lowerBounds.removeFromTop (getHeight() / 2));
    }

private:
//    OSCMonitorDemo  monitor;
//    OSCReceiverDemo receiver;
//    OSCSenderDemo   sender;
    
    GirominController giromin_controller_;
    OSCHandler osc_handler_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCDemo)
};  
