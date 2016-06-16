/*==============================================================================
//  ZenDenormals.hpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 12 Jun 2016 3:29:18pm
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Scoped class to disable SSE denormal processing
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef ZENDENORMALS_HPP_INCLUDED
#define ZENDENORMALS_HPP_INCLUDED

#include "JuceHeader.h"

class ZenScopedNoDenormal
{
public:
	ZenScopedNoDenormal() : csr(_mm_getcsr())
	{
		// sets FTZ & DNZ
		_mm_setcsr(csr | 0x8040);
	}

	~ZenScopedNoDenormal() 
	{ 
		// resets control register 
		_mm_setcsr(csr); 
	}
private:
	const unsigned int csr;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZenScopedNoDenormal);
};



#endif  // ZENDENORMALS_HPP_INCLUDED
