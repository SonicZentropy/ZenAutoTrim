/*==============================================================================
//  ZenIntParameter.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 4 Jun 2016 3:12:29pm
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Generic Header File
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef ZENINTPARAMETER_H_INCLUDED
#define ZENINTPARAMETER_H_INCLUDED

#include "JuceHeader.h"
#include "parameters/ZenParameter.h"

class ZenIntParameter : public ZenParameter
{
public:
	ZenIntParameter(ZenAudioProcessorValueTreeState& inOwner, String parameterID, String paramName,
		int minValue, int maxValue,
		float inDefaultIntValue = 0.0f, String unitLabel = "");

	ZenIntParameter(ZenAudioProcessorValueTreeState& inOwner, String parameterID, String paramName,
		int minValue, int maxValue,
		int inDefaultIntValue = 0, String unitLabel = "");

	virtual ~ZenIntParameter() {};

	void setValue(float newValue) override;
	void setValue(int newIntValue);

	void setValueRaw(float newValue) override;

	void setValueRaw(int newValue);
	/** This assumes inValue is NOT normalized and coming from NOT the host, since
	* it promptly notifies the host of the change. */
	void setValueNotifyingHost(float inValue) override;

	/**This is called by the HOST/Process block and needs to return
	* a normalized value */
	float getValue() const override;

	/** Returns raw value direct, for GUI!
	*   Same as getValue() for booleans.*/
	float getValueRaw() const override;
	int getValueAsInt() const;

	String getText(float inValue, int length) const override;

	float getValueForText(const String& text) const override;

	String getTextFromValue() const override;

	void updateFromValueTree() override;
	void copyValueToValueTree() override;
	void valueTreePropertyChanged(ValueTree& vt, const Identifier& prop) override;

	/** This assumes inValue is NOT normalized and coming from NOT the host, since
	* it promptly notifies the host of the change. */
	void setDefaultValue(float inValue) override;

	float getDefaultValue() const override;	

	/** Returns the number of discrete interval steps that this parameter's range
	should be quantised into.

	If you want a continuous range of values, don't override this method, and allow
	the default implementation to return AudioProcessor::getDefaultNumParameterSteps().
	If your parameter is boolean, then you may want to make this return 2.
	The value that is returned may or may not be used, depending on the host. */
	int getNumSteps() const override;
	
protected:
	float defaultValue;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZenIntParameter);
};

#endif  // ZENINTPARAMETER_H_INCLUDED
