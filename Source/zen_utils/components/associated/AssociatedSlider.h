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
#ifndef ZEN_ASSOCIATED_SLIDER_H_INCLUDED
#define ZEN_ASSOCIATED_SLIDER_H_INCLUDED

class AssociatedSlider : public Slider, public AssociatedComponent
{
public:

	AssociatedSlider(const String& componentName, ZenParameter* inAssociatedParam, const String& inLabel = "");

	AssociatedSlider(const String& componentName, ZenParameter* inAssociatedParam, const float& minimumOfRange, const float& maximumOfRange, const float& inStep = 0.01f, const String& inLabel = "");

	virtual ~AssociatedSlider()
	{
	}

	operator ComponentType() const override
	{
		return ASSOCSLIDER;
	}

	void setAssociatedParameterValue() override;

	void setAssociatedParameterValueNotifyingHost() override;

	float getAssociatedParameterValue() override;
	
	String getTextFromValue(double inValue) override;

	double getValueFromText(const String& text) override;

	virtual void setValueFromLinearNormalized(const float& inValue, NotificationType notification);

	virtual void setValueFromHost(const float& inValue, NotificationType notification);

	virtual float getLinearNormalizedValue();

	virtual float getValueFromLinearNormalized(const float& inLinearValue);

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AssociatedSlider);
};

#endif  // ZEN_ASSOCIATED_SLIDER_H_INCLUDED
