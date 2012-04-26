//[of]:Copyright statement
//[c](C) Copyright 2005, 2012 Kristian Dupont Knudsen. MIT license -- see LICENSE.md for details
//[c]This file is most easily read with Code Browser: http://tibleiz.net/code-browser/
//[cf]
#pragma once

#include <cppunit/extensions/HelperMacros.h>

class instructions_test : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(instructions_test);
	CPPUNIT_TEST(TestAdd);
	CPPUNIT_TEST(TestAdc);
	CPPUNIT_TEST(TestSub);
	CPPUNIT_TEST(TestSbb);
	CPPUNIT_TEST(TestCmp);
	CPPUNIT_TEST(TestOr);
	CPPUNIT_TEST(TestAnd);
	CPPUNIT_TEST(TestXor);
	CPPUNIT_TEST(TestNot);
	CPPUNIT_TEST(TestNeg);
	CPPUNIT_TEST(TestInc);
	CPPUNIT_TEST(TestDec);
	CPPUNIT_TEST(TestMov);
	CPPUNIT_TEST(TestMovIndex);
	CPPUNIT_TEST(TestRet);
	CPPUNIT_TEST(TestSet);
	CPPUNIT_TEST(TestPush);
	CPPUNIT_TEST(TestPop);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() {};
	void tearDown() {};
	
	void TestAdd();
	void TestAdc();
	void TestSub();
	void TestSbb();
	void TestCmp();
	void TestOr();
	void TestAnd();
	void TestXor();
	
	void TestNot();
	void TestNeg();
	void TestInc();
	void TestDec();
	
	void TestMov();
	void TestMovIndex();
		
	void TestRet();

	void TestSet();

	void TestPush();
	void TestPop();
};

class assembler_test : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(assembler_test);
	CPPUNIT_TEST(TestInvoke);
	CPPUNIT_TEST(TestInvokeParam);
	CPPUNIT_TEST(TestJmp);
	CPPUNIT_TEST(TestCall);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() {};
	void tearDown() {};
	
	void TestInvoke();
	void TestInvokeParam();
	void TestJmp();
	void TestCall();
};

