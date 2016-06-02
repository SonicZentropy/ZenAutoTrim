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
#include <memory>


//==============================================================================
ZenAutoTrimAudioProcessor::ZenAutoTrimAudioProcessor()
	: undoManager(),
	params(*this, &undoManager),
	currentEditor(nullptr),
	rmsWindowTime("RMS Window Time", 300)
{
	// #TODO: swap to audioprocessorvaluetreestate to save GUI component params by connecting

	params.createAndAddDecibelParameter(gainParam, "Gain", -96.0f, 18.0f, 0.0f);
	params.createAndAddDecibelParameter(targetGainParam, "TargetGain", -96.0f, 18.0f, 0.0f);
	params.createAndAddBoolParameter(autoGainParam, "AutoGain", false);
	params.createAndAddBoolParameter(bypassParam, "Bypass", false);

#ifdef JUCE_MSVC
	//Visual Studio mem leak diagnostics settings 
	_CrtSetDbgFlag(0); //Turn off VS memory dump output
	//_crtBreakAlloc = 5389;	//Break on this memory allocation number (When Debug)
#endif

#ifdef JUCE_DEBUG
	// #TODO: Change this to use Juce SharedResourcePointer - https://forum.juce.com/t/juce-singleton-implementation-confusion/17847/6
	//debugWindow = ZenDebugEditor::getInstance();
	//debugWindow->setSize(650, 400);
	//Open in bottom right corner
	//debugWindow->setTopLeftPosition(1900 - debugWindow->getWidth(), 1040 - debugWindow->getHeight());
	// #TODO: add JUCE REF COUNTED OBJECT to zen GUI
#endif
}

ZenAutoTrimAudioProcessor::~ZenAutoTrimAudioProcessor()
{
	debugWindow->deleteInstance();
}

void ZenAutoTrimAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	aPlayHead = getPlayHead();
	AudioPlayHead::CurrentPositionInfo posInfo;
	aPlayHead->getCurrentPosition(posInfo);
	//bool posFound = aPlayHead->getCurrentPosition(posInfo);

	//float* leftData = buffer.getWritePointer(0); //leftData references left channel now
	//float* rightData = buffer.getWritePointer(1); //right data references right channel now
	//unsigned int numSamples = buffer.getNumSamples();

	if (prevSampleRate != this->getSampleRate())
	{
		prevSampleRate = this->getSampleRate();
		levelAnalysisManager.sampleRateChanged(prevSampleRate);
	}

	
	if (buffer.getMagnitude(0, buffer.getNumSamples()) > 0.0f) 
		levelAnalysisManager.processSamples(&buffer, posInfo);

	if (params.getBoolParameter(autoGainParam)->isOn())
	{
		// Calibrate gain param based on which value is target
		double peakToHit = -1000;
		if (targetForAutoTrim == Peak)
		{
			peakToHit = levelAnalysisManager.getMaxChannelPeak();
		}
		else if (targetForAutoTrim == MaxRMS)
		{
			peakToHit = levelAnalysisManager.getMaxChannelRMS();
		}
		else if (targetForAutoTrim == AverageRMS)
		{
			peakToHit = levelAnalysisManager.getMaxCurrentRunningRMS();
		}
		else
		{
			jassertfalse;
		}

		double targParamGain = params.getDecibelParameter(targetGainParam)->getValueInGain();

		//division in log equiv to subtract in base
		double gainValueToAdd = targParamGain / peakToHit;

		//float maxPeakInDB = Decibels::gainToDecibels(levelAnalysisManager.getMaxChannelPeak());
		//double targParamGainInDB =  Decibels::gainToDecibels(targetParam->getValueInGain());
		//double gainValueToAddInDB = Decibels::gainToDecibels(targParamGain) - Decibels::gainToDecibels(maxPeak);
		//DBG("Max peak is: " << maxPeak);
		if (!almostEqual(gainValueToAdd, params.getDecibelParameter(gainParam)->getValueInGain()) && targParamGain > -900) // gain value changed
		{
			//DBG("\nGain To Add In DB: " << Decibels::gainToDecibels(maxPeak + gainValueToAdd) << " gainValueToAdd: " << gainValueToAdd << " != (gainParam)" << gainParam->getValueInGain());
			//DBG("Max peak is: " << maxPeak << " and gainValueToAdd value is (targParamGain)" << targParamGain << " - (maxPeak)" << maxPeak << " = (gainValueToAdd)" << gainValueToAdd);
			//DBG("Targ param gain in DB: " << Decibels::gainToDecibels(targParamGain) << " max peak in db: " << Decibels::gainToDecibels(maxPeak) << " gain to add in db: " << Decibels::gainToDecibels(abs(gainValueToAdd)));
			//DBG("MaxPeak in DB: " << maxPeakInDB << " and gainValueToAddInDB value is (targParamGainInDB)" << targParamGainInDB << " - (maxPeakInDB)" << maxPeakInDB << " = (gainValueToAddInDB)" << gainValueToAddInDB);
			//DBG("gainvaluetoaddindb to gain:" << Decibels::decibelsToGain(gainValueToAddInDB));
			//DBG("Setting gain to add to: " << gainValueToAdd << " or in DB: " << Decibels::gainToDecibels(gainValueToAdd));
			//gainParam->setValueFromDecibels(gainValueToAddInDB);
			params.getDecibelParameter(gainParam)->setValueFromGainNotifyingHost(gainValueToAdd);
		}

		params.getDecibelParameter(gainParam)->setNeedsUIUpdate(true);


		//buffer.applyGain(gainParam->getValueInGain());
		//THIS IS RIGHT in db
		//float gainToAdd = gainParam->getValueInDecibels();
		//for(unsigned int i = 0; i < numSamples; ++i)
		//{
		//	leftData[i] = Decibels::decibelsToGain(Decibels::gainToDecibels(leftData[i]) + gainToAdd);
		//	rightData[i] = Decibels::decibelsToGain( Decibels::gainToDecibels(rightData[i]) + gainToAdd);
		//}

		//in gain, multiply in log equivalent to add in base
		float gainToAdd = params.getDecibelParameter(gainParam)->getValueInGain();

		//for (unsigned int i = 0; i < numSamples; ++i)
		//{
		//	leftData[i] = leftData[i] * gainToAdd;
		//	rightData[i] = rightData[i] * gainToAdd;
		//}

		buffer.applyGain(gainToAdd);
	}
}


//==============================================================================
/** Copy current plugin state to XML */
void ZenAutoTrimAudioProcessor::getStateInformation(MemoryBlock& destData)
{
	DBG("In ZenTrimAudioProcessor::getStateInformation(destData) ");	
		XmlElement xml("ZENTRIMSETTINGS");
		
		for (int i = 0; i < getNumParameters(); ++i)
			if (AudioProcessorParameterWithID* p = dynamic_cast<AudioProcessorParameterWithID*>(getParameters().getUnchecked(i)))
			{
				DBG("Param: " << p->paramID);
				xml.setAttribute(p->paramID, p->getValue());
			}
				
		
		copyXmlToBinary(xml, destData);
}

/** Load plugin with settings from XML */
void ZenAutoTrimAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	DBG("In ZenTrimAudioProcessor::setStateInformation(data, sizeInBytes) ");
	std::unique_ptr<XmlElement> xmlState = std::unique_ptr<XmlElement>(getXmlFromBinary(data, sizeInBytes));
	if (xmlState != nullptr)
	{
		if (xmlState->hasTagName("ZENTRIMSETTINGS"))
		{
			for (int i = 0; i < getNumParameters(); ++i)
				if (AudioProcessorParameterWithID* p = dynamic_cast<AudioProcessorParameterWithID*> (getParameters().getUnchecked(i)))
				{
					p->setValueNotifyingHost((float)xmlState->getDoubleAttribute(p->paramID, p->getValue()));
					DBG("Setting " << p->name << " to " << (float)xmlState->getDoubleAttribute(p->paramID, p->getValue()));
				}
		}
	}
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

/*
bool ZenAutoTrimAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}*/

double ZenAutoTrimAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int ZenAutoTrimAudioProcessor::getNumPrograms()
{
	return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
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
