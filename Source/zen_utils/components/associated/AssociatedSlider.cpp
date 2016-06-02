#include "ZenHeader.h"

AssociatedSlider::AssociatedSlider(const String& componentName, ZenParameter* inAssociatedParam, const String& inLabel /*= ""*/) : Slider(componentName)
, AssociatedComponent(inAssociatedParam, inLabel)
{
	setRange(0.0f, 1.0f, 0.0);
	setValueFromHost(associatedParam->getValue(), dontSendNotification);
	associatedParam->setUnitLabel(inLabel);
}

AssociatedSlider::AssociatedSlider(const String& componentName, ZenParameter* inAssociatedParam, const float& minimumOfRange, const float& maximumOfRange, const float& inStep /*= 0.01f*/, const String& inLabel /*= ""*/) : Slider(componentName)
, AssociatedComponent(inAssociatedParam, inLabel)
{
	setRange(minimumOfRange, maximumOfRange, inStep);
	//setValue(associatedParam->getValueForGUIComponent(), dontSendNotification);			
	setValueFromHost(associatedParam->getValue(), dontSendNotification);
	associatedParam->setUnitLabel(inLabel);
}

void AssociatedSlider::setAssociatedParameterValue()
{
	associatedParam->setValue(getLinearNormalizedValue());
}

void AssociatedSlider::setAssociatedParameterValueNotifyingHost()
{
	DBG("In AssociatedSlider::setAssociatedParameterValueNotifyingHost() about to set associatedParam to: " + String(getLinearNormalizedValue()));
	associatedParam->setValueNotifyingHost(getLinearNormalizedValue());
}

float AssociatedSlider::getAssociatedParameterValue()
{
	return associatedParam->getValue();
}

String AssociatedSlider::getTextFromValue(double inValue)
{
	std::stringstream numberFormatter;
	numberFormatter.precision(2);
	//change getValue to getValueFromLinearNormalized
	//float nearest = roundf(val * 100) / 100;

	numberFormatter << std::fixed << getValue() + 0.0001f << getTextValueSuffix();
	String result = numberFormatter.str();
	return result;
}

double AssociatedSlider::getValueFromText(const String& text)
{
	//convert from decibel to skewed value here
	String t(text.trimStart());

	if (t.endsWith(getTextValueSuffix()))
		t = t.substring(0, t.length() - associatedParam->getUnitLabel().length());

	while (t.startsWithChar('+'))
		t = t.substring(1).trimStart();

	return t.initialSectionContainingOnly("0123456789.,-")
		.getDoubleValue();
}

void AssociatedSlider::setValueFromLinearNormalized(const float& inValue, NotificationType notification)
{
	jassert(inValue <= 1.0f && inValue >= 0.0f);
	float rawMin = getMinimum();
	float rawMax = getMaximum();
	//float denormalizedValue = (rawMin + (getMaximum() - rawMin) * inValue);
	float maxMin = rawMax - rawMin;
	float mult = inValue * maxMin;
	float denormalizedValue = mult + rawMin;
	setValue(denormalizedValue, notification);
}

void AssociatedSlider::setValueFromHost(const float& inValue, NotificationType notification)
{
	setValueFromLinearNormalized(inValue, notification);
}

float AssociatedSlider::getLinearNormalizedValue()
{
	float rawMin = getMinimum();
	float rawMax = getMaximum();

	return (getValue() - rawMin) / (rawMax - rawMin);
}

float AssociatedSlider::getValueFromLinearNormalized(const float& inLinearValue)
{
	float newValue = (inLinearValue*(getMaximum() - getMinimum())) + getMinimum();
	return newValue;
}

