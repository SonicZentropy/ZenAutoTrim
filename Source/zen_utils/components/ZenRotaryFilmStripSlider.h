/*==============================================================================
//  ZenRotaryFilmStripSlider.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 18 Apr 2016 5:51:37pm
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Generic Header File
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef ZENROTARYFILMSTRIPSLIDER_H_INCLUDED
#define ZENROTARYFILMSTRIPSLIDER_H_INCLUDED

#include "JuceHeader.h"
//#include "ZenBinaryData.h"

using std::shared_ptr;

class ZenRotaryFilmStripSlider : public virtual Slider
{
public:
				
	/// <summary>Initializes a new instance of the <see cref="ZenRotaryFilmStripSlider" /> class.</summary>
	/// <param name="imgName">Name of the img.</param>
	/// <param name="numFrames">The number frames.</param>
	/// <param name="knobWidth">Width of the knob.</param>
	/// <param name="knobHeight">Height of the knob.</param>
	/// <param name="inPos">The in position.</param>
	/// <param name="stripIsHorizontal">The strip is horizontal.</param>
	ZenRotaryFilmStripSlider(const String& imgName, const int numFrames, const int knobWidth = 64, const int knobHeight = 64, 
		const TextEntryBoxPosition inPos = TextBoxCentered, const int inTextBoxWidth = 60, const int inTextBoxHeight = 24, const bool stripIsHorizontal = false);


	~ZenRotaryFilmStripSlider();

	/// <summary>Sets the text box style.</summary>
	/// <param name="newPosition" type="const TextEntryBoxPosition">The new position.</param>
	/// <param name="isReadOnly" type="const bool">The is read only.</param>
	/// <param name="textEntryBoxWidth" type="const int">Width of the text entry box.</param>
	/// <param name="textEntryBoxHeight" type="const int">Height of the text entry box.</param>
	void setTextBoxStyle(const TextEntryBoxPosition newPosition, const bool isReadOnly, const int textEntryBoxWidth, const int textEntryBoxHeight);

	//==============================================================================


	/// <summary>Gets the width of the frame.</summary>
	int getFrameWidth() const;
	
	/// <summary>Gets the height of the frame.</summary>
	int getFrameHeight() const;
	
	void setSize(int newWidth, int newHeight) override;
	void paint(Graphics& g) override;

	const Image& getFilmStrip() const { return filmStrip; }

	/// <summary>Determines whether [has valid film strip].</summary>
	/// <returns type="bool">bool.</returns>
	bool hasValidFilmStrip() const { return filmStrip.isValid(); }

	void setKnobSize(int inWidth, int inHeight);

	int getKnobWidth() const { return knobWidth; }
	void setKnobWidth(int inValue) { knobWidth = inValue; }

	int getKnobHeight() const { return knobHeight; }
	void setKnobHeight(int inValue) { knobHeight = inValue; }

	bool getFilmstripIsHorizontal() const { return filmstripIsHorizontal; }
	void setFilmstripIsHorizontal(bool inValue) { filmstripIsHorizontal = inValue; }

	int getNumFrames() const { return numFrames; }
	void setNumFrames(int inValue) { numFrames = inValue; }

protected:
	
	/// The film strip
	Image filmStrip;
	
	/// The is small knob
	bool isSmallKnob;
	
	/// The frame width
	int frameWidth, frameHeight, knobWidth, knobHeight;
	int numFrames;
	bool filmstripIsHorizontal;

	//Intentionally non-owned, do not delete this pointer!
	LookAndFeel* currentLookAndFeel;
	Rectangle<int> sliderRect;

	
	//==============================================================================
	JUCE_PUBLIC_IN_DLL_BUILD(class Pimpl)
	friend class Pimpl;
	friend struct ContainerDeletePolicy<Pimpl>;
	void init(SliderStyle, TextEntryBoxPosition);

#if JUCE_CATCH_DEPRECATED_CODE_MISUSE
	JUCE_DEPRECATED(void setValue(double, bool));
	JUCE_DEPRECATED(void setValue(double, bool, bool));
	JUCE_DEPRECATED(void setMinValue(double, bool, bool, bool));
	JUCE_DEPRECATED(void setMinValue(double, bool, bool));
	JUCE_DEPRECATED(void setMinValue(double, bool));
	JUCE_DEPRECATED(void setMaxValue(double, bool, bool, bool));
	JUCE_DEPRECATED(void setMaxValue(double, bool, bool));
	JUCE_DEPRECATED(void setMaxValue(double, bool));
	JUCE_DEPRECATED(void setMinAndMaxValues(double, double, bool, bool));
	JUCE_DEPRECATED(void setMinAndMaxValues(double, double, bool));

#endif

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZenRotaryFilmStripSlider)
};

#endif  // ZENROTARYFILMSTRIPSLIDER_H_INCLUDED
	