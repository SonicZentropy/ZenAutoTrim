/*==============================================================================
//  ZenLookAndFeel.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 3 Apr 2016 2:53:25pm
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Generic Header File
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef ZENLOOKANDFEEL_H_INCLUDED
#define ZENLOOKANDFEEL_H_INCLUDED

#include "JuceHeader.h"
#include "components/ZenRotaryFilmStripSlider.h"
#include "components/ZenImageButton.h"
#include "components/ZenRotaryEditorLabel.h"
#include "components/ZenComboBox.h"

class ZenLookAndFeel : public LookAndFeel_V3
{
	
public:
	ZenLookAndFeel();
	void drawZenRotarySlider(Graphics&, int x, int y, int width, int height, float sliderPosProportional, ZenRotaryFilmStripSlider& slider);
	void drawLabel(Graphics& g, Label& label) override;
	Font getLabelFont(Label& label) override;
	Font getZenImageButtonFont(ZenImageButton& zenbtn);
	//int getSliderThumbRadius(Slider&) override;
	void drawButtonBackground(Graphics&, Button&, const Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown) override;
	void drawButtonShape(Graphics& g, const Path& outline, Colour baseColour, float height);

	Slider::SliderLayout getSliderLayout(Slider& slider) override;
	ZenRotaryEditorLabel* createSliderTextBox(Slider& slider) override;

	void fillTextEditorBackground(Graphics& g, int width, int height, TextEditor& textEditor) override;
	
	//Image button methods
	void drawImageButton(Graphics& g, Image* imageToDraw, int imageX, int imageY, int imageW, int imageH, const Colour& overlayColour, float imageOpacity, ImageButton& buttonToDraw) override;
	void drawZenImageButton(Graphics& g, ZenImageButton& buttonToDraw);

	//Combo box methods
	void drawZenComboBox(Graphics&, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW, int buttonH, ZenComboBox& box);
	Font getComboBoxFont(ComboBox&) override;
	Label* createComboBoxTextBox(ComboBox&) override;
	void positionComboBoxText(ComboBox&, Label&) override;

	//Combo box popup menu methods
	void drawPopupMenuBackground(Graphics&, int width, int height) override;	
	void drawPopupMenuItem(Graphics&, const Rectangle<int>& area, bool isSeparator, bool isActive, 
		bool isHighlighted, bool isTicked, bool hasSubMenu, const String& text, const String& shortcutKeyText, 
		const Drawable* icon, const Colour* textColour) override;
	void drawPopupMenuSectionHeader(Graphics&, const Rectangle<int>& area, const String& sectionName) override;	
	Font getPopupMenuFont() override;	
	void drawPopupMenuUpDownArrow(Graphics&, int width, int height, bool isScrollUpArrow) override;

	//void drawLabel(Graphics&, Label&) override;
	//Label* createSliderTextBox(Slider&) override;
protected:
	Image topImage;
	Font defaultFont;

	
	Image testImg, bypassImg, knobImage;

};

// #TODO: move this into slider class
class SliderLabelComp : public Label
{
public:
	SliderLabelComp() : Label(String::empty, String::empty) {}

	void mouseWheelMove(const MouseEvent&, const MouseWheelDetails&) override {}
};

#endif  // ZENLOOKANDFEEL_H_INCLUDED
