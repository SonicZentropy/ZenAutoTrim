/*==============================================================================
//  RMSManager.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2016/01/29
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: RMS Calculation/Management Class
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#pragma once
#ifndef RMSMANAGER_H_INCLUDED
#define RMSMANAGER_H_INCLUDED

#include <JuceHeader.h>

class RMSManager
{
public:
	RMSManager();
	~RMSManager();

	void processSamples(const float* inSamplesL, const float* inSamplesR, const unsigned int numIncomingSamples, const unsigned int numChannels = 2);

	const double getLeftAvgRms() const
	{
		return leftAvgRMS;
	}

	const double getRightAvgRms() const
	{
		return rightAvgRMS;
	}

	const double getLeftMaxRms() const
	{
		return leftMaxRMS;
	}

	const double getRightMaxRms() const
	{
		return rightMaxRMS;
	}

	const double getLeftPeak() const
	{
		return leftPeakSample;
	}

	const double getRightPeak() const
	{
		return rightPeakSample;
	}

	const double getLeftRunningRms() const
	{
		return leftRunningRMS;
	}

	const double getRightRunningRms() const
	{
		return rightRunningRMS;
	}

	const double getLeftCurrentRunningRms() const
	{
		return sqrt(leftRunningRMSSum / countTotalRunningSamples);
	}

	const double getRightCurrentRunningRms() const
	{
		return sqrt(rightRunningRMSSum / countTotalRunningSamples);
	}


private:
	//size of the averaging window. VU = 300, PPM = 10, etc
	unsigned int windowSizeInMS = 300; 
	unsigned int sampleRate = 44100;
	unsigned int samplesPerWindow = 0;
	unsigned int numSamplesCalculated = 0;
	double sumOfLeftSamples = 0;
	double sumOfRightSamples = 0;
	double sumOfLeftSamplesSqrd = 0;
	double sumOfRightSamplesSqrd = 0;
	double leftMaxRMS = 0;
	double leftAvgRMSSummed = 0;
	double leftAvgRMS = 0;
	double leftPeakSample = 0;
	double rightMaxRMS = 0;
	double rightAvgRMSSummed = 0;
	double rightAvgRMS = 0;
	double rightPeakSample = 0;
	double numRMSBatchesCollected = 0;

	double leftRunningRMS = 0, leftRunningRMSSum = 0;
	double rightRunningRMS = 0, rightRunningRMSSum = 0;
	unsigned int countTotalRunningSamples = 0;

	double prevLeftBuffer[256] = {0};
	double prevRightBuffer[256] = {0};
//#Todo: Implement prevBuffer for running RMS sum


	

};

#endif // RMSMANAGER_H_INCLUDED