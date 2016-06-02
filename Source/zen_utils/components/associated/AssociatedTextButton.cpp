#include "AssociatedTextButton.h"
#include "utilities/ZenUtils.hpp"

AssociatedTextButton::AssociatedTextButton(const String& componentName, ZenParameter* inAssociatedParam, const String& inLabel /*= ""*/) : TextButton(componentName)
, AssociatedComponent(inAssociatedParam)
{
	bool boolValue = convertFloatToBoolean(inAssociatedParam->getValue());
	setToggleState(boolValue, sendNotification);
	associatedParam->setUnitLabel(inLabel);
}

void AssociatedTextButton::setAssociatedParameterValue()
{
	associatedParam->setValue(getValueFromToggle());
}

void AssociatedTextButton::setAssociatedParameterValueNotifyingHost()
{
	associatedParam->setValueNotifyingHost(this->getValueFromToggle());
}

float AssociatedTextButton::getAssociatedParameterValue()
{
	return associatedParam->getValue();
}

float AssociatedTextButton::getValueFromToggle() const
{
	return (getToggleState() == true) ? 1.0 : 0.0;
}

bool AssociatedTextButton::isOn() const
{
	return getToggleState();
}

void AssociatedTextButton::paintButton(Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
	TextButton::paintButton(g, isMouseOverButton, isButtonDown);
}
