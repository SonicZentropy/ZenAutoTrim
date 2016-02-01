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

#pragma warning (disable : 4100 )

#include "JuceHeader.h"
#include "ZenParameter.hpp"



namespace Zen
{

class BooleanParameter : public ZenParameter
{

public:

	BooleanParameter()
	{
		jassertfalse;
	}
	BooleanParameter(const String& parameterName, const float& defaultParameterValue)
		: ZenParameter(parameterName, defaultParameterValue, false)
	{	}

	BooleanParameter(const String& parameterName, const bool& defaultBooleanValue)
		: ZenParameter(parameterName, convertBooleanToFloat(defaultBooleanValue), false)
	{	}

	virtual ~BooleanParameter()
	{	}

	virtual void BooleanParameter::setValueFromBool(bool newBool)
	{	
		setValue(convertBooleanToFloat(newBool));
		requestUIUpdate = true;
	}

	virtual void BooleanParameter::setValueNotifyingHost(bool newBoolValue) 
	{
		ZenParameter::setValueNotifyingHost(convertBooleanToFloat(newBoolValue));
		requestUIUpdate = false;
		
	}

	virtual bool isOn() const
	{
		return convertFloatToBoolean(getValue());
	}

	virtual bool isOnByDefault() const
	{
		return convertFloatToBoolean(getDefaultValue());
	}

	virtual bool isOff() const
	{
		return !convertFloatToBoolean(getValue());
	}

	virtual bool isOffByDefault() const
	{
		return !convertFloatToBoolean(getDefaultValue());
	}

	virtual float getValueAsFloat() const
	{
		return getValue();
	}

	virtual bool getValueAsBool() const
	{
		return convertFloatToBoolean(getValue());
	}

	virtual float getDefaultValueAsFloat() const
	{
		return getDefaultValue();
	}

	virtual bool getDefaultValueAsBool() const
	{
		return convertFloatToBoolean(getDefaultValue());
	}

	virtual void writeToXML(XmlElement& inXML) override
	{
		//		DBG("In ZenParameter::writeToXML(inXML) ");
		XmlElement* thisXML = inXML.createNewChildElement(this->name);
		thisXML->setAttribute("parameterValue", getValue());
		thisXML->setAttribute("defaultValue", getDefaultValue());
		//thisXML->setAttribute("isSmoothed", getShouldBeSmoothed());
	}

	virtual void setFromXML(const XmlElement& inXML) override
	{
		//		DBG("In ZenParameter::setFromXML(inXML) ");
		XmlElement* thisXML = inXML.getChildByName(this->name);
		setValue(thisXML->getDoubleAttribute("parameterValue", -9876.5));
		setDefaultValue(thisXML->getDoubleAttribute("defaultValue", -9876.5));
		//setShouldBeSmoothed(thisXML->getBoolAttribute("isSmoothed", false));
	}

protected:

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BooleanParameter);
};
}//namespace
#endif   // ZEN_BOOLEAN_PARAMETER_H_INCLUDED