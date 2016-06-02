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
#include "utilities/ZenBinaryData.h"

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

// void ZenTextEditor::repaintText(const Range<int> range)
// {
// 	if (!range.isEmpty())
// 	{
// 		float x = 0, y = 0, lh = currentFont.getHeight();
// 
// 		const float wordWrapWidth = getWordWrapWidth();
// 
// 		if (wordWrapWidth > 0)
// 		{
// 			Iterator i(sections, wordWrapWidth, passwordCharacter);
// 
// 			i.getCharPosition(range.getStart(), x, y, lh);
// 
// 			const int y1 = (int)y;
// 			int y2;
// 
// 			if (range.getEnd() >= getTotalNumChars())
// 			{
// 				y2 = textHolder->getHeight();
// 			}
// 			else
// 			{
// 				i.getCharPosition(range.getEnd(), x, y, lh);
// 				y2 = (int)(y + lh * 2.0f);
// 			}
// 
// 			textHolder->repaint(0, y1, textHolder->getWidth(), y2 - y1);
// 		}
// 	}
// }

// void ZenTextEditor::paintOverChildren(Graphics& g)
// {
// 	if (textToShowWhenEmpty.isNotEmpty()
// 		&& (!hasKeyboardFocus(false))
// 		&& getTotalNumChars() == 0)
// 	{
// 		g.setColour(colourForTextWhenEmpty);
// 		g.setFont(getFont());
// 
//  		if (isMultiLine())
//  			g.drawText(textToShowWhenEmpty, getLocalBounds(),
//  				Justification::centred, true);
//  		else
// 			
//  			g.setColour(label.findColour(Label::textColourId).withMultipliedAlpha(alpha));
//  			g.drawFittedText(label.getText(), textArea, label.getJustificationType(),
//  				jmax(1, (int)(textArea.getHeight() / font.getHeight())), 0.01f);
//  			g.drawText(textToShowWhenEmpty,
//  				leftIndent, 0, viewport->getWidth() - leftIndent, getHeight(),
//  				Justification::centredLeft, true);
// 	}
// 
// 	getLookAndFeel().drawTextEditorOutline(g, getWidth(), getHeight(), *this);
// }

// void ZenTextEditor::paint(Graphics& g)
// {
// 	getLookAndFeel().fillTextEditorBackground(g, getWidth(), getHeight(), *this);
// }

// void ZenTextEditor::drawContent(Graphics& g)
// {
// 	const float wordWrapWidth = getWordWrapWidth();
// 
// 	if (wordWrapWidth > 0)
// 	{
// 		g.setOrigin(leftIndent, topIndent);
// 		const Rectangle<int> clip(g.getClipBounds());
// 		Colour selectedTextColour;
// 
// 		Iterator i(sections, wordWrapWidth, passwordCharacter);
// 
// 		if (!selection.isEmpty())
// 		{
// 			Iterator i2(i);
// 			RectangleList<float> selectionArea;
// 
// 			while (i2.next() && i2.lineY < clip.getBottom())
// 			{
// 				if (i2.lineY + i2.lineHeight >= clip.getY()
// 					&& selection.intersects(Range<int>(i2.indexInText, i2.indexInText + i2.atom->numChars)))
// 				{
// 					i2.addSelection(selectionArea, selection);
// 				}
// 			}
// 
// 			g.setColour(findColour(highlightColourId).withMultipliedAlpha(hasKeyboardFocus(true) ? 1.0f : 0.5f));
// 			g.fillRectList(selectionArea);
// 
// 			selectedTextColour = findColour(highlightedTextColourId);
// 		}
// 
// 		const UniformTextSection* lastSection = nullptr;
// 
// 		while (i.next() && i.lineY < clip.getBottom())
// 		{
// 			if (i.lineY + i.lineHeight >= clip.getY())
// 			{
// 				if (selection.intersects(Range<int>(i.indexInText, i.indexInText + i.atom->numChars)))
// 				{
// 					i.drawSelectedText(g, selection, selectedTextColour);
// 					lastSection = nullptr;
// 				}
// 				else
// 				{
// 					i.draw(g, lastSection);
// 				}
// 			}
// 		}
// 
// 		for (int j = underlinedSections.size(); --j >= 0;)
// 		{
// 			const Range<int> underlinedSection = underlinedSections.getReference(j);
// 
// 			Iterator i2(sections, wordWrapWidth, passwordCharacter);
// 
// 			while (i2.next() && i2.lineY < clip.getBottom())
// 			{
// 				if (i2.lineY + i2.lineHeight >= clip.getY()
// 					&& underlinedSection.intersects(Range<int>(i2.indexInText, i2.indexInText + i2.atom->numChars)))
// 				{
// 					i2.drawUnderline(g, underlinedSection, findColour(textColourId));
// 				}
// 			}
// 		}
// 	}
// }