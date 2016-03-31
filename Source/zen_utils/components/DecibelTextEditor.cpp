/*==============================================================================
//  DecibelTextEditor.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 1/20/2016
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: DETAILS
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#include "./DecibelTextEditor.h"
#include <sstream>


DecibelTextEditor::DecibelTextEditor(const String& compName, const AudioParameterFloat* param, juce_wchar passwordCharacter)
	:TextEditor(compName, passwordCharacter),
	nRange(param->range.start, param->range.end, 0.01f)
{	
}

float DecibelTextEditor::getNormalizedValueFromText()
{	
	float conv = nRange.convertTo0to1(this->getText().getFloatValue());
	DBG("Key pressed, value is: " + this->getText() + " and trimmed: " + String(this->getText().getFloatValue()) + " and converted: " + String(conv));

	return conv;

}

void DecibelTextEditor::formatTextAfterEntry()
{
	std::stringstream ss;
	ss.setf(std::ios_base::fixed, std::ios_base::floatfield);
	ss.precision(2);
	ss << getText().getFloatValue();
	setText(ss.str(), false);
}

