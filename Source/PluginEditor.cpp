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
	setSize(222, 250);
		
	backgroundImg = ImageFileFormat::loadFrom(BinaryData::backgroundImg_png, (size_t)BinaryData::backgroundImg_pngSize);
	
	addAndMakeVisible(titleBar = new ZenTitleBar("Zen Title Bar", this));
	titleBar->setBounds(0, 0, getWidth(), 30);
	titleBar->addBypassListener(this);

	//https://www.reddit.com/r/cpp_questions/comments/4kisug/lambdastdfunction_this_access_confusion/
	//auto MakeGetTextParserLambda = [] (ZenLabelDisplay* labelRef)
	//{
	//	return [labelRef]
	//	{
	//		float val = labelRef->getTextValue().getValue();
	//		if (val < -96.0f)
	//			return String("-Inf");
	//		else
	//			return String(val);
	//	};
	//};

	auto MakeGetTextParserLambda = [] (ZenLabelDisplay* labelRef)
	{
		float val = labelRef->getTextValue().getValue();
		if (val < -96.0f)
			return String("-Inf");
		else
			return String(val);
	};
	

	addAndMakeVisible(gainLabel = new Label("Gain Text", "Gain:"));
	gainLabel->setColour(Label::textColourId, textColour);
	gainLabel->setBounds(40, 35, 50, 24);

	addAndMakeVisible(gainEditor = new ZenLabelDisplay("Calculated Gain Label", "0.00"));
	gainEditor->setGetTextFunction(MakeGetTextParserLambda);
	gainEditor->setBounds(90, 35, 75, 24);
	
	//addAndMakeVisible(leftWindowRMSLabel = new ZenLabelDisplay("Left Window RMS Out", "00"));
	//leftWindowRMSLabel->setColour(Label::textColourId, textColour);
	//leftWindowRMSLabel->setGetTextFunction(MakeGetTextParserLambda);
	//leftWindowRMSLabel->setBounds(8, 70, 72, 24);

	addAndMakeVisible(leftAvgRMSLabel = new ZenLabelDisplay("Left Avg RMS Out", "00"));
	leftAvgRMSLabel->setColour(Label::textColourId, textColour);
	leftAvgRMSLabel->setGetTextFunction(MakeGetTextParserLambda);
	leftAvgRMSLabel->setBounds(8, 98, 72, 24);
	
	addAndMakeVisible(leftMaxRMSLabel = new ZenLabelDisplay("Left Max RMS Out", "0.00"));
	leftMaxRMSLabel->setColour(Label::textColourId, textColour);
	leftMaxRMSLabel->setGetTextFunction(MakeGetTextParserLambda);
	leftMaxRMSLabel->setBounds(8, 126, 72, 24);
	
	addAndMakeVisible(leftPeakLabel = new ZenLabelDisplay("Left Peak Out", "0.00"));
	leftPeakLabel->setColour(Label::textColourId, textColour);
	leftPeakLabel->setGetTextFunction(MakeGetTextParserLambda);
	leftPeakLabel->setBounds(8, 154, 72, 24);

	//addAndMakeVisible(winBox = new Label("Window Label", "WIN"));
	//winBox->setColour(Label::textColourId, textColour);
	//winBox->setBounds(90, 70, 40, 24);

	addAndMakeVisible(avgBox = new Label("Avg Label", "AVG"));
	avgBox->setColour(Label::textColourId, textColour);
	avgBox->setBounds(90, 98, 40, 24);

	addAndMakeVisible(maxBox = new Label("Max Label", "MAX"));
	maxBox->setColour(Label::textColourId, textColour);
	maxBox->setBounds(90, 126, 40, 24);

	addAndMakeVisible(peakBox = new Label("Peak Label", "PEAK"));
	peakBox->setColour(Label::textColourId, textColour);
	peakBox->setBounds(90, 154, 40, 24);

	//addAndMakeVisible(rightWindowRMSLabel = new ZenLabelDisplay("Right Win RMS Out", "0.00"));
	//rightWindowRMSLabel->setColour(Label::textColourId, Colours::white);
	//rightWindowRMSLabel->setGetTextFunction(MakeGetTextParserLambda);
	//rightWindowRMSLabel->setBounds(140, 70, 72, 24);
	
	addAndMakeVisible(rightAvgRMSLabel = new ZenLabelDisplay("Right Avg RMS Out", "0.00"));
	rightAvgRMSLabel->setColour(Label::textColourId, Colours::white);
	rightAvgRMSLabel->setGetTextFunction(MakeGetTextParserLambda);
	rightAvgRMSLabel->setBounds(140, 98, 72, 24);
	
	addAndMakeVisible(rightMaxRMSLabel = new ZenLabelDisplay("Right Max RMS Out", "0.00"));
	rightMaxRMSLabel->setColour(Label::textColourId, Colours::white);
	rightMaxRMSLabel->setGetTextFunction(MakeGetTextParserLambda);
	rightMaxRMSLabel->setBounds(140, 126, 72, 24);
	
	addAndMakeVisible(rightPeakLabel = new ZenLabelDisplay("Right Peak Out", "0.00"));
	rightPeakLabel->setColour(Label::textColourId, textColour);
	rightPeakLabel->setGetTextFunction(MakeGetTextParserLambda);
	rightPeakLabel->setBounds(140, 154, 72, 24);

	addAndMakeVisible(targetComboBox = new ZenComboBox("Target Combo Box"));
	targetComboBox->addItem("Avg RMS", 1);
	targetComboBox->addItem("Max RMS", 2);
	targetComboBox->addItem("Peak", 3);
	targetComboBox->setTextWhenNothingSelected("Target");
	targetComboBox->setBounds(9, 186, 90, 24);
	//targetComboBox->setSelectedId(1, sendNotificationAsync);
	targetComboBox->addListener(this);

	addAndMakeVisible(rmsWindowComboBox = new ZenComboBox("RMS Window Combo Box"));
	rmsWindowComboBox->addItem("10ms", 10);
	rmsWindowComboBox->addItem("300ms", 300);
	rmsWindowComboBox->addItem("1000ms", 1000);
	rmsWindowComboBox->addItem("5000ms", 5000);
	rmsWindowComboBox->setTextWhenNothingSelected("Window");
	rmsWindowComboBox->setBounds(122, 186, 90, 24);
	rmsWindowComboBox->setEnabled(false);
	//rmsWindowComboBox->setSelectedId(300, sendNotificationAsync);
	rmsWindowComboBox->addListener(this);
	
	addAndMakeVisible(resetBtn = new ZenImageButton("Reset Button", "Reset"));
	resetBtn->setTooltip("Reset RMS Calculation");
	resetBtn->setBounds(16, 218, 51, 24);
	resetBtn->addListener(this);	
	
	addAndMakeVisible(autoGainBtn = new ZenImageButton("Auto Gain Button", "AutoGain"));
	autoGainBtn->setClickingTogglesState(true);
	autoGainBtn->setBounds(154, 218, 60, 24);
	autoGainBtn->setToggleState(processor.getAutoGainEnableParam()->getValueAsBool(), dontSendNotification);
	autoGainBtn->addListener(this);

	addAndMakeVisible(targetEditor = new ZenDecibelTextEditor("Target Editor", processor.getTargetParam()));
	targetEditor->setMultiLine(false);
	targetEditor->setReturnKeyStartsNewLine(false);
	targetEditor->setReadOnly(false);
	targetEditor->setScrollbarsShown(false);
	targetEditor->setCaretVisible(true);
	targetEditor->setSelectAllWhenFocused(true);
	targetEditor->setText(processor.getTargetParam()->getTextInDB());
	targetEditor->setBounds(75, 218, 71, 24);
	targetEditor->addListener(this);
		
	startTimer(100);
	processor.setCurrentEditor(this);

	//openGLContext = new OpenGLContext();
	//openGLContext->attachTo(*this);

	//ZEN_COMPONENT_DEBUG_ATTACH(this);
	//addAndMakeVisible(leftWindowRMS = new ZenLabelDisplay("Left Window RMS Label", TRANS("00")));
	//leftWindowRMS->setColour(Label::textColourId, Colours::white);
	//leftWindowRMS->setBounds();
	//addAndMakeVisible(rightWindowRMS = new ZenLabelDisplay("Right Window RMS Label", TRANS("00")));
	//rightWindowRMS->setColour(Label::textColourId, Colours::white);
	//rightWindowRMS->setBounds();
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
	
	//leftWindowRMS = nullptr;
	//rightWindowRMS = nullptr;
	
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
		DBG("Setting target gain in DB to: " << targetGain);
		processor.getTargetParam()->setValueNotifyingHost(targetGain);
	}
}


void ZenAutoTrimAudioProcessorEditor::buttonClicked(Button* pressedBtn)
{
	if (pressedBtn == resetBtn)
	{
		processor.getLevelAnalysisManager().resetCalculation();
		updateUIFromProcessor();
	} else if (pressedBtn == autoGainBtn)
		processor.getAutoGainEnableParam()->toggleValue();
	else if (pressedBtn == titleBar->getBypassBtn())
		//invert toggle state, since processor param is "Bypassed"
		processor.getBypassParam()->setValueFromBool(! pressedBtn->getToggleState());
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
			// #TODO: clear window history in processor level mgr if target changes
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

void ZenAutoTrimAudioProcessorEditor::updateUIFromProcessor()
{
	if (processor.getGainParam()->needsUIUpdate())
	{
		//gainEditor->setTextWith2Precision<double>(processor.gainParam->getValueInDecibels());
		gainEditor->setText(String(processor.getGainParam()->getValueInDecibels(), 2), dontSendNotification);
		processor.getGainParam()->setNeedsUIUpdate(false);
	}

	//Grabs instantaneous RMS of whichever process block most recently completed IGNORING WINDOW SIZE
	//leftWindowRMSLabel->setText(convertTo2PrecisionString(Decibels::gainToDecibels(processor.levelAnalysisManager.getLeftCurrentRms())), dontSendNotification);
	//rightWindowRMSLabel->setText(convertTo2PrecisionString(Decibels::gainToDecibels(processor.levelAnalysisManager.getRightCurrentRms())), dontSendNotification);

	//Window RMS now calculates proper full-length average RMS. Here just for informational purposes	
	leftAvgRMSLabel->setText(convertTo2PrecisionString(Decibels::gainToDecibels(processor.getLevelAnalysisManager().getLeftCurrentRunningRms())), dontSendNotification);
	rightAvgRMSLabel->setText(convertTo2PrecisionString(Decibels::gainToDecibels(processor.getLevelAnalysisManager().getRightCurrentRunningRms())), dontSendNotification);

	// Max RMS is the current maximum found RMS of a single window, used to set proper trim level
	leftMaxRMSLabel->setText(convertTo2PrecisionString(Decibels::gainToDecibels(processor.getLevelAnalysisManager().getLeftMaxRms())), dontSendNotification);
	rightMaxRMSLabel->setText(convertTo2PrecisionString(Decibels::gainToDecibels(processor.getLevelAnalysisManager().getRightMaxRms())), dontSendNotification);

	// Peak single sample found
	leftPeakLabel->setText(convertTo2PrecisionString(Decibels::gainToDecibels(processor.getLevelAnalysisManager().getLeftPeak())), dontSendNotification);
	rightPeakLabel->setText(convertTo2PrecisionString(Decibels::gainToDecibels(processor.getLevelAnalysisManager().getRightPeak())), dontSendNotification);

	if (processor.getTargetParam()->getNeedsUIUpdate())
	{
		String temp = String(processor.params.getDecibelParameter(processor.targetGainParam)->getValueInDecibels());
		targetEditor->setText(String(processor.params.getDecibelParameter(processor.targetGainParam)->getValueInDecibels()));
		processor.getTargetParam()->setNeedsUIUpdate(false);
	}
}

void ZenAutoTrimAudioProcessorEditor::timerCallback()
{
	if (processor.isEnabled())
	{
		updateUIFromProcessor();
	}
}
