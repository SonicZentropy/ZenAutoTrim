/*==============================================================================
//  PluginEditor.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2016/01/28
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Main GUI Editor Class
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/


#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ZenAutoTrimAudioProcessorEditor::ZenAutoTrimAudioProcessorEditor (ZenAutoTrimAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
	addAndMakeVisible(gainEditor = new DecibelTextEditor("Gain", processor.gainParam));
	gainEditor->setColour(TextEditor::textColourId, Colour(0xffd2d2d2));
	gainEditor->setColour(TextEditor::backgroundColourId, Colour(0xff484848));
	gainEditor->setColour(TextEditor::highlightColourId, Colour(0xff00f5ff));
	gainEditor->setColour(TextEditor::outlineColourId, Colours::lightslategrey);
	gainEditor->setColour(TextEditor::shadowColourId, Colour(0xff2a2a2a));
	gainEditor->setColour(CaretComponent::caretColourId, Colours::white);
	
	gainEditor->setMultiLine(false);
	gainEditor->setReturnKeyStartsNewLine(false);
	gainEditor->setReadOnly(false);
	gainEditor->setScrollbarsShown(false);
	gainEditor->setCaretVisible(true);
	gainEditor->setPopupMenuEnabled(true);
	gainEditor->setText(String(processor.gainParam->get()));
	gainEditor->setSelectAllWhenFocused(true);
	gainEditor->setPopupMenuEnabled(false);	
	gainEditor->setInputRestrictions(8, "-1234567890.");	
	gainEditor->addListener(this);

	graphicalManager = new TimeSliceThread("graphicalManagerTrd");
	graphicalManager->startThread(2);
	//addAndMakeVisible(vuMeter = new SegmentedMeter());
	addChildComponent(vuMeter = new SegmentedMeter());
	vuMeter->setNumDecibelsPerSeg(6);
	
	graphicalManager->addTimeSliceClient(vuMeter);

	addAndMakeVisible(leftAvgRMSLabel = new Label("Average RMS Label",
		TRANS("0")));
	leftAvgRMSLabel->setFont(Font(15.00f, Font::bold));
	leftAvgRMSLabel->setJustificationType(Justification::centred);
	leftAvgRMSLabel->setEditable(false, false, false);
	leftAvgRMSLabel->setColour(Label::backgroundColourId, Colour(0x00000000));
	leftAvgRMSLabel->setColour(Label::textColourId, Colours::white);
	leftAvgRMSLabel->setColour(TextEditor::textColourId, Colours::black);
	leftAvgRMSLabel->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(leftMaxRMSLabel = new Label("Max RMS Label",
		TRANS("00")));
	leftMaxRMSLabel->setFont(Font(15.00f, Font::bold));
	leftMaxRMSLabel->setJustificationType(Justification::centred);
	leftMaxRMSLabel->setEditable(false, false, false);
	leftMaxRMSLabel->setColour(Label::backgroundColourId, Colour(0x00000000));
	leftMaxRMSLabel->setColour(Label::textColourId, Colours::white);
	leftMaxRMSLabel->setColour(TextEditor::textColourId, Colours::black);
	leftMaxRMSLabel->setColour(TextEditor::backgroundColourId, Colour(0x00000000));
	
	addAndMakeVisible(leftPeakLabel = new Label("Max RMS Label",
		TRANS("00")));
	leftPeakLabel->setFont(Font(15.00f, Font::bold));
	leftPeakLabel->setJustificationType(Justification::centred);
	leftPeakLabel->setEditable(false, false, false);
	leftPeakLabel->setColour(Label::backgroundColourId, Colour(0x00000000));
	leftPeakLabel->setColour(Label::textColourId, Colours::white);
	leftPeakLabel->setColour(TextEditor::textColourId, Colours::black);
	leftPeakLabel->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(rightAvgRMSLabel = new Label("Average RMS Label",
		TRANS("0")));
	rightAvgRMSLabel->setFont(Font(15.00f, Font::bold));
	rightAvgRMSLabel->setJustificationType(Justification::centred);
	rightAvgRMSLabel->setEditable(false, false, false);
	rightAvgRMSLabel->setColour(Label::backgroundColourId, Colour(0x00000000));
	rightAvgRMSLabel->setColour(Label::textColourId, Colours::white);
	rightAvgRMSLabel->setColour(TextEditor::textColourId, Colours::black);
	rightAvgRMSLabel->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(rightMaxRMSLabel = new Label("Max RMS Label",
		TRANS("00")));
	rightMaxRMSLabel->setFont(Font(15.00f, Font::bold));
	rightMaxRMSLabel->setJustificationType(Justification::centred);
	rightMaxRMSLabel->setEditable(false, false, false);
	rightMaxRMSLabel->setColour(Label::backgroundColourId, Colour(0x00000000));
	rightMaxRMSLabel->setColour(Label::textColourId, Colours::white);
	rightMaxRMSLabel->setColour(TextEditor::textColourId, Colours::black);
	rightMaxRMSLabel->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(rightPeakLabel = new Label("Max RMS Label",
		TRANS("00")));
	rightPeakLabel->setFont(Font(15.00f, Font::bold));
	rightPeakLabel->setJustificationType(Justification::centred);
	rightPeakLabel->setEditable(false, false, false);
	rightPeakLabel->setColour(Label::backgroundColourId, Colour(0x00000000));
	rightPeakLabel->setColour(Label::textColourId, Colours::white);
	rightPeakLabel->setColour(TextEditor::textColourId, Colours::black);
	rightPeakLabel->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	setSize(550, 550);
	startTimer(100);
	processor.setCurrentEditor(this);
}

ZenAutoTrimAudioProcessorEditor::~ZenAutoTrimAudioProcessorEditor()
{
	processor.setCurrentEditor(nullptr);
	gainEditor = nullptr;
	graphicalManager = nullptr;
	vuMeter = nullptr;
	leftAvgRMSLabel = nullptr;
	leftMaxRMSLabel = nullptr;
	leftPeakLabel = nullptr;

	rightAvgRMSLabel = nullptr;
	rightMaxRMSLabel = nullptr;
	rightPeakLabel = nullptr;
}

//==============================================================================
void ZenAutoTrimAudioProcessorEditor::paint (Graphics& g)
{
	g.fillAll(Colours::grey);
}

void ZenAutoTrimAudioProcessorEditor::resized()
{
	gainEditor->setBounds(56, 8, 55, 24);
	vuMeter->setBounds(200, 200, 100, 100);
	leftAvgRMSLabel->setBounds(8, 42, 72, 24);
	leftMaxRMSLabel->setBounds(8, 68, 72, 24);
	leftPeakLabel->setBounds(8, 94, 72, 24);
	rightAvgRMSLabel->setBounds(87, 42, 72, 24);
	rightMaxRMSLabel->setBounds(87, 68, 72, 24);
	rightPeakLabel->setBounds(87, 94, 72, 24);
}

void ZenAutoTrimAudioProcessorEditor::textEditorReturnKeyPressed(TextEditor& editorChanged)
{
	if (&editorChanged == gainEditor)
	{
		processor.gainParam->setValueNotifyingHost(dynamic_cast<DecibelTextEditor&>(editorChanged).getNormalizedValueFromText());
		dynamic_cast<DecibelTextEditor&>(editorChanged).formatTextAfterEntry();
		
	}		
}

void ZenAutoTrimAudioProcessorEditor::timerCallback()
{
	if (processor.gainParam->needsUIUpdate())
	{
		gainEditor->setText(String(processor.gainParam->get()));
		gainEditor->formatTextAfterEntry();
		processor.gainParam->setNeedsUIUpdate(false);
	}
	double rmsInDB = processor.rmsManager.getAverageTotalRMS();
	rmsInDB = Decibels::gainToDecibels(rmsInDB);
	avgRMSLabel->setText(String(rmsInDB), dontSendNotification);
	double avgInDB = processor.rmsManager.getMaxFoundRMS();
	avgInDB = Decibels::gainToDecibels(avgInDB);
	maxRMSLabel->setText(String(avgInDB), dontSendNotification);
}

