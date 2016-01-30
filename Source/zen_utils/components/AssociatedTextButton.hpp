/* ==============================================================================
//  AssociatedButton.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/08
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details:  Contains a Button component and its associated Parameter
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef ZEN_ASSOCIATED_TEXT_BUTTON_H_INCLUDED
#define ZEN_ASSOCIATED_TEXT_BUTTON_H_INCLUDED
#include "JuceHeader.h"


namespace Zen
{

class AssociatedTextButton : public virtual TextButton, public AssociatedComponent
{

public:

	AssociatedTextButton(const String& componentName, ZenParameter* inAssociatedParam, const String& inLabel = "")
		: TextButton(componentName)
		, AssociatedComponent(inAssociatedParam)
	{
		setToggleState(associatedParam->getBoolFromValue(), sendNotification);
		associatedParam->setUnitLabel(inLabel);
	}

	operator ComponentType() const override
	{
		return ASSOCTEXTBUTTON;
	}

	void setAssociatedParameterValue() override
	{
		associatedParam->setValue(getValueFromToggle());
	}

	void setAssociatedParameterValueNotifyingHost() override
	{
		associatedParam->setValueNotifyingHost(this->getValueFromToggle());
	}

	float getAssociatedParameterValue() override
	{
		return associatedParam->getValue();
	}

	float getValueFromToggle()
	{
		return (getToggleState() == true) ? 1.0 : 0.0;
	}

	virtual bool isOn() const
	{
		return getToggleState();
	}

protected:
	virtual void paintButton(Graphics& g, bool isMouseOverButton, bool isButtonDown) override
	{
		TextButton::paintButton(g, isMouseOverButton, isButtonDown);
	}

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AssociatedTextButton);

};
} // Namespace
#endif  // ZEN_ASSOCIATED_TEXT_BUTTON_H_INCLUDED