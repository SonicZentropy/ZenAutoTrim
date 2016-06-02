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

 
#ifndef ZEN_DEBUG_H_INCLUDED
#define ZEN_DEBUG_H_INCLUDED

#include "JuceHeader.h"
#include <time.h>

#if JUCE_DEBUG
/** Writes a string to the standard error stream.
This is only compiled in a debug build.
@see ZenDebugPrinter::timedPrint
*/
#define TIMEDPRINT(dbgtext) MACRO_WITH_FORCED_SEMICOLON (juce::String tempDbgBuf; tempDbgBuf << dbgtext; Zen::ZenDebugPrinter::timedPrint(tempDbgBuf);)
#else
//==============================================================================
// If debugging is disabled, these dummy debug and assertion macros are used..
#define TIMEDPRINT(dbgtext)
#endif

/// <summary> Static method container for functions only used in Debugging </summary>
class ZenDebugPrinter
{
public:

	/// <summary>Sets desired time threshold between debug prints.</summary>
	/// <param name="inSeconds">The desired number of seconds between prints.</param>
	static void setSecondsBetweenDebugPrints(const unsigned int& inSeconds);

	/// <summary> Gets seconds between debug prints. </summary>
	/// <returns> The seconds between debug prints. </returns>
	static int getSecondsBetweenDebugPrints();

	/// <summary> Checks to see if a debug message should be printed or not (Based on configured time).
	///           Also resets the timer.</summary>
	/// <returns> true if print timer threshold reached, false if not. </returns>
	static bool isPrintTimerThresholdReached();

	/// <summary> Prints debug string argument if the print timer has been reached.</summary>
	/// <param name="debugStringToPrint">The string to be printed if timer is reached.</param>
	static void timedPrint(String debugStringToPrint);
private:
	static clock_t inTime;
	static int numSecondsBetweenDebugPrints;	///< Number of seconds between debug prints

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZenDebugPrinter);
};
#endif  // ZEN_DEBUG_H_INCLUDED
