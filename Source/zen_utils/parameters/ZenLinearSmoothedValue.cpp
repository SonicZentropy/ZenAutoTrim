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

#include "ZenLinearSmoothedValue.h"
#include "zen_utils/utilities/ZenUtils.hpp"

void ZenLinearSmoothedValue::setTargetValue(float newValue) noexcept
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

const float ZenLinearSmoothedValue::getNextSmoothedValue() noexcept
{
	if (countdown <= 0) return targetValue;

	--countdown;
	currentSmoothedValue += step;
	return currentSmoothedValue;
}

void ZenLinearSmoothedValue::resetSmoothedValue(float inSampleRate)
{
	jassert(inSampleRate > 0 && smoothingTime >= 0);
	//divide smoothing time by 1000 to convert to milliseconds
	stepsToTargetValue = static_cast<int>(floor((smoothingTime / 1000) * inSampleRate));
	currentSmoothedValue = targetValue;
	countdown = 0;
}

void ZenLinearSmoothedValue::resetSmoothedValue(float inSampleRate, float inSmoothingTime)
{
	jassert(inSmoothingTime > 0);
	smoothingTime = inSmoothingTime;
	resetSmoothedValue(inSampleRate);
}
