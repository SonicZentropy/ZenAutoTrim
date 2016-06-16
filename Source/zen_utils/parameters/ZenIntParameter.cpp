/*==============================================================================
//  ZenIntParameter.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 4 Jun 2016 3:12:29pm
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Generic CPP File
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#include "ZenIntParameter.h"
#include "zen_utils/tests/ZenUnitTest.hpp"

ZenIntParameter::ZenIntParameter(ZenAudioProcessorValueTreeState& inOwner,
	String parameterID, String paramName, 
	int minValue, int maxValue,
	float inDefaultFloatValue /*= 0.0*/, String unitLabel /*= ""*/)
	:
	ZenParameter(inOwner, ZenParamType::ZENINT, parameterID, paramName, inDefaultFloatValue,
		unitLabel, false, 0.0f), defaultValue(inDefaultFloatValue)
{
	range = NormalisableRange<float>(minValue, maxValue);
	range.interval = 1;
	defaultValue = range.snapToLegalValue(inDefaultFloatValue); // make sure this is an int
}

ZenIntParameter::ZenIntParameter(ZenAudioProcessorValueTreeState& inOwner,
	String parameterID, String paramName,
	int minValue, int maxValue,
	int inDefaultIntValue /*= 0*/, String unitLabel /*= ""*/)
	:
	ZenParameter(inOwner, ZenParamType::ZENINT, parameterID, paramName, inDefaultIntValue,
		unitLabel, false, 0.0f), defaultValue(inDefaultIntValue)
{
	range = NormalisableRange<float>(minValue, maxValue);
	range.interval = 1;
}

void ZenIntParameter::setValue(float newValue)
{
	newValue = range.snapToLegalValue(newValue);
	value.store(newValue);
	
	listeners.call(&ZenAudioProcessorValueTreeState::Listener::parameterChanged, paramID, 0.0f);
	listenersNeedCalling = false;
	
	needsUpdate.set(1);
}

void ZenIntParameter::setValue(int newIntValue)
{
	float newValue = range.snapToLegalValue(newIntValue);
	value.store(newValue);

	listeners.call(&ZenAudioProcessorValueTreeState::Listener::parameterChanged, paramID, 0.0f);
	listenersNeedCalling = false;

	needsUpdate.set(1);
}

void ZenIntParameter::setValueRaw(float newValue)
{
	setValue(newValue);
}

void ZenIntParameter::setValueRaw(int newValue)
{
	setValue(newValue);
}

void ZenIntParameter::setValueNotifyingHost(float inValue)
{	
	processor->setParameterNotifyingHost(getParameterIndex(), inValue);

	listeners.call(&ZenAudioProcessorValueTreeState::Listener::parameterChanged, paramID, 0.0f);
	listenersNeedCalling = false;

	needsUpdate.set(1);
}

float ZenIntParameter::getValue() const
{
	return value.load();
}

float ZenIntParameter::getValueRaw() const
{
	return value.load();
}

int ZenIntParameter::getValueAsInt() const
{
	return (int)value.load();
}

void ZenIntParameter::setDefaultValue(float inFloatValue)
{
	defaultValue = range.snapToLegalValue(inFloatValue);
}

float ZenIntParameter::getDefaultValue() const
{
	return defaultValue;
}

int ZenIntParameter::getNumSteps() const
{
	return range.end - range.start;
}

String ZenIntParameter::getText(float inValue, int length) const
{
	return String(inValue, 2).substring(0, length);
}

float ZenIntParameter::getValueForText(const String& text) const
{
	//*//
	if (textToValueFunction != nullptr)
		return textToValueFunction(text);
	return text.getFloatValue();
}

String ZenIntParameter::getTextFromValue() const
{
	//*//
	
	return String(value.load());
}

void ZenIntParameter::updateFromValueTree()
{
	float prop = state.getProperty(owner.valuePropertyID, defaultValue);
	//DBG("In ZenIntParameter::updateFromValueTree() of " << this->paramID << " calling setValue with: " << prop);
	setValue((float)state.getProperty(owner.valuePropertyID, defaultValue));
}

void ZenIntParameter::copyValueToValueTree()
{
	//DBG("In ZenParameter::copyValueToValueTree() of " << paramID << " setting state to: " << value.load());
	float prop = value.load();
	if (state.isValid())
		state.setProperty(owner.valuePropertyID, value.load(), owner.undoManager);
}

void ZenIntParameter::valueTreePropertyChanged(ValueTree& vt, const Identifier& prop)
{
	if (prop == owner.valuePropertyID)
		updateFromValueTree();
}

//==============================================================================
#ifdef ZEN_UNIT_TESTS

class ZenIntParameterTests : public ZenUnitTest
{
public:
	ZenIntParameterTests() : ZenUnitTest("ZenIntParameter class") {}

	void runTest() override
	{
		beginTest("Get&Set Bool Values");

		ZenAudioProcessorValueTreeState* avt = new ZenAudioProcessorValueTreeState()

			ZenIntParameter boolParam(ZenAudioProcessorValueTreeState, "boolTest", "Bool Test", false);

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

static ZenIntParameterTests ZenIntParameterTests;
#endif //Unit Tests