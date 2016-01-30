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


RMSManager::RMSManager()
{

}

RMSManager::~RMSManager()
{

}

void RMSManager::processSamples(const float* inSamplesL, const float* inSamplesR, const unsigned int numIncomingSamples, const unsigned int numChannels)
{
	for (unsigned int i = 0; i < numIncomingSamples; ++i) // Loops over all samples
	{
		if (inSamplesL[i] == 0.0f && inSamplesR[i] == 0.0f)
			continue; // don't process silence
		++numSamplesCalculated;
		leftRunningRMSSum += ((double)inSamplesL[i] * (double)inSamplesL[i]);
		rightRunningRMSSum += ((double)inSamplesR[i] * (double)inSamplesR[i]);
		++countTotalRunningSamples;
		/*float leftSample = inSamplesL[i];
		float rightSample = inSamplesR[i];
		*/

		leftPeakSample = (fabs(inSamplesL[i]) > leftPeakSample) ? fabs(inSamplesL[i]) : leftPeakSample;
		rightPeakSample = (fabs(inSamplesR[i]) > rightPeakSample) ? fabs(inSamplesR[i]) : rightPeakSample;

		if (numSamplesCalculated < windowSize) // We're still in window block
		{
			//sumOfSamples += sqrt((double)inSamplesL[i]* (double)inSamplesL[i] + (double)inSamplesR[i]* (double)inSamplesR[i]) ;
			//double avgSample = (inSamplesL[i] + inSamplesR[i]) / 2;
			//sumOfSamples += avgSample*avgSample;
			sumOfLeftSamples += ((double)inSamplesL[i] * (double)inSamplesL[i]);
			sumOfRightSamples += ((double)inSamplesR[i] * (double)inSamplesR[i]);
		


			
		}
		else
		{
			jassert(numSamplesCalculated <= windowSize);
			++numSamplesCalculated;
			double foundLeftRMS = (sumOfLeftSamples / windowSize);
			double foundRightRMS = (sumOfRightSamples / windowSize);
			double foundRightRMS2 = (sqrt(sumOfRightSamples) / windowSize);

			foundLeftRMS = sqrt(foundLeftRMS);
			foundRightRMS = sqrt(foundRightRMS);

			leftAvgRMSSummed += foundLeftRMS;
			rightAvgRMSSummed += foundRightRMS;
			++numRMSBatchesCollected;
			leftAvgRMS = leftAvgRMSSummed / numRMSBatchesCollected;
			rightAvgRMS = rightAvgRMSSummed / numRMSBatchesCollected;

			if (foundLeftRMS > leftMaxRMS)
			{
				leftMaxRMS = foundLeftRMS;
				//double rmsInDecibels = Decibels::gainToDecibels<double>(maxFoundRMS);
				//DBG("New max RMS found: " + String(rmsInDecibels));
			}

			if (foundRightRMS > rightMaxRMS)
			{
				rightMaxRMS = foundRightRMS;
			}

			numSamplesCalculated = 0;
			sumOfLeftSamples = 0;
			sumOfRightSamples = 0;
			//sumOfSamplesSqrd = 0;
		}
	}
}
