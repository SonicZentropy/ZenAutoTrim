/*==============================================================================
//  ZenDebugEditor.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/28
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Implementation for ZenDebugEditor.h - Wraps various debug components
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#include "ZenDebugEditor.h"

ZenDebugEditor::ZenDebugEditor() :
	DocumentWindow("Zen Debug",
	Colours::lightgrey,
	DocumentWindow::allButtons)
{
	this->setName("ValueTreeEditorWindow");

	tabsComponent = new TabbedComponent(TabbedButtonBar::TabsAtTop);
	tabsComponent->setName("DebugTabbedComponent");

	valueTreeEditorComponent = new ValueTreeEditor::Editor("ValueTreeEditor");
	tabsComponent->addTab("Params", Colours::lightgrey, valueTreeEditorComponent, false);

	bufferVisualiserComponent = new BufferVisualiser("BufferVisualiser");
	tabsComponent->addTab("Buffers", Colours::lightgrey, bufferVisualiserComponent->getComponent(), false);

	midiVisualiserComponent = new ZenMidiVisualiserComponent("MidiVisualiser");
	tabsComponent->addTab("MIDI", Colours::lightgrey, midiVisualiserComponent, false);

	notepadComponent = new NotepadComponent("Notepad", "");
	tabsComponent->addTab("Exec", Colours::lightgrey, notepadComponent, false);

	componentVisualiserComponent = nullptr;

	this->setSize(400, 400);
	tabsComponent->setCurrentTabIndex(3);
	tabsComponent->setBounds(0, 0, getWidth(), getHeight());

	setContentNonOwned(tabsComponent, false);
	setResizable(true, false);
	setUsingNativeTitleBar(true);
	centreWithSize(getWidth(), getHeight());
	setVisible(true);
}

ZenDebugEditor::~ZenDebugEditor()
{
	valueTreeEditorComponent->setTree(ValueTree::invalid);
	if (valueTreeEditorComponent->isOnDesktop())
	{
		valueTreeEditorComponent->removeFromDesktop();
	}
	valueTreeEditorComponent = nullptr;
	if (this->isOnDesktop())
	{
		this->removeFromDesktop();
	}
	tabsComponent = nullptr;
	bufferVisualiserComponent = nullptr;
	componentVisualiserComponent = nullptr;

	clearSingletonInstance();
	deleteInstance();
}

void ZenDebugEditor::addTraceLabel(const String& inName, const String& inText)
{
	this->valueTreeEditorComponent->addTraceLabel(inName, inText);
}

void ZenDebugEditor::addTraceLabel(const String& inName, Value& theValue)
{
	this->valueTreeEditorComponent->addTraceLabel(inName, theValue);
}

void ZenDebugEditor::removeTraceLabel(const String& inName)
{
	this->valueTreeEditorComponent->removeTraceLabel(inName);
}

void ZenDebugEditor::setLabelText(const String& labelName, const String& inText)
{
	this->valueTreeEditorComponent->setLabelText(labelName, String(inText));
}

void ZenDebugEditor::setLabelText(const String& labelName, const float inText)
{
	this->valueTreeEditorComponent->setLabelText(labelName, inText);
}

void ZenDebugEditor::setNoteText(const String& inText)
{
	this->notepadComponent->setNoteText(inText);
}

void ZenDebugEditor::appendNoteText(const String inText)
{
	this->notepadComponent->appendNoteText(inText);
}

void ZenDebugEditor::addOrSetTraceLabel(const String& inName, const String& inText)
{
	this->valueTreeEditorComponent->addOrSetTraceLabel(inName, inText);
}

void ZenDebugEditor::closeButtonPressed()
{
	setVisible(false);
}

void ZenDebugEditor::setSource(ValueTree& v)
{
	valueTreeEditorComponent->setTree(v);
}

void ZenDebugEditor::attachComponentDebugger(Component* rootComponent)
{
	if(! componentTabIsAttached)
	{
		componentVisualiserComponent = new ComponentDebugger(rootComponent, getWidth(), getHeight(), "ComponentDebugger");
	
		tabsComponent->addTab("Comps", Colours::lightgrey, componentVisualiserComponent, false, 3);
		componentTabIsAttached = true;
		tabsComponent->setCurrentTabIndex(3, true);
	}	
	//refreshComponentDebugger();
}

void ZenDebugEditor::removeInstanceComponentDebugger()
{
	if (componentTabIsAttached)
	{
		tabsComponent->removeTab(3);
		componentVisualiserComponent = nullptr;
		componentTabIsAttached = false;
	}
}

void ZenDebugEditor::resized()
{
	//DBG("In ZenDebugEditor::resized() ");
	tabsComponent->setSize(getWidth(), getHeight());
	/*for (auto currComponent : tabsComponent->getTabContentComponentArray())
	{
		currComponent->setSize(getWidth(), getHeight());
	}*/
}

void ZenDebugEditor::removeComponentDebugger()
{
	if (_singletonInstance != nullptr)
	{
		if (_singletonInstance->tabsComponent->getCurrentTabName() == "Comps")
			_singletonInstance->tabsComponent->setCurrentTabIndex(0);
		_singletonInstance->removeInstanceComponentDebugger();
	}
}

//Singleton
juce_ImplementSingleton(ZenDebugEditor)
