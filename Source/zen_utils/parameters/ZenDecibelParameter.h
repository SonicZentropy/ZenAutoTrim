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

 
#ifndef ZEN_DECIBEL_PARAMETER_H_INCLUDED
#define ZEN_DECIBEL_PARAMETER_H_INCLUDED

#include "JuceHeader.h"
#include "ZenParameter.h"
//#include "state/ZenAudioProcessorValueTreeState.h"
#include "utilities/ZenDecibelConversions.hpp"

//class ZenAudioProcessorValueTreeState;

class ZenDecibelParameter : public ZenParameter
{
public:

	/// Constructor for <see cref="ZenDecibelParameter"/> class.
	/// <param name="paramID" type="String">The parameter identifier.</param>
	/// <param name="paramName" type="String">Name of the parameter.</param>
	/// <param name="minValue" type="float">The minimum value.</param>
	/// <param name="maxValue" type="float">The maximum value.</param>
	/// <param name="inDefaultValue" type="float">The in default value.</param>
	/// <param name="shouldBeSmoothed" type="bool">The should be smoothed.</param>
	/// <param name="smoothingTime" type="float">The smoothing time.</param>
	explicit ZenDecibelParameter(ZenAudioProcessorValueTreeState& owner, String paramID, String paramName, float minValue, float maxValue,
		/*float midValue, float unityDecibels, */float inDefaultValue = 0.0f,
		bool shouldBeSmoothed = false, float smoothingTime = 50.0f);

	
	virtual ~ZenDecibelParameter();

	void valueChanged(Value& inValue) override;

	void writeToXML(XmlElement& inXML) override;

	void setFromXML(const XmlElement& inXML) override;
	
	//void valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property) override;

	// Value methods

	/** Sets value from 0to1 normalized (From host) - Stored as decibel */
	void setValue(float inValue) override;

	/** Sets raw decibel value directly (From GUI) */
	void setValueRaw(float inValue) override;

	/** Sets value from Decibel inValue */
	void setValueFromDecibels(float inValue);

	/** Sets raw decibel value from a gain value (From Process Block) */
	void setValueFromGain(float inValue);

	/** Sets value from gain and notifies host */
	void setValueFromGainNotifyingHost(float inValue);

	/** This assumes inValue is IN DECIBELS and coming from NOT the host, since
	* it promptly notifies the host of the change. */
	void setValueNotifyingHost(float inValue) override;

	/** Returns value properly normalized between 0 and 1 with the set midpoint.
	 ** Handles smoothed and non-smoothed values innately*/
	float getValue();

	/** Returns value properly normalized between 0 and 1 with the set midpoint.
	 ** Is not used for smoothed values */
	float getValue() const override;

	/** Returns value IN DECIBELS */
	float getValueRaw() const override;

	/** Returns decibel value as raw decibel gain.
	 ** Handles both smoothed and unsmoothed variants*/
	float getValueInGain();

	/** Returns decibel value as raw decibel gain.
	*** Does not handle smoothed values*/
	float getValueInGain() const;
	
	/** Parses an input String and converts it to a normalized numeric value based on the param's range */
	float getValueForText(const String& text) const override;
	
	virtual float getRawValueForText(const String& text) const;
	
	String getTextFromValue(int length) const override;

	virtual float getValueInDecibels() const;

	// Set/Get (Basic) Methods

	void setMinDecibels(const float inMinDecibels);
		
	void setMaxDecibels(const float inMaxDecibels);

	float getUnityDecibels() const { return unityDecibels; }

	void setUnityDecibels(const float inUnityDecibels) { unityDecibels = inUnityDecibels; }	
	
	float getMinDecibels() const { return getMinValue(); }

	float getMaxDecibels() const { return getMaxValue(); }

	float getMidValue() const { return midValue; }
	
	void setMidValue(const float inMidValue) { midValue = inMidValue; }

	void setDefaultValue(float inValue) override { defaultValue = inValue; }

	float getDefaultValue() const override;

	float getMinValue() const { return range.start; }

	void setMinValue(float inValue) { range.start = inValue; }

	float getMaxValue() const { return range.end; }
		
	void setMaxValue(float inValue)	{ range.end = inValue; }
	
	String getText(float inValue, int maxStringLength = 60) const override;

	String getTextInDB() const { return String(getValueInDecibels(), 2) + " " + String(unitLabel); }
	
	void setValueTree() override;
	
	//void setNewState(const ValueTree& v) override;
	
	//void updateFromValueTree() override;
	
	//void copyValueToValueTree() override;

protected:
	
	//NormalisableRange<float> range;
	
	float unityDecibels;
	
	float midValue;
	
	/* Added based on ProcessorValueTreeState::Parameter*/

	ValueTree state;

	/// Converts decibel to linear with set midpoint.
	/// <param name="decibels" type="const double &">The decibels.</param>
	/// <returns type="double">double.</returns>
	double convertDecibelsToLinearWithSetMidpoint(const double& decibels)
	{
		return DecibelConversions::mapDecibelsToProperNormalizedValue(decibels, getMinDecibels(), getMaxDecibels(), unityDecibels);
	}

	/// Converts the linear with set midpoint to decibels.
	/// <param name="inValue" type="const double &">The in value.</param>
	/// <returns type="double">double.</returns>
	double convertLinearWithSetMidpointToDecibels(const double& inValue)
	{
		return DecibelConversions::mapProperNormalizedValueToDecibels(inValue, getMinDecibels(), getMaxDecibels());
	}

	/// Converts the linear with set midpoint to linear.
	/// <param name="inValue" type="const double &">The in value.</param>
	/// <returns type="double">double.</returns>
	double convertLinearWithSetMidpointToLinear(const double& inValue)
	{
		double linearMPInDecibels = DecibelConversions::mapProperNormalizedValueToDecibels(inValue, getMinDecibels(), getMaxDecibels());
		return DecibelConversions::convertDecibelsToLinear(linearMPInDecibels, getMinDecibels(), getMaxDecibels());
	}

	/// Converts the linear to linear with set midpoint.
	/// <param name="inValue" type="const double &">The in value.</param>
	/// <returns type="double">double.</returns>
	double convertLinearToLinearWithSetMidpoint(const double& inValue)
	{
		double decibels = DecibelConversions::convertLinearToDecibels(inValue, getMinDecibels(), getMaxDecibels());
		return DecibelConversions::mapDecibelsToProperNormalizedValue(decibels, getMinDecibels(), getMaxDecibels(), unityDecibels);
	}
private:
	/// Jucs the e_ declar e_ no n_ copyabl e_ wit h_ lea k_ detector.
	/// <param name="" type="ZenDecibelParameter">The .</param>
	/// <returns type="int">int.</returns>
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZenDecibelParameter);
};

#endif // ZEN_DECIBEL_PARAMETER_H_INCLUDED
