/* ==============================================================================
//  AssociatedSlider.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/08
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Contains a Slider component and its associated Parameter
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#pragma once
#ifndef ZEN_ASSOCIATED_SLIDER_H_INCLUDED
#define ZEN_ASSOCIATED_SLIDER_H_INCLUDED

#include "JuceHeader.h"
#include <sstream>
#include "../parameters/ZenParameter.hpp"
#include "AssociatedComponent.hpp"
#include "../debug/ZenDebugEditor.h"


namespace Zen
{

	class AssociatedSlider : public Slider, public AssociatedComponent
	{
	public:

		AssociatedSlider(const String& componentName, ZenParameter* inAssociatedParam, const String& inLabel = "")
			: Slider(componentName)
			, AssociatedComponent(inAssociatedParam, inLabel)
		{		
			setRange(0.0f, 1.0f, 0.0);
			setValueFromHost(associatedParam->getValue(), dontSendNotification);
			associatedParam->setUnitLabel(inLabel);
		}

		AssociatedSlider(const String& componentName, ZenParameter* inAssociatedParam, const float& minimumOfRange, const float& maximumOfRange, const float& inStep = 0.01f, const String& inLabel = "")
			: Slider(componentName)
			, AssociatedComponent(inAssociatedParam, inLabel)		 
		{
			setRange(minimumOfRange, maximumOfRange, inStep);
			//setValue(associatedParam->getValueForGUIComponent(), dontSendNotification);			
			setValueFromHost(associatedParam->getValue(), dontSendNotification);
			associatedParam->setUnitLabel(inLabel);
		}

		virtual ~AssociatedSlider()
		{
			
		}

		operator ComponentType() const override
		{ return ASSOCSLIDER; }

		virtual void setAssociatedParameterValue() override
		{
			associatedParam->setValue(getLinearNormalizedValue());

		}

		virtual void setAssociatedParameterValueNotifyingHost() override
		{
			DBG("In AssociatedSlider::setAssociatedParameterValueNotifyingHost() about to set associatedParam to: " + String(getLinearNormalizedValue()));
			ZEN_LABEL_TRACE("Slider raw value: ", String(getValue()));
			ZEN_LABEL_TRACE("Slider Linear value: ", String(getLinearNormalizedValue()));
			associatedParam->setValueNotifyingHost(getLinearNormalizedValue());
		}

		virtual float getAssociatedParameterValue() override
		{
			return associatedParam->getValue();
		}
		//String Slider::getTextFromValue(double v) 

		String getTextFromValue(double inValue) override
		{
			std::stringstream numberFormatter;
			numberFormatter.precision(2);
			//change getValue to getValueFromLinearNormalized
			//float nearest = roundf(val * 100) / 100;

			numberFormatter << std::fixed << getValue()+0.0001f << getTextValueSuffix();
			String result = numberFormatter.str();					
			return result;			
		}

/*
		String getTextFromValue()
		{
			std::stringstream numberFormatter;
			numberFormatter.precision(2);
			numberFormatter << std::fixed << getValue() << associatedParam->getUnitLabel();
			String result = numberFormatter.str();
			return result;
		}*/

		virtual double getValueFromText(const String& text) override
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
		

		virtual void setValueFromLinearNormalized(const float& inValue, NotificationType notification)
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

		virtual void setValueFromHost(const float& inValue, NotificationType notification)
		{
			setValueFromLinearNormalized(inValue, notification);
		}

		virtual float getLinearNormalizedValue()
		{
			float rawMin = getMinimum();
			float rawMax = getMaximum();

			return (getValue() - rawMin) / (rawMax - rawMin);
		}

		virtual float getValueFromLinearNormalized(const float& inLinearValue)
		{
			float newValue = (inLinearValue*(getMaximum() - getMinimum())) + getMinimum();
			return newValue;
		}

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AssociatedSlider);
	
	};
}
#endif  // ZEN_ASSOCIATED_SLIDER_H_INCLUDED