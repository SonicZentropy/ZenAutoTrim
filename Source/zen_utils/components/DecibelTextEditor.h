/*
  ==============================================================================

    DecibelTextEditor.h
    Created: 20 Jan 2016 10:12:03am
    Author:  Zentropy

  ==============================================================================
*/

#ifndef DECIBELTEXTEDITOR_H_INCLUDEDa
#define DECIBELTEXTEDITOR_H_INCLUDEDa

#include "../../../JuceLibraryCode/JuceHeader.h"

class DecibelTextEditor : public TextEditor
{
public:

	DecibelTextEditor(const String& componentName, const AudioParameterFloat* param, juce_wchar passwordCharacter = 0U);
	

	float getNormalizedValueFromText();

	void formatTextAfterEntry();
private:
	NormalisableRange<float> nRange;
};



#endif  // DECIBELTEXTEDITOR_H_INCLUDEDa
