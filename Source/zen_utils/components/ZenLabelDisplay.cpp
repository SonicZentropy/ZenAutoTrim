/*==============================================================================
//  ZenLabelDisplay.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 10 May 2016 6:19:48pm
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Creates a label component optimized just for display (No editing)
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#include "ZenLabelDisplay.h"
#include "zen_utils/utilities/ZenBinaryData.h"
#include "ZenCompUtils.hpp"

ZenLabelDisplay::ZenLabelDisplay(const String& componentName /* = String::empty */, const String& labelText /* = String::empty */)
	:Label(componentName, labelText),
	shouldScaleText(false),
	fontMarginSize(0),
	hasGetTextFunction(false)
{
	setEditable(false, false, false);
	//labelImage = ImageCache::getFromMemory(ZenBinaryData::zenLabelDisplay_png, ZenBinaryData::zenLabelDisplay_pngSize);	
	
	labelTLCornerImage = ImageCache::getFromMemory(ZenBinaryData::zenLabelDisplay_topLeftCorner_png, ZenBinaryData::zenLabelDisplay_topLeftCorner_pngSize);
	labelTRCornerImage = ImageCache::getFromMemory(ZenBinaryData::zenLabelDisplay_topRightCorner_png, ZenBinaryData::zenLabelDisplay_topRightCorner_pngSize);
	labelBRCornerImage = ImageCache::getFromMemory(ZenBinaryData::zenLabelDisplay_bottomRightCorner_png, ZenBinaryData::zenLabelDisplay_bottomRightCorner_pngSize);
	labelBLCornerImage = ImageCache::getFromMemory(ZenBinaryData::zenLabelDisplay_bottomLeftCorner_png, ZenBinaryData::zenLabelDisplay_bottomLeftCorner_pngSize);

	labelTopLineImg = ImageCache::getFromMemory(ZenBinaryData::zenLabelDisplay_topLine_png, ZenBinaryData::zenLabelDisplay_topLine_pngSize);
	labelBottomLineImg = ImageCache::getFromMemory(ZenBinaryData::zenLabelDisplay_bottomLine_png, ZenBinaryData::zenLabelDisplay_bottomLine_pngSize);
	labelLeftLineImg = ImageCache::getFromMemory(ZenBinaryData::zenLabelDisplay_leftLine_png, ZenBinaryData::zenLabelDisplay_leftLine_pngSize);
	labelRightLineImg = ImageCache::getFromMemory(ZenBinaryData::zenLabelDisplay_rightLine_png, ZenBinaryData::zenLabelDisplay_rightLine_pngSize);

	//labelInteriorImg = ImageCache::getFromMemory(ZenBinaryData::zenLabelDisplay_interior_png, ZenBinaryData::zenLabelDisplay_interior_pngSize);
	
	this->setColour(textColourId, Colours::lightgrey);
	setJustificationType(Justification::centred);
	setBorderSize(BorderSize<int>(0));
	setBoundsInset(BorderSize<int>(0));

	//setFunc([&](string sIn) -> void { DBG(sIn); DBG("SETFUNC"); });
	
}

ZenLabelDisplay::~ZenLabelDisplay()
{
}


const String ZenLabelDisplay::getText()
{
	if (hasGetTextFunction)
	{
		return getTextFunction(this);		
	}
	return Label::getText();
	
}

void ZenLabelDisplay::setFontSizeForBounds()
{
	if (shouldScaleText)
		setFontSizeForBounds(getWidth(), getHeight(), getText(), fontMarginSize);
}

void ZenLabelDisplay::setFontSizeAndScaleForBounds()
{
	if (shouldScaleText)
		setFontSizeAndScaleForBounds(getWidth(), getHeight(), getText(), fontMarginSize);
}

void ZenLabelDisplay::setFontSizeForBounds(int margin)
{
	if(shouldScaleText)
		setFontSizeForBounds(getWidth(), getHeight(), getText(), margin);
}

void ZenLabelDisplay::setFontSizeAndScaleForBounds(int margin)
{
	if (shouldScaleText)
		setFontSizeAndScaleForBounds(getWidth(), getHeight(), getText(), margin);
}

void ZenLabelDisplay::setFontSizeForBounds(int w, int h, const String& text, int margin /*= 0*/)
{
// 	font.setHeight(h);
// 	font.setHorizontalScale(1);
// 
// 	// round up to pixel 
// 	int fw = font.getStringWidthFloat(text) + 0.5f;
// 
// 	if (fw + margin > w)
// 	{
// 		float s = (w - margin) / (float)fw;
// 		h = h*s; // round down to pixel
// 	}
// 	font.setHeight(h);
	if (shouldScaleText)
		ZenCompUtils::setFontSizeForBounds(font, w, h, text, margin);
}

void ZenLabelDisplay::setFontSizeAndScaleForBounds(int w, int h,
	const String& text,
	float minScale /*= 0.8f*/)
{
// 	font.setHeight(h);
// 	font.setHorizontalScale(1);
// 
// 	float scale = 1;
// 	float fw = font.getStringWidth(text);
// 	if (fw > w)
// 		scale = w / fw;
// 
// 	if (scale < minScale)
// 	{
// 		int h2 = (int)((w / minScale) / fw * h);
// 		font.setHeight(h2);
// 
// 		fw = font.getStringWidth(text);
// 		if (fw > w)
// 			scale = w / fw;
// 	}
// 
// 	font.setHorizontalScale(scale);
	if (shouldScaleText)
		ZenCompUtils::setFontSizeAndScaleForBounds(font, w, h, text, minScale);
}

// void ZenLabelDisplay::setSize(int newWidth, int newHeight)
// {
// 	Component::setSize(newWidth, newHeight);
// }

// Image& ZenLabelDisplay::getLabelImageCompositeFromSize(int inWidth, int inHeight)
// {
// 
// }
