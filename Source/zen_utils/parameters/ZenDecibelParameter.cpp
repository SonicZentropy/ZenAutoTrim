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
	: 
	ZenParameter(owner, ZenParamType::ZENDECIBEL, paramID, paramName, 
		inDefaultValue, "dB", shouldBeSmoothed, smoothingTime),
	unityDecibels(0.0f), midValue(0.0f)
{
	range = NormalisableRange<float>(minValue, maxValue);
	float normalizedValue = DecibelConversions::mapDecibelsToProperNormalizedValue
		(inDefaultValue, range.start, range.end, midValue);
	//DBG("In ZenDecibelParam constructor of: " << paramID << " setting value to : " << normalizedValue);
	setValue(normalizedValue);
	currentSmoothedValue = normalizedValue;
	targetValue = normalizedValue;
}


ZenDecibelParameter::~ZenDecibelParameter()
{

}

//Set from normalized invalue
void ZenDecibelParameter::setValue(float inNormValue)
{
	//float newValue = DecibelConversions::mapProperNormalizedValueToDecibels(getClamped(inNormValue, 0.0f, 1.0f), range.start, range.end);
	
	
	//DBG("In ZenDecibelParameter::setValue(inValue) of " << this->paramID  << " and storing: " << inNormValue);
	if (inNormValue != value.load() || listenersNeedCalling)
	{
		value.store(inNormValue);
	
		// #BUG: This should use value.load or newValue rather than value
		listeners.call(&ZenAudioProcessorValueTreeState::Listener::parameterChanged, paramID, value.load());
		listenersNeedCalling = false;

		needsUpdate.set(1);
		
		if (shouldBeSmoothed) setTargetValue(inNormValue);
	}
}


void ZenDecibelParameter::setValueRaw(float inDBValue)
{
	setValueFromDecibels(inDBValue);
}

void ZenDecibelParameter::setValueFromDecibels(float inDBValue)
{
	//value.store(getClamped(inDBValue, range.start, range.end));
	float newNormalizedValue = DecibelConversions::mapDecibelsToProperNormalizedValue(
		inDBValue, range.start, range.end, midValue);
	//DBG("In ZenDecibelParameter::setValueFromDecibels(inDBValue) of " << this->paramID << " with inDBValue: " << inDBValue << " and storing: " << newNormalizedValue);
	value.store(newNormalizedValue);
	if (shouldBeSmoothed) setTargetValue(newNormalizedValue);

	listeners.call(&ZenAudioProcessorValueTreeState::Listener::parameterChanged, paramID, value.load());
	listenersNeedCalling = false;
	
	needsUpdate.set(1);
}

void ZenDecibelParameter::setValueFromGain(float inGainValue)
{
	float newValue = getClamped(Decibels::gainToDecibels(inGainValue), range.start, range.end);
	float newNormValue = DecibelConversions::mapDecibelsToProperNormalizedValue(
		newValue, range.start, range.end, midValue);
	//DBG("In ZenDecibelParameter::setValueFromGain(inValue) of " << this->paramID << " with inValue: " << inGainValue << " and storing value: " << newNormValue);
	value.store(newNormValue);
	if (shouldBeSmoothed) setTargetValue(newNormValue);

	listeners.call(&ZenAudioProcessorValueTreeState::Listener::parameterChanged, paramID, value.load());
	listenersNeedCalling = false;

	needsUpdate.set(1);
	
}

void ZenDecibelParameter::setValueFromGainNotifyingHost(float inGainValue)
{
	float dbValue = getClamped(Decibels::gainToDecibels(inGainValue), range.start, range.end);
	float newNormValue = DecibelConversions::mapDecibelsToProperNormalizedValue(
		dbValue, range.start, range.end, midValue);
	//DBG("In ZenDecibelParameter::setValueFromGainNotifyingHost(inValue) of " << this->paramID << " with invalue: " << inGainValue << " and calling setValueNotifyingHost with value: " << newNormValue);
	
	setValueNotifyingHost(newNormValue);

}

//inNormValue expected to be normalized
void ZenDecibelParameter::setValueNotifyingHost(float inNormValue)
{
	float clampedValue = getClamped(inNormValue, 0.0f, 1.0f);
	//clampedValue = DecibelConversions::mapDecibelsToProperNormalizedValue(clampedValue, range.start, range.end, midValue);
	
	//DBG("In ZenDecibelParameter::setValueNotifyingHost(inValue) of " << this->paramID << " with invalue: " << inNormValue << " and setParameterNotifyingHost with: " << clampedValue);
	processor->setParameterNotifyingHost(getParameterIndex(), clampedValue);
	if (shouldBeSmoothed) setTargetValue(clampedValue);

	listeners.call(&ZenAudioProcessorValueTreeState::Listener::parameterChanged, paramID, clampedValue);
	listenersNeedCalling = false;
	needsUpdate.set(1);
}

void ZenDecibelParameter::setValueFromDecibelsNotifyingHost(float inDBValue)
{
	setValueNotifyingHost(DecibelConversions::mapDecibelsToProperNormalizedValue(inDBValue, range.start, range.end, midValue));
}

float ZenDecibelParameter::getValue()
{
	// #TODO: get rid of this
	if (shouldBeSmoothed)
	{
		//return DecibelConversions::mapProperNormalizedValueToRawDecibelGain(getNextSmoothedValue(), range.start, range.end, midValue);
		////DBG("In ZenDecibelParameter::getValue() of " << this->paramID << " returning smoothed value: " << getNextSmoothedValue());
		return getNextSmoothedValue();
	}
	//DBG("In ZenDecibelParameter::getValue() of " << this->paramID << " returning: " << value.load());
	//return DecibelConversions::mapDecibelsToProperNormalizedValue(
	//	value.load(), range.start, range.end, midValue);
	return value.load();
}

float ZenDecibelParameter::getValue() const
{
	//DBG("In ZenDecibelParameter::getValue() of " << this->paramID << " returning: " << value.load());
		//value.load(), range.start, range.end, midValue));

	//return DecibelConversions::mapDecibelsToProperNormalizedValue(
	//	value.load(), range.start, range.end, midValue);
	return value.load();
}

float ZenDecibelParameter::getValueRaw() const
{
	return getValueInDecibels();
}

float ZenDecibelParameter::getValueInDecibels() const
{
	float result =  DecibelConversions::mapProperNormalizedValueToDecibels(value.load(), range.start, range.end, midValue);
	
	//if (result != -96) DBG("In ZenDecibelParameter::getValueInDecibels() returning: " << result);
	return result;
}

float ZenDecibelParameter::getValueInGain()
{
	// #TODO: get rid of this
	float valConvert;
	if (shouldBeSmoothed)
	{
		valConvert = getNextSmoothedValue();
	}
	else
	{
		valConvert = value.load();
	}

	float result = DecibelConversions::mapProperNormalizedValueToRawDecibelGain(valConvert, range.start, range.end, midValue);
	
	//if (result != 0) DBG("In ZenDecibelParameter::getValueInGain() of " << this->paramID << "returning result: " << result);
	
	return result;
	
}

float ZenDecibelParameter::getValueInGain() const
{
	//////DBG("In ZenDecibelParameter::getValueInGain() of " << this->paramID << "with value: " << value.load() << " and returning: " << DecibelConversions::decibelsTo////DBGain<float>(value.load(), -96.0f));
	//////DBG("In const gain");
	float result = DecibelConversions::mapProperNormalizedValueToRawDecibelGain(value.load(), range.start, range.end, midValue);
	//DBG("In ZenDecibelParameter::getValueInGain() returning: " << result);
	return result;
}

//Returns NORMALIZED float value for input DECIBEL text value
float ZenDecibelParameter::getValueForText(const String& text) const
{
	////DBG("In ZenDecibelParameter::getValueForText(text)of " << this->paramID << " with input text: " << text);
	//return range.convertTo0to1(	text.getFloatValue());
	if (textToValueFunction != nullptr)
		return textToValueFunction(text);
	else //use default function
	{
		//return text.getFloatValue();
		float result = DecibelConversions::mapDecibelsToProperNormalizedValue(
			text.getFloatValue(), range.start, range.end, midValue);
		//DBG("In ZenDecibelParameter::getValueForText(text) returning: " << result);
		return result;
	}
}

String ZenDecibelParameter::getText(float inValue, int maxStringLength) const
{	
	////DBG("In ZenDecibelParameter::getText(inValue, maxStringLength) ");
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



void ZenDecibelParameter::setMinDecibels(const float inMinDecibels)
{
	range.start = inMinDecibels;
}

void ZenDecibelParameter::setMaxDecibels(const float inMaxDecibels)
{
	range.end = inMaxDecibels;
}

void ZenDecibelParameter::setDefaultValue(float inDBValue)
{
	defaultValue = DecibelConversions::mapDecibelsToProperNormalizedValue(inDBValue, range.start, range.end, midValue);
}

float ZenDecibelParameter::getDefaultValue() const
{
	//return DecibelConversions::mapDecibelsToProperNormalizedValue(
	//	value.load(), range.start, range.end, defaultValue);
	return defaultValue;
}

void ZenDecibelParameter::valueChanged(Value& inValue)
{
	 ////DBG("In ZenDecibelParameter::valueChanged(value) ");
	setValueTree();
}

void ZenDecibelParameter::setNewState(const ValueTree& v)
{
	state = v;
	updateFromValueTree();
}

void ZenDecibelParameter::updateFromValueTree()
{
	float updateVal = state.getProperty(owner.valuePropertyID, defaultValue);
	////DBG("In updateFromValueTree of " << this->paramID << " setting from state to: " << updateVal);
	setValue(state.getProperty(owner.valuePropertyID, defaultValue));
	//setUnnormalisedValue(state.getProperty(owner.valuePropertyID, defaultValue));
}

void ZenDecibelParameter::copyValueToValueTree()
{
	////DBG("In copyValueToValueTree of " << this->paramID << " setting property to: " << value.load());
	if (state.isValid())
		state.setProperty(owner.valuePropertyID, getValue(), owner.undoManager);
}

void ZenDecibelParameter::valueTreePropertyChanged(ValueTree& vt, const Identifier& prop)
{
	if (prop == owner.valuePropertyID)
		updateFromValueTree();
}

void ZenDecibelParameter::setUnnormalisedValue(float newUnnormalisedValue)
{
	////DBG("In setUnnormalizedvalue of " << this->paramID << " calling setValueNotifyingHost(decibelValueIn) with: " << newUnnormalisedValue);
	if (value.load() != newUnnormalisedValue)
	{
		const float newValue = DecibelConversions::mapDecibelsToProperNormalizedValue
			(newUnnormalisedValue, range.start, range.end, midValue);
		
		setValueNotifyingHost(newValue);
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
