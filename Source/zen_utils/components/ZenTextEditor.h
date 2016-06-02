/*==============================================================================
//  ZenTextEditor.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 11 May 2016 12:03:59am
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Generic Header File
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef ZENTEXTEDITOR_H_INCLUDED
#define ZENTEXTEDITOR_H_INCLUDED

#include "JuceHeader.h"

/// ZenTextEditor: Manages a text field editor the user can change as well as the display thereof.
/// It uses PNG for the look of the field and allows a choice between Small and Large borders.
/// <seealso cref="TextEditor" />
/// <seealso cref="ComponentListener" />
class ZenTextEditor : public TextEditor, public ComponentListener
{
public: //150 24

	enum ImageBorderType
	{
		SmallBorder,
		LargeBorder
	};

	explicit ZenTextEditor(const String& componentName = String::empty, const ImageBorderType& imageBorderType = SmallBorder, const String& labelText = String::empty, int componentWidth = 150, int componentHeight = 24);
	~ZenTextEditor();
	//void repaintText(const Range<int> range);
	//void paint(Graphics& g) override;
	
	//void paintOverChildren(Graphics& g) override;

	//void drawContent(Graphics& g);

	const Image& getLeftEndImage() const { return leftEndImage; }
	const Image& getRightEndImage() const { return rightEndImage; }
	const Image& getCenterImage() const { return centerImage; }
	const Image& getFullImage() const { return fullImage;  }

	void componentMovedOrResized(Component& component, bool wasMoved, bool wasResized) override;


protected:
	Image leftEndImage, rightEndImage, centerImage, fullImage;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZenTextEditor)
	
};



#endif  // ZENTEXTEDITOR_H_INCLUDED
