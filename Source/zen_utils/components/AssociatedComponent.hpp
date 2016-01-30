/*==============================================================================
//  AssociatedComponent.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/10
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Base component associated with parameter
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef ZEN_ASSOCIATED_COMPONENT_H_INCLUDED
#define ZEN_ASSOCIATED_COMPONENT_H_INCLUDED
#include "JuceHeader.h"
#include "../parameters/ZenParameter.hpp"


 
namespace Zen
{

enum ComponentType
{
	ASSOCCOMPONENT,
	ASSOCBUTTON,
	ASSOCTOGGLEBUTTON,
	ASSOCTEXTBUTTON,
	ASSOCIMAGEBUTTON,
	ASSOCCOMBOBOX,
	ASSOCGROUPBOX,
	ASSOCSLIDER,
	ASSOCTEXTEDITOR,
	ASSOCTABBEDCOMP
};

class AssociatedComponent 
{

public:

	explicit AssociatedComponent(ZenParameter* inAssocParam, const String& inLabel = "")
		: associatedParam(inAssocParam)
		, unitLabel(inLabel)
	{	}

	virtual ~AssociatedComponent()
	{	}

	ZenParameter* getAssociatedParameter() const { return associatedParam; }

	void setAssociatedParameter(ZenParameter* inValue) { associatedParam = inValue; }
	
	virtual void setAssociatedParameterValue() = 0;

	virtual void setAssociatedParameterValueNotifyingHost() = 0;

	virtual float getAssociatedParameterValue() = 0;

	unsigned int getDisplayPrecision() const { return displayPrecision; }

	void setDisplayPrecision(unsigned int inValue) { displayPrecision = inValue; }

	String getUnitLabel() const { return unitLabel; }
	
	void setUnitLabel(String inValue) { unitLabel = inValue; }
	
	virtual operator ComponentType() const { return ASSOCCOMPONENT; } //uggg C++14 where are you 

protected:
	ZenParameter* associatedParam;
	String unitLabel;
	unsigned int displayPrecision = 2;
	
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AssociatedComponent);

};
} //Namespace Zen
#endif // ZEN_ASSOCIATED_COMPONENT_H_INCLUDED
