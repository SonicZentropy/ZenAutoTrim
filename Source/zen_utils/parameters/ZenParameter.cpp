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

#include "ZenParameter.h"

ZenParameter::~ZenParameter()
{
	jassert(listeners.size() <= 1);
}

void ZenParameter::setValueTree()
{
	paramValueTree->removeAllChildren(nullptr);
	paramValueTree->removeAllProperties(nullptr);

	paramValueTree->setProperty("parameterValue", getValue(), nullptr);
	paramValueTree->setProperty("defaultValue", getDefaultValue(), nullptr);
}

bool ZenParameter::checkUIUpdateAndReset()
{
	//bool expected = true;
	//UIUpdate.compare_exchange_strong(expected, false);
	//return expected;
	bool needs = UIUpdate.load();
	UIUpdate.store(false);
	return needs;
}

void ZenParameter::setNewState(const ValueTree& v)
{
	state = v;
	updateFromValueTree();
}

void ZenParameter::updateFromValueTree()
{
	//setValue(state.getProperty(owner.valuePropertyID, defaultValue));
	setUnnormalisedValue(state.getProperty(owner.valuePropertyID, defaultValue));
}

void ZenParameter::copyValueToValueTree()
{
	if (state.isValid())
		state.setProperty(owner.valuePropertyID, value.load(), owner.undoManager);
}

void ZenParameter::valueTreePropertyChanged(ValueTree& vt, const Identifier& prop)
{
	if (prop == owner.valuePropertyID)
		updateFromValueTree();
}

void ZenParameter::setUnnormalisedValue(float newUnnormalisedValue)
{
	if (value.load() != newUnnormalisedValue)
	{
		const float newValue = range.convertTo0to1(newUnnormalisedValue);
		setValueNotifyingHost(newValue);
	}
}
