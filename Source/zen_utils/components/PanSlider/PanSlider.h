/*==============================================================================
//  PanSlider.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/10/01
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Panning control component.
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#pragma once
#ifndef PANSLIDER_H_INCLUDED
#define PANSLIDER_H_INCLUDED
#include "JuceHeader.h"
#include "../../components/AssociatedSlider.hpp"
#include "../../parameters/ZenParameter.hpp"

namespace Zen
{

	class PanSlider : public AssociatedSlider
	{
	public:
		virtual String getTextFromValue(double value) override;

		PanSlider(const String& componentName, ZenParameter* associatedParam);
		PanSlider(const String& componentName, ZenParameter* associatedParam, const String& desiredUnitLabel);
		~PanSlider();

	private:

	};
}
#endif // PANSLIDER_H_INCLUDED
