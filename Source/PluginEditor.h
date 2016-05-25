/*==============================================================================
//  PluginEditor.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2016/01/28
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Main GUI Editor Class Header
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "JuceHeader.h"
#include "PluginProcessor.h"
//#include "dRowAudio/dRowAudio/gui/dRowAudio_SegmentedMeter.h"

using drow::SegmentedMeter;

class ZenAutoTrimAudioProcessorEditor 
	: public AudioProcessorEditor,
	public TextEditor::Listener,
	public TextButton::Listener,
	public ComboBox::Listener,
	//public ZenLabelDisplay::Listener,
	public Timer
{
public:

	explicit ZenAutoTrimAudioProcessorEditor (ZenAutoTrimAudioProcessor&);
    ~ZenAutoTrimAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

	void textEditorReturnKeyPressed(TextEditor& editorChanged) override;
	void textEditorFocusLost(TextEditor& editorChanged) override;
	void textEditorUpdateDueToChange(TextEditor& editorChanged);
	void buttonClicked(Button*) override;
	void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
	void updateUIFromProcessor();
	void timerCallback() override;
	
	//ScopedPointer<SegmentedMeter> vuMeter;
	//void initializeImageCache();

private:
    ZenAutoTrimAudioProcessor& processor;
	//ScopedPointer<OpenGLContext> openGLContext;
	
	ScopedPointer<TimeSliceThread> graphicalManager;
	
	ScopedPointer<ZenTitleBar> titleBar;
	ScopedPointer<ZenDecibelTextEditor> targetEditor;
	ScopedPointer<ZenLabelDisplay> gainEditor;
	ScopedPointer<ZenLabelDisplay> leftAvgRMSLabel, rightAvgRMSLabel;
	ScopedPointer<ZenLabelDisplay> leftMaxRMSLabel, rightMaxRMSLabel;
	ScopedPointer<ZenLabelDisplay> leftPeakLabel, rightPeakLabel;
	ScopedPointer<ZenLabelDisplay> leftWindowRMSLabel, rightWindowRMSLabel;
	ScopedPointer<Label> maxBox, peakBox, avgBox, gainLabel, winBox;	
	ScopedPointer<ZenImageButton> resetBtn, autoGainBtn;
	ScopedPointer<ZenComboBox> targetComboBox, rmsWindowComboBox;
	
	ScopedPointer<ZenLookAndFeel> zenLookAndFeel;

	Colour textColour;

	//ScopedPointer<ImageCache> imageCache;
	Image backgroundImg, bypassImg;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ZenAutoTrimAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
