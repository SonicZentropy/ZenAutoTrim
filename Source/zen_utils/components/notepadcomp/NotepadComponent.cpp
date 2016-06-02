/*==============================================================================
//  NotepadComponent.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2016/02/22
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Creates component for basic text notepad
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#include "NotepadComponent.h"

NotepadComponent::NotepadComponent(const String& name, const String& contents) : FileBasedDocument(".jnote", "*.jnote",
	"Browse for note to load",
	"Choose file to save note to"),
	textValueObject(contents)
{
	// we need to use an separate Value object as our text source so it doesn't get marked
	// as changed immediately
	setName(name);

	editor.setMultiLine(true);
	editor.setReturnKeyStartsNewLine(true);
	editor.getTextValue().referTo(textValueObject);
	editor.addListener(this);
	addAndMakeVisible(editor);

	addAndMakeVisible(clearBtn = new TextButton("Clear Button", "Clear text"));
	clearBtn->setButtonText("Clear");
	clearBtn->addListener(this);	
}

NotepadComponent::~NotepadComponent()
{
	clearBtn = nullptr;
	editor.removeListener(this);
}

void NotepadComponent::resized()
{
	editor.setBounds(getLocalBounds());
	clearBtn->setBounds(getLocalBounds().getWidth() - 51, getLocalBounds().getHeight() - 25, 50, 24);
}

String NotepadComponent::getDocumentTitle()
{
	return getName();
}

Result NotepadComponent::loadDocument(const File& file)
{
	editor.setText(file.loadFileAsString());
	return Result::ok();
}

Result NotepadComponent::saveDocument(const File& file)
{
	// attempt to save the contents into the given file
	FileOutputStream os(file);

	if (os.openedOk())
		os.writeText(editor.getText(), false, false);

	return Result::ok();
}

File NotepadComponent::getLastDocumentOpened()
{
	// not interested in this for now
	return File::nonexistent;
}

void NotepadComponent::setLastDocumentOpened(const File& /*file*/)
{
	// not interested in this for now
}

void NotepadComponent::textEditorTextChanged(TextEditor& ed)
{
	// let our FileBasedDocument know we've changed
	if (&ed == &editor)
		changed();
}

void NotepadComponent::buttonClicked(Button* buttonThatWasClicked)
{
	if (buttonThatWasClicked == clearBtn)
	{
		editor.clear();
	}
}
