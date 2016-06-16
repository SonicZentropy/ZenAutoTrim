/*==============================================================================
//  ZenComboBox.cpp
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

#include "ZenComboBox.h"
#include "zen_utils/GUI/ZenLookAndFeel.h"
#include "zen_utils/utilities/ZenBinaryData.h"

ZenComboBox::ZenComboBox(const String& compName)
	:ComboBox(compName)
{
	initColors();
	loadImages();
	label->setColour(Label::ColourIds::backgroundColourId, Colour((uint8)41, 41, 41, 1.0f));
}

ZenComboBox::~ZenComboBox()
{
	
}

void ZenComboBox::initColors()
{
	this->setColour(backgroundColourId, Colour((uint8)41, 41, 41, 1.0f));
	this->setColour(arrowColourId, Colour((uint8)141, 141, 141, 1.0f));
	this->setColour(buttonColourId, Colour((uint8)41, 241, 41, 1.0f));
	this->setColour(outlineColourId, Colour((uint8)141, 141, 141, 1.0f));
	this->setColour(textColourId, Colour((uint8)241, 241, 241, 1.0f));
}

void ZenComboBox::loadImages()
{
	leftImage = ImageCache::getFromMemory(ZenBinaryData::zenComboBox_left_png, ZenBinaryData::zenComboBox_left_pngSize);
	centerImage = ImageCache::getFromMemory(ZenBinaryData::zenComboBox_center_png, ZenBinaryData::zenComboBox_center_pngSize);
	rightImage = ImageCache::getFromMemory(ZenBinaryData::zenComboBox_right_png, ZenBinaryData::zenComboBox_right_pngSize);
}

void ZenComboBox::paint(Graphics& g)
{
	static_cast<ZenLookAndFeel*>(&getLookAndFeel())->drawZenComboBox(g, getWidth(), getHeight(), isButtonDown,
		label->getRight(), 0, getWidth() - label->getRight(), getHeight(),
		*this);

	if (textWhenNothingSelected.isNotEmpty()
		&& label->getText().isEmpty()
		&& !label->isBeingEdited())
	{
		g.setColour(findColour(textColourId).withMultipliedAlpha(0.5f));
		g.setFont(label->getFont());
		g.drawFittedText(textWhenNothingSelected, label->getBounds().reduced(2, 1),
			label->getJustificationType(),
			jmax(1, (int)(label->getHeight() / label->getFont().getHeight())));
	}
}
