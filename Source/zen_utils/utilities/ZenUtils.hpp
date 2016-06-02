/*==============================================================================
//  ZenUtils.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/09/18
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: General utility functions in anon namespace
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/


#ifndef ZENUTILS_HEADER_INCLUDE
#define ZENUTILS_HEADER_INCLUDE

namespace
{
/// <summary>Return value clamped between min/max</summary>
inline float getClamped(const float& valueToClamp, const float& lowerClampBound, const float& upperClampBound)
{
	return (valueToClamp > upperClampBound) ? upperClampBound : ((valueToClamp < lowerClampBound) ? lowerClampBound : valueToClamp);
}

inline bool areAlmostEqual(const double& x, const double& y, const double& precision = 0.0001)
{
	return (std::abs(x - y) < precision) ? true : false;
}

inline bool areNotAlmostEqual(const double& x, const double& y, const double& precision = 0.0001)
{
	return (std::abs(x - y) < precision) ? false : true;
}

inline bool almostEqual(const double& x, const double& y, const double& precision = 0.0001)
{
	return (std::abs(x - y) < precision) ? true : false;
}

/// <summary> Convert float to boolean. Float 0.0 => False, any other value True </summary>
inline bool convertFloatToBoolean(const float& inFloat)
{
	return (0.0 == inFloat) ? false : true;
}

/**Convert Boolean to float. True => 1.0, False => 0.0*/
inline float convertBooleanToFloat(const bool& inBool)
{
	return (false == inBool) ? 0.0 : 1.0;
}

/** Convert normalized float to the boolean representation (true => 1.0f, false => 0.0f) */
inline float convertFloatToBoolFloat(const float& inFloat)
{
	return (0.0f == inFloat) ? 0.0f : 1.0f;
}

/// <summary>Convert a 0-1.0 normalized value into a boolean where 0.5+ is true </summary>
inline bool convertFloatToBooleanViaRange(float inputValue)
{
	if (inputValue < 0.5)
		return false;
	else
		return true;
}


inline String convertBoolToString(bool inBool)
{
	String boolString = (inBool == true) ? "True" : "False";
	return boolString;
}

inline float renormalizeToNewScale(float numToReNorm, float oldRangeMin, float oldRangeMax,
	float newRangeMin, float newRangeMax)
{
	return (((numToReNorm - oldRangeMin)*(newRangeMax - newRangeMin)) / (oldRangeMax - oldRangeMin)) + newRangeMin;
}


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
#endif