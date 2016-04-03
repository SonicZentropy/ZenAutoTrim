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
	gainEditor->setText(String(processor.gainParam->getValueInDecibels()));
	gainEditor->setSelectAllWhenFocused(true);
	gainEditor->setPopupMenuEnabled(false);	
	gainEditor->setInputRestrictions(8, "-1234567890.");	
	gainEditor->addListener(this);

	addAndMakeVisible(targetEditor = new DecibelTextEditor("Target Editor", processor.targetParam));
	targetEditor->setMultiLine(false);
	targetEditor->setReturnKeyStartsNewLine(false);
	targetEditor->setReadOnly(false);
	targetEditor->setScrollbarsShown(false);
	targetEditor->setCaretVisible(true);
	targetEditor->setPopupMenuEnabled(true);
	targetEditor->setText(TRANS("-18.00 dBFS"));
	targetEditor->addListener(this);

	graphicalManager = new TimeSliceThread("graphicalManagerTrd");
	graphicalManager->startThread(2);
	//addAndMakeVisible(vuMeter = new SegmentedMeter());
	addChildComponent(vuMeter = new SegmentedMeter());
	vuMeter->setNumDecibelsPerSeg(6);
	
	graphicalManager->addTimeSliceClient(vuMeter);

	addAndMakeVisible(leftAvgRMSLabel = new Label("Average RMS Label", TRANS("0")));
	leftAvgRMSLabel->setFont(Font(15.00f, Font::bold));
	leftAvgRMSLabel->setJustificationType(Justification::centred);
	leftAvgRMSLabel->setEditable(false, false, false);
	leftAvgRMSLabel->setColour(Label::backgroundColourId, Colour(0x00000000));
	leftAvgRMSLabel->setColour(Label::textColourId, Colours::white);
	leftAvgRMSLabel->setColour(TextEditor::textColourId, Colours::black);
	leftAvgRMSLabel->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(leftMaxRMSLabel = new Label("Max RMS Label", TRANS("00")));
	leftMaxRMSLabel->setFont(Font(15.00f, Font::bold));
	leftMaxRMSLabel->setJustificationType(Justification::centred);
	leftMaxRMSLabel->setEditable(false, false, false);
	leftMaxRMSLabel->setColour(Label::backgroundColourId, Colour(0x00000000));
	leftMaxRMSLabel->setColour(Label::textColourId, Colours::white);
	leftMaxRMSLabel->setColour(TextEditor::textColourId, Colours::black);
	leftMaxRMSLabel->setColour(TextEditor::backgroundColourId, Colour(0x00000000));
	
	addAndMakeVisible(leftPeakLabel = new Label("Max RMS Label", TRANS("00")));
	leftPeakLabel->setFont(Font(15.00f, Font::bold));
	leftPeakLabel->setJustificationType(Justification::centred);
	leftPeakLabel->setEditable(false, false, false);
	leftPeakLabel->setColour(Label::backgroundColourId, Colour(0x00000000));
	leftPeakLabel->setColour(Label::textColourId, Colours::white);
	leftPeakLabel->setColour(TextEditor::textColourId, Colours::black);
	leftPeakLabel->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(rightAvgRMSLabel = new Label("Average RMS Label", TRANS("0")));
	rightAvgRMSLabel->setFont(Font(15.00f, Font::bold));
	rightAvgRMSLabel->setJustificationType(Justification::centred);
	rightAvgRMSLabel->setEditable(false, false, false);
	rightAvgRMSLabel->setColour(Label::backgroundColourId, Colour(0x00000000));
	rightAvgRMSLabel->setColour(Label::textColourId, Colours::white);
	rightAvgRMSLabel->setColour(TextEditor::textColourId, Colours::black);
	rightAvgRMSLabel->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(rightMaxRMSLabel = new Label("Max RMS Label", TRANS("00")));
	rightMaxRMSLabel->setFont(Font(15.00f, Font::bold));
	rightMaxRMSLabel->setJustificationType(Justification::centred);
	rightMaxRMSLabel->setEditable(false, false, false);
	rightMaxRMSLabel->setColour(Label::backgroundColourId, Colour(0x00000000));
	rightMaxRMSLabel->setColour(Label::textColourId, Colours::white);
	rightMaxRMSLabel->setColour(TextEditor::textColourId, Colours::black);
	rightMaxRMSLabel->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(rightPeakLabel = new Label("Max RMS Label", TRANS("00")));
	rightPeakLabel->setFont(Font(15.00f, Font::bold));
	rightPeakLabel->setJustificationType(Justification::centred);
	rightPeakLabel->setEditable(false, false, false);
	rightPeakLabel->setColour(Label::backgroundColourId, Colour(0x00000000));
	rightPeakLabel->setColour(Label::textColourId, Colours::white);
	rightPeakLabel->setColour(TextEditor::textColourId, Colours::black);
	rightPeakLabel->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(maxBox = new TextEditor("new text editor"));
	maxBox->setMultiLine(false);
	maxBox->setReturnKeyStartsNewLine(false);
	maxBox->setReadOnly(false);
	maxBox->setScrollbarsShown(true);
	maxBox->setCaretVisible(true);
	maxBox->setPopupMenuEnabled(true);
	maxBox->setText(TRANS("MAX"));

	addAndMakeVisible(peakBox = new TextEditor("new text editor"));
	peakBox->setMultiLine(false);
	peakBox->setReturnKeyStartsNewLine(false);
	peakBox->setReadOnly(false);
	peakBox->setScrollbarsShown(true);
	peakBox->setCaretVisible(true);
	peakBox->setPopupMenuEnabled(true);
	peakBox->setText(TRANS("PEAK"));

	addAndMakeVisible(avgBox = new TextEditor("new text editor"));
	avgBox->setMultiLine(false);
	avgBox->setReturnKeyStartsNewLine(false);
	avgBox->setReadOnly(false);
	avgBox->setScrollbarsShown(true);
	avgBox->setCaretVisible(true);
	avgBox->setPopupMenuEnabled(true);
	avgBox->setText(TRANS("AVG"));

	addAndMakeVisible(leftRunningRMS = new Label("Left Running RMS Label", TRANS("00")));
	leftRunningRMS->setFont(Font(15.00f, Font::bold));
	leftRunningRMS->setJustificationType(Justification::centred);
	leftRunningRMS->setEditable(false, false, false);
	leftRunningRMS->setColour(Label::backgroundColourId, Colour(0x00000000));
	leftRunningRMS->setColour(Label::textColourId, Colours::white);
	leftRunningRMS->setColour(TextEditor::textColourId, Colours::black);
	leftRunningRMS->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(rightRunningRMS = new Label("Right Running RMS Label", TRANS("00")));
	rightRunningRMS->setFont(Font(15.00f, Font::bold));
	rightRunningRMS->setJustificationType(Justification::centred);
	rightRunningRMS->setEditable(false, false, false);
	rightRunningRMS->setColour(Label::backgroundColourId, Colour(0x00000000));
	rightRunningRMS->setColour(Label::textColourId, Colours::white);
	rightRunningRMS->setColour(TextEditor::textColourId, Colours::black);
	rightRunningRMS->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(runningBox = new TextEditor("new text editor"));
	runningBox->setMultiLine(false);
	runningBox->setReturnKeyStartsNewLine(false);
	runningBox->setReadOnly(false);
	runningBox->setScrollbarsShown(true);
	runningBox->setCaretVisible(true);
	runningBox->setPopupMenuEnabled(true);
	runningBox->setText(TRANS("AVG"));

	addAndMakeVisible(resetBtn = new TextButton("Reset Button"));
	resetBtn->setTooltip(TRANS("Reset RMS Calculation"));
	resetBtn->setButtonText(TRANS("Reset"));
	resetBtn->addListener(this);

	addAndMakeVisible(autoGainBtn = new TextButton("Auto Gain"));
	autoGainBtn->setClickingTogglesState(true);
	autoGainBtn->addListener(this);

	setSize(250, 250);
	startTimer(100);
	processor.setCurrentEditor(this);
	openGLContext = new OpenGLContext();
	openGLContext->attachTo(*this);
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

	leftRunningRMS = nullptr;
	rightRunningRMS = nullptr;

	maxBox = nullptr;
	peakBox = nullptr;
	avgBox = nullptr;
	runningBox = nullptr;

	resetBtn = nullptr;
	autoGainBtn = nullptr;
	targetEditor = nullptr;
	openGLContext->detach();
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

	maxBox->setBounds(172, 68, 40, 24);
	peakBox->setBounds(172, 96, 40, 24);
	avgBox->setBounds(172, 40, 40, 24);
	resetBtn->setBounds(16, 128, 51, 24);

	autoGainBtn->setBounds(68, 128, 75, 24);
	targetEditor->setBounds(156, 128, 75, 24);
}

void ZenAutoTrimAudioProcessorEditor::textEditorReturnKeyPressed(TextEditor& editorChanged)
{
	textEditorUpdateDueToChange(editorChanged);
}


void ZenAutoTrimAudioProcessorEditor::textEditorFocusLost(TextEditor& editorChanged)
{
	textEditorUpdateDueToChange(editorChanged);
}

void ZenAutoTrimAudioProcessorEditor::textEditorUpdateDueToChange(TextEditor& editorChanged)
{
	if (&editorChanged == gainEditor)
	{
		processor.gainParam->setValueNotifyingHost(dynamic_cast<DecibelTextEditor&>(editorChanged).getDecibelValueFromText());
		dynamic_cast<DecibelTextEditor&>(editorChanged).formatTextAfterEntry();
	} else if (&editorChanged == targetEditor)
	{
		float targetGain = dynamic_cast<DecibelTextEditor&>(editorChanged).getDecibelValueFromText();
		processor.targetParam->setValueNotifyingHost(targetGain);
	}
}


void ZenAutoTrimAudioProcessorEditor::buttonClicked(Button* pressedBtn)
{
	if (pressedBtn == resetBtn)
		processor.levelAnalysisManager.resetCalculation();
	else if (pressedBtn == autoGainBtn)
		processor.autoGainEnableParam->toggleValue();
}

void ZenAutoTrimAudioProcessorEditor::timerCallback()
{
	if (processor.gainParam->needsUIUpdate())
	{
		gainEditor->setTextWith2Precision<double>(processor.gainParam->getValueInDecibels());
		processor.gainParam->setNeedsUIUpdate(false);
	}

	//Running RMS now calculates proper full-length average RMS. Here just for informational purposes	
	leftAvgRMSLabel->setText(convertTo2PrecisionString(processor.levelAnalysisManager.getLeftCurrentRunningRmsInDB()), dontSendNotification);
	rightAvgRMSLabel->setText(convertTo2PrecisionString(processor.levelAnalysisManager.getRightCurrentRunningRmsInDB()), dontSendNotification);

	// Max RMS is the current maximum found RMS of a single window, used to set proper trim level
	leftMaxRMSLabel->setText(convertTo2PrecisionString(processor.levelAnalysisManager.getLeftMaxRmsInDB()), dontSendNotification);
	rightMaxRMSLabel->setText(convertTo2PrecisionString(processor.levelAnalysisManager.getRightMaxRmsInDB()), dontSendNotification);

	// Peak single sample found
	leftPeakLabel->setText(convertTo2PrecisionString(processor.levelAnalysisManager.getLeftPeakInDB()), dontSendNotification);
	rightPeakLabel->setText(convertTo2PrecisionString(processor.levelAnalysisManager.getRightPeakInDB()), dontSendNotification);
}
