/*==============================================================================
//  PluginProcessor.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2016/01/20
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: 
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ZenAutoTrimAudioProcessor::ZenAutoTrimAudioProcessor()
{
	// #TODO: bitwig parameter text showing normalized instead of in db
	undoManager = new UndoManager();
	params = new ZenAudioProcessorValueTreeState(*this, undoManager);
	
	gainParam = params->createAndAddDecibelParameter(gainParamID, "Gain", -96.0f, 18.0f, 0.0f);
	targetGainParam = params->createAndAddDecibelParameter(targetGainParamID, "TargetGain", -96.0f, 18.0f, -18.0f);
	autoGainEnableParam = params->createAndAddBoolParameter(autoGainEnableParamID, "AutoGain", false);
	bypassParam = params->createAndAddBoolParameter(bypassParamID, "Bypass", false);
	targetTypeParam = params->createAndAddIntParameter(targetTypeParamID, "Target Type", 0, CalibrationTarget::targetCount, CalibrationTarget::Peak);
	rmsWindowTimeParam = params->createAndAddIntParameter(rmsWindowTimeParamID, "RMS Window Time", 10, 5000, CalibrationTimeInMS::time300ms);

	params->state = ValueTree("ZenAutoTrim");

#ifdef JUCE_DEBUG
	//// #TODO: Change this to use Juce SharedResourcePointer - https://forum.juce.com/t/juce-singleton-implementation-confusion/17847/6
	//debugWindow = ZenDebugEditor::getInstance();
	//debugWindow->setSize(650, 400);
	////Open in bottom right corner
	//debugWindow->setTopLeftPosition(1900 - debugWindow->getWidth(), 1040 - debugWindow->getHeight());
	//// #TODO: add JUCE REF COUNTED OBJECT to zen GUI

#ifdef JUCE_MSVC  //Visual Studio mem leak diagnostics settings 
	_CrtSetDbgFlag(0); //Turn off VS memory dump output
	//_crtBreakAlloc = 5389;	//Break on this memory allocation number (When Debug)
#endif
#endif
}

ZenAutoTrimAudioProcessor::~ZenAutoTrimAudioProcessor()
{
	params = nullptr;
	undoManager = nullptr;
#ifdef JUCE_DEBUG
	//debugWindow->deleteInstance();
#endif
}

void ZenAutoTrimAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	if (isEnabled())
	{
		aPlayHead = getPlayHead();
		AudioPlayHead::CurrentPositionInfo posInfo;
		aPlayHead->getCurrentPosition(posInfo);
		
		//float* leftData = buffer.getWritePointer(0); //leftData references left channel now
		//float* rightData = buffer.getWritePointer(1); //right data references right channel now
		//unsigned int numSamples = buffer.getNumSamples();

		if (prevSampleRate != this->getSampleRate())
		{
			prevSampleRate = this->getSampleRate();
			levelAnalysisManager.sampleRateChanged(prevSampleRate);
		}

		//don't process if all samples are 0 or if autogain button is off
		if (buffer.getMagnitude(0, buffer.getNumSamples()) > 0.0f && autoGainEnableParam->isOn())
			levelAnalysisManager.processSamples(&buffer, posInfo);

		
			// Calibrate gain param based on which value is target
			double peakToHit;
			int targetType = targetTypeParam->getValueAsInt();
			if (targetType == Peak)
			{
				peakToHit = levelAnalysisManager.getMaxChannelPeak();
			}
			else if (targetType == MaxRMS)
			{
				peakToHit = levelAnalysisManager.getMaxChannelRMS();
			}
			else if (targetType == AverageRMS)
			{
				peakToHit = levelAnalysisManager.getMaxCurrentRunningRMS();
			}
			else
			{
				peakToHit = levelAnalysisManager.getMaxChannelPeak();
				jassertfalse;
			}

			//double targParamGain = params->getDecibelParameter(targetGainParamID)->getValueInGain();
			

			//division in log equiv to subtract in base
			double gainValueToAdd = targetGainParam->getValueInGain() / peakToHit;

			if (!almostEqual(gainValueToAdd, gainParam->getValueInGain())) // gain value changed
			{
				gainParam->setValueFromGain(gainValueToAdd);
				//gainParam->setNeedsUIUpdate(true); // removed because done in setValueFromGain
			}

			//in gain, multiply in log equivalent to add in base
			buffer.applyGain(gainParam->getValueInGain());		
	}
}


//==============================================================================
/** Copy current plugin state to XML */
void ZenAutoTrimAudioProcessor::getStateInformation(MemoryBlock& destData)
{
	DBG("In ZenTrimAudioProcessor::getStateInformation(destData) ");	
	MemoryOutputStream stream(destData, false);
	params->state.writeToStream(stream);
}

/** Load plugin with settings from XML */
void ZenAutoTrimAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	//DBG("In ZenTrimAudioProcessor::setStateInformation(data, sizeInBytes) ");
	ValueTree tree = ValueTree::readFromData(data, sizeInBytes);
	if (tree.isValid()) 
	{
		params->state = tree;
	}
}

void ZenAutoTrimAudioProcessor::resetCalculation()
{
	levelAnalysisManager.resetCalculation();
}

//==============================================================================
const String ZenAutoTrimAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool ZenAutoTrimAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
	return false;
#endif
}

bool ZenAutoTrimAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
	return false;
#endif
}

double ZenAutoTrimAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int ZenAutoTrimAudioProcessor::getNumPrograms()
{
	return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs
}

int ZenAutoTrimAudioProcessor::getCurrentProgram()
{
	return 0;
}

void ZenAutoTrimAudioProcessor::setCurrentProgram(int index)
{
}

const String ZenAutoTrimAudioProcessor::getProgramName(int index)
{
	return String();
}

void ZenAutoTrimAudioProcessor::changeProgramName(int index, const String& newName)
{
}

//==============================================================================
void ZenAutoTrimAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
}

void ZenAutoTrimAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}


//==============================================================================
bool ZenAutoTrimAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ZenAutoTrimAudioProcessor::createEditor()
{
	return new ZenAutoTrimAudioProcessorEditor(*this);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new ZenAutoTrimAudioProcessor();
}
