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
	// #TODO: ADD JUCE DETECT LEAKS TO MY ZEN_UTILS CLASSES

	zenLookAndFeel = new ZenLookAndFeel();
	LookAndFeel::setDefaultLookAndFeel(zenLookAndFeel);
	setWantsKeyboardFocus(true);
	
	setSize(222, 250);
	
		
	backgroundImg = ImageFileFormat::loadFrom(
		BinaryData::backgroundImg_png, (size_t)BinaryData::backgroundImg_pngSize);
	
	addAndMakeVisible(titleBar = new ZenTitleBar("Zen Title Bar", this));
	titleBar->setBounds(0, 0, getWidth(), 30);
	bypassBtnAttachment = new ZenAudioProcessorValueTreeState::ButtonAttachment(processor.getState(), processor.bypassParamID, *(titleBar->getBypassBtn()));
	titleBar->addBypassListener(this);

	//https://www.reddit.com/r/cpp_questions/comments/4kisug/lambdastdfunction_this_access_confusion/
	auto MakeGetTextParserLambda = [] (ZenLabelDisplay* labelRef)
	{
		float val = labelRef->getTextValue().getValue();
		if (val < -96.0f)
			return String("-Inf");
		else
			return String(val) + " dB";
	};
	

	addAndMakeVisible(gainLabel = new Label("Gain Text", "Applied Gain:"));
	gainLabel->setColour(Label::textColourId, textColour);
	gainLabel->setBounds(12, 40, 130, 48);

	addAndMakeVisible(calcGainDisplay = new ZenLabelDisplay("Calculated Gain Label", "0.00"));
	calcGainDisplay->setGetTextFunction(MakeGetTextParserLambda);
	calcGainDisplay->setShouldScaleText(true);
	calcGainDisplay->setFontMarginSize(35);
	calcGainDisplay->setTooltip("The calculated amount of gain to be applied to the incoming signal.");
	calcGainDisplay->setBounds(120, 40, 75, 48);

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

	addAndMakeVisible(avgBox = new Label("Avg Label", "AVG"));
	avgBox->setColour(Label::textColourId, textColour);
	avgBox->setBounds(90, 98, 40, 24);

	addAndMakeVisible(maxBox = new Label("Max Label", "MAX"));
	maxBox->setColour(Label::textColourId, textColour);
	maxBox->setBounds(90, 126, 40, 24);

	addAndMakeVisible(peakBox = new Label("Peak Label", "PEAK"));
	peakBox->setColour(Label::textColourId, textColour);
	peakBox->setBounds(90, 154, 40, 24);
	
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
	targetComboBox->addItem("Avg RMS", CalibrationTarget::AverageRMS);
	targetComboBox->addItem("Max RMS", CalibrationTarget::MaxRMS);
	targetComboBox->addItem("Peak", CalibrationTarget::Peak);
	targetComboBox->setTextWhenNothingSelected("Target");
	targetComboBox->setBounds(9, 186, 90, 24);
	targetComboBox->setSelectedId(processor.getTargetTypeParam()->getValueAsInt(), dontSendNotification);
	targetComboBox->setTooltip("Target to automatically gain stage toward.");
	targetComboBox->addListener(this);

	addAndMakeVisible(rmsWindowComboBox = new ZenComboBox("RMS Window Combo Box"));
	rmsWindowComboBox->addItem("10ms", CalibrationTimeInMS::time10ms);
	rmsWindowComboBox->addItem("300ms", CalibrationTimeInMS::time300ms);
	rmsWindowComboBox->addItem("1000ms", CalibrationTimeInMS::time1000ms);
	rmsWindowComboBox->addItem("5000ms", CalibrationTimeInMS::time5000ms);
	rmsWindowComboBox->setTextWhenNothingSelected("Window");
	rmsWindowComboBox->setBounds(122, 186, 90, 24);
	rmsWindowComboBox->setSelectedId(getCalibrationTimeFromMS(
		processor.getRMSWindowTimeParam()->getValueAsInt()), dontSendNotification);
	if (targetComboBox->getSelectedId() == CalibrationTarget::Peak) 
		rmsWindowComboBox->setEnabled(false);
	rmsWindowComboBox->setTooltip("Window time (in milliseconds) to use when calculating RMS");
	rmsWindowComboBox->addListener(this);
	
	addAndMakeVisible(resetBtn = new ZenImageButton("Reset Button", "Reset"));
	resetBtn->setTooltip("Reset all calculation.");
	resetBtn->setBounds(16, 218, 51, 24);
	resetBtn->addListener(this);	
	
	addAndMakeVisible(autoGainBtn = new ZenImageButton("Auto Gain Button", "AutoGain"));
	autoGainBtn->setClickingTogglesState(true);
	autoGainBtn->setTooltip("Enable automatic gain staging of incoming signals.");
	autoGainBtn->setBounds(154, 218, 60, 24);
	autoGainBtn->setToggleState(processor.getAutoGainEnableParam()->getValueAsBool(), dontSendNotification);
	autoGainBtnAttachment = new ZenAudioProcessorValueTreeState::ButtonAttachment(processor.getState(), processor.autoGainEnableParamID, *autoGainBtn);
	autoGainBtn->addListener(this);

	addAndMakeVisible(targetEditor = new ZenDecibelTextEditor("Target Editor", processor.getTargetParam()));
	targetEditor->setMultiLine(false);
	targetEditor->setReturnKeyStartsNewLine(false);
	targetEditor->setReadOnly(false);
	targetEditor->setScrollbarsShown(false);
	targetEditor->setCaretVisible(true);
	targetEditor->setSelectAllWhenFocused(true);
	targetEditor->setText(processor.getTargetParam()->getTextInDB(), dontSendNotification);
	targetEditor->setBounds(75, 218, 71, 24);
	targetEditor->setTooltip("Decibel value to gain stage the incoming signal toward.");
	targetEditor->addListener(this);
		
	addChildComponent(bypassOverlay = new Label("Bypass Overlay", String()));
	bypassOverlay->setEditable(false, false, false);
	bypassOverlay->setColour(Label::backgroundColourId, Colour(0xCC202020));
	bypassOverlay->setBounds(0, 31, 222, 220);
	bypassOverlay->setVisible(!processor.isEnabled());

	tooltipWindow.setMillisecondsBeforeTipAppears(2000);

	

	startTimer(100);
	//ZEN_COMPONENT_DEBUG_ATTACH(this);

	//openGLContext = new OpenGLContext();
	//openGLContext->attachTo(*this);
}

ZenAutoTrimAudioProcessorEditor::~ZenAutoTrimAudioProcessorEditor()
{
	bypassBtnAttachment = nullptr;
	autoGainBtnAttachment = nullptr;

	titleBar = nullptr;
	calcGainDisplay = nullptr;
	
	leftAvgRMSLabel = nullptr;
	leftMaxRMSLabel = nullptr;
	leftPeakLabel = nullptr;
	
	rightAvgRMSLabel = nullptr;
	rightMaxRMSLabel = nullptr;
	rightPeakLabel = nullptr;	

	maxBox = nullptr;
	peakBox = nullptr;
	avgBox = nullptr;
	gainLabel = nullptr;
	
	targetEditor = nullptr;
	
	resetBtn = nullptr;	
	autoGainBtn = nullptr;

	targetComboBox = nullptr;
	rmsWindowComboBox = nullptr;

	zenLookAndFeel = nullptr;
	bypassOverlay = nullptr;

	//openGLContext->detach();
	//ZEN_COMPONENT_DEBUG_DETACH();
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
	//DBG("In ZenAutoTrimAudioProcessorEditor::textEditorReturnKeyPressed(editorChanged) with editor: " << editorChanged.getName());
	editorChanged.giveAwayFocus(true);
	textEditorUpdateDueToChange(editorChanged);
}

void ZenAutoTrimAudioProcessorEditor::textEditorFocusLost(TextEditor& editorChanged)
{
	
	//gainEdit->setText(String(processor.gainParam->getTextInDB()));
	targetEditor->setText(String(processor.params->getDecibelParameter(processor.targetGainParamID)->getValueInDecibels()), dontSendNotification);
}

void ZenAutoTrimAudioProcessorEditor::textEditorEscapeKeyPressed(TextEditor& editorChanged)
{
	editorChanged.giveAwayFocus(true);
}

void ZenAutoTrimAudioProcessorEditor::textEditorUpdateDueToChange(TextEditor& editorChanged)
{
	//DBG("In ZenAutoTrimAudioProcessorEditor::textEditorUpdateDueToChange(editorChanged) with editor: " << editorChanged.getName());
	if (&editorChanged == targetEditor)
	{		
		float targetGain = static_cast<ZenDecibelTextEditor&>(editorChanged).getDecibelValueFromText();
		DBG("Setting target gain in DB to: " << targetGain);
		processor.getTargetParam()->setValueFromDecibelsNotifyingHost(targetGain);
	}
}


void ZenAutoTrimAudioProcessorEditor::buttonClicked(Button* pressedBtn)
{
	if (pressedBtn == resetBtn)
	{
		processor.resetCalculation();
		updateUIFromProcessor();
	}
	//else if (pressedBtn == autoGainBtn)
	//{
	//	processor.getAutoGainEnableParam()->toggleValue();
	//}
	else if (pressedBtn == titleBar->getBypassBtn())
	{
		//invert toggle state, since processor param is "Bypassed"
		//processor.getBypassParam()->setValueFromBool(!pressedBtn->getToggleState());
		bypassOverlay->setVisible(!pressedBtn->getToggleState());
	}
}

void ZenAutoTrimAudioProcessorEditor::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
	if (targetComboBox == comboBoxThatHasChanged)
	{
		int targetID = targetComboBox->getSelectedId();
		if (targetID == 3)
		{
			//max RMS - enable the window combo box
			rmsWindowComboBox->setEnabled(false);
			processor.getTargetTypeParam()->setValueNotifyingHost(CalibrationTarget::Peak);
			
		} else
		{
			//RMS - enable window
			rmsWindowComboBox->setEnabled(true);
			//rmsWindowComboBox->setVisible(false);
			if (targetID == 1)
			{
				//processor.setTargetForAutoTrim(CalibrationTarget::AverageRMS);
				processor.getTargetTypeParam()->setValueNotifyingHost(CalibrationTarget::AverageRMS);
			}
			else
			{
				//processor.setTargetForAutoTrim(CalibrationTarget::MaxRMS);
				processor.getTargetTypeParam()->setValueNotifyingHost(CalibrationTarget::MaxRMS);
			}

		}
		//processor.resetCalculation();
		//updateUIFromProcessor();
	} 
	else if (rmsWindowComboBox == comboBoxThatHasChanged)
	{
		processor.setRMSWindowTimeInMS(rmsWindowComboBox->getSelectedId());
	}
}

void ZenAutoTrimAudioProcessorEditor::updateUIFromProcessor()
{	
	//jassertfalse; // here! don't reset update from UI, or separate to 2 vars
	if (processor.getGainParam()->checkUIUpdateAndReset())
	{
		calcGainDisplay->setText(String(processor.getGainParam()->getValueInDecibels(), 2), dontSendNotification);
	}

	//Window RMS now calculates proper full-length average RMS. Here just for informational purposes	
	leftAvgRMSLabel->setText(convertTo2PrecisionString(Decibels::gainToDecibels(processor.getLevelAnalysisManager().getLeftCurrentRunningRms())), dontSendNotification);
	rightAvgRMSLabel->setText(convertTo2PrecisionString(Decibels::gainToDecibels(processor.getLevelAnalysisManager().getRightCurrentRunningRms())), dontSendNotification);

	// Max RMS is the current maximum found RMS of a single window, used to set proper trim level
	leftMaxRMSLabel->setText(convertTo2PrecisionString(Decibels::gainToDecibels(processor.getLevelAnalysisManager().getLeftMaxRms())), dontSendNotification);
	rightMaxRMSLabel->setText(convertTo2PrecisionString(Decibels::gainToDecibels(processor.getLevelAnalysisManager().getRightMaxRms())), dontSendNotification);

	// Peak single sample found
	leftPeakLabel->setText(convertTo2PrecisionString(Decibels::gainToDecibels(processor.getLevelAnalysisManager().getLeftPeak())), dontSendNotification);
	rightPeakLabel->setText(convertTo2PrecisionString(Decibels::gainToDecibels(processor.getLevelAnalysisManager().getRightPeak())), dontSendNotification);

	if (processor.getTargetParam()->checkUIUpdateAndReset())
	{
		targetEditor->setText(String(processor.params->getDecibelParameter(processor.targetGainParamID)->getValueInDecibels()) + " dB", dontSendNotification);			
	}

	if (processor.getTargetTypeParam()->checkUIUpdateAndReset())
	{
		targetComboBox->setSelectedId(processor.getTargetTypeParam()->getValueAsInt(), dontSendNotification);
	}

	if (processor.getRMSWindowTimeParam()->checkUIUpdateAndReset())
	{
		int timeInMSValue = processor.getRMSWindowTimeParam()->getValueAsInt();
		CalibrationTimeInMS timeInMS = getCalibrationTimeFromMS(timeInMSValue);
		rmsWindowComboBox->setSelectedId(timeInMS, dontSendNotification);
	}	
}

CalibrationTimeInMS ZenAutoTrimAudioProcessorEditor::getCalibrationTimeFromMS(int inMS)
{
	CalibrationTimeInMS timeInMS;
	switch (inMS)
	{
	case 10:
		timeInMS = time10ms;
		break;
	case 300:
		timeInMS = time300ms;
		break;
	case 1000:
		timeInMS = time1000ms;
		break;
	case 5000:
		timeInMS = time5000ms;
		break;
	default:
		timeInMS = time300ms;
		jassertfalse;
	}

	return timeInMS;
}

void ZenAutoTrimAudioProcessorEditor::timerCallback()
{
	if (processor.isEnabled())
	{
		updateUIFromProcessor();
	}
}
