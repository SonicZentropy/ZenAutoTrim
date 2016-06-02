/* ==============================================================================
//  BooleanParameter.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/08/18
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Derived AudioProcessorParameter for Boolean values.  Pure
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

#include "ZenBoolParameter.h"
#include "utilities/ZenUtils.hpp"

ZenBoolParameter::ZenBoolParameter(ZenAudioProcessorValueTreeState& inOwner, 
	String parameterID, String paramName, float inDefaultValue /*= 0.0*/, String unitLabel /*= ""*/)
	: 
	ZenParameter(inOwner, ZenParamType::ZENBOOL, parameterID, paramName, inDefaultValue,
	unitLabel, false, 0.0f), defaultValue(inDefaultValue)
{
	range = NormalisableRange<float>(0, 1);
}

ZenBoolParameter::ZenBoolParameter(ZenAudioProcessorValueTreeState& inOwner, 
	String parameterID, String paramName, 	bool inDefaultValue /*= false*/, String unitLabel) 
	: 
	ZenParameter(inOwner, ZenParamType::ZENBOOL, parameterID, paramName,
	convertBooleanToFloat(inDefaultValue), unitLabel, false, 0.0f)
	, defaultValue(convertBooleanToFloat(inDefaultValue))
{
	range = NormalisableRange<float>(0, 1);
}

void ZenBoolParameter::setValue(float newValue)
{
	if (newValue == 0.0f)
		value.store(0.0f);
	else value.store(1.0f);
}

void ZenBoolParameter::setValueRaw(float newValue)
{
	if (newValue == 0.0f)
		value.store(0.0f);
	else value.store(1.0f);
}

void ZenBoolParameter::toggleValue()
{
	if (value.load() == 0.0f)
	{
		value.store(1.0f);
	}
	else value.store(0.0f);
}

void ZenBoolParameter::setValueNotifyingHost(float inValue)
{
	float boolVal = convertFloatToBoolFloat(inValue);
	processor->setParameterNotifyingHost(getParameterIndex(), boolVal);
	setNeedsUIUpdate(false); //set this to false because change came from GUI
}

void ZenBoolParameter::setValueNotifyingHost(bool newBoolValue)
{
	processor->setParameterNotifyingHost(getParameterIndex(), convertBooleanToFloat(newBoolValue));
	setNeedsUIUpdate(false); //set this to false because change came from GUI
}

bool ZenBoolParameter::isOn() const
{
	return convertFloatToBoolean(getValue());
}

bool ZenBoolParameter::isOnByDefault() const
{
	return convertFloatToBoolean(getDefaultValue());
}

bool ZenBoolParameter::isOff() const
{
	return !convertFloatToBoolean(getValue());
}

bool ZenBoolParameter::isOffByDefault() const
{
	return !convertFloatToBoolean(getDefaultValue());
}

bool ZenBoolParameter::getValueAsBool() const
{
	return convertFloatToBoolean(getValue());
}

void ZenBoolParameter::setDefaultValue(float inValue)
{
	defaultValue = convertFloatToBoolFloat(inValue);
}

void ZenBoolParameter::setDefaultValueFromBool(bool inValue)
{
	defaultValue = convertBooleanToFloat(inValue);
}

float ZenBoolParameter::getDefaultValue() const
{
	return defaultValue;
}

bool ZenBoolParameter::getDefaultValueAsBool() const
{
	return convertFloatToBoolean(getDefaultValue());
}

void ZenBoolParameter::writeToXML(XmlElement& inXML)
{
	// DBG("In ZenParameter::writeToXML(inXML) ");
	XmlElement* thisXML = inXML.createNewChildElement(this->name);
	thisXML->setAttribute("parameterValue", getValue());
	thisXML->setAttribute("defaultValue", getDefaultValue());
	//thisXML->setAttribute("isSmoothed", getShouldBeSmoothed());
}

void ZenBoolParameter::setFromXML(const XmlElement& inXML)
{
	// DBG("In ZenParameter::setFromXML(inXML) ");
	XmlElement* thisXML = inXML.getChildByName(this->name);
	setValue(thisXML->getDoubleAttribute("parameterValue", false));
	setDefaultValue(thisXML->getDoubleAttribute("defaultValue", false));
	//setShouldBeSmoothed(thisXML->getBoolAttribute("isSmoothed", false));
}

String ZenBoolParameter::getText(float inValue, int length) const
{	
	bool text = convertFloatToBoolean(inValue);
	return (text) ? "True" : "False";
}

String ZenBoolParameter::getText(bool inValue) const
{
	return (inValue) ? "True" : "False";
}

float ZenBoolParameter::getValueForText(const String& text) const
{
	if (textToValueFunction != nullptr)
		return textToValueFunction(text);
	if (text == "TRUE" || text == "True" || text == "true") return 1.0;
	return 0.0f;
}

String ZenBoolParameter::getTextFromValue(int length) const
{
	if (value.load() == 0.0f) return "False";
	return "True";
}

void ZenBoolParameter::setValueFromBool(bool newBool)
{
	setValue(convertBooleanToFloat(newBool));
	setNeedsUIUpdate(true);
}

//==============================================================================
//==============================================================================
#ifdef ZEN_UNIT_TESTS

class ZenBoolParameterTests : public ZenUnitTest
{
public:
	ZenBoolParameterTests() : ZenUnitTest("ZenBoolParameter class") {}

	void runTest() override
	{
		beginTest("Get&Set Bool Values");

		ZenBoolParameter boolParam("boolTest", "Bool Test", false);

		expectAlmostEqual(boolParam.getDefaultValue(), 0.0f);
		expectAlmostEqual(boolParam.getValue(), 0.0f);
		expectAlmostEqual(boolParam.getValueRaw(), 0.0f);
		expectBooleanEqual(boolParam.getValueAsBool(), false);
		boolParam.setValue(0.3f);
		expectAlmostEqual(boolParam.getValueRaw(), 1.0f);
		expectAlmostEqual(boolParam.getValue(), 1.0f);
		expectBooleanEqual(boolParam.getValueAsBool(), true);
		boolParam.setValue(0.0f);
		expectAlmostEqual(boolParam.getValueRaw(), 0.0f);
		expectAlmostEqual(boolParam.getValue(), 0.0f);
		expectBooleanEqual(boolParam.getValueAsBool(), false);
		boolParam.setValue(1.0f);
		expectAlmostEqual(boolParam.getValueRaw(), 1.0f);
		expectAlmostEqual(boolParam.getValue(), 1.0f);
		expectBooleanEqual(boolParam.getValueAsBool(), true);
		boolParam.setValue(1.5f);
		expectAlmostEqual(boolParam.getValueRaw(), 1.0f);
		expectAlmostEqual(boolParam.getValue(), 1.0f);
		expectBooleanEqual(boolParam.getValueAsBool(), true);
		boolParam.setValue(-1.0f);
		expectAlmostEqual(boolParam.getValueRaw(), 1.0f);
		expectAlmostEqual(boolParam.getValue(), 1.0f);
		expectBooleanEqual(boolParam.getValueAsBool(), true);
		boolParam.setValue(0.00001f);
		expectAlmostEqual(boolParam.getValueRaw(), 1.0f);
		expectAlmostEqual(boolParam.getValue(), 1.0f);
		expectBooleanEqual(boolParam.getValueAsBool(), true);
		boolParam.setValueRaw(-7);
		expectAlmostEqual(boolParam.getValueRaw(), 1.0f);
		expectAlmostEqual(boolParam.getValue(), 1.0f);
		expectBooleanEqual(boolParam.getValueAsBool(), true);
		boolParam.setValueRaw(0);
		expectAlmostEqual(boolParam.getValueRaw(), 0.0f);
		expectAlmostEqual(boolParam.getValue(), 0.0f);
		expectBooleanEqual(boolParam.getValueAsBool(), false);
		boolParam.setValueRaw(0.1);
		expectAlmostEqual(boolParam.getValueRaw(), 1.0f);
		expectAlmostEqual(boolParam.getValue(), 1.0f);
		expectBooleanEqual(boolParam.getValueAsBool(), true);
		boolParam.setValueRaw(0.5);
		expectAlmostEqual(boolParam.getValueRaw(), 1.0f);
		expectAlmostEqual(boolParam.getValue(), 1.0f);
		expectBooleanEqual(boolParam.getValueAsBool(), true);
		boolParam.setValueRaw(0.99);
		expectAlmostEqual(boolParam.getValueRaw(), 1.0f);
		expectAlmostEqual(boolParam.getValue(), 1.0f);
		expectBooleanEqual(boolParam.getValueAsBool(), true);
		boolParam.setValueRaw(0.00001);
		expectAlmostEqual(boolParam.getValueRaw(), 1.0f);
		expectAlmostEqual(boolParam.getValue(), 1.0f);
		expectBooleanEqual(boolParam.getValueAsBool(), true);
		boolParam.setValueFromBool(true);
		expectAlmostEqual(boolParam.getValueRaw(), 1.0f);
		expectAlmostEqual(boolParam.getValue(), 1.0f);
		expectBooleanEqual(boolParam.getValueAsBool(), true);
		boolParam.setValueFromBool(false);
		expectAlmostEqual(boolParam.getValueRaw(), 0.0f);
		expectAlmostEqual(boolParam.getValue(), 0.0f);
		expectBooleanEqual(boolParam.getValueAsBool(), false);
	}
};

static ZenBoolParameterTests zenBoolParameterTests;
#endif //Unit Tests