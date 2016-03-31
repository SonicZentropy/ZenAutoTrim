/*==============================================================================
//  ZenParameter.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/08
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Base Audio Parameter class (Virtual only)
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/
#pragma warning(disable:4100 4305)

#pragma once
#include <atomic>
#include "ZenLinearSmoothedValue.hpp"
#ifndef ZEN_PARAMETER_H_INCLUDED
#define ZEN_PARAMETER_H_INCLUDED

#include "JuceHeader.h"
#include "../utilities/ZenUtils.hpp"

namespace Zen
{
using namespace juce;
using AtomicFloat = std::atomic<float>;
using AtomicBool = std::atomic<bool>;

class ZenParameter : public AudioProcessorParameterWithID, public ReferenceCountedObject,
                     public ValueListener, public ZenLinearSmoothedValue
{
public:

	ZenParameter(String paramID, String paramName, double inValue, String inUnitLabel = "",
	             bool inShouldBeSmoothed = false, float inSmoothingTime = 0.0f)
		: AudioProcessorParameterWithID(paramID, paramName),
		  ZenLinearSmoothedValue(inShouldBeSmoothed, inSmoothingTime),
		  value(inValue), unitLabel(inUnitLabel)
	{	}

	/** Set this parameter's value from 0to1 normalized input value.
	* Should be called only by HOST/Process Block, never GUI */
	void setValue(float newValue) override = 0;

	/** Directly sets value from raw input, called by GUI */
	virtual void setValueRaw(float newValue) = 0;

	/** This assumes inValue is NOT normalized and coming from NOT the host, since
	* it promptly notifies the host of the change. */
	void setValueNotifyingHost(float inValue) override = 0;

	/**Sets this parameter's value from normalized value. This should never
	*  ever be called, as the GUI does not deal in normalized values */
	//virtual void setValueNormalizedNotifyingHost(float inValue) = 0;

	/**This is called by the HOST/Process block and needs to return
	* a normalized value */
	float getValue() const override = 0;

	/** Returns raw value direct, for GUI! */
	virtual float getValueRaw() const = 0;

	void valueChanged(Value& inValue) override
	{
		DBG("In ZenParameter's value changed listener callback: " << inValue.getValue().toString());
	}

	virtual float getRawValueForText(const String& text)
	{
		return text.getFloatValue();
	}

	/** Returns input value as text string */
	String getText(float inValue, int length/*maximumStringLength*/) const override = 0;

	/** Parses an input String and converts it to a normalized numeric value based on the param's range */
	float getValueForText(const String& text) const override = 0;

	/** Returns this parameter's current value as text string */
	virtual String getTextFromValue(int length /*Max string length*/) const = 0;

	virtual void writeToXML(XmlElement& inXML) = 0;

	virtual void setFromXML(const XmlElement& inXML) = 0;

	virtual void setDefaultValue(float inValue) = 0;

	/** Returns default value as normalized 0 to 1 */
	float getDefaultValue() const override = 0;

	virtual ValueTree getValueTree()
	{
		return *paramValueTree;
	}

	virtual void setValueTree()
	{
		paramValueTree->removeAllChildren(nullptr);
		paramValueTree->removeAllProperties(nullptr);

		paramValueTree->setProperty("parameterValue", getValue(), nullptr);
		paramValueTree->setProperty("defaultValue", getDefaultValue(), nullptr);
	}


	virtual void setDisplayPrecision(unsigned int inPrecision)
	{
		precision = inPrecision;
	}

	virtual unsigned int getDisplayPrecision() const
	{
		return precision;
	}

	void setNeedsUIUpdate(bool needsUpdate) noexcept
	{
		UIUpdate.store(needsUpdate);
	}

	bool getNeedsUIUpdate() const noexcept
	{
		return UIUpdate.load();
	}

	bool needsUIUpdate() const noexcept
	{
		return UIUpdate.load();
	}

	virtual bool needsUIUpdate()
	{
		return getNeedsUIUpdate();
	}

	virtual void resetUIUpdate()
	{
		setNeedsUIUpdate(false);
	}

	virtual bool checkUIUpdateAndReset()
	{
		bool expected = true;
		UIUpdate.compare_exchange_strong(expected, false);
		return expected;
	}

	String getLabel() const override
	{
		return unitLabel;
	}

	virtual String getUnitLabel() const
	{
		return unitLabel;
	}

	virtual void setUnitLabel(String inLabel)
	{
		unitLabel = inLabel;
	}

	String getName() const
	{
		return name;
	}

protected:
	AtomicFloat value;
	AtomicBool UIUpdate;
	unsigned int precision = 2;
	String unitLabel = "", description = "";
	
	ScopedPointer<ValueTree> paramValueTree;
	// Smoothing fields
	//double currentSmoothedValue = 0, stepsToTarget = 0, target = 0, step = 0, countdown = 0, smoothTime;
	//bool shouldBeSmoothed = false;
	//==============================================================================

private:

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZenParameter);
};
} // Namespace Zen
#endif // ZEN_PARAMETER_H_INCLUDED


