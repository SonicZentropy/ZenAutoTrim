/*==============================================================================
//  ZenZenAudioProcessorValueTreeState.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 27 May 2016 8:37:34pm
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Generic CPP File
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#include "ZenAudioProcessorValueTreeState.h"

#if JUCE_COMPILER_SUPPORTS_LAMBDAS

//PARAMETER===========================================================
/*
struct ZenAudioProcessorValueTreeState::Parameter : public AudioProcessorParameter,
	private ValueTree::Listener
{
	Parameter(ZenAudioProcessorValueTreeState& s,
		String parameterID, String paramName, String labelText,
		NormalisableRange<float> r, float defaultVal,
		std::function<String(float)> valueToText,
		std::function<float(const String&)> textToValue)
		: owner(s), paramID(parameterID), name(paramName), label(labelText),
		valueToTextFunction(valueToText),
		textToValueFunction(textToValue),
		range(r), value(defaultVal), defaultValue(defaultVal),
		listenersNeedCalling(true)
	{
		state.addListener(this);
		needsUpdate.set(1);
	}

	~Parameter()
	{
		// should have detached all callbacks before destroying the parameters!
		jassert(listeners.size() <= 1);
	}

	float getValue() const override { return range.convertTo0to1(value); }
	float getDefaultValue() const override { return range.convertTo0to1(defaultValue); }
	String getName(int maximumStringLength) const override { return name.substring(0, maximumStringLength); }
	String getLabel() const override { return label; }

	float getValueForText(const String& text) const override
	{
		return range.convertTo0to1(textToValueFunction != nullptr ? textToValueFunction(text)
			: text.getFloatValue());
	}

	String getText(float v, int length) const override
	{
		return valueToTextFunction != nullptr ? valueToTextFunction(range.convertFrom0to1(v))
			: AudioProcessorParameter::getText(v, length);
	}

	void setValue(float newValue) override
	{
		newValue = range.snapToLegalValue(range.convertFrom0to1(newValue));

		if (value != newValue || listenersNeedCalling)
		{
			value = newValue;

			listeners.call(&ZenAudioProcessorValueTreeState::Listener::parameterChanged, paramID, value);
			listenersNeedCalling = false;

			needsUpdate.set(1);
		}
	}

	void setNewState(const ValueTree& v)
	{
		state = v;
		updateFromValueTree();
	}

	void setUnnormalisedValue(float newUnnormalisedValue)
	{
		if (value != newUnnormalisedValue)
		{
			const float newValue = range.convertTo0to1(newUnnormalisedValue);
			setValueNotifyingHost(newValue);
		}
	}

	void updateFromValueTree()
	{
		setUnnormalisedValue(state.getProperty(owner.valuePropertyID, defaultValue));
	}

	void copyValueToValueTree()
	{
		if (state.isValid())
			state.setProperty(owner.valuePropertyID, value, owner.undoManager);
	}

	void valueTreePropertyChanged(ValueTree&, const Identifier& property) override
	{
		if (property == owner.valuePropertyID)
			updateFromValueTree();
	}

	void valueTreeChildAdded(ValueTree&, ValueTree&) override {}
	void valueTreeChildRemoved(ValueTree&, ValueTree&, int) override {}
	void valueTreeChildOrderChanged(ValueTree&, int, int) override {}
	void valueTreeParentChanged(ValueTree&) override {}

	static Parameter* getParameterForID(AudioProcessor& processor, StringRef paramID) noexcept
	{
		const int numParams = processor.getParameters().size();

		for (int i = 0; i < numParams; ++i)
		{
			AudioProcessorParameter* const ap = processor.getParameters().getUnchecked(i);

			// When using this class, you must allow it to manage all the parameters in your AudioProcessor, and
			// not add any parameter objects of other types!
			jassert(dynamic_cast<Parameter*> (ap) != nullptr);

			Parameter* const p = static_cast<Parameter*> (ap);

			if (paramID == p->paramID)
				return p;
		}

		return nullptr;
	}

	ZenAudioProcessorValueTreeState& owner;
	ValueTree state;
	String paramID, name, label;
	ListenerList<ZenAudioProcessorValueTreeState::Listener> listeners;
	std::function<String(float)> valueToTextFunction;
	std::function<float(const String&)> textToValueFunction;
	NormalisableRange<float> range;
	float value, defaultValue;
	Atomic<int> needsUpdate;
	bool listenersNeedCalling;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Parameter)
};
*/

//==============================================================================
ZenAudioProcessorValueTreeState::ZenAudioProcessorValueTreeState(AudioProcessor& p, UndoManager* um)
	: processor(p),
	undoManager(um),
	valueType("PARAM"),
	valuePropertyID("value"),
	idPropertyID("id"),
	updatingConnections(false)
{
	startTimerHz(10);
	state.addListener(this);
}

ZenAudioProcessorValueTreeState::~ZenAudioProcessorValueTreeState() {}

AudioProcessorParameter* ZenAudioProcessorValueTreeState::createAndAddParameter(String paramID, 
	String paramName, String labelText,
	NormalisableRange<float> r, float defaultVal,
	std::function<String(float)> valueToTextFunction,
	std::function<float(const String&)> textToValueFunction)
{
	// All parameters must be created before giving this manager a ValueTree state!
	jassert(!state.isValid());
	jassert(MessageManager::getInstance()->isThisTheMessageThread());

	//addParameter(gainParam = new ZenDecibelParameter("gainParam", "Gain", -96.0f, 18.0f, 0.0f, 0.0f, 0.0f, true, 50.0f));
	//addParameter(targetParam = new ZenDecibelParameter("targetGain", "TargetGain", -96.0f, 18.0f, 0.0f, 0.0f, -18.0f, false));
	//addParameter(autoGainEnableParam = new ZenBoolParameter("autoGainParam", "AutoGain", false, ""));
	//addParameter(bypassParam = new ZenBoolParameter("bypassParam", "Bypass", false, ""));

	Parameter* p = new Parameter(*this, paramID, paramName, labelText, r,
		defaultVal, valueToTextFunction, textToValueFunction);
	processor.addParameter(p);
	return p;
}

AudioProcessorParameter* ZenAudioProcessorValueTreeState::createAndAddDecibelParameter(
	String paramID,	String paramName, float minValue, float maxValue, float defaultValue, 
	bool shouldBeSmoothed, float smoothingTime,
	std::function<String(float)> valueToTextFunction,
	std::function<float(const String&)> textToValueFunction)
{
	ZenDecibelParameter* p = new ZenDecibelParameter(paramID, paramName, minValue, maxValue, defaultValue, shouldBeSmoothed, smoothingTime);
}

void ZenAudioProcessorValueTreeState::addParameterListener(StringRef paramID, Listener* listener)
{
	if (Parameter* p = Parameter::getParameterForID(processor, paramID))
		p->listeners.add(listener);
}

void ZenAudioProcessorValueTreeState::removeParameterListener(StringRef paramID, Listener* listener)
{
	if (Parameter* p = Parameter::getParameterForID(processor, paramID))
		p->listeners.remove(listener);
}

Value ZenAudioProcessorValueTreeState::getParameterAsValue(StringRef paramID) const
{
	if (Parameter* p = Parameter::getParameterForID(processor, paramID))
		return p->state.getPropertyAsValue(valuePropertyID, undoManager);

	return Value();
}

NormalisableRange<float> ZenAudioProcessorValueTreeState::getParameterRange(StringRef paramID) const noexcept
{
	if (Parameter* p = Parameter::getParameterForID(processor, paramID))
		return p->range;

	return NormalisableRange<float>();
}

AudioProcessorParameter* ZenAudioProcessorValueTreeState::getParameter(StringRef paramID) const noexcept
{
	return Parameter::getParameterForID(processor, paramID);
}

float* ZenAudioProcessorValueTreeState::getRawParameterValue(StringRef paramID) const noexcept
{
	if (Parameter* p = Parameter::getParameterForID(processor, paramID))
		return &(p->value);

	return nullptr;
}

ValueTree ZenAudioProcessorValueTreeState::getOrCreateChildValueTree(const String& paramID)
{
	ValueTree v(state.getChildWithProperty(idPropertyID, paramID));

	if (!v.isValid())
	{
		v = ValueTree(valueType);
		v.setProperty(idPropertyID, paramID, undoManager);
		state.addChild(v, -1, undoManager);
	}

	return v;
}

void ZenAudioProcessorValueTreeState::updateParameterConnectionsToChildTrees()
{
	if (!updatingConnections)
	{
		ScopedValueSetter<bool> svs(updatingConnections, true, false);

		const int numParams = processor.getParameters().size();

		for (int i = 0; i < numParams; ++i)
		{
			AudioProcessorParameter* const ap = processor.getParameters().getUnchecked(i);
			jassert(dynamic_cast<Parameter*> (ap) != nullptr);

			Parameter* p = static_cast<Parameter*> (ap);
			p->setNewState(getOrCreateChildValueTree(p->paramID));
		}
	}
}

void ZenAudioProcessorValueTreeState::valueTreePropertyChanged(ValueTree& tree, const Identifier& property)
{
	if (property == idPropertyID && tree.hasType(valueType) && tree.getParent() == state)
		updateParameterConnectionsToChildTrees();
}

void ZenAudioProcessorValueTreeState::valueTreeChildAdded(ValueTree& parent, ValueTree& tree)
{
	if (parent == state && tree.hasType(valueType))
		updateParameterConnectionsToChildTrees();
}

void ZenAudioProcessorValueTreeState::valueTreeChildRemoved(ValueTree& parent, ValueTree& tree, int)
{
	if (parent == state && tree.hasType(valueType))
		updateParameterConnectionsToChildTrees();
}

void ZenAudioProcessorValueTreeState::valueTreeRedirected(ValueTree& v)
{
	if (v == state)
		updateParameterConnectionsToChildTrees();
}

void ZenAudioProcessorValueTreeState::valueTreeChildOrderChanged(ValueTree&, int, int) {}
void ZenAudioProcessorValueTreeState::valueTreeParentChanged(ValueTree&) {}

void ZenAudioProcessorValueTreeState::timerCallback()
{
	const int numParams = processor.getParameters().size();
	bool anythingUpdated = false;

	for (int i = 0; i < numParams; ++i)
	{
		AudioProcessorParameter* const ap = processor.getParameters().getUnchecked(i);
		jassert(dynamic_cast<Parameter*> (ap) != nullptr);

		Parameter* p = static_cast<Parameter*> (ap);

		if (p->needsUpdate.compareAndSetBool(0, 1))
		{
			p->copyValueToValueTree();
			anythingUpdated = true;
		}
	}

	startTimer(anythingUpdated ? 1000 / 50
		: jlimit(50, 500, getTimerInterval() + 20));
}

ZenAudioProcessorValueTreeState::Listener::Listener() {}
ZenAudioProcessorValueTreeState::Listener::~Listener() {}

//==============================================================================
struct AttachedControlBase : public ZenAudioProcessorValueTreeState::Listener,
	public AsyncUpdater
{
	AttachedControlBase(ZenAudioProcessorValueTreeState& s, const String& p)
		: state(s), paramID(p), lastValue(0)
	{
		state.addParameterListener(paramID, this);
	}

	void removeListener()
	{
		state.removeParameterListener(paramID, this);
	}

	void setNewUnnormalisedValue(float newUnnormalisedValue)
	{
		if (AudioProcessorParameter* p = state.getParameter(paramID))
		{
			const float newValue = state.getParameterRange(paramID)
				.convertTo0to1(newUnnormalisedValue);

			if (p->getValue() != newValue)
				p->setValueNotifyingHost(newValue);
		}
	}

	void sendInitialUpdate()
	{
		if (float* v = state.getRawParameterValue(paramID))
			parameterChanged(paramID, *v);
	}

	void parameterChanged(const String&, float newValue) override
	{
		lastValue = newValue;

		if (MessageManager::getInstance()->isThisTheMessageThread())
		{
			cancelPendingUpdate();
			setValue(newValue);
		}
		else
		{
			triggerAsyncUpdate();
		}
	}

	void handleAsyncUpdate() override
	{
		setValue(lastValue);
	}

	virtual void setValue(float) = 0;

	ZenAudioProcessorValueTreeState& state;
	String paramID;
	float lastValue;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AttachedControlBase)
};

//==============================================================================
struct ZenAudioProcessorValueTreeState::SliderAttachment::Pimpl : private AttachedControlBase,
	private Slider::Listener
{
	Pimpl(ZenAudioProcessorValueTreeState& s, const String& p, Slider& sl)
		: AttachedControlBase(s, p), slider(sl)
	{
		NormalisableRange<float> range(s.getParameterRange(paramID));
		slider.setRange(range.start, range.end, range.interval);

		if (AudioProcessorParameter* param = state.getParameter(paramID))
			slider.setDoubleClickReturnValue(true, range.convertFrom0to1(param->getDefaultValue()));

		sendInitialUpdate();
		slider.addListener(this);
	}

	~Pimpl()
	{
		slider.removeListener(this);
		removeListener();
	}

	void setValue(float newValue) override
	{
		slider.setValue(newValue, sendNotificationSync);
	}

	void sliderValueChanged(Slider* s) override
	{
		if (!ModifierKeys::getCurrentModifiers().isRightButtonDown())
			setNewUnnormalisedValue((float)s->getValue());
	}

	void sliderDragStarted(Slider*) override
	{
		if (AudioProcessorParameter* p = state.getParameter(paramID))
			p->beginChangeGesture();
	}

	void sliderDragEnded(Slider*) override
	{
		if (AudioProcessorParameter* p = state.getParameter(paramID))
			p->endChangeGesture();
	}

	Slider& slider;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Pimpl)
};

ZenAudioProcessorValueTreeState::SliderAttachment::SliderAttachment(ZenAudioProcessorValueTreeState& s, const String& p, Slider& sl)
	: pimpl(new Pimpl(s, p, sl))
{
}

ZenAudioProcessorValueTreeState::SliderAttachment::~SliderAttachment() {}

//==============================================================================
struct ZenAudioProcessorValueTreeState::ComboBoxAttachment::Pimpl : private AttachedControlBase,
	private ComboBox::Listener
{
	Pimpl(ZenAudioProcessorValueTreeState& s, const String& p, ComboBox& c)
		: AttachedControlBase(s, p), combo(c)
	{
		sendInitialUpdate();
		combo.addListener(this);
	}

	~Pimpl()
	{
		combo.removeListener(this);
		removeListener();
	}

	void setValue(float newValue) override
	{
		combo.setSelectedItemIndex(roundToInt(newValue), sendNotificationSync);
	}

	void comboBoxChanged(ComboBox* comboBox) override
	{
		setNewUnnormalisedValue((float)comboBox->getSelectedId() - 1.0f);
	}

	ComboBox& combo;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Pimpl)
};

ZenAudioProcessorValueTreeState::ComboBoxAttachment::ComboBoxAttachment(ZenAudioProcessorValueTreeState& s, const String& p, ComboBox& c)
	: pimpl(new Pimpl(s, p, c))
{
}

ZenAudioProcessorValueTreeState::ComboBoxAttachment::~ComboBoxAttachment() {}

//==============================================================================
struct ZenAudioProcessorValueTreeState::ButtonAttachment::Pimpl : private AttachedControlBase,
	private Button::Listener
{
	Pimpl(ZenAudioProcessorValueTreeState& s, const String& p, Button& b)
		: AttachedControlBase(s, p), button(b)
	{
		sendInitialUpdate();
		button.addListener(this);
	}

	~Pimpl()
	{
		button.removeListener(this);
		removeListener();
	}

	void setValue(float newValue) override
	{
		button.setToggleState(newValue >= 0.5f, sendNotificationSync);
	}

	void buttonClicked(Button* b) override
	{
		setNewUnnormalisedValue(b->getToggleState() ? 1.0f : 0.0f);
	}

	Button& button;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Pimpl)
};

ZenAudioProcessorValueTreeState::ButtonAttachment::ButtonAttachment(ZenAudioProcessorValueTreeState& s, const String& p, Button& b)
	: pimpl(new Pimpl(s, p, b))
{
}

ZenAudioProcessorValueTreeState::ButtonAttachment::~ButtonAttachment() {}

#endif
