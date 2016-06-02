/* ==============================================================================
//  ZenTime.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/08/18
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Represents a time (in milliseconds) and assorted conversions
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#include "ZenTime.h"

ZenTime::ZenTime(const String& inParamID, const TimeValue& inTimeValueInMS)
	:paramID(inParamID)
{
	setValueFromMS(inTimeValueInMS);
}

ZenTime::~ZenTime()
{
	//paramValueTree = nullptr;
}

void ZenTime::valueChanged(Value& inValue)
{
	//setValueTree();
}

String ZenTime::getTextFromValue() const
{	
	return String(value.load());
}

void ZenTime::writeToXML(XmlElement& inXML)
{
	// DBG("In ZenParameter::writeToXML(inXML) ");
	XmlElement* thisXML = inXML.createNewChildElement(paramID);
	thisXML->setAttribute("valueInMS", (int)getValueInMS());
}

void ZenTime::setFromXML(const XmlElement& inXML)
{
	//DBG("In ZenParameter::setFromXML(inXML) ");
	XmlElement* thisXML = inXML.getChildByName(paramID);
	setValueFromMS(thisXML->getIntAttribute("valueInMS", -987654321));
}

//==============================================================================
#ifdef ZEN_UNIT_TESTS

class ZenTimeTests : public ZenUnitTest
{
public:
	ZenTimeTests() : ZenUnitTest("ZenTime class") {}

	void runTest() override
	{
		beginTest("Get&Set Values");
		ZenTime floatParam("floatTest", "Float Test", -96.0f, 18.0f, -18.0f);

		expectAlmostEqual(floatParam.getDefaultValue(), 0.684210526315f);
		expectAlmostEqual(floatParam.getValue(), 0.684210526315f);
		expectAlmostEqual(floatParam.getValueRaw(), -18.0f);
		floatParam.setValue(0.3f);
		expectAlmostEqual(floatParam.getValueRaw(), -61.8f);
		expectAlmostEqual(floatParam.getValue(), 0.3f);
		floatParam.setValueRaw(-7);
		expectAlmostEqual(floatParam.getValueRaw(), -7.0f);
		expectAlmostEqual(floatParam.getValue(), 0.780701754385f);
		floatParam.setValue(0.0f);
		expectAlmostEqual(floatParam.getValueRaw(), -96.0f);
		expectAlmostEqual(floatParam.getValue(), 0.0f);
		floatParam.setValue(1.0f);
		expectAlmostEqual(floatParam.getValueRaw(), 18.0f);
		expectAlmostEqual(floatParam.getValue(), 1.0f);
		floatParam.setValue(1.5f);
		expectAlmostEqual(floatParam.getValueRaw(), 18.0f);
		expectAlmostEqual(floatParam.getValue(), 1.0f);
		floatParam.setValue(-1.0f);
		expectAlmostEqual(floatParam.getValueRaw(), -96.0f);
		expectAlmostEqual(floatParam.getValue(), 0.0f);
	}
};

static ZenTimeTests zenTimeTests;

#endif // ZEN_UNIT_TESTS
