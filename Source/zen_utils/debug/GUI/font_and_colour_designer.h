/*==============================================================================
//  font_and_colour_designer.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2016/01/31
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Realtime change of font and color
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef FONT_COLOR_DESIGNER_H_INCLUDED
#define FONT_COLOR_DESIGNER_H_INCLUDED 

#include "JuceHeader.h"

class FontAndColorContent;

/**
 * https://github.com/jcredland/juce-toys/
 *
 * An instance of FontAndColourDesign will allow you to
 * control one font and one colour while your application
 * is running.
 *
 * This is handy if you are indecisive about how something
 * should look whilst writing your UI.
 */

class FontAndColourDesigner : public Component
{
public:
	FontAndColourDesigner(Component & parentComponent,
		Colour & colourToUpdate,
		Font & fontToUpdate);

	void paint(Graphics & g) override;
	void mouseUp(const MouseEvent &) override;
private:
	DocumentWindow window;
	ScopedPointer<FontAndColorContent> content;
};

#endif