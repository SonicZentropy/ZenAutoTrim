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
#include <JuceHeader.h>

RMSManager::RMSManager()
{

}

RMSManager::~RMSManager()
{

}

void RMSManager::processSamples(const float* inSamplesL, const float* inSamplesR, const unsigned int numIncomingSamples, const unsigned int numChannels)
{
	for (unsigned int i = 0; i < numIncomingSamples; ++i)
	{
		++numSamplesCalculated;
		if (inSamplesL[i] <= 0.00001 && inSamplesR[i] <= 0.00001)
			continue; // don't process silence
		/*float leftSample = inSamplesL[i];
		float rightSample = inSamplesR[i];
		*/

		if (numSamplesCalculated < windowSize) // start new window
		{
			//sumOfSamples += sqrt((double)inSamplesL[i]* (double)inSamplesL[i] + (double)inSamplesR[i]* (double)inSamplesR[i]) ;
			//double avgSample = (inSamplesL[i] + inSamplesR[i]) / 2;
			//sumOfSamples += avgSample*avgSample;
			sumOfSamples += ((double)inSamplesL[i] * (double)inSamplesL[i]) + ((double)inSamplesR[i] * (double)inSamplesR[i]);
			
			
		}
		else
		{
			jassert(numSamplesCalculated <= windowSize);
			++numSamplesCalculated;
			double foundRMS = (sumOfSamples / windowSize);
			foundRMS = sqrt(foundRMS);
			collectedRMS += foundRMS;
			++numRMSBatchesCollected;
			avgTotalRMS = collectedRMS / numRMSBatchesCollected;

			if (foundRMS > maxFoundRMS)
			{
				maxFoundRMS = foundRMS;
				//double rmsInDecibels = Decibels::gainToDecibels<double>(maxFoundRMS);
				//DBG("Ne max RMS found: " + String(rmsInDecibels));
			}
			numSamplesCalculated = 0;
			sumOfSamples = 0;
			sumOfSamplesSqrd = 0;
		}
	}
}
