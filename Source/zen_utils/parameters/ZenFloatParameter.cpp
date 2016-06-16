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

#include "ZenFloatParameter.h"
#include "zen_utils/utilities/ZenUtils.hpp"

ZenFloatParameter::ZenFloatParameter(ZenAudioProcessorValueTreeState& owner, String paramID, String paramName,
	NormalisableRange<float> normalisableRange, float inDefaultValue, 
	const String& unitLabel /*= ""*/, const bool& inShouldBeSmoothed /*= false*/, 
	const float& smoothingTime /*= 0.01f*/) 
	: 
	ZenParameter(owner, ZenParamType::ZENFLOAT, paramID, paramName, inDefaultValue, unitLabel, 
		inShouldBeSmoothed, smoothingTime),
	range(normalisableRange), defaultValue(inDefaultValue)
{
	if (inShouldBeSmoothed)
	{
		currentSmoothedValue = range.convertTo0to1(inDefaultValue);
		targetValue = currentSmoothedValue;
	}
}

ZenFloatParameter::ZenFloatParameter(ZenAudioProcessorValueTreeState& owner, String paramID, 
	String paramName, float minValue, float maxValue, float inDefaultValue, 
	const String& unitLabel /*= ""*/, const bool& inShouldBeSmoothed /*= false*/, const float& smoothingTime /*= 0.01f*/) 
	: ZenParameter(owner, ZenParamType::ZENFLOAT, paramID, paramName, inDefaultValue, unitLabel, inShouldBeSmoothed, smoothingTime),
range(minValue, maxValue), defaultValue(inDefaultValue)
{
	if (inShouldBeSmoothed)
	{
		currentSmoothedValue = range.convertTo0to1(inDefaultValue);
		targetValue = currentSmoothedValue;
	}
}

ZenFloatParameter::~ZenFloatParameter()
{
	paramValueTree = nullptr;
}

void ZenFloatParameter::setValue(float inValue)
{
	inValue = getClamped(inValue, 0.0f, 1.0f);
	value.store(range.convertFrom0to1(inValue));
	setNeedsUIUpdate(true);
	if (shouldBeSmoothed) setTargetValue(inValue);
}

void ZenFloatParameter::setValueRaw(float inValue)
{
	value.store(range.snapToLegalValue(inValue));
	if (shouldBeSmoothed) setTargetValue(range.convertTo0to1(inValue));
}

void ZenFloatParameter::setValueNotifyingHost(float inValue)
{
	float clampedValue = range.snapToLegalValue(inValue);
	processor->setParameterNotifyingHost(getParameterIndex(), clampedValue);
	setNeedsUIUpdate(false); //set this to false because change came from GUI
	if (shouldBeSmoothed) setTargetValue(range.convertTo0to1(inValue));
}

float ZenFloatParameter::getValue() const
{
	return range.convertTo0to1(value);
}

float ZenFloatParameter::getValue()
{
	if (shouldBeSmoothed)
	{
		return getNextSmoothedValue();
	}
	return range.convertTo0to1(value);
}

float ZenFloatParameter::getValueRaw() const
{
	return value;
}

float ZenFloatParameter::getValueRaw()
{
	if (shouldBeSmoothed)
	{
		return range.convertFrom0to1(getNextSmoothedValue());
	}
	return value;
}

void ZenFloatParameter::setDefaultValue(float inValue)
{
	defaultValue = inValue;
}

float ZenFloatParameter::getDefaultValue() const
{
	return range.convertTo0to1(defaultValue);
}

float ZenFloatParameter::getDefaultValueRaw() const
{
	return defaultValue;
}

void ZenFloatParameter::valueChanged(Value& inValue)
{
	//DBG("In ZenParameter::valueChanged(value) ");
	setValueTree();
}

float ZenFloatParameter::getValueForText(const String& text) const
{
	return range.convertTo0to1(text.getFloatValue());
}

float ZenFloatParameter::getRawValueForText(const String& text) const
{
	return text.getFloatValue();
}

String ZenFloatParameter::getText(float inValue, int length/*maximumStringLength*/) const
{
	String asText(range.convertFrom0to1(inValue), 2);
	return length > 0 ? asText.substring(0, length) : asText;
}

String ZenFloatParameter::getTextFromValue() const
{
	return getText(value, 0);
}

void ZenFloatParameter::writeToXML(XmlElement& inXML)
{
	// DBG("In ZenParameter::writeToXML(inXML) ");
	XmlElement* thisXML = inXML.createNewChildElement(this->name);
	thisXML->setAttribute("parameterValue", getValue());
	thisXML->setAttribute("defaultValue", getDefaultValue());
	thisXML->setAttribute("isSmoothed", getShouldBeSmoothed());
}

void ZenFloatParameter::setFromXML(const XmlElement& inXML)
{
	//DBG("In ZenParameter::setFromXML(inXML) ");
	XmlElement* thisXML = inXML.getChildByName(this->name);
	setValue(thisXML->getDoubleAttribute("parameterValue", -9876.5));
	setDefaultValue(thisXML->getDoubleAttribute("defaultValue", -9876.5));
	setShouldBeSmoothed(thisXML->getBoolAttribute("isSmoothed", false));
}


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
