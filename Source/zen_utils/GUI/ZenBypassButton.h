/*==============================================================================
//  ZenBypassButton.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2016/05/22
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Plugin bypass toggle button
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#pragma once
#ifndef ZENBYPASSBUTTON_H_INCLUDED
#define ZENBYPASSBUTTON_H_INCLUDED
#include <JuceHeader.h>

class ZenBypassButton : public ImageButton
{
public:	
	ZenBypassButton(const String& compName = "ZenBypassButton");
	~ZenBypassButton();

protected:
	void paintButton(Graphics&, bool isMouseOver, bool isButtonDown) override;
	Image bypassOnImage, bypassOffImage;
	
	
};

#endif // ZENBYPASSBUTTON_H_INCLUDED
