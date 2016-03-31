/*==============================================================================
//  LevelAnalysisManager.h
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
#include "boost/circular_buffer.hpp"
#include "boost/optional.hpp"
#include <map>


namespace Zen
{

typedef std::map<unsigned int, double> TimeRMSMap;
typedef std::pair<unsigned int, double> TimeRMSPair;

class LevelAnalysisManager
{
public:
	LevelAnalysisManager();
	~LevelAnalysisManager();

	void processSamples(const AudioBuffer<float>* buffer, AudioPlayHead::CurrentPositionInfo inPosInfo);

	double getLeftMaxRms() const
	{
		return sqrt(leftMaxSamplesSquaredWindowFound / getMinOfTotalWindowSamples());
	}

	double getRightMaxRms() const
	{
		return sqrt(rightMaxSamplesSquaredWindowFound / getMinOfTotalWindowSamples());
	}

	double getLeftMaxRmsInDB() const
	{
		return (leftMaxSamplesSquaredWindowFound != 0) ? 
			(10*log10(leftMaxSamplesSquaredWindowFound / getMinOfTotalWindowSamples())) + decibelRMSCalibration : 0;
	}

	double getRightMaxRmsInDB() const
	{
		return (rightMaxSamplesSquaredWindowFound != 0) ? 
			(10*log10(rightMaxSamplesSquaredWindowFound / getMinOfTotalWindowSamples())) + decibelRMSCalibration : 0;
	}

	const double getLeftPeak() const
	{
		return leftPeakSample;
	}

	const double getRightPeak() const
	{
		return rightPeakSample;
	}

	double getLeftPeakInDB() const
	{
		return (Decibels::gainToDecibels(leftPeakSample) + decibelRMSCalibration);
	}

	double getRightPeakInDB() const
	{
		return (Decibels::gainToDecibels(rightPeakSample) + decibelRMSCalibration);
	}

	double getMaxChannelPeak() const
	{
		return std::max(getLeftPeak(), getRightPeak());
	}

	double getLeftCurrentRunningRms() const
	{
		return sqrt(leftRunningSamplesSquaredSum / countTotalRunningSamples);
	}

	double getRightCurrentRunningRms() const
	{
		return sqrt(rightRunningSamplesSquaredSum / countTotalRunningSamples);
	}

	double getMaxCurrentRunningRMS() const
	{
		return std::max(getLeftCurrentRunningRms(), getRightCurrentRunningRms());
	}

	double getLeftCurrentRunningRmsInDB() const
	{
		return (leftRunningSamplesSquaredSum != 0) ? 
			(10*log10(leftRunningSamplesSquaredSum / countTotalRunningSamples)) +decibelRMSCalibration : 0.0f;
	}

	double getRightCurrentRunningRmsInDB() const
	{
		return (rightRunningSamplesSquaredSum != 0) ? 
			(10*log10(rightRunningSamplesSquaredSum / countTotalRunningSamples)) +decibelRMSCalibration : 0.0f;
	}	

	double getMinOfTotalWindowSamples() const
	{
		return (numWindowSamplesCalculated < samplesPerWindow) ? numWindowSamplesCalculated : samplesPerWindow;
	}

	double getMaxChannelRMS() const
	{
		return std::max(getLeftMaxRms(), getRightMaxRms());
	}

	void sampleRateChanged(const double& newSampleRate);

	void resetCalculation();	

	TimeRMSMap getRMSMap() const
	{
		return rmsMap;
	}

	boost::optional<TimeRMSPair> getMostRecentRMSWindow() const
	{		
		if (!rmsMap.empty())
		{
			unsigned int rmsTime = rmsMap.rbegin()->first;
			double rmsValue = rmsMap.rbegin()->second;
			return TimeRMSPair(rmsTime, rmsValue);
		}
		else
			return boost::none;
	}

	double getDecibelRMSCalibration() const { return decibelRMSCalibration; }
	void setDecibelRMSCalibration(double inValue) { decibelRMSCalibration = inValue; }

private:
	//size of the averaging window. VU = 300, PPM = 10, etc
	unsigned int windowSizeInMS = 300;
	unsigned int sampleRate = 44100;
	unsigned int samplesPerWindow = 0;

	unsigned int numWindowSamplesCalculated = 0;
	unsigned long long countTotalRunningSamples = 0;

	double leftMaxSamplesSquaredWindowFound = 0;
	double rightMaxSamplesSquaredWindowFound = 0;

	double leftPeakSample = -1000;
	double rightPeakSample = -1000;
	
	double leftRunningSamplesSquaredSum = 0;
	double rightRunningSamplesSquaredSum = 0;

	double leftSumSquares = 0, rightSumSquares = 0;
	double leftCurrRMS = 0, rightCurrRMS = 0;
	
	//TODO: Use posInfo for time tracking
	AudioPlayHead::CurrentPositionInfo posInfo;

	TimeRMSMap rmsMap;
	TimeRMSPair rmsPair;
	
	double samplesPerSecond = 0;
	unsigned int secondsOfAudioCalculated = 0;

	// #TODO: add menu option to change RMS calibration level
	double decibelRMSCalibration = 3.0f;

	std::unique_ptr<boost::circular_buffer<double>> prevLeftBuf, prevRightBuf;

};
} // Namespace Zen

#endif // RMSMANAGER_H_INCLUDED