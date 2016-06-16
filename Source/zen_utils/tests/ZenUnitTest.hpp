/*==============================================================================
//  ZenUnitTests.cpp
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2016/03/27
//  Copyright (C) 2016 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: 
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
===============================================================================*/

#ifndef ZEN_UNIT_TEST_H_INCLUDE
#define ZEN_UNIT_TEST_H_INCLUDE

#include "JuceHeader.h"
#include "zen_utils/utilities/ZenUtils.hpp"

/** Adds expectAlmostEqual to base UnitTest functionality
	To write a test using this class, your code should look something like this:

	@code
	class MyTest  : public UnitTest
	{
	public:
	MyTest()  : UnitTest ("Foobar testing") {}

	void runTest() override
	{
	beginTest ("Part 1");

	expect (myFoobar.doesSomething());
	expect (myFoobar.doesSomethingElse());

	beginTest ("Part 2");

	expect (myOtherFoobar.doesSomething());
	expect (myOtherFoobar.doesSomethingElse());

	...etc..
	}
	};

	// Creating a static instance will automatically add the instance to the array
	// returned by UnitTest::getAllTests(), so the test will be included when you call
	// UnitTestRunner::runAllTests()
	static MyTest test;
	@endcode

	To run a test, use the UnitTestRunner class.

	@see UnitTestRunner
*/
class ZenUnitTest : public UnitTest
{
public:
	explicit ZenUnitTest(const String& name) : UnitTest(name) {};

	/** Compares two values, and if they aren't almost equal (Avoid precision errors), 
	prints out a message containing the	expected and actual result values.*/
	template <class ValueType>
	void expectAlmostEqual(ValueType actual, ValueType expected, const double& precision=0.0001, String failureMessage = String())
	{
		const bool result = areAlmostEqual(actual, expected, precision);

		if (!result)
		{
			if (failureMessage.isNotEmpty())
				failureMessage << " -- ";

			failureMessage << "Expected value: " << expected << ", Actual value: " << actual;
		}

		expect(result, failureMessage);
	}

	/** Compares boolean value to bool	*/
	void expectBooleanEqual(bool actual, bool expected, String failureMessage = String())
	{
		const bool result = (actual == expected);

		if (!result)
		{
			if (failureMessage.isNotEmpty())
				failureMessage << " -- ";

			failureMessage << "Expected value: ";
			failureMessage << convertBoolToString(expected);
			failureMessage << convertBoolToString(actual);
		}

		expect(result, failureMessage);
	}

};
#endif //header guard