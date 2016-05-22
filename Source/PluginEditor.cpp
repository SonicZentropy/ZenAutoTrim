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
	setSize(234, 250);
		
	backgroundImg = ImageFileFormat::loadFrom(BinaryData::backgroundImg_png, (size_t)BinaryData::backgroundImg_pngSize);
	
	addAndMakeVisible(titleBar = new ZenTitleBar());
	titleBar->setBounds(0, 0, getWidth(), 30);

	addAndMakeVisible(gainLabel = new Label("Gain Text", "Gain:"));
	gainLabel->setColour(Label::textColourId, textColour);
	gainLabel->setBounds(40, 40, 50, 24);

	addAndMakeVisible(gainEditor = new ZenLabelDisplay("Calculated Gain Label", "0.00"));
	gainEditor->setBounds(90, 40, 55, 24);
	
	addAndMakeVisible(leftAvgRMSLabel = new ZenLabelDisplay("Left Avg RMS Out", "00"));
	leftAvgRMSLabel->setColour(Label::textColourId, Colours::white);
	leftAvgRMSLabel->setBounds(8, 80, 72, 24);
	
	addAndMakeVisible(leftMaxRMSLabel = new ZenLabelDisplay("Left Max RMS Out", "0.00"));
	leftMaxRMSLabel->setColour(Label::textColourId, Colours::white);
	leftMaxRMSLabel->setBounds(8, 108, 72, 24);
	
	addAndMakeVisible(leftPeakLabel = new ZenLabelDisplay("Left Peak Out", "0.00"));
	leftPeakLabel->setColour(Label::textColourId, Colours::white);
	leftPeakLabel->setBounds(8, 136, 72, 24);

	addAndMakeVisible(avgBox = new Label("Avg Label", "AVG"));
	avgBox->setColour(Label::textColourId, textColour);
	avgBox->setBounds(90, 80, 40, 24);

	addAndMakeVisible(maxBox = new Label("Max Label", "MAX"));
	maxBox->setColour(Label::textColourId, textColour);
	maxBox->setBounds(90, 108, 40, 24);

	addAndMakeVisible(peakBox = new Label("Peak Label", "PEAK"));
	peakBox->setColour(Label::textColourId, textColour);
	peakBox->setBounds(90, 136, 40, 24);
	
	addAndMakeVisible(rightAvgRMSLabel = new ZenLabelDisplay("Right Avg RMS Out", "0.00"));
	rightAvgRMSLabel->setColour(Label::textColourId, Colours::white);
	rightAvgRMSLabel->setBounds(140, 80, 72, 24);
	
	addAndMakeVisible(rightMaxRMSLabel = new ZenLabelDisplay("Right Max RMS Out", "0.00"));
	rightMaxRMSLabel->setColour(Label::textColourId, Colours::white);
	rightMaxRMSLabel->setBounds(140, 108, 72, 24);
	
	addAndMakeVisible(rightPeakLabel = new ZenLabelDisplay("Right Peak Out", "0.00"));
	rightPeakLabel->setColour(Label::textColourId, textColour);
	rightPeakLabel->setBounds(140, 136, 72, 24);

	addAndMakeVisible(targetComboBox = new ZenComboBox("Target Combo Box"));
	targetComboBox->addItem("Avg RMS", 1);
	targetComboBox->addItem("Max RMS", 2);
	targetComboBox->addItem("Peak", 3);
	targetComboBox->setTextWhenNothingSelected("Target");
	targetComboBox->setBounds(9, 168, 90, 24);
	//targetComboBox->setSelectedId(1, sendNotificationAsync);
	targetComboBox->addListener(this);

	addAndMakeVisible(rmsWindowComboBox = new ZenComboBox("RMS Window Combo Box"));
	rmsWindowComboBox->addItem("10ms", 10);
	rmsWindowComboBox->addItem("300ms", 300);
	rmsWindowComboBox->addItem("1000ms", 1000);
	rmsWindowComboBox->addItem("5000ms", 5000);
	rmsWindowComboBox->setTextWhenNothingSelected("Window");
	rmsWindowComboBox->setBounds(122, 168, 90, 24);
	rmsWindowComboBox->setEnabled(false);
	//rmsWindowComboBox->setSelectedId(300, sendNotificationAsync);
	rmsWindowComboBox->addListener(this);
	
	addAndMakeVisible(resetBtn = new ZenImageButton("Reset Button", "Reset"));
	resetBtn->setTooltip("Reset RMS Calculation");
	resetBtn->setBounds(16, 200, 51, 24);
	resetBtn->addListener(this);	
	
	addAndMakeVisible(autoGainBtn = new ZenImageButton("Auto Gain Button", "AutoGain"));
	autoGainBtn->setClickingTogglesState(true);
	autoGainBtn->setBounds(154, 200, 60, 24);
	autoGainBtn->addListener(this);

	addAndMakeVisible(targetEditor = new ZenDecibelTextEditor("Target Editor", processor.targetParam));
	targetEditor->setMultiLine(false);
	targetEditor->setReturnKeyStartsNewLine(false);
	targetEditor->setReadOnly(false);
	targetEditor->setScrollbarsShown(false);
	targetEditor->setCaretVisible(true);
	targetEditor->setSelectAllWhenFocused(true);
	targetEditor->setText("-18.00 dB");
	targetEditor->setBounds(75, 200, 71, 24);
	targetEditor->addListener(this);
		
	startTimer(100);
	processor.setCurrentEditor(this);
	//openGLContext = new OpenGLContext();
	//openGLContext->attachTo(*this);

	ZEN_COMPONENT_DEBUG_ATTACH(this);
	//addAndMakeVisible(leftRunningRMS = new ZenLabelDisplay("Left Running RMS Label", TRANS("00")));
	//leftRunningRMS->setColour(Label::textColourId, Colours::white);
	//leftRunningRMS->setBounds();
	//addAndMakeVisible(rightRunningRMS = new ZenLabelDisplay("Right Running RMS Label", TRANS("00")));
	//rightRunningRMS->setColour(Label::textColourId, Colours::white);
	//rightRunningRMS->setBounds();
	//graphicalManager = new TimeSliceThread("graphicalManagerTrd");
	//graphicalManager->startThread(2);
	////addAndMakeVisible(vuMeter = new SegmentedMeter());
	//addChildComponent(vuMeter = new SegmentedMeter());
	//vuMeter->setNumDecibelsPerSeg(6);
	//vuMeter->setBounds(200, 200, 100, 100);
	//graphicalManager->addTimeSliceClient(vuMeter);
}

ZenAutoTrimAudioProcessorEditor::~ZenAutoTrimAudioProcessorEditor()
{
	processor.setCurrentEditor(nullptr);
	gainEditor = nullptr;
	
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
	gainLabel = nullptr;
	
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
		float targetGain = static_cast<ZenDecibelTextEditor&>(editorChanged).getDecibelValueFromText();
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

void ZenAutoTrimAudioProcessorEditor::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
	if (targetComboBox == comboBoxThatHasChanged)
	{
		int targetID = targetComboBox->getSelectedId();
		if (targetID == 2)
		{
			//max RMS - enable the window combo box
			rmsWindowComboBox->setEnabled(true);
			//rmsWindowComboBox->setVisible(true);
			processor.setTargetForAutoTrim(processor.MaxRMS);
		} else
		{
			//Avg RMS or Peak - disable window
			rmsWindowComboBox->setEnabled(false);
			//rmsWindowComboBox->setVisible(false);
			if (targetID == 1)
			{
				processor.setTargetForAutoTrim(processor.AverageRMS);				
			}
			else
			{
				processor.setTargetForAutoTrim(processor.Peak);
			}
		}
	} else if (rmsWindowComboBox == comboBoxThatHasChanged)
	{
		processor.setRMSWindowTimeInMS(rmsWindowComboBox->getSelectedId());
	}
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
