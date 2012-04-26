//[of]:Copyright statement
//[c](C) Copyright 2005, 2012 Kristian Dupont Knudsen. MIT license -- see LICENSE.md for details
//[c]This file is most easily read with Code Browser: http://tibleiz.net/code-browser/
//[cf]
//[of]:header
#include "stdafx.h"
#include "language_test.h"

#include "language.h"

CPPUNIT_TEST_SUITE_REGISTRATION(language_test);

using namespace rtasm;
//[cf]
//[c]
//[l]:Declaration:language_test.h
//[c]
//[of]:void language_test\:\:TestReturn()
void language_test::TestReturn()
{
	struct fctint : public functor_base<int()>
	{
		fctint() { return_d(10); }
	} fctint_instance;
	
	functor<int()> fi(fctint_instance);

	CPPUNIT_ASSERT_EQUAL(10, fi());
	CPPUNIT_ASSERT_EQUAL(10, fi());

	struct fctbool : public functor_base<bool()>
	{
		fctbool() { return_d(true); }
	} fctbool_instance;

	functor<bool()> fb(fctbool_instance);

	CPPUNIT_ASSERT_EQUAL(true, fb());
	CPPUNIT_ASSERT_EQUAL(true, fb());
}
//[cf]
//[of]:void language_test\:\:TestInteger()
void language_test::TestInteger()
{
	struct fct : public functor_base<int()>
	{
		fct() { int_d x(10); return_d(x); }
	} fct_instance;

	functor<int()> f(fct_instance);

	CPPUNIT_ASSERT_EQUAL(10, f());
	CPPUNIT_ASSERT_EQUAL(10, f());
}
//[cf]
//[of]:void language_test\:\:TestIntegerComplex()
void language_test::TestIntegerComplex()
{
	struct fct : public functor_base<int()>
	{
		fct() 
		{
			int_d x(10);
			int_d y(20);

			return_d(x);
		}
	} fct_instance;

	functor<int()> f(fct_instance);

	CPPUNIT_ASSERT_EQUAL(10, f());
	CPPUNIT_ASSERT_EQUAL(10, f());
}
//[cf]
//[of]:void language_test\:\:TestIntegerAdd()
void language_test::TestIntegerAdd()
{
	struct fct1 : public functor_base<int()>
	{
		fct1() 
		{
			int_d x(10);

			x += 1;

			return_d(x);
		}
	} fct1_instance;
	functor<int()> f1(fct1_instance);

	CPPUNIT_ASSERT_EQUAL(11, f1());
	
	struct fct2 : public functor_base<int()>
	{
		fct2() 
		{
			int_d x(10);
			int_d y(1);

			x += y;

			return_d(x);
		}
	} fct2_instance;
	functor<int()> f2(fct2_instance);

	CPPUNIT_ASSERT_EQUAL(11, f2());

	struct fct3 : public functor_base<int()>
	{
		fct3() 
		{
			int_d x(10);

			x += 10;

			int_d y(x);

			return_d(y);
		}
	} fct3_instance;
	functor<int()> f3(fct3_instance);

	CPPUNIT_ASSERT_EQUAL(20, f3());


	struct fct4 : public functor_base<int()>
	{
		fct4() 
		{
			int_d x(1);
			int_d y(2);

			return_d(x + y);
		}
	} fct4_instance;
	functor<int()> f4(fct4_instance);

	CPPUNIT_ASSERT_EQUAL(3, f4());


	struct fct5 : public functor_base<int()>
	{
		fct5() 
		{
			int_d x(1);
			int_d y(2);
			
			int_d z(x + y);

			return_d(z);
		}
	} fct5_instance;
	functor<int()> f5(fct5_instance);

	CPPUNIT_ASSERT_EQUAL(3, f5());
}
//[cf]
//[of]:void language_test\:\:TestIntegerSubtract()
void language_test::TestIntegerSubtract()
{
	struct fct1 : public functor_base<int()>
	{
		fct1() 
		{
			int_d x(10);

			x -= 1;

			return_d(x);
		}
	} fct1_instance;
	functor<int()> f1(fct1_instance);

	CPPUNIT_ASSERT_EQUAL(9, f1());
	
	struct fct2 : public functor_base<int()>
	{
		fct2() 
		{
			int_d x(10);
			int_d y(1);

			x -= y;

			return_d(x);
		}
	} fct2_instance;
	functor<int()> f2(fct2_instance);

	CPPUNIT_ASSERT_EQUAL(9, f2());

	struct fct3 : public functor_base<int()>
	{
		fct3() 
		{
			int_d x(10);
			int_d y(7);

			return_d(x - y);
		}
	} fct3_instance;
	functor<int()> f3(fct3_instance);

	CPPUNIT_ASSERT_EQUAL(3, f3());
}
//[cf]
//[of]:void language_test\:\:TestIntegerAssign()
void language_test::TestIntegerAssign()
{
	struct fct1 : public functor_base<int()>
	{
		fct1()
		{
			int_d x(1);
			int_d y(3);
			
			x = y;

			return_d(x);
		}
	} fct1_instance;
	functor<int()> f1(fct1_instance);
	
	CPPUNIT_ASSERT_EQUAL(3, f1());
	
	// Test assignment to temporaries
	struct fct2 : public functor_base<int()>
	{
		fct2() 
		{
			int_d x(1);
			int_d y(2);
			
			int_d z;
			z = x + y;

			return_d(z);
		}
	} fct2_instance;
	functor<int()> f2(fct2_instance);

	CPPUNIT_ASSERT_EQUAL(3, f2());
}
//[cf]
//[c]
//[of]:void language_test\:\:TestBool()
void language_test::TestBool()
{
	struct fct : public functor_base<bool()>
	{
		fct() { bool_d x(true); return_d(x); }
	} fct_instance;

	functor<bool()> f(fct_instance);

	CPPUNIT_ASSERT_EQUAL(true, f());
	CPPUNIT_ASSERT_EQUAL(true, f());
}
//[cf]
//[of]:void language_test\:\:TestBoolComplex()
void language_test::TestBoolComplex()
{
	struct fct : public functor_base<bool()>
	{
		fct() 
		{
			bool_d x(true);
			bool_d y(false);

			return_d(x);
		}
	} fct_instance;

	functor<bool()> f(fct_instance);

	CPPUNIT_ASSERT_EQUAL(true, f());
	CPPUNIT_ASSERT_EQUAL(true, f());
}
//[cf]
//[of]:void language_test\:\:TestBoolAssign()
void language_test::TestBoolAssign()
{
	struct fct1 : public functor_base<bool()>
	{
		fct1()
		{
			bool_d x(true);
			bool_d y(false);
			
			x = y;
			
			return_d(x);
		}
	} fct1_instance;
	functor<bool()> f1(fct1_instance);
	
	CPPUNIT_ASSERT(!f1());
}
//[cf]
//[of]:void language_test\:\:TestBoolEqualityTest()
void language_test::TestBoolEqualityTest()
{
	struct fct1 : public functor_base<bool()>
	{
		fct1()
		{
			int_d x(1);
			int_d y(2);
			
			return_d(x == y);
		}
	} fct1_instance;
	functor<bool()> f1(fct1_instance);
	
	CPPUNIT_ASSERT(!f1());
	
	struct fct2 : public functor_base<bool()>
	{
		fct2()
		{
			int_d x(1);
			int_d y(1);
			
			return_d(x == y);
		}
	} fct2_instance;
	functor<bool()> f2(fct2_instance);
	
	CPPUNIT_ASSERT(f2());

	struct fct3 : public functor_base<bool()>
	{
		fct3()
		{
			bool_d x(true);
			bool_d y(false);
			
			return_d(x == y);
		}
	} fct3_instance;
	functor<bool()> f3(fct3_instance);

	CPPUNIT_ASSERT(!f3());

	struct fct4 : public functor_base<bool()>
	{
		fct4()
		{
			int_d x(1);
			int_d y(2);
			
			return_d(x != y);
		}
	} fct4_instance;
	functor<bool()> f4(fct4_instance);
	
	CPPUNIT_ASSERT(f4());


	struct fct5 : public functor_base<bool()>
	{
		fct5()
		{
			int_d x(10);
			int_d y(10);
			
			return_d(x != y);
		}
	} fct5_instance;
	functor<bool()> f5(fct5_instance);
	
	CPPUNIT_ASSERT(!f5());
}
//[cf]
//[c]
//[of]:void language_test\:\:TestDynamicIf()
void language_test::TestDynamicIf()
{
	struct fct1 : public functor_base<int()>
	{
		fct1() 
		{
			int_d x(1);
			int_d y(2);
			
			if_d(x == y);
			{
				scope s(this);
				return_d(100);
			}
			return_d(200);
		}
	} fct1_instance;

	functor<int()> f1(fct1_instance);

	CPPUNIT_ASSERT_EQUAL(200, f1());

	struct fct2 : public functor_base<int()>
	{
		fct2() 
		{
			int_d x(1);
			int_d y(1);
			
			if_d(x == y);
			{
				scope s(this);
				return_d(100);
			}
			return_d(200);
		}
	} fct2_instance;

	functor<int()> f2(fct2_instance);

	CPPUNIT_ASSERT_EQUAL(100, f2());

	
	struct fct3 : public functor_base<int()>
	{
		fct3()
		{
			int_d x(1);
			int_d y(2);
			
			if_d(x == y);
			{
				scope s1(this);
				return_d(100);
			}
			else_d();
			{
				scope s2(this);
				return_d(200);
			}
			return_d(300);
		}
	} fct3_instance;
	
	functor<int()> f3(fct3_instance);
	
	CPPUNIT_ASSERT_EQUAL(200, f3());
}
//[cf]
//[of]:void language_test\:\:TestParams()
void language_test::TestParams()
{
	struct fct_unary : public functor_base<int(int)>
	{
		fct_unary() 
		{
			int_d result(0);
			result = *(param_[0]);
			return_d(result);
		}
	} fct_unary_instance;

	functor<int(int)> f1(fct_unary_instance);

	CPPUNIT_ASSERT_EQUAL(100, f1(100));

	struct fct_binary_first : public functor_base<int(int, int)>
	{
		fct_binary_first() 
		{
			int_d result(0);
			result = *(param_[0]);
			return_d(result);
		}
	} fct_binary_first_instance;

	functor<int(int, int)> f2_1(fct_binary_first_instance);

	CPPUNIT_ASSERT_EQUAL(100, f2_1(100, 200));	

	struct fct_binary_second: public functor_base<int(int, int)>
	{
		fct_binary_second() 
		{
			int_d result(0);
			result = *(param_[1]);
			return_d(result);
		}
	} fct_binary_second_instance;

	functor<int(int, int)> f2_2(fct_binary_second_instance);

	CPPUNIT_ASSERT_EQUAL(200, f2_2(100, 200));	
}
//[cf]
//[c]
//[c]
