/*==============================================================================
//  FILENAME
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date DATE
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Generic GUI Component Class
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "NewComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
NewComponent::NewComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (textEditor = new TextEditor ("new text editor"));
    textEditor->setMultiLine (false);
    textEditor->setReturnKeyStartsNewLine (false);
    textEditor->setReadOnly (false);
    textEditor->setScrollbarsShown (true);
    textEditor->setCaretVisible (true);
    textEditor->setPopupMenuEnabled (true);
    textEditor->setColour (TextEditor::backgroundColourId, Colour (0x82000000));
    textEditor->setText (String());

    addAndMakeVisible (label2 = new Label ("new label",
                                           TRANS("label text")));
    label2->setFont (Font (15.00f, Font::plain));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (Label::backgroundColourId, Colour (0x9e000000));
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label3 = new Label ("new label",
                                           TRANS("label text")));
    label3->setFont (Font (15.00f, Font::plain));
    label3->setJustificationType (Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (Label::backgroundColourId, Colour (0x9e000000));
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label6 = new Label ("new label",
                                           TRANS("label text")));
    label6->setFont (Font (15.00f, Font::plain));
    label6->setJustificationType (Justification::centredLeft);
    label6->setEditable (false, false, false);
    label6->setColour (Label::backgroundColourId, Colour (0x9e000000));
    label6->setColour (TextEditor::textColourId, Colours::black);
    label6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label8 = new Label ("new label",
                                           TRANS("label text")));
    label8->setFont (Font (15.00f, Font::plain));
    label8->setJustificationType (Justification::centredLeft);
    label8->setEditable (false, false, false);
    label8->setColour (Label::backgroundColourId, Colour (0x9e000000));
    label8->setColour (TextEditor::textColourId, Colours::black);
    label8->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label9 = new Label ("new label",
                                           TRANS("label text")));
    label9->setFont (Font (15.00f, Font::plain));
    label9->setJustificationType (Justification::centredLeft);
    label9->setEditable (false, false, false);
    label9->setColour (Label::backgroundColourId, Colour (0x9e000000));
    label9->setColour (TextEditor::textColourId, Colours::black);
    label9->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (label10 = new Label ("new label",
                                            TRANS("label text")));
    label10->setFont (Font (15.00f, Font::plain));
    label10->setJustificationType (Justification::centredLeft);
    label10->setEditable (false, false, false);
    label10->setColour (Label::backgroundColourId, Colour (0x9e000000));
    label10->setColour (TextEditor::textColourId, Colours::black);
    label10->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (maxBox = new TextEditor ("new text editor"));
    maxBox->setMultiLine (false);
    maxBox->setReturnKeyStartsNewLine (false);
    maxBox->setReadOnly (false);
    maxBox->setScrollbarsShown (true);
    maxBox->setCaretVisible (true);
    maxBox->setPopupMenuEnabled (true);
    maxBox->setText (TRANS("MAX"));

    addAndMakeVisible (peakBox = new TextEditor ("new text editor"));
    peakBox->setMultiLine (false);
    peakBox->setReturnKeyStartsNewLine (false);
    peakBox->setReadOnly (false);
    peakBox->setScrollbarsShown (true);
    peakBox->setCaretVisible (true);
    peakBox->setPopupMenuEnabled (true);
    peakBox->setText (TRANS("PEAK"));

    addAndMakeVisible (avgBox = new TextEditor ("new text editor"));
    avgBox->setMultiLine (false);
    avgBox->setReturnKeyStartsNewLine (false);
    avgBox->setReadOnly (false);
    avgBox->setScrollbarsShown (true);
    avgBox->setCaretVisible (true);
    avgBox->setPopupMenuEnabled (true);
    avgBox->setText (TRANS("AVG"));

    addAndMakeVisible (resetBtn = new TextButton ("Reset Button"));
    resetBtn->setButtonText (TRANS("Reset"));
    resetBtn->addListener (this);

    addAndMakeVisible (autoGainBtn = new TextButton ("Auto Gain"));
    autoGainBtn->addListener (this);

    addAndMakeVisible (targetEditor = new DecibelTextEditor ("Target Editor"));
    targetEditor->setMultiLine (false);
    targetEditor->setReturnKeyStartsNewLine (false);
    targetEditor->setReadOnly (false);
    targetEditor->setScrollbarsShown (false);
    targetEditor->setCaretVisible (true);
    targetEditor->setPopupMenuEnabled (true);
    targetEditor->setText (TRANS("-18.00 dBFS"));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

NewComponent::~NewComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    textEditor = nullptr;
    label2 = nullptr;
    label3 = nullptr;
    label6 = nullptr;
    label8 = nullptr;
    label9 = nullptr;
    label10 = nullptr;
    maxBox = nullptr;
    peakBox = nullptr;
    avgBox = nullptr;
    resetBtn = nullptr;
    autoGainBtn = nullptr;
    targetEditor = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void NewComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void NewComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    textEditor->setBounds (56, 8, 55, 24);
    label2->setBounds (8, 42, 72, 24);
    label3->setBounds (8, 68, 72, 24);
    label6->setBounds (8, 94, 72, 24);
    label8->setBounds (87, 42, 72, 24);
    label9->setBounds (87, 68, 72, 24);
    label10->setBounds (87, 94, 72, 24);
    maxBox->setBounds (172, 68, 40, 24);
    peakBox->setBounds (172, 96, 40, 24);
    avgBox->setBounds (172, 40, 40, 24);
    resetBtn->setBounds (8, 128, 51, 24);
    autoGainBtn->setBounds (68, 128, 75, 24);
    targetEditor->setBounds (156, 128, 75, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void NewComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == resetBtn)
    {
        //[UserButtonCode_resetBtn] -- add your button handler code here..
        //[/UserButtonCode_resetBtn]
    }
    else if (buttonThatWasClicked == autoGainBtn)
    {
        //[UserButtonCode_autoGainBtn] -- add your button handler code here..
        //[/UserButtonCode_autoGainBtn]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="NewComponent" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="4" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ffffffff"/>
  <TEXTEDITOR name="new text editor" id="508c40d7595c85bf" memberName="textEditor"
              virtualName="" explicitFocusOrder="0" pos="56 8 55 24" bkgcol="82000000"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <LABEL name="new label" id="129ad4ff0dbba9ce" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="8 42 72 24" bkgCol="9e000000" edTextCol="ff000000"
         edBkgCol="0" labelText="label text" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="cf3b88eca5e26785" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="8 68 72 24" bkgCol="9e000000" edTextCol="ff000000"
         edBkgCol="0" labelText="label text" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="feeb63faa46d05db" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="8 94 72 24" bkgCol="9e000000" edTextCol="ff000000"
         edBkgCol="0" labelText="label text" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="d0e6ab678ea1399a" memberName="label8" virtualName=""
         explicitFocusOrder="0" pos="87 42 72 24" bkgCol="9e000000" edTextCol="ff000000"
         edBkgCol="0" labelText="label text" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="745745f591566cbe" memberName="label9" virtualName=""
         explicitFocusOrder="0" pos="87 68 72 24" bkgCol="9e000000" edTextCol="ff000000"
         edBkgCol="0" labelText="label text" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="a90ec086cbfe4329" memberName="label10" virtualName=""
         explicitFocusOrder="0" pos="87 94 72 24" bkgCol="9e000000" edTextCol="ff000000"
         edBkgCol="0" labelText="label text" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="new text editor" id="54d7b3e98994550c" memberName="maxBox"
              virtualName="" explicitFocusOrder="0" pos="172 68 40 24" initialText="MAX"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="84a1ae3eaf296967" memberName="peakBox"
              virtualName="" explicitFocusOrder="0" pos="172 96 40 24" initialText="PEAK"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="be6e1acc0edb1ea3" memberName="avgBox"
              virtualName="" explicitFocusOrder="0" pos="172 40 40 24" initialText="AVG"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="Reset Button" id="680d78160c28fa2" memberName="resetBtn"
              virtualName="" explicitFocusOrder="0" pos="8 128 51 24" buttonText="Reset"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Auto Gain" id="b96ce12aa0d8544a" memberName="autoGainBtn"
              virtualName="" explicitFocusOrder="0" pos="68 128 75 24" buttonText="Auto Gain"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTEDITOR name="Target Editor" id="4cfb8d149191bd88" memberName="targetEditor"
              virtualName="DecibelTextEditor" explicitFocusOrder="0" pos="156 128 75 24"
              initialText="-18.00 dBFS" multiline="0" retKeyStartsLine="0"
              readonly="0" scrollbars="0" caret="1" popupmenu="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
