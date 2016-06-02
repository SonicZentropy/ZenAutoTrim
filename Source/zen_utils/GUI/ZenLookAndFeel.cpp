/*==============================================================================
//  ZenLookAndFeel.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 3 Apr 2016 2:53:25pm
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Generic CPP File
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

//#include "ZenLookAndFeel.h"
//#include "ZenRotaryFilmStripSlider.h"
//#include "ZenRotaryEditorLabel.h"
//#include "ZenLabelDisplay.h"
//#include "ZenTextEditor.h"
//#include "ZenImageButton.h"
#include "ZenLookAndFeel.h"
#include "utilities/ZenBinaryData.h"
#include "components/ZenLabelDisplay.h"
#include "components/ZenTextEditor.h"

ZenLookAndFeel::ZenLookAndFeel()
{
	defaultFont = Typeface::createSystemTypefaceFor(ZenBinaryData::futurabook_ttf, ZenBinaryData::futurabook_ttfSize);
	setColour(PopupMenu::backgroundColourId, Colour((juce::uint8) 41, 41, 41, (float)1.0f));
	setColour(PopupMenu::textColourId, Colours::lightgrey);
	setColour(PopupMenu::highlightedBackgroundColourId, Colours::darkslateblue);
	setColour(PopupMenu::highlightedTextColourId, Colours::lightgrey);
	setColour(PopupMenu::headerTextColourId, Colours::red);
}

void ZenLookAndFeel::drawZenRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, ZenRotaryFilmStripSlider& zenSlider)
{
	g.setOpacity(1.0f);
	//ZenRotaryFilmStripSlider* zenSlider = dynamic_cast<ZenRotaryFilmStripSlider*>(&slider);
// 	if (zenSlider != nullptr)
// 	{
		if (zenSlider.hasValidFilmStrip())
		{
			Rectangle<int> sliderRect = zenSlider.getSliderRect();

			int value = (zenSlider.getValue() - zenSlider.getMinimum()) / (zenSlider.getMaximum() - zenSlider.getMinimum()) * (zenSlider.getNumFrames() - 1);

			if (zenSlider.getFilmstripIsHorizontal())
			{
				g.drawImage(zenSlider.getFilmStrip(), sliderRect.getX(), sliderRect.getY(), zenSlider.getKnobWidth(), zenSlider.getKnobHeight(), value * zenSlider.getFrameWidth(), 0, zenSlider.getFrameWidth(), zenSlider.getFrameHeight());
			}
			else
			{
				g.drawImage(zenSlider.getFilmStrip(), sliderRect.getX(), sliderRect.getY(), zenSlider.getKnobWidth(), zenSlider.getKnobHeight(), 0, value * zenSlider.getFrameHeight(), zenSlider.getFrameWidth(), zenSlider.getFrameHeight());
			}
		}
	//}
// 	else //If not Zen Slider
// 	{
// 		LookAndFeel_V2::drawRotarySlider(g, x, y, width, height, sliderPos, rotaryStartAngle, rotaryEndAngle, slider);
// 	}
}

void ZenLookAndFeel::drawLabel(Graphics& g, Label& label)
{
	//Draw non-editable label
	g.setOpacity(1.0f);
	ZenLabelDisplay* zenLabelDisplay = dynamic_cast<ZenLabelDisplay*>(&label);
	if (nullptr != zenLabelDisplay)
	{					
		int labelWidth = zenLabelDisplay->getWidth();
		int labelHeight = zenLabelDisplay->getHeight();

		//draw top left cornerTL
		const Image& cornerTL = zenLabelDisplay->getLabelTLCornerImg();
		int cornerTLWidth = cornerTL.getWidth();
		int cornerTLHeight = cornerTL.getHeight();
		g.drawImage(cornerTL, 0, 0, cornerTLWidth, cornerTLHeight,
			0, 0, cornerTLWidth, cornerTLHeight);
		
		//draw top right corner
		const Image& cornerTR = zenLabelDisplay->getLabelTRCornerImage();
		int cornerTRWidth = cornerTR.getWidth();
		int cornerTRHeight = cornerTR.getHeight();
		g.drawImage(cornerTR, labelWidth - cornerTRWidth, 0, cornerTRWidth, cornerTRHeight,
			0, 0, cornerTRWidth, cornerTRHeight);

 		const Image& cornerBL = zenLabelDisplay->getLabelBLCornerImage();
		int cornerBLWidth = cornerBL.getWidth();
		int cornerBLHeight = cornerBL.getHeight();
 		g.drawImage(cornerBL, 0, labelHeight - cornerBLHeight, cornerBLWidth, cornerBLHeight,
 			0, 0, cornerBLWidth, cornerBLHeight);
 
 		const Image& cornerBR = zenLabelDisplay->getLabelBRCornerImage();
		int cornerBRWidth = cornerBR.getWidth();
		int cornerBRHeight = cornerBR.getHeight();
 		g.drawImage(cornerBR, labelWidth - cornerBRWidth, labelHeight - cornerBRHeight, cornerBRWidth, cornerBRHeight,
 			0, 0, cornerBRWidth, cornerBRHeight);
 
 			
 		const Image& topLine = zenLabelDisplay->getLabelTopLineImg();
		int topLineWidth = topLine.getWidth();
		int topLineHeight = topLine.getHeight();
 		g.drawImage(topLine, cornerTLWidth, 0, labelWidth - cornerTLWidth - cornerTRWidth, 
			topLineHeight, 0, 0, topLineWidth, topLineHeight);
 
		const Image& bottomLine = zenLabelDisplay->getLabelBottomLineImg();
		int bottomLineWidth = bottomLine.getWidth();
		int bottomLineHeight = bottomLine.getHeight();
		g.drawImage(bottomLine, 
			cornerBLWidth, labelHeight - bottomLineHeight, labelWidth - cornerBLWidth - cornerBRWidth, bottomLineHeight,
			0, 0, bottomLineWidth, bottomLineHeight);

 		const Image& leftLine = zenLabelDisplay->getLabelLeftLineImg();
		int leftLineWidth  = leftLine.getWidth();
		int leftLineHeight = leftLine.getHeight();
 		g.drawImage(leftLine, 0, cornerTLHeight, leftLineWidth, labelHeight - cornerTLHeight - cornerBLHeight,
 			0, 0, leftLineWidth, leftLineHeight);
 
 		const Image& rightLine = zenLabelDisplay->getLabelRightLineImg();
		int rightLineWidth = rightLine.getWidth();
		int rightLineHeight = rightLine.getHeight();
		g.drawImage(rightLine, labelWidth - rightLineWidth, cornerTLHeight, rightLineWidth, labelHeight - cornerTLHeight - cornerBLHeight,
			0, 0, rightLineWidth, rightLineHeight);
 		
		g.setColour(Colour(38, 41, 41));
		g.fillRect(cornerTLWidth, cornerTLHeight, labelWidth - cornerTLWidth - cornerTRWidth, labelHeight - cornerTLHeight - cornerBLHeight);
		
		//Draw Text
		g.setFont(getLabelFont(*zenLabelDisplay));		
		g.setColour(zenLabelDisplay->findColour(ZenLabelDisplay::ColourIds::textColourId));
		g.drawFittedText(zenLabelDisplay->getText(), 7, 0, zenLabelDisplay->getWidth()-14, zenLabelDisplay->getHeight(), zenLabelDisplay->getJustificationType(), 1, 0.01f);
		return;
	}

	//Rotary Slider Label draw
	g.fillAll(label.findColour(Label::backgroundColourId));

	if (!label.isBeingEdited())
	{
		const float alpha = label.isEnabled() ? 1.0f : 0.5f;
		const Font font(getLabelFont(label));

		g.setFont(font);

		Rectangle<int> textArea(label.getBorderSize().subtractedFrom(label.getLocalBounds()));
		
		// Draw shadow
		ZenRotaryEditorLabel* zenLabel = dynamic_cast<ZenRotaryEditorLabel*>(&label);
		if (zenLabel != nullptr && zenLabel->getTextHasShadow() && zenLabel->getWidth() > 28)
		{
			Rectangle<int> shadowArea = textArea;
			shadowArea.setPosition(textArea.getTopLeft().translated(2, 2));
			g.setColour(Colour((juce::uint8)0, 0, 0, (float)0.50f));

			g.drawFittedText(label.getText(), shadowArea, label.getJustificationType(),
				jmax(1, (int)(textArea.getHeight() / font.getHeight())), 0.01f);
		}		
		
		//Draw actual text
		g.setColour(label.findColour(Label::textColourId).withMultipliedAlpha(alpha));
		g.drawFittedText(label.getText(), textArea, label.getJustificationType(),
		                 jmax(1, (int)(textArea.getHeight() / font.getHeight())), 0.01f);

		g.setColour(label.findColour(Label::outlineColourId).withMultipliedAlpha(alpha));
	}
	else if (label.isEnabled())
	{
		g.setColour(label.findColour(Label::outlineColourId));
	}	
	g.drawRect(label.getLocalBounds());
}

void ZenLookAndFeel::drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour,
                                          bool isMouseOverButton, bool isButtonDown)
{
	g.setOpacity(1.0f);
	Colour darkColor(40, 40, 40);

	Colour baseColour(darkColor.withMultipliedSaturation(button.hasKeyboardFocus(true) ? 1.3f : 0.9f)
	                           .withMultipliedAlpha(button.isEnabled() ? 0.9f : 0.5f));

	if (isButtonDown || isMouseOverButton) baseColour = baseColour.contrasting(isButtonDown ? 0.2f : 0.1f);

	const bool flatOnLeft = button.isConnectedOnLeft();
	const bool flatOnRight = button.isConnectedOnRight();
	const bool flatOnTop = button.isConnectedOnTop();
	const bool flatOnBottom = button.isConnectedOnBottom();

	const float width = button.getWidth() - 1.0f;
	const float height = button.getHeight() - 1.0f;

	if (width > 0 && height > 0)
	{
		const float cornerSize = 4.0f;

		Path outline;
		outline.addRoundedRectangle(0.5f, 0.5f, width, height, cornerSize, cornerSize,
		                            !(flatOnLeft || flatOnTop),
		                            !(flatOnRight || flatOnTop),
		                            !(flatOnLeft || flatOnBottom),
		                            !(flatOnRight || flatOnBottom));

		drawButtonShape(g, outline, baseColour, height);
	}
}

void ZenLookAndFeel::drawButtonShape(Graphics& g, const Path& outline, Colour baseColour, float height)
{
	g.setOpacity(1.0f);
	const float mainBrightness = baseColour.getBrightness();
	const float mainAlpha = baseColour.getFloatAlpha();

	g.setGradientFill(ColourGradient(baseColour.brighter(0.2f), 0.0f, 0.0f,
	                                 baseColour.darker(0.25f), 0.0f, height, false));
	g.fillPath(outline);

	g.setColour(Colours::white.withAlpha(0.4f * mainAlpha * mainBrightness * mainBrightness));
	g.strokePath(outline, PathStrokeType(1.0f), AffineTransform::translation(0.0f, 1.0f)
	             .scaled(1.0f, (height - 1.6f) / height));

	g.setColour(Colours::black.withAlpha(0.4f * mainAlpha));
	g.strokePath(outline, PathStrokeType(1.0f));
}

Slider::SliderLayout ZenLookAndFeel::getSliderLayout(Slider& slider)
{
	// 1. compute the actually visible textBox size from the slider textBox size and some additional constraints
	ZenRotaryFilmStripSlider* zenSlider = dynamic_cast<ZenRotaryFilmStripSlider*>(&slider);	

	int minXSpace = 0;
	int minYSpace = 0;

	if (zenSlider != nullptr)
	{
		Slider::TextEntryBoxPosition textBoxPos = zenSlider->getTextBoxPosition();

		if (textBoxPos == Slider::TextBoxLeft || textBoxPos == Slider::TextBoxRight) minXSpace = 30;
		else minYSpace = 15;

		Rectangle<int> localBounds = zenSlider->getLocalBounds();

		const int textBoxWidth = jmax(0, jmin(zenSlider->getTextBoxWidth(), localBounds.getWidth() - minXSpace));
		const int textBoxHeight = jmax(0, jmin(zenSlider->getTextBoxHeight(), localBounds.getHeight() - minYSpace));

		Slider::SliderLayout layout;

		// 2. set the textBox bounds

		if (textBoxPos != Slider::NoTextBox)
		{
			if (zenSlider->isBar())
			{
				layout.textBoxBounds = localBounds;
			} else
			{
				layout.textBoxBounds.setWidth(textBoxWidth);
				layout.textBoxBounds.setHeight(textBoxHeight);				

				//Set X Coordinate of text box
				if (textBoxPos == Slider::TextBoxLeft)
				{
					layout.textBoxBounds.setX(0);
				}
				else if (textBoxPos == Slider::TextBoxRight)
				{
					layout.textBoxBounds.setX(localBounds.getWidth() - textBoxWidth);
				} else /* above or below -> centre horizontally */
				{
					layout.textBoxBounds.setX((localBounds.getWidth() - textBoxWidth) / 2);
				}

				//Set Y Coordinate of text box
				if (textBoxPos == Slider::TextBoxCentered)
				{
					layout.textBoxBounds.setY((localBounds.getHeight() - textBoxHeight) / 2);					
				} else if (textBoxPos == Slider::TextBoxAbove)
				{
					layout.textBoxBounds.setY(0); // Original
					
				} else if (textBoxPos == Slider::TextBoxBelow)
				{
					layout.textBoxBounds.setY(localBounds.getHeight() - textBoxHeight);
				}
				else /* left or right -> centre vertically */
				{
					layout.textBoxBounds.setY((localBounds.getHeight() - textBoxHeight) / 2);
				}
			}
		}

		// 3. set the zenSlider bounds
		layout.sliderBounds = localBounds;

		if (zenSlider->isBar())
		{
			layout.sliderBounds.reduce(1, 1); // bar border
		} else
		{
			if (textBoxPos == Slider::TextBoxLeft) 
				layout.sliderBounds.removeFromLeft(textBoxWidth);
			else if (textBoxPos == Slider::TextBoxRight) 
				layout.sliderBounds.removeFromRight(textBoxWidth);
			else if (textBoxPos == Slider::TextBoxAbove) 
				layout.sliderBounds.removeFromTop(textBoxHeight);
			else if (textBoxPos == Slider::TextBoxBelow) 
				layout.sliderBounds.removeFromBottom(textBoxHeight);

			const int thumbIndent = getSliderThumbRadius(*zenSlider);

			if (zenSlider->isHorizontal()) layout.sliderBounds.reduce(thumbIndent, 0);
			else if (zenSlider->isVertical()) layout.sliderBounds.reduce(0, thumbIndent);
		}

		return layout;
	}
	else
	{
		Slider::TextEntryBoxPosition textBoxPos = slider.getTextBoxPosition();

		if (textBoxPos == Slider::TextBoxLeft || textBoxPos == Slider::TextBoxRight) minXSpace = 30;
		else minYSpace = 15;

		Rectangle<int> localBounds = slider.getLocalBounds();

		const int textBoxWidth = jmax(0, jmin(slider.getTextBoxWidth(), localBounds.getWidth() - minXSpace));
		const int textBoxHeight = jmax(0, jmin(slider.getTextBoxHeight(), localBounds.getHeight() - minYSpace));

		Slider::SliderLayout layout;

		// 2. set the textBox bounds

		if (textBoxPos != Slider::NoTextBox)
		{
			if (slider.isBar())
			{
				layout.textBoxBounds = localBounds;
			}
			else
			{
				layout.textBoxBounds.setWidth(textBoxWidth);
				layout.textBoxBounds.setHeight(textBoxHeight);

				if (textBoxPos == Slider::TextBoxLeft) layout.textBoxBounds.setX(0);
				else if (textBoxPos == Slider::TextBoxRight) layout.textBoxBounds.setX(localBounds.getWidth() - textBoxWidth);
				else /* above or below -> centre horizontally */ layout.textBoxBounds.setX((localBounds.getWidth() - textBoxWidth) / 2);

				if (textBoxPos == Slider::TextBoxAbove)
				{
					//layout.textBoxBounds.setY(0); // Original
					layout.textBoxBounds.setY((localBounds.getHeight() - textBoxHeight) / 2);
				}
				else if (textBoxPos == Slider::TextBoxBelow) layout.textBoxBounds.setY(localBounds.getHeight() - textBoxHeight);
				else /* left or right -> centre vertically */ layout.textBoxBounds.setY((localBounds.getHeight() - textBoxHeight) / 2);
			}
		}

		// 3. set the slider bounds

		layout.sliderBounds = localBounds;

		if (slider.isBar())
		{
			layout.sliderBounds.reduce(1, 1); // bar border
		}
		else
		{
			if (textBoxPos == Slider::TextBoxLeft) layout.sliderBounds.removeFromLeft(textBoxWidth);
			else if (textBoxPos == Slider::TextBoxRight) layout.sliderBounds.removeFromRight(textBoxWidth);
			else if (textBoxPos == Slider::TextBoxAbove) layout.sliderBounds.removeFromTop(textBoxHeight);
			else if (textBoxPos == Slider::TextBoxBelow) layout.sliderBounds.removeFromBottom(textBoxHeight);

			const int thumbIndent = getSliderThumbRadius(slider);

			if (slider.isHorizontal()) layout.sliderBounds.reduce(thumbIndent, 0);
			else if (slider.isVertical()) layout.sliderBounds.reduce(0, thumbIndent);
		}

		return layout;
	}
}


ZenRotaryEditorLabel* ZenLookAndFeel::createSliderTextBox(Slider& slider)
{
	// Overridden code:
	ZenRotaryEditorLabel* const zLabel = new ZenRotaryEditorLabel(slider.getName(), true);

	zLabel->setJustificationType(Justification::centred);
	zLabel->setKeyboardType(TextInputTarget::decimalKeyboard);

	zLabel->setColour(Label::textColourId, slider.findColour(Slider::textBoxTextColourId));
	zLabel->setColour(Label::backgroundColourId,
		(slider.getSliderStyle() == Slider::LinearBar || slider.getSliderStyle() == Slider::LinearBarVertical)
		? Colours::transparentBlack
		: slider.findColour(Slider::textBoxBackgroundColourId));
	zLabel->setColour(Label::outlineColourId, slider.findColour(Slider::textBoxOutlineColourId));
	zLabel->setColour(TextEditor::textColourId, slider.findColour(Slider::textBoxTextColourId));
	zLabel->setColour(TextEditor::backgroundColourId,
		slider.findColour(Slider::textBoxBackgroundColourId)
		.withAlpha((slider.getSliderStyle() == Slider::LinearBar || slider.getSliderStyle() == Slider::LinearBarVertical)
			? 0.7f : 1.0f));
	zLabel->setColour(TextEditor::outlineColourId, slider.findColour(Slider::textBoxOutlineColourId));
	zLabel->setColour(TextEditor::highlightColourId, slider.findColour(Slider::textBoxHighlightColourId));

	ZenRotaryFilmStripSlider* zSlider = dynamic_cast<ZenRotaryFilmStripSlider*>(&slider);
	if (NULL != zSlider && zSlider->getHeight() != 0)
	{
		//slider->pimpl->sliderRect
		int sWidth = zSlider->getSliderRect().getWidth();
		int sHeight = zSlider->getSliderRect().getHeight();
	}
	
	return zLabel;
}

void ZenLookAndFeel::fillTextEditorBackground(Graphics& g, int width, int height, TextEditor& textEditor)
{
	g.setOpacity(1.0f);
	ZenTextEditor* zenTextEditor = dynamic_cast<ZenTextEditor*>(&textEditor);
	if (zenTextEditor != nullptr)
	{		
		//Draw left end
		int teWidth = zenTextEditor->getWidth();
		int teHeight = zenTextEditor->getHeight();
		Image imageToDraw = zenTextEditor->getLeftEndImage();
		
		int leftImageWidth = imageToDraw.getWidth();
 		g.drawImage(imageToDraw, 
 			(0),   //destX
 			(0), //dest Y
 			(leftImageWidth), //dest width
 			(teHeight), //dest height
 			(0), //sourceX
 			(0), //sourceY
 			(leftImageWidth), //source width 
 			(imageToDraw.getHeight()), //source height
 			false);

		//Draw right end
		imageToDraw = zenTextEditor->getRightEndImage();
		int rightImageWidth = imageToDraw.getWidth();
		g.drawImage(imageToDraw,
			(teWidth - rightImageWidth),   //destX
			(0), //dest Y
			(rightImageWidth), //dest width
			(teHeight), //dest height
			(0), //sourceX
			(0), //sourceY
			(rightImageWidth), //source width 
			(imageToDraw.getHeight()), //source height
			false);

		// Draw center
		imageToDraw = zenTextEditor->getCenterImage();
		int remainingWidth = teWidth - leftImageWidth - rightImageWidth;
		g.drawImage(imageToDraw,
			(leftImageWidth),   //destX
			(0), //dest Y
			(remainingWidth), //dest width
			(teHeight), //dest height
			(0), //sourceX
			(0), //sourceY
			(imageToDraw.getWidth()), //source width 
			(imageToDraw.getHeight()), //source height
			false);

	}
	else //If not Zen Slider
	{
		//LookAndFeel_V2::fillTextEditorBackground(g, width, height, textEditor);
	}
}


void ZenLookAndFeel::drawImageButton(Graphics& g, Image* imageToDraw, int imageX, int imageY, int imageW, int imageH, const Colour& overlayColour, float imageOpacity, ImageButton& buttonToDraw)
{
	if (!buttonToDraw.isEnabled())
		imageOpacity *= 0.3f;

	AffineTransform t = RectanglePlacement(RectanglePlacement::stretchToFit)
		.getTransformToFit(imageToDraw->getBounds().toFloat(),
			Rectangle<int>(imageX, imageY, imageW, imageH).toFloat());

	if (!overlayColour.isOpaque())
	{
		g.setOpacity(imageOpacity);
		g.drawImageTransformed(*imageToDraw, t, false);
	}

	if (!overlayColour.isTransparent())
	{
		g.setColour(overlayColour);
		g.drawImageTransformed(*imageToDraw, t, true);
	}
}

void ZenLookAndFeel::drawZenImageButton(Graphics& g, ZenImageButton& buttonToDraw)
{
	Image left, center, right;
	g.setOpacity(1.0f);
	bool toggleable = buttonToDraw.getClickingTogglesState();
	
	if (toggleable)
	{
		if (buttonToDraw.getToggleState() )
		{
			if (buttonToDraw.isMouseOver())
			{			
				left = buttonToDraw.getPressedHoverLeft();
				center = buttonToDraw.getPressedHoverCenter();
				right = buttonToDraw.getPressedHoverRight();
			} else
			{
				left = buttonToDraw.getPressedLeft();
				center = buttonToDraw.getPressedCenter();
				right = buttonToDraw.getPressedRight();
			}
		} else //button not pressed
		{
			if (buttonToDraw.isMouseOver())
			{
				left = buttonToDraw.getHoverLeft();
				center = buttonToDraw.getHoverCenter();
				right = buttonToDraw.getHoverRight();
			} else
			{
				left = buttonToDraw.getNormalLeft();
				center = buttonToDraw.getNormalCenter();
				right = buttonToDraw.getNormalRight();
			}
		} 
	} else //not toggleable -- draw hover and show click if mouse pressed
	{
		if (buttonToDraw.isMouseOver())
		{
			if (buttonToDraw.isMouseButtonDown())
			{
				left = buttonToDraw.getPressedHoverLeft();
				center = buttonToDraw.getPressedHoverCenter();
				right = buttonToDraw.getPressedHoverRight();
			} else //mouse not down
			{
				left = buttonToDraw.getHoverLeft();
				center = buttonToDraw.getHoverCenter();
				right = buttonToDraw.getHoverRight();
			}
		} else // mouse not over button
		{
			left = buttonToDraw.getNormalLeft();
			center = buttonToDraw.getNormalCenter();
			right = buttonToDraw.getNormalRight();			
		}
	}

	g.drawImage(left, 0, 0, left.getWidth(), buttonToDraw.getHeight(),
		0, 0, left.getWidth(), left.getHeight());
	g.drawImage(center, left.getWidth(), 0, buttonToDraw.getWidth() - left.getWidth() - right.getWidth(), buttonToDraw.getHeight(),
		0, 0, center.getWidth(), center.getHeight());
	g.drawImage(right, buttonToDraw.getWidth() - right.getWidth(), 0, right.getWidth(), buttonToDraw.getHeight(),
		0, 0, right.getWidth(), right.getHeight());
	
	if (buttonToDraw.getToggleState() || (!toggleable && buttonToDraw.isMouseButtonDown()))
		g.setColour(buttonToDraw.getTextColorButtonOn()); // Draw text as green when pressed
	else 
		g.setColour(buttonToDraw.getTextColorButtonOff());
	g.setOpacity(1.0f);
	g.setFont(getZenImageButtonFont(buttonToDraw));
	g.drawFittedText(buttonToDraw.getButtonText(), buttonToDraw.getLocalBounds().reduced(5), Justification::centred, 1, 0.01f);

}

void ZenLookAndFeel::drawZenComboBox(Graphics& g, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW, int buttonH, ZenComboBox& inBox)
{
	const Image& left = inBox.getLeftImage();
	const Image& center = inBox.getCenterImage();
	const Image& right = inBox.getRightImage();
	g.setOpacity(1.0f);
	g.drawImage(left, 0, 0, left.getWidth(), inBox.getHeight(),
		0, 0, left.getWidth(), left.getHeight());
	g.drawImage(center, left.getWidth(), 0, inBox.getWidth() - left.getWidth() - right.getWidth(), inBox.getHeight(),
		0, 0, center.getWidth(), center.getHeight());
	g.drawImage(right, inBox.getWidth() - right.getWidth(), 0, right.getWidth(), inBox.getHeight(),
		0, 0, right.getWidth(), right.getHeight());

	//draw disabled overlay
	if(! inBox.isEnabled())
	{
		g.setColour(Colours::black);
		g.setOpacity(0.6f);		
		g.fillRoundedRectangle(0, 0, inBox.getWidth(), inBox.getHeight(), 5);		
	}
}

Label* ZenLookAndFeel::createComboBoxTextBox(ComboBox& inBox)
{
	return new Label(String::empty, String::empty);
	/* Overridden method
	return new Label(String::empty, String::empty);
	*/
}

void ZenLookAndFeel::positionComboBoxText(ComboBox& inBox, Label& inLabel)
{
	inLabel.setBounds(1, 1,
		inBox.getWidth() + 3 - inBox.getHeight(),
		inBox.getHeight() - 2);

	inLabel.setFont(getComboBoxFont(inBox));
}

//***********************POPUP MENU METHODS*****************************************/

void ZenLookAndFeel::drawPopupMenuBackground(Graphics& g, int width, int height)
{
	g.fillAll(findColour(PopupMenu::backgroundColourId));
	ignoreUnused(width, height);

#if ! JUCE_MAC
	g.setColour(findColour(PopupMenu::textColourId).withAlpha(0.6f));
	g.drawRect(0, 0, width, height);
#endif
}

void ZenLookAndFeel::drawPopupMenuItem(Graphics& g, const Rectangle<int>& area, bool isSeparator, bool isActive, 
	bool isHighlighted, bool isTicked, bool hasSubMenu, const String& text, 
	const String& shortcutKeyText, const Drawable* icon, const Colour* textColourToUse)
{
	if (isSeparator)
	{
		Rectangle<int> r(area.reduced(5, 0));
		r.removeFromTop(r.getHeight() / 2 - 1);

		g.setColour(Colour(0x33000000));
		g.fillRect(r.removeFromTop(1));

		g.setColour(Colour(0x66ffffff));
		g.fillRect(r.removeFromTop(1));
	}
	else
	{
		Colour textColour(findColour(PopupMenu::textColourId));

		if (textColourToUse != nullptr)
			textColour = *textColourToUse;

		Rectangle<int> r(area.reduced(1));

		if (isHighlighted)
		{
			g.setColour(findColour(PopupMenu::highlightedBackgroundColourId));
			g.fillRect(r);

			g.setColour(findColour(PopupMenu::highlightedTextColourId));
		}
		else
		{
			g.setColour(textColour);
		}

		if (!isActive)
			g.setOpacity(0.3f);

		Font font(getPopupMenuFont());

		const float maxFontHeight = area.getHeight() / 1.3f;

		if (font.getHeight() > maxFontHeight)
			font.setHeight(maxFontHeight);

		g.setFont(font);

		Rectangle<float> iconArea(r.removeFromLeft((r.getHeight() * 5) / 4).reduced(3).toFloat());

		if (icon != nullptr)
		{
			icon->drawWithin(g, iconArea, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
		}
		else if (isTicked)
		{
			const Path tick(getTickShape(1.0f));
			g.fillPath(tick, tick.getTransformToScaleToFit(iconArea, true));
		}

		if (hasSubMenu)
		{
			const float arrowH = 0.6f * getPopupMenuFont().getAscent();

			const float x = (float)r.removeFromRight((int)arrowH).getX();
			const float halfH = (float)r.getCentreY();

			Path p;
			p.addTriangle(x, halfH - arrowH * 0.5f,
				x, halfH + arrowH * 0.5f,
				x + arrowH * 0.6f, halfH);

			g.fillPath(p);
		}

		r.removeFromRight(3);
		g.drawFittedText(text, r, Justification::centredLeft, 1);

		if (shortcutKeyText.isNotEmpty())
		{
			Font f2(font);
			f2.setHeight(f2.getHeight() * 0.75f);
			f2.setHorizontalScale(0.95f);
			g.setFont(f2);

			g.drawText(shortcutKeyText, r, Justification::centredRight, true);
		}
	}
}

void ZenLookAndFeel::drawPopupMenuSectionHeader(Graphics& g, const Rectangle<int>& area, const String& sectionName)
{
	g.setFont(getPopupMenuFont().boldened());
	g.setColour(findColour(PopupMenu::headerTextColourId));

	g.drawFittedText(sectionName,
		area.getX() + 12, area.getY(), area.getWidth() - 16, (int)(area.getHeight() * 0.8f),
		Justification::bottomLeft, 1);
}

Font ZenLookAndFeel::getPopupMenuFont()
{
	return Font(17.0f);
}

void ZenLookAndFeel::drawPopupMenuUpDownArrow(Graphics& g, int width, int height, bool isScrollUpArrow)
{
	const Colour background(findColour(PopupMenu::backgroundColourId));

	g.setGradientFill(ColourGradient(background, 0.0f, height * 0.5f,
		background.withAlpha(0.0f),
		0.0f, isScrollUpArrow ? ((float)height) : 0.0f,
		false));

	g.fillRect(1, 1, width - 2, height - 2);

	const float hw = width * 0.5f;
	const float arrowW = height * 0.3f;
	const float y1 = height * (isScrollUpArrow ? 0.6f : 0.3f);
	const float y2 = height * (isScrollUpArrow ? 0.3f : 0.6f);

	Path p;
	p.addTriangle(hw - arrowW, y1,
		hw + arrowW, y1,
		hw, y2);

	g.setColour(findColour(PopupMenu::textColourId).withAlpha(0.5f));
	g.fillPath(p);
}

Font ZenLookAndFeel::getComboBoxFont(ComboBox& inBox)
{
	return Font("Futura Book", jmin(15.0f, inBox.getHeight() * 0.85f), Font::plain);
}

Font ZenLookAndFeel::getLabelFont(Label& label)
{
	if (label.getWidth() > 30)
		return Font("Futura Book", 16, Font::plain);
	else
		return Font("Futura Book", 12, Font::plain);
}

Font ZenLookAndFeel::getZenImageButtonFont(ZenImageButton& zenbtn)
{
	if (zenbtn.getHeight() > 30)
	{
		
		Font theFont("Futura Book", 18, Font::plain);
		theFont.setExtraKerningFactor(0.06f);
		return theFont;
	}
	else
	{		
		Font theFont("Futura Book", 14, Font::plain);
		theFont.setExtraKerningFactor(0.06f);
		return theFont;
	}
}


