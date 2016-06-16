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

#ifndef RMSMANAGER_H_INCLUDED
#define RMSMANAGER_H_INCLUDED

#include "JuceHeader.h"
#include <map>
#include <utility>
#include <boost/optional.hpp>
#include <boost/circular_buffer.hpp>
#include <atomic>
#include "zen_utils/utilities/ZenConstants.h"

typedef std::map<unsigned int, double> TimeRMSMap;
typedef std::pair<unsigned int, double> TimeRMSPair;

class LevelAnalysisManager
{
public:
	LevelAnalysisManager();
	~LevelAnalysisManager();

	void processSamples(AudioBuffer<float>* buffer, AudioPlayHead::CurrentPositionInfo inPosInfo);

	//double getLeftCurrentRms() const
	//{
	//	return leftCurrRMS;
	//}
	//
	//double getRightCurrentRms() const
	//{
	//	return rightCurrRMS;
	//}

	//double getLeftCurrentRmsInDB()
	//{
	//	return ((10 * log10(leftCurrRMS)) + decibelRMSCalibration);
	//}
	//
	//double getRightCurrentRmsInDB()
	//{
	//	return ((10 * log10(rightCurrRMS)) + decibelRMSCalibration);
	//}

	double getLeftMaxRms() const
	{
		return sqrt(leftMaxSamplesSquaredWindowFound / getMinOfTotalWindowSamples());
	}

	double getRightMaxRms() const
	{
		return sqrt(rightMaxSamplesSquaredWindowFound / getMinOfTotalWindowSamples());
	}

	//double getLeftMaxRmsInDB() const
	//{
	//	return (leftMaxSamplesSquaredWindowFound != 0) ?
	//		(10 * log10(leftMaxSamplesSquaredWindowFound / getMinOfTotalWindowSamples())) + decibelRMSCalibration : 0;
	//}
	//
	//double getRightMaxRmsInDB() const
	//{
	//	return (rightMaxSamplesSquaredWindowFound != 0) ?
	//		(10 * log10(rightMaxSamplesSquaredWindowFound / getMinOfTotalWindowSamples())) + decibelRMSCalibration : 0;
	//}

	const double getLeftPeak() const
	{
		return leftPeakSample;
	}

	const double getRightPeak() const
	{
		return rightPeakSample;
	}

	//double getLeftPeakInDB() const
	//{
	//	return (Decibels::gainToDecibels(leftPeakSample) + decibelRMSCalibration);
	//}
	//
	//double getRightPeakInDB() const
	//{
	//	return (Decibels::gainToDecibels(rightPeakSample) + decibelRMSCalibration);
	//}

	double getMaxChannelPeak() const
	{
		return std::max(getLeftPeak(), getRightPeak());
	}

	//double getMaxChannelPeakInDB() const
	//{
	//	// #TODO: chagne for performance reasons to only convert the max to decibels
	//	return std::max(getLeftPeakInDB(), getRightPeakInDB());
	//}

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

	//double getLeftCurrentRunningRmsInDB() const
	//{
	//	return (leftRunningSamplesSquaredSum != 0) ?
	//		(10 * log10(leftRunningSamplesSquaredSum / countTotalRunningSamples)) + decibelRMSCalibration : 0.0f;
	//}
	//
	//double getRightCurrentRunningRmsInDB() const
	//{
	//	return (rightRunningSamplesSquaredSum != 0) ?
	//		(10 * log10(rightRunningSamplesSquaredSum / countTotalRunningSamples)) + decibelRMSCalibration : 0.0f;
	//}

	// checks to see if total samples processed is bigger than a single window's worth, returns the smaller
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
		} else
			return boost::none;
	}

	double getDecibelRMSCalibration() const { return decibelRMSCalibration; }
	void setDecibelRMSCalibration(double inValue) { decibelRMSCalibration = inValue; }

	unsigned int getWindowSizeInMS() const { return windowSizeInMS; }
	void setWindowSizeInMS(TimeValue inValue) { windowSizeInMS = inValue; }

	unsigned long long getTotalSamplesProcessed() { return countTotalRunningSamples; }
private:
	//size of the averaging window. VU = 300, PPM = 10, etc
	TimeValue windowSizeInMS = 300;
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

	double leftSumSquares = 0;
	double rightSumSquares = 0;
	double leftCurrRMS = 0;
	double rightCurrRMS = 0;

	AudioPlayHead::CurrentPositionInfo posInfo;

	TimeRMSMap rmsMap;
	TimeRMSPair rmsPair;

	double samplesPerSecond = 0;
	unsigned int secondsOfAudioCalculated = 0;

	double decibelRMSCalibration = 0.0f;

	std::unique_ptr<boost::circular_buffer<double>> prevLeftBuf, prevRightBuf;

	//prevent processing while reset is happening -- used SpinLock instead of
	//CriticalSection because it's only contested during Reset button press
	SpinLock processLock;
};

#endif // RMSMANAGER_H_INCLUDED
