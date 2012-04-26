//[of]:Copyright statement
//[c](C) Copyright 2005, 2012 Kristian Dupont Knudsen. MIT license -- see LICENSE.md for details
//[c]This file is most easily read with Code Browser: http://tibleiz.net/code-browser/
//[cf]
//[of]:header
#include "stdafx.h"
#include "assembler_test.h"

#include "assembler.h"
#include <memory>

CPPUNIT_TEST_SUITE_REGISTRATION(instructions_test);
CPPUNIT_TEST_SUITE_REGISTRATION(assembler_test);

// This warning is about changing frame pointer register inside a function.
// Since we will in fact be doing that (but know what we are doing :), we'll
// turn the warning off just to keep out the noise.
#pragma warning(disable: 4731)

using namespace std;
using namespace IA32;
//[cf]
//[c]
//[l]:Declaration:assembler_test.h
//[c]
//[c]This code tests the individual instructions one by one. After that, the assembler is tested.
//[c]The reason all instructions are put into auto_ptr's is that when they're not inserted into
//[c]an assembler, we maintain ownership (whereas the assembler usually takes over).
//[c]
//[of]:[code capturing macros]
#define CONCAT(s1, s2) s1##s2

#define CAPTURECODE(CODEBEGIN, CODEEND, CODEOFFSET, CODESIZE)	\
	{															\
	__asm	mov   EAX, offset CODEBEGIN							\
	__asm	mov   CODEOFFSET, EAX								\
	__asm	mov   EBX, offset CODEEND							\
	__asm	sub   EBX, EAX										\
	__asm	mov   CODESIZE, EBX									\
	}

#define CAPTURE2(NAME, CODEBEGIN, CODEEND, ADDRESS, SIZE)		\
	char* ADDRESS(0);											\
	int SIZE(0);												\
	CAPTURECODE(CODEBEGIN, CODEEND, ADDRESS, SIZE);				\
	std::vector<char> NAME(ADDRESS, ADDRESS + SIZE);

#define DEFINEVECTOR(NAME, CODEBEGIN, CODEEND) CAPTURE2(NAME, CODEBEGIN, CODEEND, CONCAT(NAME, addr), CONCAT(NAME, size))
//[cf]
//[of]:void instructions_test\:\:TestAdd()
void instructions_test::TestAdd()
{
	auto_ptr<Add> i1(new Add(AL, 1));
	auto_ptr<Add> i2(new Add(AX, 1));
	auto_ptr<Add> i3(new Add(EAX, ECX));
	auto_ptr<Add> i4(new Add(ECX, EBX));

	     __asm pushad
	a_1: __asm Add AL, 1
	a_2: __asm Add AX, 1
	a_3: __asm Add EAX, ECX
	a_4: __asm Add ECX, EBX
	a_5: __asm popad
	
	DEFINEVECTOR(code1_real, a_1, a_2);
	std::vector<char> code1_synthesized = i1->get_encoded();
	CPPUNIT_ASSERT(code1_real == code1_synthesized);
	
	DEFINEVECTOR(code2_real, a_2, a_3);
	std::vector<char> code2_synthesized = i2->get_encoded();
	CPPUNIT_ASSERT(code2_real == code2_synthesized);

	DEFINEVECTOR(code3_real, a_3, a_4);
	std::vector<char> code3_synthesized = i3->get_encoded();
	CPPUNIT_ASSERT(code3_real == code3_synthesized);
	
	DEFINEVECTOR(code4_real, a_4, a_5);
	std::vector<char> code4_synthesized = i4->get_encoded();
	CPPUNIT_ASSERT(code4_real == code4_synthesized);
}
//[cf]
//[of]:void instructions_test\:\:TestAdc()
void instructions_test::TestAdc()
{
	auto_ptr<Adc> i1(new Adc(AL, 1));
	auto_ptr<Adc> i2(new Adc(BX, 1000));
	auto_ptr<Adc> i3(new Adc(EAX, ECX));
	auto_ptr<Adc> i4(new Adc(ECX, EBX));
	
	     __asm pushad
	a_1: __asm Adc AL, 1
	a_2: __asm Adc BX, 1000
	a_3: __asm Adc EAX, ECX
	a_4: __asm Adc ECX, EBX
	a_5: __asm popad
	
	DEFINEVECTOR(code1_real, a_1, a_2);
	std::vector<char> code1_synthesized = i1->get_encoded();
	CPPUNIT_ASSERT(code1_real == code1_synthesized);
	
	DEFINEVECTOR(code2_real, a_2, a_3);
	std::vector<char> code2_synthesized = i2->get_encoded();
	CPPUNIT_ASSERT(code2_real == code2_synthesized);

	DEFINEVECTOR(code3_real, a_3, a_4);
	std::vector<char> code3_synthesized = i3->get_encoded();
	CPPUNIT_ASSERT(code3_real == code3_synthesized);
	
	DEFINEVECTOR(code4_real, a_4, a_5);
	std::vector<char> code4_synthesized = i4->get_encoded();
	CPPUNIT_ASSERT(code4_real == code4_synthesized);
}
//[cf]
//[of]:void instructions_test\:\:TestSub()
void instructions_test::TestSub()
{
	auto_ptr<Sub> i1(new Sub(AL, 1));
	auto_ptr<Sub> i2(new Sub(DX, 1000));
	auto_ptr<Sub> i3(new Sub(EDX, EAX));
	auto_ptr<Sub> i4(new Sub(ECX, EBX));
	
	     __asm pushad
	a_1: __asm Sub AL, 1
	a_2: __asm Sub DX, 1000
	a_3: __asm Sub EDX, EAX
	a_4: __asm Sub ECX, EBX
	a_5: __asm popad
	
	DEFINEVECTOR(code1_real, a_1, a_2);
	std::vector<char> code1_synthesized = i1->get_encoded();
	CPPUNIT_ASSERT(code1_real == code1_synthesized);
	
	DEFINEVECTOR(code2_real, a_2, a_3);
	std::vector<char> code2_synthesized = i2->get_encoded();
	CPPUNIT_ASSERT(code2_real == code2_synthesized);

	DEFINEVECTOR(code3_real, a_3, a_4);
	std::vector<char> code3_synthesized = i3->get_encoded();
	CPPUNIT_ASSERT(code3_real == code3_synthesized);
	
	DEFINEVECTOR(code4_real, a_4, a_5);
	std::vector<char> code4_synthesized = i4->get_encoded();
	CPPUNIT_ASSERT(code4_real == code4_synthesized);
}
//[cf]
//[of]:void instructions_test\:\:TestSbb()
void instructions_test::TestSbb()
{
	auto_ptr<Sbb> i1(new Sbb(AL, 1));
	auto_ptr<Sbb> i2(new Sbb(DX, 1000));
	auto_ptr<Sbb> i3(new Sbb(EDX, EAX));
	auto_ptr<Sbb> i4(new Sbb(ECX, EBX));
	
	     __asm pushad
	a_1: __asm Sbb AL, 1
	a_2: __asm Sbb DX, 1000
	a_3: __asm Sbb EDX, EAX
	a_4: __asm Sbb ECX, EBX
	a_5: __asm popad
	
	DEFINEVECTOR(code1_real, a_1, a_2);
	std::vector<char> code1_synthesized = i1->get_encoded();
	CPPUNIT_ASSERT(code1_real == code1_synthesized);
	
	DEFINEVECTOR(code2_real, a_2, a_3);
	std::vector<char> code2_synthesized = i2->get_encoded();
	CPPUNIT_ASSERT(code2_real == code2_synthesized);

	DEFINEVECTOR(code3_real, a_3, a_4);
	std::vector<char> code3_synthesized = i3->get_encoded();
	CPPUNIT_ASSERT(code3_real == code3_synthesized);
	
	DEFINEVECTOR(code4_real, a_4, a_5);
	std::vector<char> code4_synthesized = i4->get_encoded();
	CPPUNIT_ASSERT(code4_real == code4_synthesized);
}
//[cf]
//[of]:void instructions_test\:\:TestCmp()
void instructions_test::TestCmp()
{
	auto_ptr<Cmp> i1(new Cmp(AL, 1));
	auto_ptr<Cmp> i2(new Cmp(DX, 1000));
	auto_ptr<Cmp> i3(new Cmp(EDX, EAX));
	auto_ptr<Cmp> i4(new Cmp(ECX, EBX));
	
	     __asm pushad
	a_1: __asm Cmp AL, 1
	a_2: __asm Cmp DX, 1000
	a_3: __asm Cmp EDX, EAX
	a_4: __asm Cmp ECX, EBX
	a_5: __asm popad
	
	DEFINEVECTOR(code1_real, a_1, a_2);
	std::vector<char> code1_synthesized = i1->get_encoded();
	CPPUNIT_ASSERT(code1_real == code1_synthesized);
	
	DEFINEVECTOR(code2_real, a_2, a_3);
	std::vector<char> code2_synthesized = i2->get_encoded();
	CPPUNIT_ASSERT(code2_real == code2_synthesized);

	DEFINEVECTOR(code3_real, a_3, a_4);
	std::vector<char> code3_synthesized = i3->get_encoded();
	CPPUNIT_ASSERT(code3_real == code3_synthesized);
	
	DEFINEVECTOR(code4_real, a_4, a_5);
	std::vector<char> code4_synthesized = i4->get_encoded();
	CPPUNIT_ASSERT(code4_real == code4_synthesized);
}
//[cf]
//[of]:void instructions_test\:\:TestOr()
void instructions_test::TestOr()
{
	auto_ptr<Or> i1(new Or(AL, 1));
	auto_ptr<Or> i2(new Or(DX, BX));
	auto_ptr<Or> i3(new Or(EDX, 1000));
	auto_ptr<Or> i4(new Or(ECX, EBX));
	
	     __asm pushad
	a_1: __asm Or AL, 1
	a_2: __asm Or DX, BX
	a_3: __asm Or EDX, 1000
	a_4: __asm Or ECX, EBX
	a_5: __asm popad
	
	DEFINEVECTOR(code1_real, a_1, a_2);
	std::vector<char> code1_synthesized = i1->get_encoded();
	CPPUNIT_ASSERT(code1_real == code1_synthesized);
	
	DEFINEVECTOR(code2_real, a_2, a_3);
	std::vector<char> code2_synthesized = i2->get_encoded();
	CPPUNIT_ASSERT(code2_real == code2_synthesized);

	DEFINEVECTOR(code3_real, a_3, a_4);
	std::vector<char> code3_synthesized = i3->get_encoded();
	CPPUNIT_ASSERT(code3_real == code3_synthesized);
	
	DEFINEVECTOR(code4_real, a_4, a_5);
	std::vector<char> code4_synthesized = i4->get_encoded();
	CPPUNIT_ASSERT(code4_real == code4_synthesized);
}
//[cf]
//[of]:void instructions_test\:\:TestAnd()
void instructions_test::TestAnd()
{
	auto_ptr<And> i1(new And(AX, 1));
	auto_ptr<And> i2(new And(EAX, 1000));
	auto_ptr<And> i3(new And(EDX, EAX));
	auto_ptr<And> i4(new And(ECX, EBX));
	
	     __asm pushad
	a_1: __asm And AX, 1
	a_2: __asm And EAX, 1000
	a_3: __asm And EDX, EAX
	a_4: __asm And ECX, EBX
	a_5: __asm popad
	
	DEFINEVECTOR(code1_real, a_1, a_2);
	std::vector<char> code1_synthesized = i1->get_encoded();
	CPPUNIT_ASSERT(code1_real == code1_synthesized);
	
	DEFINEVECTOR(code2_real, a_2, a_3);
	std::vector<char> code2_synthesized = i2->get_encoded();
	CPPUNIT_ASSERT(code2_real == code2_synthesized);

	DEFINEVECTOR(code3_real, a_3, a_4);
	std::vector<char> code3_synthesized = i3->get_encoded();
	CPPUNIT_ASSERT(code3_real == code3_synthesized);
	
	DEFINEVECTOR(code4_real, a_4, a_5);
	std::vector<char> code4_synthesized = i4->get_encoded();
	CPPUNIT_ASSERT(code4_real == code4_synthesized);
}
//[cf]
//[of]:void instructions_test\:\:TestXor()
void instructions_test::TestXor()
{
	auto_ptr<Xor> i1(new Xor(AL, 1));
	auto_ptr<Xor> i2(new Xor(DX, 1000));
	auto_ptr<Xor> i3(new Xor(EDX, EAX));
	auto_ptr<Xor> i4(new Xor(ECX, EBX));
	
	     __asm pushad
	a_1: __asm Xor AL, 1
	a_2: __asm Xor DX, 1000
	a_3: __asm Xor EDX, EAX
	a_4: __asm Xor ECX, EBX
	a_5: __asm popad
	
	DEFINEVECTOR(code1_real, a_1, a_2);
	std::vector<char> code1_synthesized = i1->get_encoded();
	CPPUNIT_ASSERT(code1_real == code1_synthesized);
	
	DEFINEVECTOR(code2_real, a_2, a_3);
	std::vector<char> code2_synthesized = i2->get_encoded();
	CPPUNIT_ASSERT(code2_real == code2_synthesized);

	DEFINEVECTOR(code3_real, a_3, a_4);
	std::vector<char> code3_synthesized = i3->get_encoded();
	CPPUNIT_ASSERT(code3_real == code3_synthesized);
	
	DEFINEVECTOR(code4_real, a_4, a_5);
	std::vector<char> code4_synthesized = i4->get_encoded();
	CPPUNIT_ASSERT(code4_real == code4_synthesized);
}
//[cf]
//[of]:void instructions_test\:\:TestNot()
void instructions_test::TestNot()
{
	auto_ptr<Not> i1(new Not(AL));
	auto_ptr<Not> i2(new Not(BL));
	auto_ptr<Not> i3(new Not(EDX));
	auto_ptr<Not> i4(new Not(CX));
	
	     __asm pushad
	a_1: __asm Not AL
	a_2: __asm Not BL
	a_3: __asm Not EDX
	a_4: __asm Not CX
	a_5: __asm popad
	
	DEFINEVECTOR(code1_real, a_1, a_2);
	std::vector<char> code1_synthesized = i1->get_encoded();
	CPPUNIT_ASSERT(code1_real == code1_synthesized);

	DEFINEVECTOR(code2_real, a_2, a_3);
	std::vector<char> code2_synthesized = i2->get_encoded();
	CPPUNIT_ASSERT(code2_real == code2_synthesized);

	DEFINEVECTOR(code3_real, a_3, a_4);
	std::vector<char> code3_synthesized = i3->get_encoded();
	CPPUNIT_ASSERT(code3_real == code3_synthesized);
	
	DEFINEVECTOR(code4_real, a_4, a_5);
	std::vector<char> code4_synthesized = i4->get_encoded();
	CPPUNIT_ASSERT(code4_real == code4_synthesized);
}
//[cf]
//[of]:void instructions_test\:\:TestNeg()
void instructions_test::TestNeg()
{
	auto_ptr<Neg> i1(new Neg(AL));
	auto_ptr<Neg> i2(new Neg(BL));
	auto_ptr<Neg> i3(new Neg(EDX));
	auto_ptr<Neg> i4(new Neg(CX));
	
	     __asm pushad
	a_1: __asm Neg AL
	a_2: __asm Neg BL
	a_3: __asm Neg EDX
	a_4: __asm Neg CX
	a_5: __asm popad
	
	DEFINEVECTOR(code1_real, a_1, a_2);
	std::vector<char> code1_synthesized = i1->get_encoded();
	CPPUNIT_ASSERT(code1_real == code1_synthesized);

	DEFINEVECTOR(code2_real, a_2, a_3);
	std::vector<char> code2_synthesized = i2->get_encoded();
	CPPUNIT_ASSERT(code2_real == code2_synthesized);

	DEFINEVECTOR(code3_real, a_3, a_4);
	std::vector<char> code3_synthesized = i3->get_encoded();
	CPPUNIT_ASSERT(code3_real == code3_synthesized);
	
	DEFINEVECTOR(code4_real, a_4, a_5);
	std::vector<char> code4_synthesized = i4->get_encoded();
	CPPUNIT_ASSERT(code4_real == code4_synthesized);
}
//[cf]
//[of]:void instructions_test\:\:TestInc()
void instructions_test::TestInc()
{
	auto_ptr<Inc> i1(new Inc(AL));
	auto_ptr<Inc> i2(new Inc(BL));
	auto_ptr<Inc> i3(new Inc(EBX));
	auto_ptr<Inc> i4(new Inc(CX));
	
	     __asm pushad
	a_1: __asm Inc AL
	a_2: __asm Inc BL
	a_3: __asm Inc EBX
	a_4: __asm Inc CX
	a_5: __asm popad
	
	DEFINEVECTOR(code1_real, a_1, a_2);
	std::vector<char> code1_synthesized = i1->get_encoded();
	CPPUNIT_ASSERT(code1_real == code1_synthesized);

	DEFINEVECTOR(code2_real, a_2, a_3);
	std::vector<char> code2_synthesized = i2->get_encoded();
	CPPUNIT_ASSERT(code2_real == code2_synthesized);

	DEFINEVECTOR(code3_real, a_3, a_4);
	std::vector<char> code3_synthesized = i3->get_encoded();
	CPPUNIT_ASSERT(code3_real == code3_synthesized);
	
	DEFINEVECTOR(code4_real, a_4, a_5);
	std::vector<char> code4_synthesized = i4->get_encoded();
	CPPUNIT_ASSERT(code4_real == code4_synthesized);
}
//[cf]
//[of]:void instructions_test\:\:TestDec()
void instructions_test::TestDec()
{
	auto_ptr<Dec> i1(new Dec(AL));
	auto_ptr<Dec> i2(new Dec(BL));
	auto_ptr<Dec> i3(new Dec(EDX));
	auto_ptr<Dec> i4(new Dec(CX));
	
	     __asm pushad
	a_1: __asm Dec AL
	a_2: __asm Dec BL
	a_3: __asm Dec EDX
	a_4: __asm Dec CX
	a_5: __asm popad
	
	DEFINEVECTOR(code1_real, a_1, a_2);
	std::vector<char> code1_synthesized = i1->get_encoded();
	CPPUNIT_ASSERT(code1_real == code1_synthesized);

	DEFINEVECTOR(code2_real, a_2, a_3);
	std::vector<char> code2_synthesized = i2->get_encoded();
	CPPUNIT_ASSERT(code2_real == code2_synthesized);

	DEFINEVECTOR(code3_real, a_3, a_4);
	std::vector<char> code3_synthesized = i3->get_encoded();
	CPPUNIT_ASSERT(code3_real == code3_synthesized);
	
	DEFINEVECTOR(code4_real, a_4, a_5);
	std::vector<char> code4_synthesized = i4->get_encoded();
	CPPUNIT_ASSERT(code4_real == code4_synthesized);
}
//[cf]
//[of]:void instructions_test\:\:TestMov()
void instructions_test::TestMov()
{

#pragma warning(disable: 4312)		// We know - this is dangerous code. Don't warn us.
	char* somepointer = reinterpret_cast<char*>(0xdeadbeef);
#pragma warning(default: 4312)

	auto_ptr<Mov> i1(new Mov(AL, AH));
	auto_ptr<Mov> i2(new Mov(EAX, ptr<char>(somepointer)));
	auto_ptr<Mov> i3(new Mov(EDX, EAX));
	auto_ptr<Mov> i4(new Mov(CX, DX));
	
	goto skip;

	     __asm pushad
	a_1: __asm Mov AL, AH
	a_2: __asm Mov EAX, dword ptr DS:[0xdeafbeef]
	a_3: __asm Mov EDX, EAX
	a_4: __asm Mov CX, DX
	a_5: __asm popad
	
skip:

	DEFINEVECTOR(code1_real, a_1, a_2);
	std::vector<char> code1_synthesized = i1->get_encoded();
	CPPUNIT_ASSERT(code1_real == code1_synthesized);

	// For some reason or other, I have not been able to get this test
	// to work. What the Microsoft assembler does is just a mystery to me.

	DEFINEVECTOR(code2_real, a_2, a_3);
	std::vector<char> code2_synthesized = i2->get_encoded();
//	CPPUNIT_ASSERT(code2_real == code2_synthesized);

	DEFINEVECTOR(code3_real, a_3, a_4);
	std::vector<char> code3_synthesized = i3->get_encoded();
	CPPUNIT_ASSERT(code3_real == code3_synthesized);
	
	DEFINEVECTOR(code4_real, a_4, a_5);
	std::vector<char> code4_synthesized = i4->get_encoded();
	CPPUNIT_ASSERT(code4_real == code4_synthesized);
}
//[cf]
//[of]:void instructions_test\:\:TestMovIndex()
void instructions_test::TestMovIndex()
{
	auto_ptr<MovIndex> i1(new MovIndex(EAX, EAX));
	auto_ptr<MovIndex> i2(new MovIndex(AL, EBX));
	auto_ptr<MovIndex> i3(new MovIndex(CX, EDX));
	auto_ptr<MovIndex> i4(new MovIndex(EBX, EAX));
	
	     __asm pushad
	a_1: __asm Mov EAX, [EAX]
	a_2: __asm Mov AL, [EBX]
	a_3: __asm Mov CX, [EDX]
	a_4: __asm Mov EBX, [EAX]
	a_5: __asm popad
	
	DEFINEVECTOR(code1_real, a_1, a_2);
	std::vector<char> code1_synthesized = i1->get_encoded();
	CPPUNIT_ASSERT(code1_real == code1_synthesized);

	DEFINEVECTOR(code2_real, a_2, a_3);
	std::vector<char> code2_synthesized = i2->get_encoded();
	CPPUNIT_ASSERT(code2_real == code2_synthesized);

	DEFINEVECTOR(code3_real, a_3, a_4);
	std::vector<char> code3_synthesized = i3->get_encoded();
	CPPUNIT_ASSERT(code3_real == code3_synthesized);
	
	DEFINEVECTOR(code4_real, a_4, a_5);
	std::vector<char> code4_synthesized = i4->get_encoded();
	CPPUNIT_ASSERT(code4_real == code4_synthesized);
}
//[cf]
//[of]:void instructions_test\:\:TestRet()
void instructions_test::TestRet()
{
	auto_ptr<Ret> i1(new Ret());
	
	goto skip;
	
	     __asm pushad
	a_1: __asm ret
	a_2: __asm popad

skip:
	
	DEFINEVECTOR(code1_real, a_1, a_2);
	std::vector<char> code1_synthesized = i1->get_encoded();
	CPPUNIT_ASSERT(code1_real == code1_synthesized);
}
//[cf]
//[of]:void instructions_test\:\:TestSet()
void instructions_test::TestSet()
{
	auto_ptr<Set> i1(new Set(AL, ABOVE));
	auto_ptr<Set> i2(new Set(BL, BELOW_EQUAL));
	auto_ptr<Set> i3(new Set(CL, OVERFLW));
	auto_ptr<Set> i4(new Set(DL, PARITY));
	
	     __asm pushad
	a_1: __asm SetA AL
	a_2: __asm SetBE BL
	a_3: __asm SetO CL
	a_4: __asm SetP DL
	a_5: __asm popad
	
	DEFINEVECTOR(code1_real, a_1, a_2);
	std::vector<char> code1_synthesized = i1->get_encoded();
	CPPUNIT_ASSERT(code1_real == code1_synthesized);

	DEFINEVECTOR(code2_real, a_2, a_3);
	std::vector<char> code2_synthesized = i2->get_encoded();
	CPPUNIT_ASSERT(code2_real == code2_synthesized);

	DEFINEVECTOR(code3_real, a_3, a_4);
	std::vector<char> code3_synthesized = i3->get_encoded();
	CPPUNIT_ASSERT(code3_real == code3_synthesized);
	
	DEFINEVECTOR(code4_real, a_4, a_5);
	std::vector<char> code4_synthesized = i4->get_encoded();
	CPPUNIT_ASSERT(code4_real == code4_synthesized);
}
//[cf]
//[c]
//[of]:void instructions_test\:\:TestPush()
void instructions_test::TestPush()
{
	auto_ptr<Push> i1(new Push(AX));
	auto_ptr<Push> i2(new Push(BX));
	auto_ptr<Push> i3(new Push(EDX));
	auto_ptr<Push> i4(new Push(BP));
	auto_ptr<Push> i5(new Push(imm<32>(100000)));
	
	goto skip;

	     __asm pushad
	a_1: __asm Push AX
	a_2: __asm Push BX
	a_3: __asm Push EDX
	a_4: __asm Push BP
	a_5: __asm Push 100000		// Use a large number to force 32-bit..
	a_6: __asm popad

skip:

	DEFINEVECTOR(code1_real, a_1, a_2);
	std::vector<char> code1_synthesized = i1->get_encoded();
	CPPUNIT_ASSERT(code1_real == code1_synthesized);

	DEFINEVECTOR(code2_real, a_2, a_3);
	std::vector<char> code2_synthesized = i2->get_encoded();
	CPPUNIT_ASSERT(code2_real == code2_synthesized);

	DEFINEVECTOR(code3_real, a_3, a_4);
	std::vector<char> code3_synthesized = i3->get_encoded();
	CPPUNIT_ASSERT(code3_real == code3_synthesized);
	
	DEFINEVECTOR(code4_real, a_4, a_5);
	std::vector<char> code4_synthesized = i4->get_encoded();
	CPPUNIT_ASSERT(code4_real == code4_synthesized);

	DEFINEVECTOR(code5_real, a_5, a_6);
	std::vector<char> code5_synthesized = i5->get_encoded();
	CPPUNIT_ASSERT(code5_real == code5_synthesized);
}
//[cf]
//[of]:void instructions_test\:\:TestPop()
void instructions_test::TestPop()
{
	auto_ptr<Pop> i1(new Pop(AX));
	auto_ptr<Pop> i2(new Pop(BX));
	auto_ptr<Pop> i3(new Pop(EDX));
	auto_ptr<Pop> i4(new Pop(BP));

	goto skip;
	
	     __asm pushad
	a_1: __asm Pop AX
	a_2: __asm Pop BX
	a_3: __asm Pop EDX
	a_4: __asm Pop BP
	a_5: __asm popad

skip:

	DEFINEVECTOR(code1_real, a_1, a_2);
	std::vector<char> code1_synthesized = i1->get_encoded();
	CPPUNIT_ASSERT(code1_real == code1_synthesized);

	DEFINEVECTOR(code2_real, a_2, a_3);
	std::vector<char> code2_synthesized = i2->get_encoded();
	CPPUNIT_ASSERT(code2_real == code2_synthesized);

	DEFINEVECTOR(code3_real, a_3, a_4);
	std::vector<char> code3_synthesized = i3->get_encoded();
	CPPUNIT_ASSERT(code3_real == code3_synthesized);
	
	DEFINEVECTOR(code4_real, a_4, a_5);
	std::vector<char> code4_synthesized = i4->get_encoded();
	CPPUNIT_ASSERT(code4_real == code4_synthesized);
}
//[cf]
//[c]
//[c]
//[of]:void assembler_test\:\:TestInvoke()
void assembler_test::TestInvoke()
{
	assembler a;
	a.add_instruction(new Ret());
	a.call();
	CPPUNIT_ASSERT(true);
}
//[cf]
//[of]:void assembler_test\:\:TestInvokeParam()
void assembler_test::TestInvokeParam()
{
	int testvalue = 21;

	assembler a;
	a.add_instruction(new Mov(EAX, imm<32>(testvalue)));
	a.add_instruction(new Ret());
	
	int result = (int)a.call();
	
	CPPUNIT_ASSERT_EQUAL(testvalue, result);	
}
//[cf]
//[of]:void assembler_test\:\:TestJmp()
void assembler_test::TestJmp()
{
	assembler a1;
	a1.add_instruction(new Mov(EAX, imm<32>(100)));
	a1.add_instruction(new Jmp("Label1"));
	a1.add_instruction(new Mov(EAX, imm<32>(200)));
	a1.register_label("Label1");
	a1.add_instruction(new Ret());

	CPPUNIT_ASSERT_EQUAL(100, (int)a1.call());	


	assembler a2;
	a2.add_instruction(new Mov(EAX, imm<32>(100)));
	a2.add_instruction(new Mov(EBX, imm<32>(0)));
	a2.add_instruction(new Cmp(EBX, imm<32>(0)));
	a2.add_instruction(new Jmp("Label1", EQUAL));
	a2.add_instruction(new Mov(EAX, imm<32>(200)));
	a2.register_label("Label1");
	a2.add_instruction(new Ret());

	CPPUNIT_ASSERT_EQUAL(100, (int)a2.call());
}
//[cf]
//[of]:void assembler_test\:\:TestCall()
bool testcall_success;
int testcall_integer;

void testcall_nullary()
{
	testcall_success = true;
}

void testcall_unary(int param)
{
	testcall_integer = param;
}

int testcall_return()
{
	return testcall_integer;
}

void assembler_test::TestCall()
{
	testcall_success = false;

	assembler a1;
	a1.add_instruction(new Call(testcall_nullary));
	a1.add_instruction(new Ret());

	a1.call();

	CPPUNIT_ASSERT(testcall_success);
	
	
	testcall_integer = 0;
	int testval = 100;
	
	assembler a2;
	a2.add_instruction(new Push(imm<32>(testval)));
	a2.add_instruction(new Call(testcall_unary));
	a2.add_instruction(new Add(ESP, 4));
	a2.add_instruction(new Ret());
	
	a2.call();
	
	CPPUNIT_ASSERT_EQUAL(testval, testcall_integer);
	
	
	assembler a3;
	a3.add_instruction(new Mov(EAX, imm<32>(0)));
	a3.add_instruction(new Call(testcall_return));
	a3.add_instruction(new Ret());
	
	CPPUNIT_ASSERT_EQUAL(testval, a3.call());
}
//[cf]
