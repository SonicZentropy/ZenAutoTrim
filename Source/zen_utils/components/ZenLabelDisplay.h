/*==============================================================================
//  ZenLabelDisplay.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 10 May 2016 6:19:48pm
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Generic Header File
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef ZENLABELDISPLAY_H_INCLUDED
#define ZENLABELDISPLAY_H_INCLUDED

#include "JuceHeader.h"

using std::function;
using std::string;

class ZenLookAndFeel;

/// ZenLabelDisplay: Creates a non-editable label display area.
/// This is for purposes of displaying relevant information from a plugin
/// with no chance of someone mistaking it for a field that can be 
/// manually changed.  Use by setting size, then simply changing
/// the displayed text with setText
/// <seealso cref="Label" />
class ZenLabelDisplay : public Label
{
public:
	explicit ZenLabelDisplay(const String& componentName = String::empty, const String& labelText = String::empty);
	~ZenLabelDisplay();

	const Image& getLabelTLCornerImg() const { return labelTLCornerImage; }
	const Image& getLabelTRCornerImage() const { return labelTRCornerImage; }
	const Image& getLabelBLCornerImage() const { return labelBLCornerImage; }
	const Image& getLabelBRCornerImage() const { return labelBRCornerImage; }

	const Image& getLabelTopLineImg() const { return labelTopLineImg; }
	const Image& getLabelBottomLineImg() const { return labelBottomLineImg; }
	const Image& getLabelLeftLineImg() const { return labelLeftLineImg; }
	const Image& getLabelRightLineImg() const { return labelRightLineImg; }

	const Image& getLabelInteriorImg() const { return labelInteriorImg; }

	const String getText();

	void setFontSizeForBounds();
	void setFontSizeForBounds(int margin);
	void setFontSizeForBounds(int w, int h, const String& text, int margin /*= 0*/);
	void setFontSizeAndScaleForBounds();
	void setFontSizeAndScaleForBounds(int margin);
	void setFontSizeAndScaleForBounds(int w, int h, const String& text, float minScale /*= 0.8f*/);
	void setGetTextFunction(function<String(ZenLabelDisplay*)> f)
	{
		getTextFunction = f;
		hasGetTextFunction = true;
	}

	const float getValueAsFloat() 
	{
		return this->getTextValue().getValue();
	}	

	bool getShouldScaleText() const { return shouldScaleText; }
	void setShouldScaleText(bool inValue) { shouldScaleText = inValue; }

	int getFontMarginSize() const { return fontMarginSize; }
	void setFontMarginSize(int inValue) { fontMarginSize = inValue; }

protected:
	friend class ZenLookAndFeel;
	Image labelTLCornerImage, labelTRCornerImage, labelBLCornerImage, labelBRCornerImage;
	Image labelTopLineImg, labelBottomLineImg, labelLeftLineImg, labelRightLineImg;
	Image labelInteriorImg;

	bool shouldScaleText;
	int fontMarginSize;

	function<String(ZenLabelDisplay*)> getTextFunction;
	bool hasGetTextFunction;
};



#endif  // ZENLABELDISPLAY_H_INCLUDED
