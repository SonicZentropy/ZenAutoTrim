/*==============================================================================
//  DecibelParameter.h
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

#pragma once
#ifndef ZEN_DECIBEL_PARAMETER_H_INCLUDED
#define ZEN_DECIBEL_PARAMETER_H_INCLUDED

#include "ZenFloatParameter.hpp"
#include "..\utilities\ZenDecibelConversions.hpp"

namespace Zen
{

class ZenDecibelParameter : public ZenParameter
{
public:
	explicit ZenDecibelParameter(String paramID, String paramName, float minValue, float maxValue, 
		float midValue, float unityDecibels, float inDefaultValue = 0.0f, 
		bool shouldBeSmoothed = false, float smoothingTime = 50.0f)
		: ZenParameter(paramID, paramName, inDefaultValue, "dB", shouldBeSmoothed, smoothingTime),
		range(minValue, maxValue), unityDecibels(unityDecibels), midValue(midValue),defaultValue(inDefaultValue)
	{ 
		float normalizedValue = DecibelConversions::mapDecibelsToProperNormalizedValue(inDefaultValue, range.start, range.end, unityDecibels);
		currentSmoothedValue = normalizedValue;
		targetValue = normalizedValue;
	}	

	virtual ~ZenDecibelParameter()
	{ }	

	/** Sets value from 0to1 normalized (From host) - Stored as decibel */
	void setValue(float inValue) override
	{
		float newValue = DecibelConversions::mapProperNormalizedValueToDecibels(getClamped(inValue, 0.0f, 1.0f), range.start, range.end);
		value.store(newValue);
		if (shouldBeSmoothed) setTargetValue(inValue);
	}

	/** Sets raw decibel value directly (From GUI) */
	void setValueRaw(float inValue) override
	{
		value.store(getClamped(inValue, range.start, range.end));		
		if (shouldBeSmoothed) setTargetValue(DecibelConversions::mapDecibelsToProperNormalizedValue(
			inValue, range.start, range.end, midValue));
	}

	/** Sets raw decibel value from a gain value (From Process Block) */
	void setValueGain(float inValue)
	{
		float newValue = getClamped(DecibelConversions::gainToDecibels(inValue), range.start, range.end);
		value.store(newValue);
		if (shouldBeSmoothed)
		{			
			setTargetValue(DecibelConversions::mapDecibelsToProperNormalizedValue(
				newValue, range.start, range.end, midValue));
		}
	}
	/** This assumes inValue is NOT normalized and coming from NOT the host, since
	* it promptly notifies the host of the change. */
	void setValueNotifyingHost(float inValue) override
	{		
//		DBG("In DecibelParameter::setValueNotifyingHost() ");
		//This actually gets a normalized value already, not decibels
		//float dbConvertedToLinear = convertDecibelsToLinearWithSetMidpoint(newValue);
		float clampedValue = range.snapToLegalValue(inValue);
		clampedValue = DecibelConversions::mapDecibelsToProperNormalizedValue(clampedValue, range.start, range.end, midValue);
		processor->setParameterNotifyingHost(getParameterIndex(), clampedValue);
		/*if (shouldBeSmoothed)
		{
			setTargetValue(clampedValue);
		}*/
	}
	/** Returns value properly normalized between 0 and 1 with the set midpoint.
	 ** Handles smoothed and non-smoothed values innately*/
	float getValue() 
	{
		//DBG("IN non-const");
		if(shouldBeSmoothed)
		{			
			//return DecibelConversions::mapProperNormalizedValueToRawDecibelGain(getNextSmoothedValue(), range.start, range.end, midValue);
			return getNextSmoothedValue();
		}
		return DecibelConversions::mapDecibelsToProperNormalizedValue(
			value.load(), range.start, range.end, midValue);
	}

	/** Returns value properly normalized between 0 and 1 with the set midpoint.
	 ** Is not used for smoothed values */
	float getValue() const override
	{
		return DecibelConversions::mapDecibelsToProperNormalizedValue(
			value.load(), range.start, range.end, midValue);
	}

	/** Return raw Decibel value (For GUI) */
	float getValueRaw() const override
	{
		//DBG("IN CONST raw");
		return value.load();
	}

	/** Returns decibel value as raw decibel gain.
	 ** Handles both smoothed and unsmoothed variants*/
	float getValueGain()
	{
		if (shouldBeSmoothed)
		{			
			return DecibelConversions::mapProperNormalizedValueToRawDecibelGain(getNextSmoothedValue(), range.start, range.end, midValue);			
		}
		return DecibelConversions::decibelsToDBGain(value.load(), -96.0f);
	}

	/** Returns decibel value as raw decibel gain.
	*** Does not handle smoothed values*/
	float getValueGain() const
	{
		//DBG("In const gain");
		return DecibelConversions::decibelsToDBGain(value.load(), -96.0f);
	}
	/** Parses an input String and converts it to a normalized numeric value based on the param's range */
	float getValueForText(const String& text) const override
	{
		return range.convertTo0to1(text.getFloatValue());
	}
	virtual float getRawValueForText(const String& text) const 
	{
		return 0;
	}

	String getTextFromValue(int length) const override
	{
		return String(value.load());
	}

	virtual float getValueInDecibels() const
	{		
		return getValueRaw();
	}

#pragma region GET/SET	

	void setMinDecibels(const float inMinDecibels) { range.start = inMinDecibels; }	

	void setMaxDecibels(const float inMaxDecibels) { range.end = inMaxDecibels; }

	float getUnityDecibels() const { return unityDecibels; }

	void setUnityDecibels(const float inUnityDecibels) { unityDecibels = inUnityDecibels; }

	void setValueTree() override
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

	float getMinDecibels() const { return getMinValue(); }

	float getMaxDecibels() const { return getMaxValue(); }

	float getMidValue() const { return midValue; }

	void setMidValue(const float inMidValue) { midValue = inMidValue; }

	void setDefaultValue(float inValue) override { defaultValue = inValue; }

	/** Returns default value as normalized 0 to 1 */
	float getDefaultValue() const override
	{
		return DecibelConversions::mapDecibelsToProperNormalizedValue(
			value.load(), range.start, range.end, defaultValue);
	}

	float getMinValue() const { return range.start; }

	void setMinValue(float inValue) { range.start = inValue; }

	float getMaxValue() const { return range.end; }

	void setMaxValue(float inValue) { range.end = inValue; }

	String getText(float inValue, int maxStringLength) const override
	{
		return String(getValueInDecibels(), 2) + " " + String(unitLabel);
	}

#pragma endregion

	void valueChanged(Value& inValue) override
	{
		// DBG("In ZenParameter::valueChanged(value) ");
		setValueTree();
	}
	
	void writeToXML(XmlElement& inXML) override
	{
        //DBG("In DecibelParameter::writeToXML(inXML) ");
		XmlElement* thisXML = inXML.createNewChildElement(this->name);
		thisXML->setAttribute("parameterValue", getValue());
		thisXML->setAttribute("defaultValue", getDefaultValue());
		thisXML->setAttribute("isSmoothed", getShouldBeSmoothed());
		thisXML->setAttribute("minDecibels", getMinDecibels());
		thisXML->setAttribute("maxDecibels", getMaxDecibels());
		thisXML->setAttribute("unityDecibels", getUnityDecibels());
		thisXML->setAttribute("midValue", getMidValue());
	}

	void setFromXML(const XmlElement& inXML) override
	{
		//DBG("In DecibelParameter::setFromXML(inXML) ");
		XmlElement* thisXML = inXML.getChildByName(this->name);
		setValue(thisXML->getDoubleAttribute("parameterValue", -9876.5));
		setDefaultValue(thisXML->getDoubleAttribute("defaultValue", -9876.5));
		setShouldBeSmoothed(thisXML->getBoolAttribute("isSmoothed", false));
		setMinDecibels(thisXML->getDoubleAttribute("minDecibels", -9876.5));
		setMaxDecibels(thisXML->getDoubleAttribute("maxDecibels", -9876.5));
		setUnityDecibels(thisXML->getDoubleAttribute("unityDecibels", -9876.5));
		setMidValue(thisXML->getDoubleAttribute("midValue", -9876.5));
	}

protected:	
	NormalisableRange<float> range;
	float unityDecibels;
	float midValue;
	float defaultValue;	

	virtual double convertDecibelsToLinearWithSetMidpoint(const double& decibels)
	{
        // DBG("In DecibelParameter::convertDecibelsToLinearWithSetMidpoint() with result: " + String(result));
		float result = DecibelConversions::mapDecibelsToProperNormalizedValue(decibels, getMinDecibels(), getMaxDecibels(), unityDecibels);
		return result;
	}

	virtual double convertLinearWithSetMidpointToDecibels(const double& inValue)
	{
        //DBG("In DecibelParameter::convertLinearWithSetMidpointToDecibels() ");
		return DecibelConversions::mapProperNormalizedValueToDecibels(inValue, getMinDecibels(), getMaxDecibels());
	}

	virtual double convertLinearWithSetMidpointToLinear(const double& inValue)
	{
		double linearMPInDecibels = DecibelConversions::mapProperNormalizedValueToDecibels(inValue, getMinDecibels(), getMaxDecibels());
		double decibels = DecibelConversions::convertDecibelsToLinear(linearMPInDecibels, getMinDecibels(), getMaxDecibels());
		return decibels;
	}

	virtual double convertLinearToLinearWithSetMidpoint(const double& inValue)
	{
		double decibels = DecibelConversions::convertLinearToDecibels(inValue, getMinDecibels(), getMaxDecibels()); 
		double linearMP = DecibelConversions::mapDecibelsToProperNormalizedValue(decibels, getMinDecibels(), getMaxDecibels(), unityDecibels);
		return linearMP;
	}
	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZenDecibelParameter);
};

//==============================================================================
//==============================================================================
#ifdef ZEN_UNIT_TESTS

class ZenDecibelParameterTests : public ZenUnitTest
{
public:
	ZenDecibelParameterTests() : ZenUnitTest("ZenDecibelParameter class") {}

	void runTest() override
	{
		beginTest("Get&Set Values");
		
		ZenDecibelParameter decibelParam("testDBID", "Decibel param test",
			-96.0f, 12.0f, 0.0f, 0.0f, 0.0f);
		expectAlmostEqual(decibelParam.getDefaultValue(), 0.5f);
		expectAlmostEqual(decibelParam.getValue(), 0.5f);
		expectAlmostEqual(decibelParam.getValueRaw(), 0.0f);
		expectAlmostEqual(decibelParam.getValueGain(), 1.0f);
		decibelParam.setValueRaw(12.0f);
		expectAlmostEqual(decibelParam.getValue(), 1.0f);
		expectAlmostEqual(decibelParam.getValueRaw(), 12.0f);
		expectAlmostEqual(decibelParam.getValueGain(), 3.98107f);
		decibelParam.setValueRaw(0.0f);
		expectAlmostEqual(decibelParam.getValue(), 0.50f);
		expectAlmostEqual(decibelParam.getValueRaw(), 0.0f);
		expectAlmostEqual(decibelParam.getValueGain(), 1.0f);
		decibelParam.setValueRaw(-18.0f);
		expectAlmostEqual(decibelParam.getValue(), 0.40625f);//11
		expectAlmostEqual(decibelParam.getValueRaw(), -18.0f);
		expectAlmostEqual(decibelParam.getValueGain(), 0.125893f);
		decibelParam.setValueRaw(-96.0f);
		expectAlmostEqual(decibelParam.getValue(), 0.0f);
		expectAlmostEqual(decibelParam.getValueRaw(), -96.0f);
		expectAlmostEqual(decibelParam.getValueGain(), 0.0f);
		decibelParam.setValue(1.0f);
		expectAlmostEqual(decibelParam.getValue(), 1.0f);
		expectAlmostEqual(decibelParam.getValueRaw(), 12.0f);
		expectAlmostEqual(decibelParam.getValueGain(), 3.98107f);
		decibelParam.setValue(0.5f);
		expectAlmostEqual(decibelParam.getValue(), 0.50f); //20
		expectAlmostEqual(decibelParam.getValueRaw(), 0.0f);
		expectAlmostEqual(decibelParam.getValueGain(), 1.0f);
		decibelParam.setValue(0.40625f);
		expectAlmostEqual(decibelParam.getValue(), 0.40625f);
		expectAlmostEqual(decibelParam.getValueRaw(), -18.0f);
		expectAlmostEqual(decibelParam.getValueGain(), 0.125893f);
		decibelParam.setValue(0.0f);
		expectAlmostEqual(decibelParam.getValue(), 0.0f);
		expectAlmostEqual(decibelParam.getValueRaw(), -96.0f);
		expectAlmostEqual(decibelParam.getValueGain(), 0.0f);
		decibelParam.setValueGain(3.98107f);
		expectAlmostEqual(decibelParam.getValue(), 1.0f);
		expectAlmostEqual(decibelParam.getValueRaw(), 12.0f); //30
		expectAlmostEqual(decibelParam.getValueGain(), 3.98107f);
		decibelParam.setValueGain(1.0f);
		expectAlmostEqual(decibelParam.getValue(), 0.50f);
		expectAlmostEqual(decibelParam.getValueRaw(), 0.0f);
		expectAlmostEqual(decibelParam.getValueGain(), 1.0f);
		decibelParam.setValueGain(0.125893f);
		expectAlmostEqual(decibelParam.getValue(), 0.40625f);
		expectAlmostEqual(decibelParam.getValueRaw(), -18.0f);
		expectAlmostEqual(decibelParam.getValueGain(), 0.125893f);
		decibelParam.setValueGain(0.0f);
		expectAlmostEqual(decibelParam.getValue(), 0.0f);
		expectAlmostEqual(decibelParam.getValueRaw(), -96.0f);
		expectAlmostEqual(decibelParam.getValueGain(), 0.0f);				
	}
};

static ZenDecibelParameterTests zenDecibelParameterTests;
#endif // ZEN_UNIT_TESTS

} //Namespace Zen

#endif // ZEN_DECIBEL_PARAMETER_H_INCLUDED



  /*/// <summary> Processes one sample worth of value smoothing and returns the raw decibel gain representing it.
  /// This should be called once for every sample's process block and the result considered
  /// that sample's gain adjustment</summary>
  /// <returns> The smoothed raw decibel gain value. </returns>
  virtual double getSmoothedRawDecibelGainValue()
  {
  return DecibelConversions::mapProperNormalizedValueToRawDecibelGain(getNextSmoothedValue(), getMinDecibels(), getMaxDecibels());
  }*/