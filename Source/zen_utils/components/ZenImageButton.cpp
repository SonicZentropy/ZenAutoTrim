/*==============================================================================
//  ZenImageButton.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 12 May 2016 7:15:54am
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Generic Component Class
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

//#include "JuceHeader.h"
#include "ZenImageButton.h"
#include "zen_utils/utilities/ZenBinaryData.h"
#include "zen_utils/GUI/ZenLookAndFeel.h"


//==============================================================================
ZenImageButton::ZenImageButton(const String& buttonName, const String& buttonText_)
	:ImageButton(buttonName)
	,textColorButtonOn(Colour(29, 212, 12 ))
	,textColorButtonOff(Colours::lightgrey)
	,buttonText(buttonText_)
{	
	normalLeft   = ImageCache::getFromMemory(ZenBinaryData::zenImageButton_left_png, ZenBinaryData::zenImageButton_left_pngSize);
	normalRight  = ImageCache::getFromMemory(ZenBinaryData::zenImageButton_right_png, ZenBinaryData::zenImageButton_right_pngSize);
	normalCenter = ImageCache::getFromMemory(ZenBinaryData::zenImageButton_center_png, ZenBinaryData::zenImageButton_center_pngSize);
	
	hoverLeft = ImageCache::getFromMemory(ZenBinaryData::zenImageButton_leftHover_png, ZenBinaryData::zenImageButton_leftHover_pngSize);
	hoverRight = ImageCache::getFromMemory(ZenBinaryData::zenImageButton_rightHover_png, ZenBinaryData::zenImageButton_rightHover_pngSize);
	hoverCenter = ImageCache::getFromMemory(ZenBinaryData::zenImageButton_centerHover_png, ZenBinaryData::zenImageButton_centerHover_pngSize);

	pressedLeft   = ImageCache::getFromMemory(ZenBinaryData::zenImageButton_leftDown_png, ZenBinaryData::zenImageButton_leftDown_pngSize);
	pressedRight  = ImageCache::getFromMemory(ZenBinaryData::zenImageButton_rightDown_png, ZenBinaryData::zenImageButton_rightDown_pngSize);
	pressedCenter = ImageCache::getFromMemory(ZenBinaryData::zenImageButton_centerDown_png, ZenBinaryData::zenImageButton_centerDown_pngSize);

	pressedHoverLeft   = ImageCache::getFromMemory(ZenBinaryData::zenImageButton_activeHoverLeft_png, ZenBinaryData::zenImageButton_activeHoverLeft_pngSize);
	pressedHoverRight  = ImageCache::getFromMemory(ZenBinaryData::zenImageButton_activeHoverRight_png, ZenBinaryData::zenImageButton_activeHoverRight_pngSize);
	pressedHoverCenter = ImageCache::getFromMemory(ZenBinaryData::zenImageButton_activeHoverCenter_png, ZenBinaryData::zenImageButton_activeHoverCenter_pngSize);
}

ZenImageButton::~ZenImageButton()
{
}

// bool ZenImageButton::hitTest(int x, int y)
// {
// 
// }

void ZenImageButton::paintButton(Graphics& g, bool isMouseOver, bool isButtonDown)
{
	// Check to ensure the proper L&F has been applied
	jassert(nullptr != dynamic_cast<ZenLookAndFeel*>(&getLookAndFeel()));
	if (!isEnabled())
	{
		isMouseOver = false;
		isButtonDown = false;
		return;
	}

	// Avoid dynamic cast check here via the above jassert, as the type is compile time static
	ZenLookAndFeel* zenLF = static_cast<ZenLookAndFeel*>(&getLookAndFeel());
	zenLF->drawZenImageButton(g, *this);
	
}

// void ZenImageButton::paint (Graphics& g)
// {
//     /* This demo code just fills the component's background and
//        draws some placeholder text to get you started.
// 
//        You should replace everything in this method with your own
//        drawing code..
//     */
// 
//     g.fillAll (Colours::white);   // clear the background
// 
//     g.setColour (Colours::grey);
//     g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
// 
//     g.setColour (Colours::lightblue);
//     g.setFont (14.0f);
//     g.drawText ("ZenImageButton", getLocalBounds(),
//                 Justification::centred, true);   // draw some placeholder text
// }
// 
// void ZenImageButton::resized()
// {
//     // This method is where you should set the bounds of any child
//     // components that your component contains..
// 
// }
