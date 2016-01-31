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

#include "RMSManager.h"

namespace Zen
{

LevelAnalysisManager::LevelAnalysisManager()
{
	samplesPerWindow = sampleRate*(windowSizeInMS/1000.0f);
	prevLeftBuf = std::make_unique<boost::circular_buffer<double>>(samplesPerWindow);
	prevRightBuf = std::make_unique<boost::circular_buffer<double>>(samplesPerWindow);

	for (unsigned int i = 0; i < samplesPerWindow; ++i)
	{
		//fill both circular rms buffers with 0 to start
		prevLeftBuf->push_back(0.0f);
		prevRightBuf->push_back(0.0f);
	}
}

LevelAnalysisManager::~LevelAnalysisManager()
{
}

void LevelAnalysisManager::processSamples(const AudioBuffer<float>* buffer)
{	
	//#TODO: rms calc based only on # samples is wrong b/c it adds a full window worth of 0s

	unsigned int _numIncomingSamples = buffer->getNumSamples();
	const float* inSamplesL = buffer->getReadPointer(0);
	const float* inSamplesR = buffer->getReadPointer(1);

	float leftMag = buffer->getMagnitude(0, 0, _numIncomingSamples);
	float rightMag = buffer->getMagnitude(1, 0, _numIncomingSamples);

	leftPeakSample = (leftMag > leftPeakSample) ? leftMag : leftPeakSample;
	rightPeakSample = (rightMag > rightPeakSample) ? rightMag : rightPeakSample;

	DBG("Incoming samples: " + String(_numIncomingSamples) + " TotalSamp: " + String(countTotalRunningSamples));
	for (unsigned int i = 0; i < _numIncomingSamples; ++i) // Loops over all samples
	{		
		++countTotalRunningSamples; // total count of computed samples

		double _leftSampleSquared = inSamplesL[i] * inSamplesL[i];
		double _rightSampleSquared = inSamplesR[i] * inSamplesR[i];
		
		leftRunningSamplesSquaredSum += _leftSampleSquared;
		rightRunningSamplesSquaredSum += _rightSampleSquared;		

		// circular buffer area
		double _leftSquaredSubBuffer = prevLeftBuf->front();
		double _rightSquaredSubBuffer = prevRightBuf->front();

		leftSumSquares -= _leftSquaredSubBuffer;
		rightSumSquares -= _rightSquaredSubBuffer; // remove the sample that falls out of RMS window
		leftSumSquares += _leftSampleSquared;      // add new sample to the RMS window 
		rightSumSquares += _rightSampleSquared;

		prevLeftBuf->push_back(_leftSampleSquared);
		prevRightBuf->push_back(_rightSampleSquared);

		leftCurrRMS = sqrt(leftSumSquares / samplesPerWindow); // Calculate current RMS
		rightCurrRMS = sqrt(rightSumSquares / samplesPerWindow);

		
		if (leftSumSquares > leftMaxSamplesSquaredWindowFound) leftMaxSamplesSquaredWindowFound = leftSumSquares;
		if (rightSumSquares > rightMaxSamplesSquaredWindowFound) rightMaxSamplesSquaredWindowFound = rightSumSquares;
		
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
}

void LevelAnalysisManager::resetCalculation()
{
	numWindowSamplesCalculated = 0;
	countTotalRunningSamples = 0;

	leftMaxSamplesSquaredWindowFound = 0;
	rightMaxSamplesSquaredWindowFound = 0;

	leftPeakSample = 0;
	rightPeakSample = 0;

	leftRunningSamplesSquaredSum = 0;
	rightRunningSamplesSquaredSum = 0;
	leftSumSquares = 0;
	rightSumSquares = 0;
	leftCurrRMS = 0;
	rightCurrRMS = 0;	
}

	
} //Namespace Zen