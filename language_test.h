//[of]:Copyright statement
//[c](C) Copyright 2005, 2012 Kristian Dupont Knudsen. MIT license -- see LICENSE.md for details
//[c]This file is most easily read with Code Browser: http://tibleiz.net/code-browser/
//[cf]
#pragma once

#include <cppunit/extensions/HelperMacros.h>

class language_test : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(language_test);
	CPPUNIT_TEST(TestReturn);
	CPPUNIT_TEST(TestInteger);
	CPPUNIT_TEST(TestIntegerComplex);
	CPPUNIT_TEST(TestIntegerAdd);
	CPPUNIT_TEST(TestIntegerSubtract);
	CPPUNIT_TEST(TestIntegerAssign);
	CPPUNIT_TEST(TestBool);
	CPPUNIT_TEST(TestBoolComplex);
	CPPUNIT_TEST(TestBoolAssign);
	CPPUNIT_TEST(TestBoolEqualityTest);
	CPPUNIT_TEST(TestDynamicIf);
	//CPPUNIT_TEST(TestParams);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() {};
	void tearDown() {};
	
	void TestReturn();
	void TestInteger();	
	void TestIntegerComplex();
	void TestIntegerAdd();
	void TestIntegerSubtract();
	void TestIntegerAssign();
	void TestBool();	
	void TestBoolComplex();
	void TestBoolAssign();
	void TestBoolEqualityTest();
	void TestDynamicIf();
	void TestParams();
};

