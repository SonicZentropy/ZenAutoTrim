/* ==============================================================================
//  BufferSampleProcesses.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/08
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details:   Details: All Static Methods for discrete sample processing live here
//	and are generally called from PluginEditor.cpp
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#include "BufferSampleProcesses.h"
#include "utilities/ZenConstants.h"

//static clock_t BufferSampleProcesses::inTime = clock();
//bool BufferSampleProcesses::shouldPrint = false;

void BufferSampleProcesses::processStereoWidth(float* leftSample, float* rightSample, 
	const float& widthIn)
{
	float width = widthIn * 2;
	float balancedCoeff = 1 / std::max(1 + widthIn, 2.0f);

	float sumGain = balancedCoeff;  ///sumGain is gain coefficient for Mid
	float diffGain = width * balancedCoeff;  ///diffGain is gain coefficient for Side 

	float mid = sumGain * (*leftSample + *rightSample);
	float side = diffGain * (*rightSample - *leftSample);
	*leftSample = mid - side;
	*rightSample = mid + side;
}

void BufferSampleProcesses::processGain(float* leftSample, float* rightSample, const float& gainValue)
{
	*leftSample *= gainValue;
	*rightSample *= gainValue;
}

void BufferSampleProcesses::processInvertLeftChannel(float* leftSample)
{
	*leftSample *= -1;
}

void BufferSampleProcesses::processInvertRightChannel(float* rightSample)
{
	*rightSample *= -1;
}

void BufferSampleProcesses::processPanning(float* leftSample, float* rightSample, const float& panRatio)
{
	//can be optimized later, leaving for clarity. 6db compensated linear
	//float leftRatio = 2.0f - (panRatio * 2);
	//float rightRatio = 0.0f + (panRatio * 2);
	//*leftSample *= leftRatio;
	//*rightSample *= rightRatio;

	//non-compensated
	//float leftRatio = 1.0f - (panRatio);
	//float rightRatio = 0.0f + (panRatio);

	//sqrt compensation - preserves perceived volume but pan is no longer linear
	//float leftRatio = sqrt(1.0f - panRatio);
	//float rightRatio = sqrt(panRatio);

	// #FUTURE: fix sinusoidal ratio to work properly in middle
	// also not linear but perceived as such
	float pan = panRatio * ZEN_PI_OVER_2F;
	*leftSample *= cos(pan);
	*rightSample *= sin(pan);
}
