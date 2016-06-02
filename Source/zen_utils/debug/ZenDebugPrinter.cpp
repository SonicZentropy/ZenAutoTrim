/* ==============================================================================
//  ZenDebugPrinter.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/08
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Static Utility Class for Debug-related code
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#include "ZenDebugPrinter.h"

clock_t ZenDebugPrinter::inTime = clock();

int ZenDebugPrinter::numSecondsBetweenDebugPrints = 1;

void ZenDebugPrinter::setSecondsBetweenDebugPrints(const unsigned int& inSeconds)
{
	numSecondsBetweenDebugPrints = inSeconds;
}

int ZenDebugPrinter::getSecondsBetweenDebugPrints()
{
	return numSecondsBetweenDebugPrints;
}

bool ZenDebugPrinter::isPrintTimerThresholdReached()
{
	if (((clock() - inTime) / CLOCKS_PER_SEC) > numSecondsBetweenDebugPrints)
	{
		inTime = clock();
		return true;
	}
	return false;
}

void ZenDebugPrinter::timedPrint(String inString)
{
	if (isPrintTimerThresholdReached())
	{
		DBG(inString);
	}
}
