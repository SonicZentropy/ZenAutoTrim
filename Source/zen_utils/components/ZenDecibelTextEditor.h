#include "ZenComponentType.hpp"

/*==============================================================================
//  ZenDecibelTextEditor.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 1/20/2016
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Text editor component specifically for handling Decibel values
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef ZenDecibelTextEditor_H_INCLUDED
#define ZenDecibelTextEditor_H_INCLUDED

#include "JuceHeader.h"
#include "components/ZenTextEditor.h"
#include "parameters/ZenDecibelParameter.h"


class ZenDecibelTextEditor : public ZenTextEditor
{
public:

	ZenDecibelTextEditor(String componentName, ZenDecibelParameter* param, juce_wchar passwordCharacter = 0U);

	/** Returns linear 0.0 to 1.0 representation of given decibel value from value text */
	float getNormalizedValueFromText() const;

	/** Returns decibel value from value text */
	float getDecibelValueFromText() const;

	/** Formats value to readable text string with Decibel units*/
	void formatTextAfterEntry();

	template <class Type>
	void setTextWith2Precision(const Type inText)
	{
		this->setText(String(inText, 2));
	}

private:
	NormalisableRange<float> nRange;
};

#endif  // ZenDecibelTextEditor_H_INCLUDEDa
