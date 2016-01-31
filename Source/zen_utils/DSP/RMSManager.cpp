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

RMSManager::RMSManager()
{
	samplesPerWindow = sampleRate*(windowSizeInMS/1000.0f);
	prevLeftBuf = std::make_unique<boost::circular_buffer<double>>(samplesPerWindow);
	prevRightBuf = std::make_unique<boost::circular_buffer<double>>(samplesPerWindow);

	for (int i = 0; i < samplesPerWindow; ++i)
	{
		//fill both circular rms buffers with 0 to start
		prevLeftBuf->push_back(0.0f);
		prevRightBuf->push_back(0.0f);
	}
}

RMSManager::~RMSManager()
{

}

void RMSManager::processSamples(const float* inSamplesL, const float* inSamplesR, const unsigned int numIncomingSamples, const unsigned int numChannels)
{
	DBG("Incoming samples: " + String(numIncomingSamples) + " numSampCalc: " + String(numSamplesCalculated) + " TotalSamp: " + String(countTotalRunningSamples));
	for (unsigned int i = 0; i < numIncomingSamples; ++i) // Loops over all samples
	{
		++numSamplesCalculated;
		leftRunningRMSSum += ((double)inSamplesL[i] * (double)inSamplesL[i]);
		rightRunningRMSSum += ((double)inSamplesR[i] * (double)inSamplesR[i]);
		++countTotalRunningSamples;
		double leftSampleSquared = inSamplesL[i]*inSamplesL[i];
		double rightSampleSquared = inSamplesR[i]*inSamplesR[i];
		

		// circular buffer area
		double leftSquaredSubBuffer = prevLeftBuf->front();
		double rightSquaredSubBuffer = prevRightBuf->front();
		leftSumSquares -= leftSquaredSubBuffer;
		rightSumSquares -= rightSquaredSubBuffer;
		leftSumSquares += leftSampleSquared;
		rightSumSquares += rightSampleSquared;

		prevLeftBuf->push_back(leftSampleSquared);
		prevRightBuf->push_back(rightSampleSquared);

		leftCurrRMS = sqrt(leftSumSquares / samplesPerWindow);
		rightCurrRMS = sqrt(rightSumSquares / samplesPerWindow);

		if (leftMaxRMS < leftCurrRMS) leftMaxRMS = leftCurrRMS;
		if (rightMaxRMS < rightCurrRMS) rightMaxRMS = rightCurrRMS;

		leftPeakSample = (fabs(inSamplesL[i]) > leftPeakSample) ? fabs(inSamplesL[i]) : leftPeakSample;
		rightPeakSample = (fabs(inSamplesR[i]) > rightPeakSample) ? fabs(inSamplesR[i]) : rightPeakSample;

		if (numSamplesCalculated < samplesPerWindow) // We're still in window block
		{			
			sumOfLeftSamples += ((double)inSamplesL[i] * (double)inSamplesL[i]);
			sumOfRightSamples += ((double)inSamplesR[i] * (double)inSamplesR[i]);

		} else
		{
			jassert(numSamplesCalculated == samplesPerWindow);

			double foundLeftRMS = (sumOfLeftSamples / samplesPerWindow);
			double foundRightRMS = (sumOfRightSamples / samplesPerWindow);

			foundLeftRMS = sqrt(foundLeftRMS);
			foundRightRMS = sqrt(foundRightRMS);

			leftAvgRMSSummed += foundLeftRMS;
			rightAvgRMSSummed += foundRightRMS;
			++numRMSBatchesCollected;
			leftAvgRMS = leftAvgRMSSummed / numRMSBatchesCollected;
			rightAvgRMS = rightAvgRMSSummed / numRMSBatchesCollected;

			if (foundLeftRMS > leftMaxRMS)
			{
				//leftMaxRMS = foundLeftRMS;
				//double rmsInDecibels = Decibels::gainToDecibels<double>(maxFoundRMS);
				//DBG("New max RMS found: " + String(rmsInDecibels));
			}

			if (foundRightRMS > rightMaxRMS)
			{
			//	rightMaxRMS = foundRightRMS;
			}

			numSamplesCalculated = 0;
			sumOfLeftSamples = 0;
			sumOfRightSamples = 0;
			//sumOfSamplesSqrd = 0;
		}
	}
}
} //Namespace Zen