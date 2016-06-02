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
#include "AssociatedComponent.h"
#include "parameters/ZenParameter.h"
 
/// <summary>
/// Wrapper for a processor parameter and its text button GUI representation.
/// No longer needed as of JUCE4, here only for backwards compatibility
/// </summary>
/// <seealso cref="TextButton" />
/// <seealso cref="AssociatedComponent" />
class AssociatedTextButton : public virtual TextButton, public AssociatedComponent
{
public:

	AssociatedTextButton(const String& componentName, ZenParameter* inAssociatedParam, const String& inLabel = "");

	void setAssociatedParameterValue() override;

	void setAssociatedParameterValueNotifyingHost() override;

	float getAssociatedParameterValue() override;

	float getValueFromToggle() const;

	virtual bool isOn() const;

	operator ComponentType() const override	{ return ASSOCTEXTBUTTON; }

protected:
	void paintButton(Graphics& g, bool isMouseOverButton, bool isButtonDown) override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AssociatedTextButton);

};

#endif  // ZEN_ASSOCIATED_TEXT_BUTTON_H_INCLUDED