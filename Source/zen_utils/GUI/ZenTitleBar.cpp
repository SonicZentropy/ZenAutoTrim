/*==============================================================================
//  ZenTitleBar.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2016/05/19
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Zen implementation of ComboBox GUI component
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#include "ZenTitleBar.h"
#include "zen_utils/utilities/ZenBinaryData.h"

ZenTitleBar::ZenTitleBar(const String& compName, Component* inOwner)
	:Component(compName)
{
	//titleTextImage = ImageCache::getFromMemory(ZenBinaryData::zenTitleBarTextWithEffects_png, ZenBinaryData::zenTitleBarTextWithEffects_pngSize);
	titleTextImage = ImageCache::getFromMemory(ZenBinaryData::zenTitleBarTextWithoutEffects_png, ZenBinaryData::zenTitleBarTextWithoutEffects_pngSize);
	init();
}

ZenTitleBar::ZenTitleBar(const String& compName, Component* inOwner, const Image& inTitleTextImage)
	:Component(compName)
{
	titleTextImage = inTitleTextImage;
	init();
}

void ZenTitleBar::init()
{
	loadImages();

	addAndMakeVisible(enableBtn = new ImageButton("Enable Button"));
	enableBtn->setBounds(9, 7, enableOffImg.getWidth(), enableOffImg.getHeight());
	enableBtn->setImages(false, true, true,
		enableOffImg, 1.0f, Colour(0x00000000),
		enableOffImg, 0.6f, Colour(0x00000000),
		enableOnImg, 1.0f, Colour(0x00000000));
	enableBtn->setToggleState(true, dontSendNotification);
	enableBtn->setClickingTogglesState(true);
	enableBtn->addListener(this);
}

void ZenTitleBar::addEnableListener(Listener* theListener)
{
	enableBtn->addListener(theListener);
}

ZenTitleBar::~ZenTitleBar()
{

}

// void ZenTitleBar::initColors()
// {
// 	this->setColour(backgroundColourId, Colour((uint8)41, 41, 41, 1.0f));
// 	this->setColour(arrowColourId, Colour((uint8)141, 141, 141, 1.0f));
// 	this->setColour(buttonColourId, Colour((uint8)41, 241, 41, 1.0f));
// 	this->setColour(outlineColourId, Colour((uint8)141, 141, 141, 1.0f));
// 	this->setColour(textColourId, Colour((uint8)241, 241, 241, 1.0f));
// }

void ZenTitleBar::loadImages()
{
	backgroundImg = ImageCache::getFromMemory(ZenBinaryData::zenTitleBarBg_png, ZenBinaryData::zenTitleBarBg_pngSize);
	enableOffImg = ImageCache::getFromMemory(ZenBinaryData::zenBypassButtonOff_png, ZenBinaryData::zenBypassButtonOff_pngSize);
	enableOnImg = ImageCache::getFromMemory(ZenBinaryData::zenBypassButtonOn_png, ZenBinaryData::zenBypassButtonOn_pngSize);
}

void ZenTitleBar::paint(Graphics& g)
{
	g.setOpacity(1.0f);
	
	g.drawImage(backgroundImg, bounds.getX(), bounds.getY(), getWidth(), getHeight(),
		0, 0, backgroundImg.getWidth(), backgroundImg.getHeight(), false);

	//enable
// 	g.drawImage(enableOffImg, 
// 		(9), 
// 		(6), 
// 		(enableOffImg.getWidth()), 
// 		(enableOffImg.getHeight()),
// 		0, 0, enableOffImg.getWidth(), enableOffImg.getHeight(), false);

	//Title text
	int startX = (getWidth() / 2);
	startX = startX - (titleTextImage.getWidth() / 2);
	int startY = (getHeight() / 2);
	startY = startY - (titleTextImage.getHeight() / 2);
	g.drawImage(titleTextImage, startX, startY, titleTextImage.getWidth(), titleTextImage.getHeight(),
		0, 0, titleTextImage.getWidth(), titleTextImage.getHeight(), false);
}

void ZenTitleBar::buttonClicked(Button* clickedButton)
{
	//DBG("In ZenTitleBar::buttonClicked(clickedButton) ");
}
