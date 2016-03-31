/* ==============================================================================
//  FloatParameter.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/08/18
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Derived AudioProcessorParameter for Float values.  Pure
//  virtual methods that must be overridden:
//   destructor
//   getValue
//   setValue
//   getDefaultValue
//   getName
//   getLabel (units string, example "Hz" or "%")
//   getValueForText (parse a String and return appropriate value for it)
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#pragma once
#ifndef ZEN_FLOAT_PARAMETER_H_INCLUDED
#define ZEN_FLOAT_PARAMETER_H_INCLUDED

#include "JuceHeader.h"
#include "ZenParameter.hpp"
#include "../utilities/ZenUtils.hpp"
#include "../tests/ZenUnitTest.hpp"

namespace Zen
{
using namespace juce;

class ZenFloatParameter : public ZenParameter
{
public:

	ZenFloatParameter(String paramID, String paramName, NormalisableRange<float> normalisableRange,
		float inDefaultValue, const String& unitLabel = "", const bool& inShouldBeSmoothed = false,
		const float& smoothingTime = 0.01f)
		: ZenParameter(paramID, paramName, inDefaultValue, unitLabel, inShouldBeSmoothed, smoothingTime),
		range(normalisableRange), defaultValue(inDefaultValue)
	{			
		if (inShouldBeSmoothed)
		{
			currentSmoothedValue = range.convertTo0to1(inDefaultValue);
			targetValue = currentSmoothedValue;
		}
		
	}

	ZenFloatParameter(String paramID, String paramName, float minValue, float maxValue, float inDefaultValue,
		const String& unitLabel = "", const bool& inShouldBeSmoothed = false, const float& smoothingTime = 0.01f)
		: ZenParameter(paramID, paramName, inDefaultValue, unitLabel, inShouldBeSmoothed, smoothingTime),
		range(minValue, maxValue), defaultValue(inDefaultValue)
	{
		if (inShouldBeSmoothed)
		{
			currentSmoothedValue = range.convertTo0to1(inDefaultValue);
			targetValue = currentSmoothedValue;
		}
	}

	virtual ~ZenFloatParameter()
	{
		paramValueTree = nullptr;
	};

	/** Set this parameter's value from 0to1 normalized input value.
	* Should be called only by HOST/Process Block, never GUI */
	void setValue(float inValue) override
	{
		inValue = getClamped(inValue, 0.0f, 1.0f);
		value.store(range.convertFrom0to1(inValue));
		setNeedsUIUpdate(true);
		if (shouldBeSmoothed) setTargetValue(inValue);
	}
	
		
	/** Directly sets value from raw input, called by GUI */
	void setValueRaw(float inValue) override
	{
		value.store(range.snapToLegalValue(inValue));
		if (shouldBeSmoothed) setTargetValue(range.convertTo0to1(inValue));
	}

	/** This assumes inValue is NOT normalized and coming from NOT the host, since
	* it promptly notifies the host of the change. */
	void setValueNotifyingHost(float inValue) override
	{		
		float clampedValue = range.snapToLegalValue(inValue);
		processor->setParameterNotifyingHost(getParameterIndex(), clampedValue);
		setNeedsUIUpdate(false); //set this to false because change came from GUI
		if (shouldBeSmoothed) setTargetValue(range.convertTo0to1(inValue));
	}

	/**Sets this parameter's value from normalized value. This should never
	*  ever be called, as the GUI does not deal in normalized values */
	/*void setValueNormalizedNotifyingHost(float inValue) override
	{
		DBG("This should never be called!");
		float rawValue = range.convertFrom0to1(inValue);
		processor->setParameterNotifyingHost(getParameterIndex(), rawValue);
		setNeedsUIUpdate(false); //set this to false because change came from GUI
	}*/

	/**This is called by the HOST/Process block and needs to return
	* a normalized value */
	float getValue() const override { return range.convertTo0to1(value); }

	float getValue()
	{
		if (shouldBeSmoothed)
		{
			return getNextSmoothedValue();
		}
		return range.convertTo0to1(value);
	}

	/** Returns raw value direct, for GUI! */
	float getValueRaw() const override { return value; }

	float getValueRaw()
	{
		if (shouldBeSmoothed)
		{
			return range.convertFrom0to1(getNextSmoothedValue());
		}
		return value;
	}

	void setDefaultValue(float inValue) override { defaultValue = inValue; }

	/** Returns default value as normalized 0 to 1 */
	float getDefaultValue() const override
	{
		return range.convertTo0to1(defaultValue);
	}

	float getDefaultValueRaw() const
	{
		return defaultValue;
	}

	float getMinValue() const { return range.start; }

	void setMinValue(float inValue) { range.start = inValue; }

	float getMaxValue() const { return range.end; }

	void setMaxValue(float inValue) { range.end = inValue; }

	/** Calls setValueTree */
	void valueChanged(Value& inValue) override
	{
	    //DBG("In ZenParameter::valueChanged(value) ");
		setValueTree();
	}

	/** Parses an input String and converts it to a normalized numeric value based on the param's range */
	float getValueForText(const String& text) const override
	{
		return range.convertTo0to1(text.getFloatValue());
	}

	/** Parses an input String and converts it to a raw, non-normalized numeric value based on the param's range */
	virtual float getRawValueForText(const String& text) const
	{
		return text.getFloatValue();
	}

	/** Takes an input normalized value, denormalizes it, and returns the result as text */
	String getText(float inValue, int length/*maximumStringLength*/) const override
	{
		String asText(range.convertFrom0to1(inValue), 2);
		return length > 0 ? asText.substring(0, length) : asText;
	}

	/** Zen::ZenFloatParameter::getTextFromValue
	* Returns this parameter's current value as text */
	String getTextFromValue(int length /*Max string length*/) const override
	{
		return getText(value, length);
	}

	void writeToXML(XmlElement& inXML) override
	{
	    // DBG("In ZenParameter::writeToXML(inXML) ");
		XmlElement* thisXML = inXML.createNewChildElement(this->name);
		thisXML->setAttribute("parameterValue", getValue());
		thisXML->setAttribute("defaultValue", getDefaultValue());
		thisXML->setAttribute("isSmoothed", getShouldBeSmoothed());
	}

	void setFromXML(const XmlElement& inXML) override
	{
	    //DBG("In ZenParameter::setFromXML(inXML) ");
		XmlElement* thisXML = inXML.getChildByName(this->name);
		setValue(thisXML->getDoubleAttribute("parameterValue", -9876.5));
		setDefaultValue(thisXML->getDoubleAttribute("defaultValue", -9876.5));
		setShouldBeSmoothed(thisXML->getBoolAttribute("isSmoothed", false));
	}

	/** Returns the parameter's current value (Normalized). */
	float get() const noexcept { return getValue(); }
	/** Returns the parameter's current value (Normalized). */
	operator float() const noexcept { return value; }

	ZenFloatParameter& operator=(float inValue)
	{
		if (value.load() != inValue) setValueNotifyingHost(range.convertTo0to1(inValue));
		return *this;
	}

protected:
	NormalisableRange<float> range;
	float defaultValue;

private:

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZenFloatParameter); 
};

//==============================================================================
//==============================================================================
#ifdef ZEN_UNIT_TESTS

class ZenFloatParameterTests : public ZenUnitTest
{
public:
	ZenFloatParameterTests() : ZenUnitTest("ZenFloatParameter class") {}

	void runTest() override
	{		
		beginTest("Get&Set Values");
		ZenFloatParameter floatParam("floatTest", "Float Test", -96.0f, 18.0f, -18.0f);
									
		expectAlmostEqual(floatParam.getDefaultValue(), 0.684210526315f);
		expectAlmostEqual(floatParam.getValue(), 0.684210526315f);
		expectAlmostEqual(floatParam.getValueRaw(), -18.0f);
		floatParam.setValue(0.3f);
		expectAlmostEqual(floatParam.getValueRaw(), -61.8f);
		expectAlmostEqual(floatParam.getValue(), 0.3f);
		floatParam.setValueRaw(-7);
		expectAlmostEqual(floatParam.getValueRaw(), -7.0f);
		expectAlmostEqual(floatParam.getValue(), 0.780701754385f);
		floatParam.setValue(0.0f);
		expectAlmostEqual(floatParam.getValueRaw(), -96.0f);
		expectAlmostEqual(floatParam.getValue(), 0.0f);
		floatParam.setValue(1.0f);
		expectAlmostEqual(floatParam.getValueRaw(), 18.0f);
		expectAlmostEqual(floatParam.getValue(), 1.0f);
		floatParam.setValue(1.5f);
		expectAlmostEqual(floatParam.getValueRaw(), 18.0f);
		expectAlmostEqual(floatParam.getValue(), 1.0f);
		floatParam.setValue(-1.0f);
		expectAlmostEqual(floatParam.getValueRaw(), -96.0f);
		expectAlmostEqual(floatParam.getValue(), 0.0f);
	}
};

static ZenFloatParameterTests zenFloatParameterTests;


#endif // ZEN_UNIT_TESTS

}//namespace
#endif // ZEN_FLOAT_PARAMETER_H_INCLUDED

/* <summary> Values that represent parameter units. </summary>
enum ParameterUnit
{
UnitGeneric = 0,	/ * untyped value generally between 0.0 and 1.0 * /
UnitIndexed = 1,	/ * takes an integer value (good for menu selections) * /
UnitBoolean = 2,	/ * 0.0 means FALSE, non-zero means TRUE * /
UnitPercent = 3,	/ * usually from 0 -> 100, sometimes -50 -> +50 * /
UnitSeconds = 4,	/ * absolute or relative time * /
UnitSampleFrames = 5,	/ * one sample frame equals (1.0/sampleRate) seconds * /
UnitPhase = 6,	/ * -180 to 180 degrees * /
UnitRate = 7,	/ * rate multiplier, for playback speed, etc. (e.g. 2.0 == twice as fast) * /
UnitHertz = 8,	/ * absolute frequency/pitch in cycles/second * /
UnitCents = 9,	/ * unit of relative pitch * /
UnitRelativeSemiTones = 10,	/ * useful for coarse detuning * /
UnitMIDINoteNumber = 11,	/ * absolute pitch as defined in the MIDI spec (exact freq may depend on tuning table) * /
UnitMIDIController = 12,	/ * a generic MIDI controller value from 0 -> 127 * /
UnitDecibels = 13,	/ * logarithmic relative gain * /
UnitLinearGain = 14,	/ * linear relative gain * /
UnitDegrees = 15,	/ * -180 to 180 degrees, similar to phase but more general (good for 3D coord system) * /
UnitEqualPowerCrossfade = 16,	/ * 0 -> 100, crossfade mix two sources according to sqrt(x) and sqrt(1.0 - x) * /
UnitMixerFaderCurve1 = 17,	/ * 0.0 -> 1.0, pow(x, 3.0) -> linear gain to simulate a reasonable mixer channel fader response * /
UnitPan = 18,	/ * standard left to right mixer pan * /
UnitMeters = 19,	/ * distance measured in meters * /
UnitAbsoluteCents = 20,	/ * absolute frequency measurement : if f is freq in hertz then 	* /
/ * absoluteCents = 1200 * log2(f / 440) + 6900					* /
UnitOctaves = 21,	/ * octaves in relative pitch where a value of 1 is equal to 1200 cents* /
UnitBPM = 22,	/ * beats per minute, ie tempo * /
UnitBeats = 23,	/ * time relative to tempo, ie. 1.0 at 120 BPM would equal 1/2 a second * /
UnitMilliseconds = 24,	/ * parameter is expressed in milliseconds * /
UnitRatio = 25,	/ * for compression, expansion ratio, etc. * /

UnitCustomUnit = 26	/ * this is the parameter unit type for parameters that present a custom unit name * /
};*/
