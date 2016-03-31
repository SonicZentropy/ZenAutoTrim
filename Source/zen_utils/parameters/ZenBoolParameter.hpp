/* ==============================================================================
//  BooleanParameter.hpp
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

#pragma once
#ifndef ZEN_BOOLEAN_PARAMETER_H_INCLUDED
#define ZEN_BOOLEAN_PARAMETER_H_INCLUDED

#include "JuceHeader.h"
#include "ZenParameter.hpp"

namespace Zen
{

/** Float 0.0 => False, else True */
class ZenBoolParameter : public ZenParameter
{
public:
	ZenBoolParameter(String parameterID, String paramName, float inDefaultValue = 0.0, String unitLabel = "") 
		: ZenParameter(parameterID, paramName, inDefaultValue, unitLabel, false, 0.0f), defaultValue(inDefaultValue)
	{	}

	ZenBoolParameter(String parameterID, String paramName, bool inDefaultValue = false, String unitLabel = "")
		: ZenParameter(parameterID, paramName, inDefaultValue, unitLabel, false, 0.0f), defaultValue(inDefaultValue)
	{
	}
	
	virtual ~ZenBoolParameter()
	{	}

	void setValue(float newValue) override
	{
		if (newValue == 0.0f)
			value.store(0.0f);
		else value.store(1.0f);
	}

	void setValueRaw(float newValue) override
	{
		if (newValue == 0.0f)
			value.store(0.0f);
		else value.store(1.0f);
	}

	/** This assumes inValue is NOT normalized and coming from NOT the host, since
	* it promptly notifies the host of the change. */
	void setValueNotifyingHost(float inValue) override
	{		
		float boolVal = convertFloatToBoolFloat(inValue);
		processor->setParameterNotifyingHost(getParameterIndex(), boolVal);
		setNeedsUIUpdate(false); //set this to false because change came from GUI
	}

	/**Sets this parameter's value from normalized value. This should never
	*  ever be called, as the GUI does not deal in normalized values */
	/*void setValueNormalizedNotifyingHost(float inValue)
	{
		setValueNotifyingHost(inValue);
	}*/
	
	/**This is called by the HOST/Process block and needs to return
	* a normalized value */
	float getValue() const override { return value.load(); }

	/** Returns raw value direct, for GUI!
	*   Same as getValue() for booleans.*/
	float getValueRaw() const override { return value.load(); }

	String getText(float inValue, int length) const override
	{
		bool text = convertFloatToBoolean(inValue);
		return (text) ? "True" : "False";
	}

	String getText(bool inValue) const
	{		
		return (inValue) ? "True" : "False";
	}

	float getValueForText(const String& text) const override
	{
		if (text == "TRUE" || text == "True" || text == "true") return 1.0;
		return 0.0f;
	}
	String getTextFromValue(int length) const override
	{
		if (value.load() == 0.0f) return "False";
		return "True";
	}

	virtual void setValueFromBool(bool newBool)
	{	
		setValue(convertBooleanToFloat(newBool));
		setNeedsUIUpdate(true);
	}

	/** This assumes inValue is NOT normalized and coming from NOT the host, since
	* it promptly notifies the host of the change. */
	virtual void setValueNotifyingHost(bool newBoolValue) 
	{
		processor->setParameterNotifyingHost(getParameterIndex(), convertBooleanToFloat(newBoolValue));
		setNeedsUIUpdate(false); //set this to false because change came from GUI		
	}

	virtual bool isOn() const { return convertFloatToBoolean(getValue()); }

	virtual bool isOnByDefault() const { return convertFloatToBoolean(getDefaultValue()); }

	virtual bool isOff() const { return !convertFloatToBoolean(getValue()); }

	virtual bool isOffByDefault() const { return !convertFloatToBoolean(getDefaultValue()); }

	virtual bool getValueAsBool() const { return convertFloatToBoolean(getValue()); }

	void setDefaultValue(float inValue) override { defaultValue = convertFloatToBoolFloat(inValue); }

	void setDefaultValueFromBool(bool inValue)	{ defaultValue = convertBooleanToFloat(inValue); }
	
	float getDefaultValue() const override	{ return defaultValue; }
	
	virtual bool getDefaultValueAsBool() const
	{
		return convertFloatToBoolean(getDefaultValue());
	}

	void writeToXML(XmlElement& inXML) override
	{
		// DBG("In ZenParameter::writeToXML(inXML) ");
		XmlElement* thisXML = inXML.createNewChildElement(this->name);
		thisXML->setAttribute("parameterValue", getValue());
		thisXML->setAttribute("defaultValue", getDefaultValue());
		//thisXML->setAttribute("isSmoothed", getShouldBeSmoothed());
	}

	void setFromXML(const XmlElement& inXML) override
	{
		// DBG("In ZenParameter::setFromXML(inXML) ");
		XmlElement* thisXML = inXML.getChildByName(this->name);
		setValue(thisXML->getDoubleAttribute("parameterValue", false));
		setDefaultValue(thisXML->getDoubleAttribute("defaultValue", false));
		//setShouldBeSmoothed(thisXML->getBoolAttribute("isSmoothed", false));
	}

	/** Returns the number of discrete interval steps that this parameter's range
	should be quantised into.

	If you want a continuous range of values, don't override this method, and allow
	the default implementation to return AudioProcessor::getDefaultNumParameterSteps().
	If your parameter is boolean, then you may want to make this return 2.
	The value that is returned may or may not be used, depending on the host.
	*/
	int getNumSteps() const override	{ return 2;	}

protected:
	float defaultValue;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZenBoolParameter);

};

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

		ZenBoolParameter boolParam("boolTest", "Bool Test");

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

}//namespace
#endif   // ZEN_BOOLEAN_PARAMETER_H_INCLUDED