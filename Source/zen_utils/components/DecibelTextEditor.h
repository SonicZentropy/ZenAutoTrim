/*
  ==============================================================================

    DecibelTextEditor.h
    Created: 20 Jan 2016 10:12:03am
    Author:  Zentropy

  ==============================================================================
*/

#pragma once
#ifndef DECIBELTEXTEDITOR_H_INCLUDEDa
#define DECIBELTEXTEDITOR_H_INCLUDEDa

#include "../../../JuceLibraryCode/JuceHeader.h"

class DecibelTextEditor : public TextEditor
{
public:

	DecibelTextEditor(const String& componentName, const AudioParameterFloat* param, juce_wchar passwordCharacter = 0U);
	

	float getNormalizedValueFromText();

	void formatTextAfterEntry();
	
	template <class Type>
	void DecibelTextEditor::setTextWith2Precision(const Type inText)
	{
		this->setText(convertTo2PrecisionString(inText));
	}

private:
	NormalisableRange<float> nRange;
};




#endif  // DECIBELTEXTEDITOR_H_INCLUDEDa
