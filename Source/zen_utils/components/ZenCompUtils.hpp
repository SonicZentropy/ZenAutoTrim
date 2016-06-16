/*==============================================================================
//  ZenCompUtils.hpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 3 Jun 2016 2:13:46am
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Generic Header File
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef ZENCOMPUTILS_HPP_INCLUDED
#define ZENCOMPUTILS_HPP_INCLUDED

#include "JuceHeader.h"

class ZenCompUtils {

public:

	static void setFontSizeForBounds(Font& font, int w, int h, const String& text, int margin /*= 0*/)
	{
		font.setHeight(h);
		font.setHorizontalScale(1);

		// round up to pixel 
		int fw = font.getStringWidthFloat(text) + 0.5f;

		if (fw + margin > w)
		{
			float s = (w - margin) / (float)fw;
			h = h*s; // round down to pixel
		}
		font.setHeight(h);
	}

	static void setFontSizeAndScaleForBounds(Font& font, int w, int h,
		const String& text,
		float minScale /*= 0.8f*/)
	{
		font.setHeight(h);
		font.setHorizontalScale(1);

		float scale = 1;
		float fw = font.getStringWidth(text);
		if (fw > w)
			scale = w / fw;

		if (scale < minScale)
		{
			int h2 = (int)((w / minScale) / fw * h);
			font.setHeight(h2);

			fw = font.getStringWidth(text);
			if (fw > w)
				scale = w / fw;
		}

		font.setHorizontalScale(scale);
	}

};

#endif  // ZENCOMPUTILS_HPP_INCLUDED
