/*==============================================================================
//  ZenStringUtils.hpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2016/02/01
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: General String utility functions in anon namespace
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#pragma once
#ifndef ZENSTRINGUTILS_H_INCLUDED
#define ZENSTRINGUTILS_H_INCLUDED
#include <JuceHeader.h>
#include <sstream>

namespace
{

/// <summary>Convert a numeric format to a string with 2 decimal precision</summary>
template <class Type>
String convertTo2PrecisionString(Type inNumber)
{
	std::stringstream numberFormatter;
	numberFormatter.setf(std::ios_base::fixed, std::ios_base::floatfield);
	numberFormatter.precision(2);
	numberFormatter << inNumber;
	return numberFormatter.str();
}

/// <summary>Convert a numeric format to a string with 2 decimal precision</summary>
template <class Type>
String convertTo3PrecisionString(Type inNumber)
{
	std::stringstream numberFormatter;
	numberFormatter.setf(std::ios_base::fixed, std::ios_base::floatfield);
	numberFormatter.precision(3);
	numberFormatter << inNumber;
	return numberFormatter.str();
}

} // namespace
#endif // ZENSTRINGUTILS_H_INCLUDED
