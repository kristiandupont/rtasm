rtasm
=====

Runtime Assembler for C++

This library allows you to generate highly efficient code by assembling it runtime. It consists of an approxmiation of x86 assembly language which generated the corresponding IA32 instructions.

Example
----------

	int testvalue = 21;

	assembler a;
	a.add_instruction(new Mov(EAX, imm<32>(testvalue)));
	a.add_instruction(new Ret());
	
	int result = (int)a.call();

This code creates an assembler, adds two instructions and calls the assembled code. After that, the result variable will be equals to 21.
	

Copyright
-----------
(C) Copyright 2005, 2012 Kristian Dupont Knudsen. MIT license -- see LICENSE.md for details