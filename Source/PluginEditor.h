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

	void timerCallback() override;
	ScopedPointer<SegmentedMeter> vuMeter;

	void initializeImageCache();

private:
    ZenAutoTrimAudioProcessor& processor;
	// #TODO: Implement opengl context
	ScopedPointer<OpenGLContext> openGLContext;

	ScopedPointer<ZenDecibelTextEditor> gainEditor, targetEditor;
	ScopedPointer<TimeSliceThread> graphicalManager;
	ScopedPointer<ZenLabelDisplay> leftAvgRMSLabel, rightAvgRMSLabel;
	ScopedPointer<ZenLabelDisplay> leftMaxRMSLabel, rightMaxRMSLabel;
	ScopedPointer<ZenLabelDisplay> leftPeakLabel, rightPeakLabel;
	ScopedPointer<ZenLabelDisplay> leftRunningRMS, rightRunningRMS;
	ScopedPointer<ZenTextEditor> maxBox, peakBox, avgBox, runningBox;	
	ScopedPointer<ZenImageButton> resetBtn, autoGainBtn;

	ScopedPointer<ZenLookAndFeel> zenLookAndFeel;

	//ScopedPointer<ImageCache> imageCache;
	Image testImg, bypassImg;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ZenAutoTrimAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
