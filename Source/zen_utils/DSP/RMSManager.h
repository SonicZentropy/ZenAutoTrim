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

class RMSManager
{
public:
	RMSManager();
	~RMSManager();

	void processSamples(const float* inSamplesL, const float* inSamplesR, const unsigned int numIncomingSamples, const unsigned int numChannels = 2);

	double returnCurrentMax() const
	{
		return maxFoundRMS;
	}

	const double getAverageTotalRMS() const { return avgTotalRMS; }
	const double getMaxFoundRMS() const { return maxFoundRMS; }


	const double& getLeftAvgRms() const
	{
		return leftAvgRMS;
	}

	const double& getRightAvgRms() const
	{
		return rightAvgRMS;
	}

	const double& getLeftMaxRms() const
	{
		return leftMaxRMS;
	}

	const double& getRightMaxRms() const
	{
		return rightMaxRMS;
	}

	const double& getLeftPeak() const
	{
		return leftPeak;
	}

	const double& getRightPeak() const
	{
		return rightPeak;
	}

private:
	//size of the averaging window. VU = 300, PPM = 10, etc
	unsigned int windowSize = 300; 
	unsigned int numSamplesCalculated = 0;
	double sumOfSamples = 0;
	double sumOfSamplesSqrd = 0;
	double maxFoundRMS = 0;
	double collectedRMS = 0;
	double numRMSBatchesCollected = 0;
	double avgTotalRMS = 0;

	double leftAvgRMS = 0, rightAvgRMS = 0, leftMaxRMS = 0, rightMaxRMS = 0, leftPeak = 0, rightPeak = 0;

public:

};