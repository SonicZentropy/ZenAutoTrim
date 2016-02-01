/*==============================================================================
//  NotepadComponent.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2016/01/31
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Creates a component basic notepad
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#pragma once
#ifndef NOTEPAD_COMPONENT_H_INCLUDED
#define NOTEPAD_COMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class NotepadComponent : public Component,
	public FileBasedDocument,
	private TextEditor::Listener
{

public:
	NotepadComponent(const String& name, const String& contents);

	~NotepadComponent();

	void resized() override;

	String getDocumentTitle() override;

	Result loadDocument(const File& file) override;

	Result saveDocument(const File& file) override;

	File getLastDocumentOpened() override;

	void setLastDocumentOpened(const File& /*file*/) override;

#if JUCE_MODAL_LOOPS_PERMITTED
	File getSuggestedSaveAsFile(const File&) override;
#endif

private:
	Value textValueObject;
	TextEditor editor;

	void textEditorTextChanged(TextEditor& ed) override;

	void textEditorReturnKeyPressed(TextEditor&) override {}
	void textEditorEscapeKeyPressed(TextEditor&) override {}
	void textEditorFocusLost(TextEditor&) override {}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NotepadComponent);
};

#endif //NOTEPAD_COMPONENT_H_INCLUDED