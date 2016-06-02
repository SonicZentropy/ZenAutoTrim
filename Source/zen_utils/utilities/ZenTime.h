/* ==============================================================================
//  TimeParameter.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/08/18
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Represents a time (in milliseconds) and assorted conversions
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef ZEN_TIME_PARAMETER_H_INCLUDED
#define ZEN_TIME_PARAMETER_H_INCLUDED

#include "JuceHeader.h"
#include "utilities/ZenConstants.h"

/** Handles time conversions for a value.  Stored inherently as a number of milliseconds */
class ZenTime 
{
public:

	explicit ZenTime(const String& inParamID, const TimeValue& inTimeValueInMS );

	~ZenTime();
	
	TimeValue getValueInMS() const { return value.load(); }
	TimeValue getValueInSamples(uint8 sampleRate) const { return sampleRate*(value.load() / 1000); }
	float getValueInSec() const { return value.load() / 1000; }

	void setValueFromMS(TimeValue inValue) { value.store(inValue); }
	void setValueFromSec(TimeValue inValue) { value.store(inValue*1000); }
	void setValueFromSamples(TimeValue samples, uint8 sampleRate) { value.store(samples/(sampleRate / 1000)); }
	
	/** Calls setValueTree */
	void valueChanged(Value& inValue);

	String getTextFromValue() const;

	void writeToXML(XmlElement& inXML);

	void setFromXML(const XmlElement& inXML);

	operator float() const noexcept { return (float)value.load(); }
	operator int() const noexcept { return value.load(); }

	ZenTime& operator=(float inValue)
	{
		if (value.load() != inValue) value.store(inValue);
		return *this;
	}

protected:
	AtomicTimeValue value;
	String paramID;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZenTime);
};

#endif // ZEN_TIME_PARAMETER_H_INCLUDED
