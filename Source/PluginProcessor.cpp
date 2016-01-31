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
#include "juce_audio_basics\effects\juce_Decibels.h"
#include <memory>


//==============================================================================
ZenAutoTrimAudioProcessor::ZenAutoTrimAudioProcessor()
	:currentEditor(nullptr)
{
	addParameter(gainParam = new AudioParameterFloat("gainParam", "Trim", -96.0f, 18.0f, 0.0f));
}

ZenAutoTrimAudioProcessor::~ZenAutoTrimAudioProcessor()
{
}

void ZenAutoTrimAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	if (gainParam->get() != 0.0f)
	{
		float gain = Decibels::decibelsToGain<float>(*gainParam);
		buffer.applyGain(gain);
		
	}

	if (currentEditor != nullptr)
	{
		dynamic_cast<ZenAutoTrimAudioProcessorEditor*>(currentEditor)->vuMeter->copySamples(buffer.getReadPointer(0), buffer.getNumSamples());
	}
	if (buffer.getMagnitude(0, buffer.getNumSamples()) > 0.0f)
		rmsManager.processSamples(buffer.getReadPointer(0), buffer.getReadPointer(1), buffer.getNumSamples());
}


//==============================================================================
void ZenAutoTrimAudioProcessor::getStateInformation(MemoryBlock& destData)
{
	DBG("In ZenTrimAudioProcessor::getStateInformation(destData) ");
	XmlElement xml("ZENTRIMSETTINGS");
	
	for (int i = 0; i < getNumParameters(); ++i)
		if (AudioProcessorParameterWithID* p = dynamic_cast<AudioProcessorParameterWithID*>(getParameters().getUnchecked(i)))
			xml.setAttribute(p->paramID, p->getValue());
	
	copyXmlToBinary(xml, destData);
}

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
					p->setValueNotifyingHost((float)xmlState->getDoubleAttribute(p->paramID, p->getValue()));
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

bool ZenAutoTrimAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double ZenAutoTrimAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ZenAutoTrimAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ZenAutoTrimAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ZenAutoTrimAudioProcessor::setCurrentProgram (int index)
{
}

const String ZenAutoTrimAudioProcessor::getProgramName (int index)
{
    return String();
}

void ZenAutoTrimAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void ZenAutoTrimAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
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
