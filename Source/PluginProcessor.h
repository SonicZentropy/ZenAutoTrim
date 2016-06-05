/*==============================================================================
//  PluginProcessor.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2016/01/28
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Main DSP Processing class header
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/


#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "JuceHeader.h"
#include "debug/ZenDebugEditor.h"
#include "parameters/ZenBoolParameter.h"
#include "parameters/ZenIntParameter.h"
#include "processing/RMSManager.h"
#include "state/ZenAudioProcessorValueTreeState.h"
// #include "utilities/ZenConstants.h"
// #include "utilities/ZenTime.h"

enum CalibrationTarget
{
	AverageRMS = 1,
	MaxRMS,
	Peak,
	targetCount = 3
};

enum CalibrationTimeInMS
{
	time10ms = 1,
	time300ms,
	time1000ms,
	time5000ms,
	timeCount = 4
};

//==============================================================================
class ZenAutoTrimAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    ZenAutoTrimAudioProcessor();
    ~ZenAutoTrimAudioProcessor();
	
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;
    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	//==============================================================================
	void resetCalculation();

	//==============================================================================
	//void setCurrentEditor(AudioProcessorEditor* inEditor) { currentEditor = inEditor; }

	//CalibrationTarget getTargetForAutoTrim() const { return targetForAutoTrim; }
	//void setTargetForAutoTrim(CalibrationTarget inValue) { targetForAutoTrim = inValue; }
	void setRMSWindowTimeInMS(int inCalibrationTime)
	{
		//rmsWindowTime.setValueFromMS(inTime);
		int timeInMS;
		switch(inCalibrationTime)
		{
		case time10ms:
			timeInMS = 10;
			break;
		case time300ms:
			timeInMS = 300;
			break;
		case time1000ms:
			timeInMS = 1000;
			break;
		case time5000ms:
			timeInMS = 5000;
			break;
		default:
			timeInMS = 300;
			jassertfalse;
		}

		levelAnalysisManager.setWindowSizeInMS(timeInMS);
		rmsWindowTimeParam->setValue((int)timeInMS);
	}

	bool isBypassed() { return bypassParam->isOn(); }

	bool isEnabled() { return bypassParam->isOff(); }

	void setEnabled(bool shouldBeEnabled) { bypassParam->setValueFromBool(shouldBeEnabled); }

	LevelAnalysisManager& getLevelAnalysisManager() { return levelAnalysisManager; }	
	ZenDecibelParameter* getGainParam() { return gainParam; }
	ZenDecibelParameter* getTargetParam() { return targetGainParam; }
	ZenBoolParameter* getAutoGainEnableParam() { return autoGainEnableParam; }
	ZenBoolParameter* getBypassParam() { return bypassParam; }
	ZenIntParameter* getTargetTypeParam() { return targetTypeParam; }
	ZenIntParameter* getRMSWindowTimeParam() { return rmsWindowTimeParam; }

	//==============================================================================

	ScopedPointer<ZenAudioProcessorValueTreeState> params;
	ScopedPointer<UndoManager> undoManager;

	const String gainParamID = "gainParam";
	const String targetGainParamID = "targetGainParam";
	const String autoGainEnableParamID = "autoGainParam";
	const String bypassParamID = "bypassParam";
	const String targetTypeParamID = "targetTypeParam";
	const String rmsWindowTimeParamID = "rmsWindowTimeParam";

private:
	//friend class ZenAutoTrimAudioProcessorEditor;
	ZenDecibelParameter* gainParam; 
	ZenDecibelParameter* targetGainParam;
	ZenBoolParameter* autoGainEnableParam;
	ZenBoolParameter* bypassParam;
	ZenIntParameter*  targetTypeParam;
	ZenIntParameter*  rmsWindowTimeParam;
	
	//AudioProcessorEditor* currentEditor;
	LevelAnalysisManager levelAnalysisManager;
	AudioPlayHead* aPlayHead;
	
	//ZenTime rmsWindowTime;	
	//CalibrationTarget targetForAutoTrim = Peak;

	double prevSampleRate = 44100;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ZenAutoTrimAudioProcessor)
		ZenDebugEditor* debugWindow;
};


#endif  // PLUGINPROCESSOR_H_INCLUDED

// #TODO: fix process block speed by offloading autogain calc

// #TODO: autotrim to peak sample or max rms
// #TODO: rms window length
// #TODO: rms calibration(+3 or 0)
