/*==============================================================================
//  ZenLabel.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 21 Apr 2016 3:15:40pm
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Generic CPP File
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#include "ZenRotaryEditorLabel.h"


ZenRotaryEditorLabel::ZenRotaryEditorLabel(String inName, bool inTextHasShadow)
	:Label(inName)
{
	textHasShadow = inTextHasShadow;
	this->setEditable(false, true, false);
}


void ZenRotaryEditorLabel::showEditor()
{	
	if (editor == nullptr)
	{
		addAndMakeVisible(editor = createEditorComponent());
		
		//editor->setColour(TextEditor::textColourId, Colours::black);
		editor->setColour(TextEditor::backgroundColourId, Colour(0x00ffffff));
		editor->setColour(TextEditor::highlightColourId, Colour(0xBBffffff));
		//editor->setColour(TextEditor::outlineColourId, Colour(0xFF00FF00));
		editor->setColour(TextEditor::focusedOutlineColourId, Colour(0xFFFFFFFF));
		editor->setText(getText(), false);
		editor->setKeyboardType(keyboardType);
		editor->addListener(this);
		editor->grabKeyboardFocus();

		if (editor == nullptr) // may be deleted by a callback
			return;

		editor->setHighlightedRegion(Range<int>(0, textValue.toString().length()));
		resized();
		repaint();

		editorShown(editor);
		enterModalState(false);
		editor->grabKeyboardFocus();
	}
}

// Pass mouse event on to parent so label doesn't interfere with mouse drag changing values
void ZenRotaryEditorLabel::mouseUp(const MouseEvent& event)
{
	if (isEnabled())
	{
		Component* parentComp = getParentComponent();
		parentComp->mouseUp(event.getEventRelativeTo(parentComp));
	}
}

// Pass mouse event on to parent so label doesn't interfere with mouse drag changing values
void ZenRotaryEditorLabel::mouseDrag(const MouseEvent& event)
{
	if (isEnabled())
	{
		Component* parentComp = getParentComponent();
		parentComp->mouseDrag(event.getEventRelativeTo(parentComp));
	}
}

void ZenRotaryEditorLabel::mouseDoubleClick(const MouseEvent& event)
{
	if (isEnabled()
		&& contains(event.getPosition())
		&& !(event.mouseWasDraggedSinceMouseDown() || event.mods.isPopupMenu()))
	{
		setColour(Slider::textBoxTextColourId, Colours::black);
		showEditor();
	}
}

// Pass mouse event on to parent so label doesn't interfere with mouse drag changing values
void ZenRotaryEditorLabel::mouseDown(const MouseEvent& event)
{
	if (isEnabled())
	{
		Component* parentComp = getParentComponent();
		parentComp->mouseDown(event.getEventRelativeTo(parentComp));
	}
}


static void copyColourIfSpecified(Label& l, TextEditor& ed, int colourID, int targetColourID)
{
	if (l.isColourSpecified(colourID) || l.getLookAndFeel().isColourSpecified(colourID))
		ed.setColour(targetColourID, l.findColour(colourID));
}

TextEditor* ZenRotaryEditorLabel::createEditorComponent()
{
	//DBG("In ZenRotaryEditorLabel::createEditorComponent() ");
	TextEditor* const ed = new TextEditor(getName());
	ed->applyFontToAllText(getLookAndFeel().getLabelFont(*this));
	copyAllExplicitColoursTo(*ed);

	copyColourIfSpecified(*this, *ed, textWhenEditingColourId, TextEditor::textColourId);
	copyColourIfSpecified(*this, *ed, backgroundWhenEditingColourId, TextEditor::backgroundColourId);
	copyColourIfSpecified(*this, *ed, outlineWhenEditingColourId, TextEditor::focusedOutlineColourId);

	//ed->setSize(getWidth() * 2, getHeight());
	//ed->setTopLeftPosition(Point<int>(ed->get)

	return ed;
}

// void ZenLabel::editorAboutToBeHidden(TextEditor* textEditor)
// {
// 	if (ComponentPeer* const peer = getPeer())
// 		peer->dismissPendingTextInput();
// 
// 	Component::BailOutChecker checker(this);
// 	listeners.callChecked(checker, &LabelListener::editorHidden, this, *textEditor);
// }

void ZenRotaryEditorLabel::resized()
{
	if (editor != nullptr)
	{		
		Rectangle<int> local(getLocalBounds());
		local.expand(2, 0);
		
		editor->setBounds(local);
	}
}
