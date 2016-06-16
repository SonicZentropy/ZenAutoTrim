/*==============================================================================
//  ZenDebugEditor.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/28
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Zen Debug Component for window - contains Value Tree Parameter tab,
//  Buffer Visualization Tab, and Labels tab
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef ZENDEBUGEDITORCOMPONENT_H_INCLUDED
#define ZENDEBUGEDITORCOMPONENT_H_INCLUDED

#include "JuceHeader.h"
#include "zen_utils/components/notepadcomp/NotepadComponent.h"
#include "GUI/component_debugger.h"
#include "GUI/ZenMidiVisualiserComponent.h"
#include "GUI/buffer_visualiser.h"
#include "GUI/value_tree_editor.h"

/** Creates an integrated debugging utility framework. The window allows for
display and live editing of all components, used fonts, and chosen colors.
It also provides a MIDI console that reports on detected MIDI events, a realtime
Value Tree editor, and a buffer visualization framework.

At minimum, you will need the following in your code to manage the debugger:

Application member field:
ScopedPointer<ZenDebugEditor> debugWindow;

Applicaton initializer:

	#ifdef JUCE_DEBUG
	debugWindow = ZenDebugEditor::getInstance();
	debugWindow->setSize(650, 400);
	//Open in bottom right corner
	debugWindow->setTopLeftPosition(1900 - debugWindow->getWidth(), 1040 - debugWindow->getHeight());
	#endif

Application destructor:
	debugWindow->deleteInstance();

For component debugging, you will use the following within your main content component

Component initializer:
ZEN_COMPONENT_DEBUG_ATTACH(this);

Component destructor:
ZEN_COMPONENT_DEBUG_DETACH();

To add buffers to be visualized, you must call the buffer function every time you wish
to display the buffer.  Note that this is most likely to happen in your process block
AND note that this functionality tanks the speed of said block.  You WILL suffer horrendous
performance by using this.

//Audio buffer visualization
//ZEN_DEBUG_BUFFER("Left Buffer Pre", leftData, buffer.getNumSamples(), -1, 1);
//ZEN_DEBUG_BUFFER("Right Buffer Pre", rightData, buffer.getNumSamples(), -1, 1);
*/
class ZenDebugEditor : public DocumentWindow
{
public:

	ZenDebugEditor();
	~ZenDebugEditor();

	/// <summary>
	/// Adds a text label to the main Debug Window
	/// </summary>
	void addTraceLabel(const String& inName, const String& inText);

	/// <summary>
	/// Adds a text label to the main Debug Window with text set to the Value passed in
	/// </summary>
	void addTraceLabel(const String& inName, Value& theValue);

	/// <summary>
	/// Adds a text label to the main Debug window or updates the value of an existing
	/// label matching inName.
	/// </summary>
	void addOrSetTraceLabel(const String& inName, const String& inText);

	/// <summary>
	/// Removes a trace label from the debug window and stops updating.
	/// </summary>
	void removeTraceLabel(const String& inName);

	/// <summary>
	/// Changes the displayed text of the requested label
	/// </summary>
	void setLabelText(const String& labelName, const String& inText);

	/// <summary>
	/// Changes the displayed text of the requested label
	/// </summary>
	void setLabelText(const String& labelName, const float inText);

	/// <summary>Sets the note text component.</summary>
	void setNoteText(const String& inText);

	void appendNoteText(String inText);

	void closeButtonPressed() override;

	/// <summary>
	/// Sets the main debug source for display and editing a value tree.  This is normally used
	/// for parameter debugging.
	/// </summary>
	/// <param name="rootTree">The root value tree node to display and manage</param>
	void setSource(ValueTree& rootTree);

	/// <summary>
	/// Attaches a component to the component debugging tab.  This is
	/// basically only for use in a JUCE Editor class
	/// </summary>
	/// <param name="rootComponent"></param>
	void attachComponentDebugger(Component* rootComponent);

	void removeInstanceComponentDebugger();

	//void refreshComponentDebugger();
	void resized() override;

	//Singleton
	juce_DeclareSingleton(ZenDebugEditor, false);
	//End Singleton

	static void removeComponentDebugger();

private:

	ScopedPointer<TabbedComponent> tabsComponent;
	ScopedPointer<ValueTreeEditor::Editor> valueTreeEditorComponent;
	ScopedPointer<BufferVisualiser> bufferVisualiserComponent;
	ScopedPointer<ZenMidiVisualiserComponent> midiVisualiserComponent;
	ScopedPointer<ComponentDebugger> componentVisualiserComponent;
	ScopedPointer<NotepadComponent> notepadComponent;

	bool componentTabIsAttached = false;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZenDebugEditor);
};


//============================================================================== 
// INLINE MACRO FUNCTION TRANSLATIONS 
//============================================================================== 

#ifdef JUCE_DEBUG 
inline void ZEN_LABEL_TRACE(const String& labelName, const String& labelText)
{
	ZenDebugEditor::getInstance()->addOrSetTraceLabel(labelName, labelText);
};

inline void ZEN_REMOVE_LABEL_TRACE(const String& labelName)
{
	ZenDebugEditor::getInstance()->removeTraceLabel(labelName);
}

inline void ZEN_DEBUG_BUFFER(const String & name, float * data, int size, float min, float max)
{
	Store::getInstance()->record(name, data, size, min, max);
}

inline void ZEN_COMPONENT_DEBUG_ATTACH(Component* rootComponent)
{
	ZenDebugEditor::getInstance()->attachComponentDebugger(rootComponent);
	
}

inline void ZEN_COMPONENT_DEBUG_DETACH()
{
	ZenDebugEditor::getInstanceWithoutCreating()->removeComponentDebugger();
}

#else 
inline void ZEN_LABEL_TRACE(const String& labelName, const String& labelText)
{};

inline void ZEN_REMOVE_LABEL_TRACE(const String& labelName)
{};

inline void ZEN_DEBUG_BUFFER(const String & name, float * data, int size, float min, float max)
{};

inline void ZEN_COMPONENT_DEBUG_ATTACH(Component* rootComponent)
{};

inline void ZEN_COMPONENT_DEBUG_DETACH()
{};


#endif // JUCE_DEBUG 

#endif // ZENDEBUGCOMPONENT_H_INCLUDED 
