/*==============================================================================
//  RMSManager.cpp
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

//#include "ipp.h"
#include "RMSManager.h"
#include <memory>

LevelAnalysisManager::LevelAnalysisManager()
{
	secondsOfAudioCalculated = 0;
	samplesPerWindow = sampleRate*(windowSizeInMS / 1000.0f);
	prevLeftBuf = std::make_unique<boost::circular_buffer<double>>(samplesPerWindow);
	prevRightBuf = std::make_unique<boost::circular_buffer<double>>(samplesPerWindow);
}

LevelAnalysisManager::~LevelAnalysisManager()
{
}

void LevelAnalysisManager::processSamples(AudioBuffer<float>* buffer, AudioPlayHead::CurrentPositionInfo inPosInfo)
{
	const GenericScopedLock<SpinLock> sl(processLock);
	unsigned int _numIncomingSamples = buffer->getNumSamples();
	const float* inSamplesL = buffer->getReadPointer(0);
	const float* inSamplesR = buffer->getReadPointer(1);
	//float* inSampTest = buffer->getWritePointer(0);
	//double getLRMS = buffer->getRMSLevel(0, 0, _numIncomingSamples);
	//double getRRMS = buffer->getRMSLevel(1, 0, _numIncomingSamples);

	//Update overall peak sample found
	//float leftMag = buffer->getMagnitude(0, 0, _numIncomingSamples);
	//float rightMag = buffer->getMagnitude(1, 0, _numIncomingSamples);
	//leftPeakSample = (leftMag > leftPeakSample) ? leftMag : leftPeakSample;
	//rightPeakSample = (rightMag > rightPeakSample) ? rightMag : rightPeakSample;
	leftPeakSample = jmax((double)leftPeakSample, (double)buffer->getMagnitude(0, 0, _numIncomingSamples));
	rightPeakSample = jmax((double)rightPeakSample, (double)buffer->getMagnitude(1, 0, _numIncomingSamples));

	//Find JUCE's reported RMS for the sample block
	//leftCurrRMS = buffer->getRMSLevel(0, 0, _numIncomingSamples);
	//rightCurrRMS = buffer->getRMSLevel(1, 0, _numIncomingSamples);

	for (unsigned int i = 0; i < _numIncomingSamples; ++i) // Loops over all samples
	{
		++countTotalRunningSamples; // total count of computed samples
		++numWindowSamplesCalculated;

		//Ipp64f* test = inSampTest;
				
		double _leftSampleSquared = inSamplesL[i] * inSamplesL[i];		
		double _rightSampleSquared = inSamplesR[i] * inSamplesR[i];
	
		//ippsSqr_64f_A53(&_leftSampleSquared, inSampTest[i], 1);

		leftRunningSamplesSquaredSum += _leftSampleSquared;
		rightRunningSamplesSquaredSum += _rightSampleSquared;

		// circular buffer area
		if (numWindowSamplesCalculated >= samplesPerWindow)  //We've calculated at least a window worth of samples
		{
			double _leftSquaredSubBuffer = prevLeftBuf->front();
			double _rightSquaredSubBuffer = prevRightBuf->front();

			leftSumSquares -= _leftSquaredSubBuffer;
			rightSumSquares -= _rightSquaredSubBuffer; // subtract the sample that falls out of RMS window
		}

		leftSumSquares += _leftSampleSquared;      // add new sample to the RMS window 
		rightSumSquares += _rightSampleSquared;
		prevLeftBuf->push_back(_leftSampleSquared);
		prevRightBuf->push_back(_rightSampleSquared);

		// Calc current RMS, can Add 3db to meet AES17-2015 specification, so calibration sine wave peaks at 0
		//leftCurrRMS = sqrt(leftSumSquares / getMinOfTotalWindowSamples());// +decibelRMSCalibration;
		//rightCurrRMS = sqrt(rightSumSquares / getMinOfTotalWindowSamples());// +decibelRMSCalibration;		

		//Check for max RMS window found so far
		if (leftSumSquares > leftMaxSamplesSquaredWindowFound) leftMaxSamplesSquaredWindowFound = leftSumSquares;
		if (rightSumSquares > rightMaxSamplesSquaredWindowFound) rightMaxSamplesSquaredWindowFound = rightSumSquares;

		#ifdef JUCE_DEBUG	
		if (countTotalRunningSamples % sampleRate == 0) // happens once per second
		{
			++secondsOfAudioCalculated;
			double Xrms = 10 * log10((leftSumSquares + rightSumSquares) / (2 * samplesPerWindow));

			Xrms += decibelRMSCalibration;

			rmsPair.first = inPosInfo.timeInSeconds + 1;
			rmsPair.second = Xrms;

			rmsMap.insert_or_assign(rmsPair.first, rmsPair.second);
		}
		#endif // JUCE_DEBUG
	}
}

void LevelAnalysisManager::sampleRateChanged(const double& newSampleRate)
{
	sampleRate = newSampleRate;
	samplesPerWindow = sampleRate * (windowSizeInMS / 1000.0f);
	// don't add any new elements to the buffer, just change size
	// if the size is reduced, the last samples are truncated so the
	// measurement is no longer accurate (Intended)
	prevLeftBuf->rset_capacity(samplesPerWindow);
	prevRightBuf->rset_capacity(samplesPerWindow);
}

void LevelAnalysisManager::resetCalculation()
{
	numWindowSamplesCalculated = 0;
	countTotalRunningSamples = 0;

	leftMaxSamplesSquaredWindowFound = 0;
	rightMaxSamplesSquaredWindowFound = 0;

	leftPeakSample = -1000;
	rightPeakSample = -1000;

	leftRunningSamplesSquaredSum = 0;
	rightRunningSamplesSquaredSum = 0;
	
	leftSumSquares = 0;
	rightSumSquares = 0;
	leftCurrRMS = 0;
	rightCurrRMS = 0;

	secondsOfAudioCalculated = 0;

	const GenericScopedLock<SpinLock> sl(processLock);
	rmsMap.clear();
	prevLeftBuf->clear();
	prevRightBuf->clear();
}
