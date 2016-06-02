/*==============================================================================
//  ZenRotaryFilmStripSlider.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 18 Apr 2016 5:51:37pm
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Generic CPP File
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#include "ZenRotaryFilmStripSlider.h"
#include "utilities/ZenBinaryData.h"
#include "components/ZenRotaryEditorLabel.h"
#include "GUI/ZenLookAndFeel.h"

using std::make_shared;

ZenRotaryFilmStripSlider::ZenRotaryFilmStripSlider(const String& imgName, const int numFrames, const int inKnobWidth, const int inKnobHeight, 
	TextEntryBoxPosition inPos, const int inTextBoxWidth, const int inTextBoxHeight, bool stripIsHorizontal)
	: Slider(imgName, RotaryHorizontalVerticalDrag, inPos) 
	, numFrames(numFrames)
	, filmstripIsHorizontal(stripIsHorizontal)
{	
	setColour(textBoxTextColourId, Colours::white);
	setColour(textBoxBackgroundColourId, Colour(0x00ffffff));
	setColour(textBoxHighlightColourId, Colour(0x001111ee));
	setColour(textBoxOutlineColourId, Colour(0x00808080));

	if (inKnobWidth > 0 && inKnobWidth < 48 && inKnobHeight > 0 && inKnobHeight < 48)
	{
		filmStrip = ImageCache::getFromMemory(ZenBinaryData::zenRotaryKnobNoGrates_png, (size_t)ZenBinaryData::zenRotaryKnobNoGrates_pngSize);
		isSmallKnob = true;
	} else
	{
		filmStrip = ImageCache::getFromMemory(ZenBinaryData::zenRotaryKnob_png, (size_t)ZenBinaryData::zenRotaryKnob_pngSize);
		isSmallKnob = false;
	}

	if (filmStrip.isValid())
	{
		if (filmstripIsHorizontal) {
			frameHeight = filmStrip.getHeight();
			frameWidth = filmStrip.getWidth() / numFrames;
		} else {
			frameHeight = filmStrip.getHeight() / numFrames;
			frameWidth = filmStrip.getWidth();
		}
	}

	knobWidth = (inKnobWidth == -1) ? frameWidth : inKnobWidth;
	knobHeight = (inKnobHeight == -1) ? frameHeight : inKnobHeight;
	
	setNumDecimalPlacesToDisplay(2);
	setTextBoxStyle(inPos, false, inTextBoxWidth, inTextBoxHeight);
	setSize(knobWidth, knobHeight);

	currentLookAndFeel = &getLookAndFeel();
}

ZenRotaryFilmStripSlider::~ZenRotaryFilmStripSlider() {};

/** Sets the label/text box alignment, position, and size.  Note that the entry box width and height
/*  Are discrete and do not take into account any indent border size */
void ZenRotaryFilmStripSlider::setTextBoxStyle(const TextEntryBoxPosition newPosition, const bool isReadOnly,
	const int textEntryBoxWidth, const int textEntryBoxHeight)
{
	TextEntryBoxPosition oldPos = this->getTextBoxPosition();
	Slider::setTextBoxStyle(newPosition, isReadOnly, textEntryBoxWidth, textEntryBoxHeight);
	
}


int ZenRotaryFilmStripSlider::getFrameWidth() const
{
	return filmStrip.isValid() ? frameWidth : 100;
}

int ZenRotaryFilmStripSlider::getFrameHeight() const
{
	return filmStrip.isValid() ? frameHeight : 24;
}

void ZenRotaryFilmStripSlider::setSize(int newWidth, int newHeight)
{
	jassert(nullptr != dynamic_cast<ZenRotaryEditorLabel*>(this->getValueBox()));

	if (isSmallKnob && (newWidth >= 48 || newHeight >= 48))
	{
		filmStrip = ImageCache::getFromMemory(ZenBinaryData::zenRotaryKnob_png, ZenBinaryData::zenRotaryKnob_pngSize);
		isSmallKnob = false;
	} else if (!isSmallKnob && (newWidth < 48 || newHeight < 48))
	{
		filmStrip = ImageCache::getFromMemory(ZenBinaryData::zenRotaryKnobNoGrates_png, ZenBinaryData::zenRotaryKnobNoGrates_pngSize);
		isSmallKnob = true;
		
	}

	if (isSmallKnob) //static cast guaranteed to work b/c FilmStripSlider can only contain this
		static_cast<ZenRotaryEditorLabel*>(this->getValueBox())->setTextHasShadow(false);
	else
		static_cast<ZenRotaryEditorLabel*>(this->getValueBox())->setTextHasShadow(true);

	if (getTextBoxPosition() == TextBoxCentered)
	{
		Component::setSize(newWidth, newHeight);
		int textWidth = jmin(getTextBoxWidth(), newWidth - 13); //Make sure text box fits into knob
		setTextBoxStyle(getTextBoxPosition(), this->isTextBoxEditable(), textWidth, getTextBoxHeight());
	}
	else if (getTextBoxPosition() == TextBoxLeft || getTextBoxPosition() == TextBoxRight)
	{
		Component::setSize(newWidth + getTextBoxWidth(), newHeight);
		
	}
	else if (getTextBoxPosition() == TextBoxAbove || getTextBoxPosition() == TextBoxBelow)
	{
		Component::setSize(newWidth, newHeight + getTextBoxHeight());
	}
	
}

void ZenRotaryFilmStripSlider::paint(Graphics& g)
{
	const float sliderPos = (float)valueToProportionOfLength(getLastCurrentValue());
	jassert(sliderPos >= 0 && sliderPos <= 1.0f);
	jassert(nullptr != dynamic_cast<ZenLookAndFeel*>(currentLookAndFeel));

	sliderRect = getSliderRect();

	static_cast<ZenLookAndFeel*>(currentLookAndFeel)->drawZenRotarySlider(g,
		sliderRect.getX(), sliderRect.getY(),
		getWidth(), getHeight(),
		sliderPos,  *this);
	
}

void ZenRotaryFilmStripSlider::setKnobSize(int inWidth, int inHeight)
{
	knobWidth = inWidth;
	knobHeight = inHeight;
}




