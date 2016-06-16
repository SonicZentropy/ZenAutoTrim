/*==============================================================================
//  ZenTextEditor.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 11 May 2016 12:03:59am
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Generic CPP File
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#include "ZenTextEditor.h"
#include "zen_utils/utilities/ZenBinaryData.h"

ZenTextEditor::ZenTextEditor(const String& componentName, const ImageBorderType& imageBorderType, const String& labelText, int componentWidth, int componentHeight)
	:TextEditor(componentName)
{
	Component::setSize(componentWidth, componentHeight);
	setText(labelText);
	if (imageBorderType == LargeBorder)
	{
		leftEndImage = ImageCache::getFromMemory(ZenBinaryData::textFieldLargeBorderLeftEnd_png, ZenBinaryData::textFieldLargeBorderLeftEnd_pngSize);
		rightEndImage = ImageCache::getFromMemory(ZenBinaryData::textFieldLargeBorderRightEnd_png, ZenBinaryData::textFieldLargeBorderRightEnd_pngSize);
		centerImage = ImageCache::getFromMemory(ZenBinaryData::textFieldLargeBorderCenter_png, ZenBinaryData::textFieldLargeBorderCenter_pngSize);
		fullImage = ImageCache::getFromMemory(ZenBinaryData::textFieldLargeBorderFull_png, ZenBinaryData::textFieldLargeBorderFull_pngSize);
	} else
	{
		leftEndImage = ImageCache::getFromMemory(ZenBinaryData::textFieldSmallBorderLeftEnd_png, ZenBinaryData::textFieldSmallBorderLeftEnd_pngSize);
		rightEndImage = ImageCache::getFromMemory(ZenBinaryData::textFieldSmallBorderRightEnd_png, ZenBinaryData::textFieldSmallBorderRightEnd_pngSize);
		centerImage = ImageCache::getFromMemory(ZenBinaryData::textFieldSmallBorderCenter_png, ZenBinaryData::textFieldSmallBorderCenter_pngSize);
		fullImage = ImageCache::getFromMemory(ZenBinaryData::textFieldSmallBorderFull_png, ZenBinaryData::textFieldSmallBorderFull_pngSize);
	}
	this->setColour(textColourId, Colours::antiquewhite);
	this->setColour(backgroundColourId, Colour(0.0f));  //transparent background since we're drawing image
	this->setColour(highlightColourId, Colours::white);
	this->setColour(CaretComponent::caretColourId, Colour(0xffc3c3c3));
	
	Typeface::Ptr tface = Typeface::createSystemTypefaceFor(ZenBinaryData::futurabook_ttf, ZenBinaryData::futurabook_ttfSize);
	Font textFont(tface);
	
	textFont.setHeight(14.0f);
	this->setFont(textFont);
	addComponentListener(this);
	
	setBorder(BorderSize<int>(1, 5, 1, 5 ));	
}



ZenTextEditor::~ZenTextEditor()
{

}



void ZenTextEditor::componentMovedOrResized(Component& component, bool wasMoved, bool wasResized)
{	
	//resize font height to fully fill the vertical space if multiLine
	//if(!isMultiLine())
	//	this->applyFontToAllText(getFont().withHeight(getHeight()*getFont().getHeightToPointsFactor()));
	
}

//void ZenTextEditor::returnPressed()
//{
//	// #TODO: remove this if not necessary
//	//throw std::logic_error("The method or operation is not implemented.");
//}

void ZenTextEditor::paintOverChildren(Graphics& g)
{
	if (textToShowWhenEmpty.isNotEmpty()
		&& (!hasKeyboardFocus(false))
		&& getTotalNumChars() == 0)
	{
		g.setColour(colourForTextWhenEmpty);
		g.setFont(getFont());

		if (isMultiLine())
			g.drawText(textToShowWhenEmpty, getLocalBounds(),
				Justification::centred, true);
		else
			g.drawText(textToShowWhenEmpty,
				leftIndent, 0, viewport->getWidth() - leftIndent, getHeight(),
				Justification::centredLeft, true);
	}

	getLookAndFeel().drawTextEditorOutline(g, getWidth(), getHeight(), *this);
}

void ZenTextEditor::focusLost(FocusChangeType fcType)
{
	DBG("In ZenTextEditor::focusLost(fcType) ");
	this->selection.setLength(0); //Clear selected text (for loss of focus because of clicks)
	TextEditor::focusLost(fcType);
}
