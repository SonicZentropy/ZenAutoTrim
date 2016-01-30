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

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "zen_utils/components/DecibelTextEditor.h"
#include "dRowAudio/dRowAudio/gui/dRowAudio_SegmentedMeter.h"
using drow::SegmentedMeter;

class ZenAutoTrimAudioProcessorEditor : public AudioProcessorEditor,
	                                public TextEditor::Listener,
	                                public Timer
{
public:
	
	explicit ZenAutoTrimAudioProcessorEditor (ZenAutoTrimAudioProcessor&);
    ~ZenAutoTrimAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void textEditorReturnKeyPressed(TextEditor& editorChanged) override;
	virtual void timerCallback() override;
	ScopedPointer<SegmentedMeter> vuMeter;

private:
    ZenAutoTrimAudioProcessor& processor;

	ScopedPointer<DecibelTextEditor> gainEditor;
	ScopedPointer<TimeSliceThread> graphicalManager;
	ScopedPointer<Label> leftAvgRMSLabel, rightAvgRMSLabel;
	ScopedPointer<Label> leftMaxRMSLabel, rightMaxRMSLabel;
	ScopedPointer<Label> leftPeakLabel, rightPeakLabel;
	
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ZenAutoTrimAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
