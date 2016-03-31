/*==============================================================================
//  LinearSmoothedValue.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/15
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Smoothes value to prevent clicks/pops
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#pragma once
#ifndef LINEARSMOOTHEDVALUE_H_INCLUDED
#define LINEARSMOOTHEDVALUE_H_INCLUDED

#include "JuceHeader.h"
#include "../utilities/ZenUtils.hpp"

namespace Zen
{

/** To use, place code similar to the below in your audio processor's 
 ** prepareToPlay method

	// This Iterates over parameters and resets Smooth for the ones who need it
	for (auto param : getParameters())
	{
		ZenParameter* zenParam = dynamic_cast<ZenParameter*>(param);
		if (zenParam != nullptr)
		{
			if (zenParam->checkShouldBeSmoothed())
			{
				zenParam->resetSmoothedValue(inSampleRate);
			}
		}
	} 
*/
class ZenLinearSmoothedValue
{
public:
	ZenLinearSmoothedValue() noexcept
		: currentSmoothedValue(1.0), targetValue(1.0), step(0), smoothingTime(0), 
		countdown(0), stepsToTargetValue(0), shouldBeSmoothed(false)
	{	}

	ZenLinearSmoothedValue(bool inShouldBeSmoothed, float inSmoothingTime = 0.0f) noexcept
		: currentSmoothedValue(1.0), targetValue(1.0), step(0), smoothingTime(inSmoothingTime), 
		countdown(0), stepsToTargetValue(0), shouldBeSmoothed(inShouldBeSmoothed)
	{
	}

	/// <summary> Sets smoothed value. Should be called every time value is set/changed to a new value 
	/// (Inside setvalue method)</summary>
	/// <param name="newValue"> The new value that the smoothing is targeting toward (NORMALIZED). </param>
	void setTargetValue(float newValue) noexcept
	{
		if (!areAlmostEqual(targetValue, newValue))
		{
			targetValue = newValue;
			countdown = stepsToTargetValue;

			if (countdown <= 0)
				currentSmoothedValue = targetValue;
			else
				step = (targetValue - currentSmoothedValue) / static_cast<float>(countdown);
		}
	}

	/// <summary> Processes one sample worth of smoothing and returns the next (smoothed) value.
	/// Should be called ONCE on EVERY sample's process cycle.</summary>
	const float getNextSmoothedValue() noexcept
	{
		if (countdown <= 0) return targetValue;

		--countdown;
		currentSmoothedValue += step;
		return currentSmoothedValue;
	}

	/// <summary> Re-initializes the smoothed value parameter configuration.  Should be called from
	/// parameter constructors in parameters that allow smoothing.  Should also be called on any change
	/// of sample rate or desired smoothing time.  This can all be done within the processor's prepareToPlay()
	/// Smooth Time does not change.</summary>
	/// <param name="inSampleRate"> The in sample rate. </param>
	void resetSmoothedValue(float inSampleRate)
	{
		jassert(inSampleRate > 0 && smoothingTime >= 0);
		//divide smoothing time by 1000 to convert to milliseconds
		stepsToTargetValue = static_cast<int>(floor((smoothingTime/1000) * inSampleRate));
		currentSmoothedValue = targetValue;
		countdown = 0;
	}

	/// <summary> Re-initializes the smoothed value parameter configuration.  Should be called from
	/// parameter constructors in parameters that allow smoothing.  Should also be called on any change
	/// of sample rate or desired smoothing time.  This can all be done within the processor's prepareToPlay()</summary>
	/// <param name="inSampleRate"> The current sample rate. </param>
	/// <param name="inSmoothTime"> Time (in ms) to interpolate changes to this parameter </param>
	void resetSmoothedValue(float inSampleRate, float inSmoothingTime) 
	{
		jassert(inSmoothingTime > 0);
		smoothingTime = inSmoothingTime;		
		resetSmoothedValue(inSampleRate);
	}

	bool getShouldBeSmoothed() const { return shouldBeSmoothed; }
	bool checkShouldBeSmoothed() const { return shouldBeSmoothed; }

	void setShouldBeSmoothed(bool inValue) { shouldBeSmoothed = inValue; }

	float getSmoothingTime() const { return smoothingTime; }
	void setSmoothingTime(float inTime) { smoothingTime = inTime; }


protected:
	float currentSmoothedValue, targetValue, step, smoothingTime;
	int countdown, stepsToTargetValue;
	bool shouldBeSmoothed;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZenLinearSmoothedValue)
};
}

#endif // LINEARSMOOTHEDVALUE_H_INCLUDED


/// <summary> Sets smoothed value. Should be called every time value is set/changed to a new value 
/// (Inside setvalue method)</summary>
/*
/// <param name="newValue"> The new value that the smoothing is TARGETING. </param>
void setSmoothedValue(float newValue)
{
if (!areAlmostEqual(target, newValue))
{
target = newValue;
countdown = stepsToTarget;

if (countdown <= 0)
currentSmoothedValue = target;
else
step = (target - currentSmoothedValue) / static_cast<float>(countdown);
}
}

/// <summary> Processes one sample worth of smoothing and returns the next (smoothed) value.
/// Should be called ONCE on EVERY sample's process cycle.</summary>
float getNextSmoothedValue() noexcept
{
if (countdown <= 0)
return target;

--countdown;
currentSmoothedValue += step;
return currentSmoothedValue;
}

bool checkShouldBeSmoothed() const
{
//		DBG("In ZenParameter::checkShouldBeSmoothed() ");
return shouldBeSmoothed;
}

/// <summary> Re-initializes the smoothed value parameter configuration.  Should be called from
/// parameter constructors in parameters that allow smoothing.  Should also be called on any change
/// of sample rate or desired smoothing time.  This can all be done within the processor's prepareToPlay()</summary>
/// <param name="inSampleRate"> The current sample rate. </param>
/// <param name="inSmoothTime"> Time (in ms) to interpolate changes to this parameter </param>
void resetSmoothedValue(float inSampleRate, float inSmoothTime)
{
jassert(inSampleRate > 0 && inSmoothTime >= 0);
stepsToTarget = static_cast<int>(std::floor(inSmoothTime * inSampleRate));
currentSmoothedValue = target;
countdown = 0;
smoothTime = inSmoothTime;
}

/// <summary> Re-initializes the smoothed value parameter configuration.  Should be called from
/// parameter constructors in parameters that allow smoothing.  Should also be called on any change
/// of sample rate or desired smoothing time.  This can all be done within the processor's prepareToPlay()
/// Smooth Time does not change.</summary>
/// <param name="inSampleRate"> The in sample rate. </param>
void resetSmoothedValue(float inSampleRate)
{
jassert(inSampleRate > 0 && smoothTime >= 0);
stepsToTarget = static_cast<int>(std::floor(smoothTime * inSampleRate));
currentSmoothedValue = target;
countdown = 0;
}

*/