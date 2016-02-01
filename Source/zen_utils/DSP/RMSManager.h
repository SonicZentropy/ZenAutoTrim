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
#include <boost/circular_buffer.hpp>


namespace Zen
{

class LevelAnalysisManager
{
public:
	LevelAnalysisManager();
	~LevelAnalysisManager();

	void processSamples(const AudioBuffer<float>* buffer);

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
			10*log10(leftMaxSamplesSquaredWindowFound / getMinOfTotalWindowSamples()) : 0;
	}

	double getRightMaxRmsInDB() const
	{
		return (rightMaxSamplesSquaredWindowFound != 0) ? 
			10*log10(rightMaxSamplesSquaredWindowFound / getMinOfTotalWindowSamples()) : 0;
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
		return Decibels::gainToDecibels(leftPeakSample);
	}

	double getRightPeakInDB() const
	{
		return Decibels::gainToDecibels(rightPeakSample);
	}

	double getLeftCurrentRunningRms() const
	{
		return sqrt(leftRunningSamplesSquaredSum / countTotalRunningSamples);
	}

	double getRightCurrentRunningRms() const
	{
		return sqrt(rightRunningSamplesSquaredSum / countTotalRunningSamples);
	}

	double getLeftCurrentRunningRmsInDB() const
	{
		return (leftRunningSamplesSquaredSum != 0) ? 
			10*log10(leftRunningSamplesSquaredSum / countTotalRunningSamples) : 0.0f;
	}

	double getRightCurrentRunningRmsInDB() const
	{
		return (rightRunningSamplesSquaredSum != 0) ? 
			10*log10(rightRunningSamplesSquaredSum / countTotalRunningSamples) : 0.0f;
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

private:
	//size of the averaging window. VU = 300, PPM = 10, etc
	unsigned int windowSizeInMS = 300;
	unsigned int sampleRate = 44100;
	unsigned int samplesPerWindow = 0;

	unsigned int numWindowSamplesCalculated = 0;
	unsigned long long countTotalRunningSamples = 0;

	double leftMaxSamplesSquaredWindowFound = 0;
	double rightMaxSamplesSquaredWindowFound = 0;

	double leftPeakSample = 0;
	double rightPeakSample = 0;
	
	double leftRunningSamplesSquaredSum = 0;
	double rightRunningSamplesSquaredSum = 0;

	double leftSumSquares = 0, rightSumSquares = 0;
	double leftCurrRMS = 0, rightCurrRMS = 0;

	std::unique_ptr<boost::circular_buffer<double>> prevLeftBuf, prevRightBuf;

};
} // Namespace Zen

#endif // RMSMANAGER_H_INCLUDED