/*==============================================================================
//  ZenZenAudioProcessorValueTreeState.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 27 May 2016 8:37:34pm
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Generic Header File
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef ZEN_AUDIOPROCESSORVALUETREESTATE_H_INCLUDED
#define ZEN_AUDIOPROCESSORVALUETREESTATE_H_INCLUDED

#include "JuceHeader.h"
//#include <parameters/ZenIntParameter.h>
//#include "parameters/ZenParameter.h"
//#include "parameters/ZenBoolParameter.h"
//#include "parameters/ZenDecibelParameter.h"

class ZenParameter;
class ZenBoolParameter;
class ZenDecibelParameter;
class ZenIntParameter;

#if JUCE_COMPILER_SUPPORTS_LAMBDAS || defined (DOXYGEN)

/**
This class contains a ValueTree which is used to manage an AudioProcessor's entire state.

It has its own internal class of parameter object which are linked to values
within its ValueTree, and which are each identified by a string ID.

To use: Create a ZenAudioProcessorValueTreeState, and give it some parameters
using createParameter().

You can get access to the underlying ValueTree object via the state member variable,
so you can add extra properties to it as necessary.

It also provides some utility child classes for connecting parameters directly to
GUI controls like sliders.
*/
class ZenAudioProcessorValueTreeState : private Timer,
	private ValueTree::Listener
{
public:
	/** Creates a state object for a given processor.

	The UndoManager is optional and can be a nullptr.
	After creating your state object, you should add parameters with the
	createAndAddParameter() method. Note that each ZenAudioProcessorValueTreeState
	should be attached to only one processor, and must have the same lifetime as the
	processor, as they will have dependencies on each other.
	*/
	ZenAudioProcessorValueTreeState(AudioProcessor& processorToConnectTo,
		UndoManager* undoManagerToUse);

	/** Destructor. */
	~ZenAudioProcessorValueTreeState();

	ZenDecibelParameter* createAndAddDecibelParameter(String paramID, String paramName, 
		float minValue, float maxValue, float defaultValue, 
		bool shouldBeSmoothed = false, float smoothingTime = 0.0f, 
		std::function<String(float)> valueToTextFunction = nullptr, 
		std::function<float(const String&)> textToValueFunction = nullptr);
	
	ZenDecibelParameter* addDecibelParameter(ZenDecibelParameter* inDecibelParam, 
		std::function<String(float)> valueToTextFunction = nullptr, 
		std::function<float(const String&)> textToValueFunction = nullptr);

	ZenBoolParameter* createAndAddBoolParameter(String paramID, String paramName,
		float inDefaultValue, String unitLabel = "");
	
	ZenBoolParameter* addBoolParameter(ZenBoolParameter* inBoolParam);

	ZenIntParameter* createAndAddIntParameter(String paramID, String paramName, int minValue, int maxValue, int inDefaultValue, String unitLabel = "");
	ZenIntParameter* addIntParameter(ZenIntParameter* inIntParam);
	/** Returns a parameter by its ID string. */
	AudioProcessorParameter* getParameter(StringRef parameterID) const noexcept;

	ZenBoolParameter* getBoolParameter(StringRef parameterID);

	ZenDecibelParameter* getDecibelParameter(StringRef parameterID);

	/** Returns a pointer to a floating point representation of a particular
	parameter which a realtime process can read to find out its current value.
	*/
	float getRawParameterValue(StringRef parameterID) const noexcept;

	/** A listener class that can be attached to an ZenAudioProcessorValueTreeState.
	Use ZenAudioProcessorValueTreeState::addParameterListener() to register a callback.
	*/
	struct JUCE_API  Listener
	{
		Listener();
		virtual ~Listener();

		/** This callback method is called by the ZenAudioProcessorValueTreeState when a parameter changes. */
		virtual void parameterChanged(const String& parameterID, float newValue) = 0;
	};

	/** Attaches a callback to one of the parameters, which will be called when the parameter changes. */
	void addParameterListener(StringRef parameterID, Listener* listener);

	/** Removes a callback that was previously added with addParameterCallback(). */
	void removeParameterListener(StringRef parameterID, Listener* listener);

	/** Returns a Value object that can be used to control a particular parameter. */
	Value getParameterAsValue(StringRef parameterID) const;

	/** Returns the range that was set when the given parameter was created. */
	NormalisableRange<float> getParameterRange(StringRef parameterID) const noexcept;

	/** A reference to the processor with which this state is associated. */
	AudioProcessor& processor;

	/** The state of the whole processor.
	You can replace this with your own ValueTree object, and can add properties and
	children to the tree. This class will automatically add children for each of the
	parameter objects that are created by createParameter().
	*/
	ValueTree state;

	/** Provides access to the undo manager that this object is using. */
	UndoManager* const undoManager;

	//==============================================================================
	/** An object of this class maintains a connection between a Slider and a parameter
	in an ZenAudioProcessorValueTreeState.

	During the lifetime of this SliderAttachment object, it keeps the two things in
	sync, making it easy to connect a slider to a parameter. When this object is
	deleted, the connection is broken. Make sure that your ZenAudioProcessorValueTreeState
	and Slider aren't deleted before this object!
	*/
	class JUCE_API  SliderAttachment
	{
	public:
		SliderAttachment(ZenAudioProcessorValueTreeState& stateToControl,
			const String& parameterID,
			Slider& sliderToControl);
		~SliderAttachment();

	private:
		struct Pimpl;
		friend struct ContainerDeletePolicy<Pimpl>;
		ScopedPointer<Pimpl> pimpl;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SliderAttachment)
	};

	//==============================================================================
	/** An object of this class maintains a connection between a ComboBox and a parameter
	in an ZenAudioProcessorValueTreeState.

	During the lifetime of this SliderAttachment object, it keeps the two things in
	sync, making it easy to connect a combo box to a parameter. When this object is
	deleted, the connection is broken. Make sure that your ZenAudioProcessorValueTreeState
	and ComboBox aren't deleted before this object!
	*/
	class JUCE_API  ComboBoxAttachment
	{
	public:
		ComboBoxAttachment(ZenAudioProcessorValueTreeState& stateToControl,
			const String& parameterID,
			ComboBox& comboBoxToControl);
		~ComboBoxAttachment();

	private:
		struct Pimpl;
		friend struct ContainerDeletePolicy<Pimpl>;
		ScopedPointer<Pimpl> pimpl;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ComboBoxAttachment)
	};

	//==============================================================================
	/** An object of this class maintains a connection between a Button and a parameter
	in an ZenAudioProcessorValueTreeState.

	During the lifetime of this SliderAttachment object, it keeps the two things in
	sync, making it easy to connect a button to a parameter. When this object is
	deleted, the connection is broken. Make sure that your ZenAudioProcessorValueTreeState
	and Button aren't deleted before this object!
	*/
	class JUCE_API  ButtonAttachment
	{
	public:
		ButtonAttachment(ZenAudioProcessorValueTreeState& stateToControl,
			const String& parameterID,
			Button& buttonToControl);
		~ButtonAttachment();

	private:
		struct Pimpl;
		friend struct ContainerDeletePolicy<Pimpl>;
		ScopedPointer<Pimpl> pimpl;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ButtonAttachment)
	};

public:
	Identifier valueType, valuePropertyID, idPropertyID;
	bool updatingConnections;

protected:
	//==============================================================================
	//struct Parameter;
	friend class ZenParameter;
	
	ValueTree getOrCreateChildValueTree(const String&);
	void timerCallback() override;

	void valueTreePropertyChanged(ValueTree&, const Identifier&) override;
	void valueTreeChildAdded(ValueTree&, ValueTree&) override;
	void valueTreeChildRemoved(ValueTree&, ValueTree&, int) override;
	void valueTreeChildOrderChanged(ValueTree&, int, int) override;
	void valueTreeParentChanged(ValueTree&) override;
	void valueTreeRedirected(ValueTree&) override;
	void updateParameterConnectionsToChildTrees();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZenAudioProcessorValueTreeState)
};

#endif // LAMBDAS

#endif  // ZENAUDIOPROCESSORVALUETREESTATE_H_INCLUDED
