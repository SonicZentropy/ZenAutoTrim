/*==============================================================================
//  ZenParameter.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/08
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Base Audio Parameter class (Virtual only)
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/
#ifndef ZEN_PARAMETER_H_INCLUDED
#define ZEN_PARAMETER_H_INCLUDED

#include "JuceHeader.h"
#include "state/ZenAudioProcessorValueTreeState.h"
#include "parameters/ZenLinearSmoothedValue.h"
#include <atomic>


using AtomicFloat = std::atomic<float>;
using AtomicBool = std::atomic<bool>;

enum ZenParamType
{
	ZENFLOAT,
	ZENDECIBEL,
	ZENBOOL,
	NONE
};


class ZenParameter
	: public AudioProcessorParameterWithID
	, public ReferenceCountedObject
	//, public ValueListener //Might not need this anymore?
	, public ZenLinearSmoothedValue
	, public ValueTree::Listener
{
public:

	virtual ~ZenParameter() = 0;

	/** Set this parameter's value from 0to1 normalized input value.
	* Should be called only by HOST/Process Block, never GUI */
	virtual void setValue(float newValue) override = 0;

	/** Directly sets value from raw input, called by GUI */
	virtual void setValueRaw(float newValue) = 0;

	/** This assumes inValue is NOT normalized and coming from NOT the host, since
	* it promptly notifies the host of the change. */
	void setValueNotifyingHost(float inValue) override = 0;

	/**Sets this parameter's value from normalized value. This should never
	*  ever be called, as the GUI does not deal in normalized values */
	//virtual void setValueNormalizedNotifyingHost(float inValue) = 0;

	/**This is called by the HOST/Process block and needs to return
	* a normalized value */
	float getValue() const override = 0;

	/** Returns raw value direct, for GUI! */
	virtual float getValueRaw() const = 0;

	virtual void valueChanged(Value& inValue)
	{
		DBG("In ZenParameter's value changed listener callback: " << inValue.getValue().toString());
	}

	virtual float getRawValueForText(const String& text)
	{
		return text.getFloatValue();
	}

	/** Returns input value as text string */
	String getText(float inValue, int length/*maximumStringLength*/) const override = 0;

	/** Parses an input String and converts it to a normalized numeric value based on the param's range */
	float getValueForText(const String& text) const override = 0;

	/** Returns this parameter's current value as text string */
	virtual String getTextFromValue() const = 0;

	virtual void writeToXML(XmlElement& inXML) {};

	virtual void setFromXML(const XmlElement& inXML) {};

	virtual void setDefaultValue(float inValue) = 0;

	/** Returns default value as normalized 0 to 1 */
	float getDefaultValue() const override = 0;

	virtual ValueTree getValueTree()
	{
		return *paramValueTree;
	}

	virtual void setValueTree();

	virtual void setDisplayPrecision(unsigned int inPrecision)
	{
		precision = inPrecision;
	}

	virtual unsigned int getDisplayPrecision() const
	{
		return precision;
	}

	void setNeedsUIUpdate(bool needsUpdate) noexcept
	{
		UIUpdate.store(needsUpdate);
	}

	bool getNeedsUIUpdate() const noexcept
	{
		return UIUpdate.load();
	}

	bool needsUIUpdate() const noexcept
	{
		return UIUpdate.load();
	}

	virtual bool needsUIUpdate()
	{
		return getNeedsUIUpdate();
	}

	virtual void resetUIUpdate()
	{
		setNeedsUIUpdate(false);
	}

	virtual bool checkUIUpdateAndReset();

	String getLabel() const override
	{
		return unitLabel;
	}

	virtual String getUnitLabel() const
	{
		return unitLabel;
	}

	virtual void setUnitLabel(String inLabel)
	{
		unitLabel = inLabel;
	}

	String getName() const
	{
		return name;
	}

	const ZenParamType zenType;

	/* FROM ValueTreeState */

	virtual void setNewState(const ValueTree& v);
	
	virtual void updateFromValueTree();
	
	virtual void copyValueToValueTree();

	//ValueTree::Listener overrides
	void valueTreePropertyChanged(ValueTree&, const Identifier& property) override;
	void valueTreeChildAdded(ValueTree&, ValueTree&) override {}
	void valueTreeChildRemoved(ValueTree&, ValueTree&, int) override {}
	void valueTreeChildOrderChanged(ValueTree&, int, int) override {}
	void valueTreeParentChanged(ValueTree&) override {}

	static ZenParameter* getParameterForID(AudioProcessor& processor, StringRef paramID) noexcept
	{
		const int numParams = processor.getParameters().size();

		for (int i = 0; i < numParams; ++i)
		{
			AudioProcessorParameter* const ap = processor.getParameters().getUnchecked(i);

			// When using this class, you must allow it to manage all the parameters in your 
			// AudioProcessor, and not add any parameter objects of other types!
			jassert(dynamic_cast<ZenParameter*> (ap) != nullptr);

			ZenParameter* const p = static_cast<ZenParameter*> (ap);

			if (paramID == p->paramID)
				return p;
		}

		return nullptr;
	}	

	

protected:
	AtomicFloat value;
	AtomicBool UIUpdate;
	unsigned int precision = 2;
	String unitLabel = "", description = "";
	ScopedPointer<ValueTree> paramValueTree;

	//From ProcessorValueStateTree::Parameter
	friend class ZenAudioProcessorValueTreeState;
	ZenAudioProcessorValueTreeState& owner;
	ValueTree state;
	ListenerList<ZenAudioProcessorValueTreeState::Listener> listeners;
	std::function<String(float)> valueToTextFunction;
	std::function<float(const String&)> textToValueFunction;
	float defaultValue;
	Atomic<int> needsUpdate;
	bool listenersNeedCalling;
	NormalisableRange<float> range;

	//protected constructor because noone should instantiate a ZenParameter directly
	ZenParameter(ZenAudioProcessorValueTreeState& inOwner, ZenParamType paramType, 
		const String& paramID, const String& paramName, double inValue, String inUnitLabel = "",
		bool inShouldBeSmoothed = false, float inSmoothingTime = 0.0f)
		: AudioProcessorParameterWithID(paramID, paramName),
		ZenLinearSmoothedValue(inShouldBeSmoothed, inSmoothingTime),
		zenType(paramType), value(inValue), unitLabel(inUnitLabel),
		owner(inOwner), defaultValue(inValue), needsUpdate(true), listenersNeedCalling(false)
	{
		state.addListener(this);
		needsUpdate.set(1);
	}

private:

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZenParameter);
	void setUnnormalisedValue(float newUnnormalisedValue);
};

#endif // ZEN_PARAMETER_H_INCLUDED
