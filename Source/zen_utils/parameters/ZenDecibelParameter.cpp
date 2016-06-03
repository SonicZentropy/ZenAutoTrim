/*==============================================================================
//  DecibelParameter.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/08
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Decibel Parameter Class - Handles Log/linear scaling
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#include "ZenDecibelParameter.h"
#include "utilities/ZenUtils.hpp"
#include "utilities/ZenDecibelConversions.hpp"

ZenDecibelParameter::ZenDecibelParameter(ZenAudioProcessorValueTreeState& owner, 
	String paramID, String paramName,
	float minValue, float maxValue,  
	float inDefaultValue /*= 0.0f*/, bool shouldBeSmoothed /*= false*/, 
	float smoothingTime /*= 50.0f*/) 
	: ZenParameter(owner, ZenParamType::ZENDECIBEL, paramID, paramName, 
		inDefaultValue
		, "dB", shouldBeSmoothed, smoothingTime),
	unityDecibels(0.0f), midValue(0.0f)
{
	range = NormalisableRange<float>(minValue, maxValue);
	float normalizedValue = DecibelConversions::mapDecibelsToProperNormalizedValue(inDefaultValue, minValue, maxValue, unityDecibels);
	currentSmoothedValue = normalizedValue;
	targetValue = normalizedValue;
}


ZenDecibelParameter::~ZenDecibelParameter()
{

}

void ZenDecibelParameter::setValue(float inValue)
{
	float newValue = DecibelConversions::mapProperNormalizedValueToDecibels(getClamped(inValue, 0.0f, 1.0f), range.start, range.end);
	
	//DBG("In ZenDecibelParameter::setValue(inValue) of " << this->paramID << " with inValue: " << inValue << " and storing: " << newValue);
	if (newValue != value.load() || listenersNeedCalling)
	{
		value.store(newValue);
	
		// #BUG: This should use value.load or newValue rather than value
		listeners.call(&ZenAudioProcessorValueTreeState::Listener::parameterChanged, paramID, value.load());
		listenersNeedCalling = false;

		needsUpdate.set(1);
	}
	if (shouldBeSmoothed) setTargetValue(inValue);
}

void ZenDecibelParameter::setValueRaw(float inValue)
{
	//DBG("In ZenDecibelParameter::setValueRaw(inValue) of " << this->paramID << " with inValue: " << inValue << " and storing: " << getClamped(inValue, range.start, range.end));
	value.store(getClamped(inValue, range.start, range.end));
	listeners.call(&ZenAudioProcessorValueTreeState::Listener::parameterChanged, paramID, value.load());
	listenersNeedCalling = false;

	needsUpdate.set(1);
	if (shouldBeSmoothed) setTargetValue(DecibelConversions::mapDecibelsToProperNormalizedValue(
		inValue, range.start, range.end, midValue));
}

void ZenDecibelParameter::setValueFromDecibels(float inValue)
{
	//DBG("In ZenDecibelParameter::setValueFromDecibels(inValue) of " << this->paramID << " with inValue: " << inValue);
	setValueRaw(inValue);
}

void ZenDecibelParameter::setValueFromGain(float inValue)
{
	float newValue = getClamped(Decibels::gainToDecibels(inValue), range.start, range.end);
	//DBG("In ZenDecibelParameter::setValueFromGain(inValue) of " << this->paramID << " with inValue: " << inValue << " and storing dbValue: " << newValue);
	value.store(newValue);
	listeners.call(&ZenAudioProcessorValueTreeState::Listener::parameterChanged, paramID, value.load());
	listenersNeedCalling = false;

	needsUpdate.set(1);
	if (shouldBeSmoothed)
	{
		setTargetValue(DecibelConversions::mapDecibelsToProperNormalizedValue(
			newValue, range.start, range.end, midValue));
	}
}

void ZenDecibelParameter::setValueFromGainNotifyingHost(float inValue)
{
	float dbValue = Decibels::gainToDecibels(inValue);
	//DBG("In ZenDecibelParameter::setValueFromGainNotifyingHost(inValue) of " << this->paramID << " with invalue: " << inValue << " and calling setValueNotifyingHost with decibel value: " << dbValue);
	
	setValueNotifyingHost(dbValue);

}

//inValue expected to be in Decibels
void ZenDecibelParameter::setValueNotifyingHost(float inValue)
{
	float clampedValue = range.snapToLegalValue(inValue);
	clampedValue = DecibelConversions::mapDecibelsToProperNormalizedValue(clampedValue, range.start, range.end, midValue);
	
	//DBG("In ZenDecibelParameter::setValueNotifyingHost(inValue) of " << this->paramID << " with invalue: " << inValue << " and setParameterNotifyingHost with: " << clampedValue);
	processor->setParameterNotifyingHost(getParameterIndex(), clampedValue);
	listeners.call(&ZenAudioProcessorValueTreeState::Listener::parameterChanged, paramID, clampedValue);
	listenersNeedCalling = false;

	needsUpdate.set(1);
	/*if (shouldBeSmoothed)
	{
		setTargetValue(clampedValue);
	}*/
}

float ZenDecibelParameter::getValue()
{
	// #TODO: get rid of this
	if (shouldBeSmoothed)
	{
		//return DecibelConversions::mapProperNormalizedValueToRawDecibelGain(getNextSmoothedValue(), range.start, range.end, midValue);
		//DBG("In ZenDecibelParameter::getValue() of " << this->paramID << " returning smoothed value: " << getNextSmoothedValue());
		return getNextSmoothedValue();
	}
	//DBG("In ZenDecibelParameter::getValue() of " << this->paramID << " returning: " << DecibelConversions::mapDecibelsToProperNormalizedValue(
		//value.load(), range.start, range.end, midValue));
	return DecibelConversions::mapDecibelsToProperNormalizedValue(
		value.load(), range.start, range.end, midValue);
}

float ZenDecibelParameter::getValue() const
{
	//DBG("In ZenDecibelParameter::getValue() of " << this->paramID << " returning: " << DecibelConversions::mapDecibelsToProperNormalizedValue(
		//value.load(), range.start, range.end, midValue));

	return DecibelConversions::mapDecibelsToProperNormalizedValue(
		value.load(), range.start, range.end, midValue);
}

float ZenDecibelParameter::getValueRaw() const
{
	//DBG("In ZenDecibelParameter::getValueRaw() of " << this->paramID << "returning: " << value.load());
	return value.load();
}

float ZenDecibelParameter::getValueInGain()
{
	// #TODO: get rid of this
	float result;
	if (shouldBeSmoothed)
	{
		result = DecibelConversions::mapProperNormalizedValueToRawDecibelGain(getNextSmoothedValue(), range.start, range.end, midValue);
	}
	else
		result = DecibelConversions::decibelsToDBGain<float>(value.load(), -96.0f);

	////DBG("In ZenDecibelParameter::getValueInGain() of " << this->paramID << "returning result: " << result);
	return result;
}

float ZenDecibelParameter::getValueInGain() const
{
	////DBG("In ZenDecibelParameter::getValueInGain() of " << this->paramID << "with value: " << value.load() << " and returning: " << DecibelConversions::decibelsTo//DBGain<float>(value.load(), -96.0f));
	////DBG("In const gain");
	return DecibelConversions::decibelsToDBGain<float>(value.load(), -96.0f);
}

//Returns NORMALIZED float value for input DECIBEL text value
float ZenDecibelParameter::getValueForText(const String& text) const
{
	//DBG("In ZenDecibelParameter::getValueForText(text)of " << this->paramID << " with input text: " << text);
	//return range.convertTo0to1(	text.getFloatValue());
	if (textToValueFunction != nullptr)
		return textToValueFunction(text);
	else //use default function
	{
		//return text.getFloatValue();
		return DecibelConversions::mapDecibelsToProperNormalizedValue(
			text.getFloatValue(), range.start, range.end, midValue);
	}
}

String ZenDecibelParameter::getText(float inValue, int maxStringLength) const
{	
	//DBG("In ZenDecibelParameter::getText(inValue, maxStringLength) ");
	if (valueToTextFunction != nullptr)
		return valueToTextFunction(DecibelConversions::mapProperNormalizedValueToDecibels(
			inValue, range.start, range.end, midValue));
	else
		return String(value.load(), 2) + " " + String(unitLabel);
}

float ZenDecibelParameter::getRawValueForText(const String& text) const
{
	return text.getFloatValue();
}


String ZenDecibelParameter::getTextFromValue() const
{
	return String(value.load(), 2) + " " + String(unitLabel);
}

float ZenDecibelParameter::getValueInDecibels() const
{
	return value.load();
}

void ZenDecibelParameter::setMinDecibels(const float inMinDecibels)
{
	range.start = inMinDecibels;
}

void ZenDecibelParameter::setMaxDecibels(const float inMaxDecibels)
{
	range.end = inMaxDecibels;
}

void ZenDecibelParameter::setValueTree()
{
	paramValueTree->removeAllChildren(nullptr);
	paramValueTree->removeAllProperties(nullptr);

	paramValueTree->setProperty("parameterValue", getValue(), nullptr);
	paramValueTree->setProperty("defaultValue", getDefaultValue(), nullptr);
	//ZenParameter::setValueTree();
	paramValueTree->setProperty("isSmoothed", getShouldBeSmoothed(), nullptr);
	paramValueTree->setProperty("minDecibels", getMinDecibels(), nullptr);
	paramValueTree->setProperty("maxDecibels", getMaxDecibels(), nullptr);
	paramValueTree->setProperty("unityDecibels", getUnityDecibels(), nullptr);
	paramValueTree->setProperty("midValue", getMidValue(), nullptr);
}

float ZenDecibelParameter::getDefaultValue() const
{
	return DecibelConversions::mapDecibelsToProperNormalizedValue(
		value.load(), range.start, range.end, defaultValue);
}

void ZenDecibelParameter::valueChanged(Value& inValue)
{
	 //DBG("In ZenDecibelParameter::valueChanged(value) ");
	setValueTree();
}

// void ZenDecibelParameter::writeToXML(XmlElement& inXML)
// {
// 	////DBG("In DecibelParameter::writeToXML(inXML) ");
// 	XmlElement* thisXML = inXML.createNewChildElement(this->name);
// 	thisXML->setAttribute("parameterValue", getValue());
// 	thisXML->setAttribute("defaultValue", getDefaultValue());
// 	thisXML->setAttribute("isSmoothed", getShouldBeSmoothed());
// 	thisXML->setAttribute("minDecibels", getMinDecibels());
// 	thisXML->setAttribute("maxDecibels", getMaxDecibels());
// 	thisXML->setAttribute("unityDecibels", getUnityDecibels());
// 	thisXML->setAttribute("midValue", getMidValue());
// }
// 
// void ZenDecibelParameter::setFromXML(const XmlElement& inXML)
// {
// 	////DBG("In DecibelParameter::setFromXML(inXML) ");
// 	XmlElement* thisXML = inXML.getChildByName(this->name);
// 	setValue(thisXML->getDoubleAttribute("parameterValue", -9876.5));
// 	setDefaultValue(thisXML->getDoubleAttribute("defaultValue", -9876.5));
// 	setShouldBeSmoothed(thisXML->getBoolAttribute("isSmoothed", false));
// 	setMinDecibels(thisXML->getDoubleAttribute("minDecibels", -9876.5));
// 	setMaxDecibels(thisXML->getDoubleAttribute("maxDecibels", -9876.5));
// 	setUnityDecibels(thisXML->getDoubleAttribute("unityDecibels", -9876.5));
// 	setMidValue(thisXML->getDoubleAttribute("midValue", -9876.5));
// }

void ZenDecibelParameter::setNewState(const ValueTree& v)
{
	state = v;
	updateFromValueTree();
}

void ZenDecibelParameter::updateFromValueTree()
{
	float updateVal = state.getProperty(owner.valuePropertyID, defaultValue);
	//DBG("In updateFromValueTree of " << this->paramID << " setting from state to: " << updateVal);
	setValueFromDecibels(state.getProperty(owner.valuePropertyID, defaultValue));
	//setUnnormalisedValue(state.getProperty(owner.valuePropertyID, defaultValue));
}

void ZenDecibelParameter::copyValueToValueTree()
{
	//DBG("In copyValueToValueTree of " << this->paramID << " setting property to: " << value.load());
	if (state.isValid())
		state.setProperty(owner.valuePropertyID, value.load(), owner.undoManager);
}

void ZenDecibelParameter::valueTreePropertyChanged(ValueTree& vt, const Identifier& prop)
{
	if (prop == owner.valuePropertyID)
		updateFromValueTree();
}

void ZenDecibelParameter::setUnnormalisedValue(float newUnnormalisedValue)
{
	//DBG("In setUnnormalizedvalue of " << this->paramID << " calling setValueNotifyingHost(decibelValueIn) with: " << newUnnormalisedValue);
	if (value.load() != newUnnormalisedValue)
	{
		//const float newValue = range.convertTo0to1(newUnnormalisedValue);
		setValueNotifyingHost(newUnnormalisedValue);
	}
}

//==============================================================================
//==============================================================================
#ifdef ZEN_UNIT_TESTS

/// ZenDecibelParameterTests: 
/// <seealso cref="ZenUnitTest" />
class ZenDecibelParameterTests : public ZenUnitTest
{
public:
	/// /*Initializes a new instance of the <see cref="ZenDecibelParameterTests"/> class.
	/// */
	ZenDecibelParameterTests() : ZenUnitTest("ZenDecibelParameter class") {}

	/// /* Runs the test.
	/// */
	void runTest() override
	{
		beginTest("Get&Set Values");

		ZenDecibelParameter decibelParam("testDBID", "Decibel param test",
			-96.0f, 12.0f, 0.0f, 0.0f, 0.0f);
		expectAlmostEqual(decibelParam.getDefaultValue(), 0.5f);
		expectAlmostEqual(decibelParam.getValue(), 0.5f);
		expectAlmostEqual(decibelParam.getValueInDB(), 0.0f);
		expectAlmostEqual(decibelParam.getValueInGain(), 1.0f);
		decibelParam.setValueRaw(12.0f);
		expectAlmostEqual(decibelParam.getValue(), 1.0f);
		expectAlmostEqual(decibelParam.getValueInDB(), 12.0f);
		expectAlmostEqual(decibelParam.getValueInGain(), 3.98107f);
		decibelParam.setValueRaw(0.0f);
		expectAlmostEqual(decibelParam.getValue(), 0.50f);
		expectAlmostEqual(decibelParam.getValueInDB(), 0.0f);
		expectAlmostEqual(decibelParam.getValueInGain(), 1.0f);
		decibelParam.setValueRaw(-18.0f);
		expectAlmostEqual(decibelParam.getValue(), 0.40625f);//11
		expectAlmostEqual(decibelParam.getValueInDB(), -18.0f);
		expectAlmostEqual(decibelParam.getValueInGain(), 0.125893f);
		decibelParam.setValueRaw(-96.0f);
		expectAlmostEqual(decibelParam.getValue(), 0.0f);
		expectAlmostEqual(decibelParam.getValueInDB(), -96.0f);
		expectAlmostEqual(decibelParam.getValueInGain(), 0.0f);
		decibelParam.setValue(1.0f);
		expectAlmostEqual(decibelParam.getValue(), 1.0f);
		expectAlmostEqual(decibelParam.getValueInDB(), 12.0f);
		expectAlmostEqual(decibelParam.getValueInGain(), 3.98107f);
		decibelParam.setValue(0.5f);
		expectAlmostEqual(decibelParam.getValue(), 0.50f); //20
		expectAlmostEqual(decibelParam.getValueInDB(), 0.0f);
		expectAlmostEqual(decibelParam.getValueInGain(), 1.0f);
		decibelParam.setValue(0.40625f);
		expectAlmostEqual(decibelParam.getValue(), 0.40625f);
		expectAlmostEqual(decibelParam.getValueInDB(), -18.0f);
		expectAlmostEqual(decibelParam.getValueInGain(), 0.125893f);
		decibelParam.setValue(0.0f);
		expectAlmostEqual(decibelParam.getValue(), 0.0f);
		expectAlmostEqual(decibelParam.getValueInDB(), -96.0f);
		expectAlmostEqual(decibelParam.getValueInGain(), 0.0f);
		decibelParam.setValueFromGain(3.98107f);
		expectAlmostEqual(decibelParam.getValue(), 1.0f);
		expectAlmostEqual(decibelParam.getValueInDB(), 12.0f); //30
		expectAlmostEqual(decibelParam.getValueInGain(), 3.98107f);
		decibelParam.setValueFromGain(1.0f);
		expectAlmostEqual(decibelParam.getValue(), 0.50f);
		expectAlmostEqual(decibelParam.getValueInDB(), 0.0f);
		expectAlmostEqual(decibelParam.getValueInGain(), 1.0f);
		decibelParam.setValueFromGain(0.125893f);
		expectAlmostEqual(decibelParam.getValue(), 0.40625f);
		expectAlmostEqual(decibelParam.getValueInDB(), -18.0f);
		expectAlmostEqual(decibelParam.getValueInGain(), 0.125893f);
		decibelParam.setValueFromGain(0.0f);
		expectAlmostEqual(decibelParam.getValue(), 0.0f);
		expectAlmostEqual(decibelParam.getValueInDB(), -96.0f);
		expectAlmostEqual(decibelParam.getValueInGain(), 0.0f);
	}
};

/// /* The zen decibel parameter tests*/
static ZenDecibelParameterTests zenDecibelParameterTests;
#endif // ZEN_UNIT_TESTS
