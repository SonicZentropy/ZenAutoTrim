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
#include "utilities/ZenBinaryData.h"

ZenLabelDisplay::ZenLabelDisplay(const String& componentName /* = String::empty */, const String& labelText /* = String::empty */)
	:Label(componentName, labelText),
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

// void ZenLabelDisplay::setSize(int newWidth, int newHeight)
// {
// 	Component::setSize(newWidth, newHeight);
// }

// Image& ZenLabelDisplay::getLabelImageCompositeFromSize(int inWidth, int inHeight)
// {
// 
// }
