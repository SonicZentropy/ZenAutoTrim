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
#include "ZenHeader.h"
//using namespace Zen; test

//==============================================================================
class ZenAutoTrimAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    ZenAutoTrimAudioProcessor();
    ~ZenAutoTrimAudioProcessor();

	//==============================================================================
	enum CalibrationTarget
	{
		AverageRMS,
		MaxRMS,
		Peak
	};
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

	void setCurrentEditor(AudioProcessorEditor* inEditor) { currentEditor = inEditor; }

	CalibrationTarget getTargetForAutoTrim() const { return targetForAutoTrim; }
	void setTargetForAutoTrim(CalibrationTarget inValue) { targetForAutoTrim = inValue; }
	void setRMSWindowTimeInMS(TimeValue inTime)
	{
		rmsWindowTime.setValueFromMS(inTime);
		levelAnalysisManager.setWindowSizeInMS(inTime);
	}

	bool isBypassed() const	{ return bypassParam->isOn(); }

	bool isEnabled() const { return bypassParam->isOff(); }	

	LevelAnalysisManager& getLevelAnalysisManager() { return levelAnalysisManager; }	
	ZenDecibelParameter* getGainParam() { return gainParam; }
	ZenBoolParameter* getAutoGainEnableParam() { return autoGainEnableParam; }
	ZenDecibelParameter* getTargetParam() { return targetParam; }
	ZenBoolParameter* getBypassParam() { return bypassParam; }

	//==============================================================================

	UndoManager undoManager;
	AudioProcessorValueTreeState apTree;

private:
	//friend class ZenAutoTrimAudioProcessorEditor;
	ZenDecibelParameter* gainParam; 
	ZenDecibelParameter* targetParam;
	ZenBoolParameter* autoGainEnableParam;
	ZenBoolParameter* bypassParam;
	AudioProcessorEditor* currentEditor;
	LevelAnalysisManager levelAnalysisManager;
	AudioPlayHead* aPlayHead;
	
	ZenTime rmsWindowTime;	
	CalibrationTarget targetForAutoTrim = Peak;

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
