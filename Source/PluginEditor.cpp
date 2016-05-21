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
    : AudioProcessorEditor (&p), processor (p), textColour(Colours::lightgrey)
{	
	
	zenLookAndFeel = new ZenLookAndFeel();
	LookAndFeel::setDefaultLookAndFeel(zenLookAndFeel);
		
	backgroundImg = ImageFileFormat::loadFrom(BinaryData::backgroundImg_png, (size_t)BinaryData::backgroundImg_pngSize);
	//bypassImg = ImageFileFormat::loadFrom(BinaryData::bypassBtnImg_png, (size_t)BinaryData::bypassBtnImg_pngSize);
	
	addAndMakeVisible(gainEditor = new ZenLabelDisplay("Gain", "00"));
	gainEditor->setBounds(90, 8, 55, 24);
	
	addAndMakeVisible(targetEditor = new ZenDecibelTextEditor("Target Editor", processor.targetParam));
	targetEditor->setMultiLine(false);
	targetEditor->setReturnKeyStartsNewLine(false);
	targetEditor->setReadOnly(false);
	targetEditor->setScrollbarsShown(false);
	targetEditor->setCaretVisible(true);
	targetEditor->setSelectAllWhenFocused(true);
	targetEditor->setText("-18.00 dB");
	targetEditor->setBounds(156, 128, 70, 24);
	targetEditor->addListener(this);
	
	//graphicalManager = new TimeSliceThread("graphicalManagerTrd");
	//graphicalManager->startThread(2);
	////addAndMakeVisible(vuMeter = new SegmentedMeter());
	//addChildComponent(vuMeter = new SegmentedMeter());
	//vuMeter->setNumDecibelsPerSeg(6);
	//vuMeter->setBounds(200, 200, 100, 100);
	//
	//graphicalManager->addTimeSliceClient(vuMeter);
	
	addAndMakeVisible(leftAvgRMSLabel = new ZenLabelDisplay("Average RMS Label", "00"));
	leftAvgRMSLabel->setColour(Label::textColourId, Colours::white);
	leftAvgRMSLabel->setBounds(8, 42, 72, 24);
	
	addAndMakeVisible(leftMaxRMSLabel = new ZenLabelDisplay("Max RMS Label", "00"));
	leftMaxRMSLabel->setColour(Label::textColourId, Colours::white);
	leftMaxRMSLabel->setBounds(8, 68, 72, 24);
	
	addAndMakeVisible(leftPeakLabel = new ZenLabelDisplay("Max RMS Label", "00"));
	leftPeakLabel->setColour(Label::textColourId, Colours::white);
	leftPeakLabel->setBounds(8, 94, 72, 24);
	
	addAndMakeVisible(rightAvgRMSLabel = new ZenLabelDisplay("Average RMS Label", "0"));
	rightAvgRMSLabel->setColour(Label::textColourId, Colours::white);
	rightAvgRMSLabel->setBounds(87, 42, 72, 24);
	
	addAndMakeVisible(rightMaxRMSLabel = new ZenLabelDisplay("Max RMS Label", "00"));
	rightMaxRMSLabel->setColour(Label::textColourId, Colours::white);
	rightMaxRMSLabel->setBounds(87, 68, 72, 24);
	
	addAndMakeVisible(rightPeakLabel = new ZenLabelDisplay("Max RMS Label", "00"));
	rightPeakLabel->setColour(Label::textColourId, textColour);
	rightPeakLabel->setBounds(87, 94, 72, 24);
	
	addAndMakeVisible(maxBox = new Label("maxBox", "MAX"));
	maxBox->setColour(Label::textColourId, textColour);
	maxBox->setBounds(172, 68, 40, 24);
	
	addAndMakeVisible(peakBox = new Label("peakBox", "PEAK"));
	peakBox->setColour(Label::textColourId, textColour);
	peakBox->setBounds(172, 96, 40, 24);
	
	addAndMakeVisible(avgBox = new Label("avgBox", "AVG"));
	avgBox->setColour(Label::textColourId, textColour);
	avgBox->setBounds(172, 40, 40, 24);
	
	addAndMakeVisible(runningBox = new Label("RunningBox", "RUN"));
	runningBox->setColour(Label::textColourId, textColour);
	runningBox->setBounds(0, 8, 70, 24);
	
	//addAndMakeVisible(leftRunningRMS = new ZenLabelDisplay("Left Running RMS Label", TRANS("00")));
	//leftRunningRMS->setColour(Label::textColourId, Colours::white);
	//leftRunningRMS->setBounds();
	
	//addAndMakeVisible(rightRunningRMS = new ZenLabelDisplay("Right Running RMS Label", TRANS("00")));
	//rightRunningRMS->setColour(Label::textColourId, Colours::white);
	//rightRunningRMS->setBounds();
	
	addAndMakeVisible(resetBtn = new ZenImageButton("Reset Button", "Reset"));
	resetBtn->setTooltip("Reset RMS Calculation");
	resetBtn->setBounds(16, 128, 51, 24);
	resetBtn->addListener(this);
	
	
	addAndMakeVisible(autoGainBtn = new ZenImageButton("Auto Gain", "AutoGain"));
	autoGainBtn->setClickingTogglesState(true);
	autoGainBtn->setBounds(68, 128, 75, 24);
	autoGainBtn->addListener(this);

	setSize(250, 250);
	startTimer(100);
	processor.setCurrentEditor(this);
	//openGLContext = new OpenGLContext();
	//openGLContext->attachTo(*this);

	ZEN_COMPONENT_DEBUG_ATTACH(this);
}

ZenAutoTrimAudioProcessorEditor::~ZenAutoTrimAudioProcessorEditor()
{
	processor.setCurrentEditor(nullptr);
	gainEditor = nullptr;
	//graphicalManager = nullptr;
	//vuMeter = nullptr;
	leftAvgRMSLabel = nullptr;
	leftMaxRMSLabel = nullptr;
	leftPeakLabel = nullptr;
	
	rightAvgRMSLabel = nullptr;
	rightMaxRMSLabel = nullptr;
	rightPeakLabel = nullptr;
	
	//leftRunningRMS = nullptr;
	//rightRunningRMS = nullptr;
	
	maxBox = nullptr;
	peakBox = nullptr;
	avgBox = nullptr;
	runningBox = nullptr;
	
	resetBtn = nullptr;
	autoGainBtn = nullptr;
	targetEditor = nullptr;
	//openGLContext->detach();
	ZEN_COMPONENT_DEBUG_DETACH();
}


//==============================================================================
void ZenAutoTrimAudioProcessorEditor::paint (Graphics& g)
{
	g.fillAll(Colours::darkslategrey);
	g.drawImage(backgroundImg, 0, 0, getWidth(), getHeight(), 0, 0, backgroundImg.getWidth(), backgroundImg.getHeight());
	//g.drawImageAt(bypassImg, 0, 0);
}

void ZenAutoTrimAudioProcessorEditor::resized()
{ }

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
	if (&editorChanged == targetEditor)
	{
		// #TODO: DYNAMIC CAST
		float targetGain = dynamic_cast<ZenDecibelTextEditor&>(editorChanged).getDecibelValueFromText();
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
		//gainEditor->setTextWith2Precision<double>(processor.gainParam->getValueInDecibels());
		gainEditor->setText(String(processor.gainParam->getValueInDecibels(), 2), dontSendNotification);
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
