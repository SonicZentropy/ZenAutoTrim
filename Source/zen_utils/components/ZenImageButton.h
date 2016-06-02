/*==============================================================================
//  ZenImageButton.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 12 May 2016 7:15:54am
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Generic Component Header
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef ZENIMAGEBUTTON_H_INCLUDED
#define ZENIMAGEBUTTON_H_INCLUDED

#include "JuceHeader.h"

/// ZenImageButton provides a general button that can display text on top of
/// a png-based image that scales to button size and implements hover animations.
/// It operates automatically, just treat it as a JUCE Text button
class ZenImageButton    : virtual public ImageButton
{
public:
	
	explicit ZenImageButton(const String& buttonName, const String& buttonText);
    ~ZenImageButton();	

	const Image& getNormalLeft() const { return normalLeft; }
	void setNormalLeft(Image inValue) { normalLeft = inValue; }

	const Image& getNormalRight() const { return normalRight; }
	void setNormalRight(Image inValue) { normalRight = inValue; }

	const Image& getNormalCenter() const { return normalCenter; }
	void setNormalCenter(Image inValue) { normalCenter = inValue; }

	const Image& getHoverLeft() const { return hoverLeft; }
	void setHoverLeft(Image inValue) { hoverLeft = inValue; }

	const Image& getHoverRight() const { return hoverRight; }
	void setHoverRight(Image inValue) { hoverRight = inValue; }

	const Image& getHoverCenter() const { return hoverCenter; }
	void setHoverCenter(Image inValue) { hoverCenter = inValue; }

	const Image& getPressedLeft() const { return pressedLeft; }
	void setPressedLeft(Image inValue) { pressedLeft = inValue; }

	const Image& getPressedRight() const { return pressedRight; }
	void setPressedRight(Image inValue) { pressedRight = inValue; }

	const Image& getPressedCenter() const { return pressedCenter; }
	void setPressedCenter(Image inValue) { pressedCenter = inValue; }

	String getButtonText() const { return buttonText; }
	void setButtonText(String inValue) { buttonText = inValue; }

	Image getPressedHoverLeft() const { return pressedHoverLeft; }
	void setPressedHoverLeft(Image inValue) { pressedHoverLeft = inValue; }

	Image getPressedHoverRight() const { return pressedHoverRight; }
	void setPressedHoverRight(Image inValue) { pressedHoverRight = inValue; }

	Image getPressedHoverCenter() const { return pressedHoverCenter; }
	void setPressedHoverCenter(Image inValue) { pressedHoverCenter = inValue; }

	Colour getTextColorButtonOn() const { return textColorButtonOn; }
	void setTextColorButtonOn(Colour inValue) { textColorButtonOn = inValue; }

	Colour getTextColorButtonOff() const { return textColorButtonOff; }
	void setTextColorButtonOff(Colour inValue) { textColorButtonOff = inValue; }

	//void paint (Graphics&) override;
    //void resized() override;


protected:
	//bool hitTest(int x, int y) override;
	void paintButton(Graphics&, bool isMouseOver, bool isButtonDown) override;

private:
	Image normalLeft, normalRight, normalCenter;
	Image hoverLeft, hoverRight, hoverCenter;
	Image pressedLeft, pressedRight, pressedCenter;
	Image pressedHoverLeft, pressedHoverRight, pressedHoverCenter;
	Colour textColorButtonOn, textColorButtonOff;
	String buttonText;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ZenImageButton)
};


#endif  // ZENIMAGEBUTTON_H_INCLUDED
