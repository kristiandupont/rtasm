//[of]:Copyright statement
//[c](C) Copyright 2005, 2012 Kristian Dupont Knudsen. MIT license -- see LICENSE.md for details
//[c]This file is most easily read with Code Browser: http://tibleiz.net/code-browser/
//[cf]
//[of]:header
#pragma once

#pragma warning(disable: 4311)

#include <vector>
#include <map>
#include <cassert>
#include <Windows.h>

namespace IA32
{

// Since calling conventions are something various platforms have real trouble agreeing upon,
// we'll try to abstract away from it using macros. Currently, only WIN32 __cdecl is supported.
#ifdef _WIN32
#define CALLINGCONVENTION __cdecl
#endif

//[cf]
//[c]
//[of]:struct operand
template<int Width>
struct operand
{
	enum { width = Width };
};
//[cf]
//[of]:Immediate definitions
//[of]:struct imm
template<int Width>
struct imm : public operand<Width>
{
	imm(int _value) : value(_value) {};
	
	int value;
};
//[cf]

typedef imm<8> i8;
typedef imm<16> i16;
typedef imm<32> i32;
//[cf]
//[of]:struct ptr
template<typename ValueType>
struct ptr : public operand<32>
{
	typedef ValueType* PtrType;
	
	ptr(PtrType _pointer) : pointer(_pointer) {};
	
	union
	{
		PtrType pointer;
		int address;
	};
};
//[cf]
//[of]:Register definitions
//[of]:struct reg
template<int Width>
struct reg : public operand<Width>
{
	explicit reg(int _opcode) : opcode(_opcode) {};
	
	int opcode;
};
//[cf]

typedef reg<8> r8;
typedef reg<16> r16;
typedef reg<32> r32;

struct regAL : public r8	{ regAL()	:  r8(0) {} };
struct regAX : public r16	{ regAX()	: r16(0) {} };
struct regEAX : public r32	{ regEAX()	: r32(0) {} };
struct regCL : public r8	{ regCL()	:  r8(1) {} };
struct regCX : public r16	{ regCX()	: r16(1) {} };
struct regECX : public r32	{ regECX()	: r32(1) {} };
struct regDL : public r8	{ regDL()	:  r8(2) {} };
struct regDX : public r16	{ regDX()	: r16(2) {} };
struct regEDX : public r32	{ regEDX()	: r32(2) {} };
struct regBL : public r8	{ regBL()	:  r8(3) {} };
struct regBX : public r16	{ regBX()	: r16(3) {} };
struct regEBX : public r32	{ regEBX()	: r32(3) {} };
struct regAH : public r8	{ regAH()	:  r8(4) {} };
struct regSP : public r16	{ regSP()	: r16(4) {} };
struct regESP : public r32	{ regESP()	: r32(4) {} };
struct regCH : public r8	{ regCH()	:  r8(5) {} };
struct regBP : public r16	{ regBP()	: r16(5) {} };
struct regEBP : public r32	{ regEBP()	: r32(5) {} };
struct regDH : public r8	{ regDH()	:  r8(6) {} };
struct regSI : public r16	{ regSI()	: r16(6) {} };
struct regESI : public r32	{ regESI()	: r32(6) {} };
struct regBH : public r8	{ regBH()	:  r8(7) {} };
struct regDI : public r16	{ regDI()	: r16(7) {} };
struct regEDI : public r32	{ regEDI()	: r32(7) {} };

extern const regAL AL; 
extern const regAX AX; 
extern const regEAX EAX;
extern const regCL CL; 
extern const regCX CX; 
extern const regECX ECX;
extern const regDL DL; 
extern const regDX DX; 
extern const regEDX EDX;
extern const regBL BL; 
extern const regBX BX; 
extern const regEBX EBX;
extern const regAH AH; 
extern const regSP SP; 
extern const regESP ESP;
extern const regCH CH; 
extern const regBP BP; 
extern const regEBP EBP;
extern const regDH DH; 
extern const regSI SI; 
extern const regESI ESI;
extern const regBH BH; 
extern const regDI DI; 
extern const regEDI EDI;
//[cf]
//[of]:Conditions
enum condition
{
	NONE			= 0xff,

	ABOVE			= 0x07,
	ABOVE_EQUAL		= 0x03,
	BELOW			= 0x02,
	BELOW_EQUAL		= 0x06,
	GREATER			= 0x0f,
	GREATER_EQUAL	= 0x0d,
	LESS			= 0x0c,
	LESS_EQUAL		= 0x0e,
	EQUAL			= 0x04,
	NOT_EQUAL		= 0x05,
	OVERFLW		= 0x00,
	NO_OVERFLOW		= 0x01,
	CARRY			= 0x02,
	NO_CARRY		= 0x03,
	SIGN			= 0x08,
	NO_SIGN			= 0x09,
	PARITY			= 0x0a,
	NO_PARITY		= 0x0b
};
//[cf]
//[of]:Instruction definitions
//[of]:struct instruction
struct instruction
{
	typedef std::vector<char>::size_type size_type;

	std::vector<char> const& get_encoded() const { return code_; }
	virtual size_type get_size() const { return size_; }

protected:
	template<int w>
	void push(int code) 
	{
		if(w == 8)
		{
			code_.push_back(code);
		}
		else if(w == 16)
		{
			code_.push_back(code & 0xff);
			code_.push_back(code >> 8);
		}
		else if(w == 32)
		{
			code_.push_back(code & 0xff);
			code_.push_back((code >> 8) & 0xff);
			code_.push_back((code >> 16) & 0xff);
			code_.push_back(code >> 24);
		}
		else
		{
			assert(!"Not implemented.");
		}
		size_ = code_.size();
	}

	void push_octet(int a, int b, int c)
	{
		code_.push_back((a << 6) | (b << 3) | c);
		size_ = code_.size();
	}
	
	void mode16() { push<8>(0x66); }
//	void clear() { code_.clear(); }

	std::vector<char>::size_type size_;

private:
	std::vector<char> code_;
};
//[cf]
//[of]:struct arithmetic_logic_binary
template<int o>
struct arithmetic_logic_binary : public instruction
{
	arithmetic_logic_binary(regAL, i8 i)
	{
		push_octet(0, o, 4);
		push<8>(i.value);
	}

	arithmetic_logic_binary(regAX, i16 i)
	{
		mode16();
		push_octet(0, o, 5);
		push<16>(i.value);
	}

	arithmetic_logic_binary(regEAX, i32 i)
	{
		push_octet(0, o, 5);
		push<32>(i.value);		
	}

	template<int w>
	arithmetic_logic_binary(reg<w> src, reg<w> dst)
	{
		if(w == 16) mode16();
		push_octet(0, o, (w == 8 ? 2 : 3));
		push_octet(3, src.opcode, dst.opcode);
	}

	template<int w>
	arithmetic_logic_binary(reg<w> src, imm<w> i)
	{
		if(w == 16) mode16();
		push_octet(2, 0, (w == 8 ? 0 : 1));
		push_octet(3, o, src.opcode);
		push<w>(i.value);
	}

	arithmetic_logic_binary(r8 src, i8 i)
	{
		push_octet(2, 0, 0);
		push_octet(3, o, src.opcode);
		push<8>(i.value);
	}

	arithmetic_logic_binary(r16 src, i16 i)
	{
		mode16();
		push_octet(2, 0, 1);
		push_octet(3, o, src.opcode);
		push<16>(i.value);
	}

	arithmetic_logic_binary(r32 src, i32 i)
	{
		push_octet(2, 0, 1);
		push_octet(3, o, src.opcode);
		push<32>(i.value);
	}

};
//[cf]
//[of]:struct arithmetic_logic_unary
template<int o>
struct arithmetic_logic_unary : public instruction
{
	template<int w>
	arithmetic_logic_unary(reg<w> reg)
	{
		if(w == 16) mode16();

		if(w != 8 && (o == 0 || o == 1))	// Inc and Dec have special, short versions..
		{
			push_octet(1, o, reg.opcode);
			return;
		}
		
		push_octet(3, (o == 2 || o == 3) ? 6 : 7, (w == 8 ? 6 : 7));
		push_octet(3, o, reg.opcode);
	}
};
//[cf]
//[c]
typedef arithmetic_logic_binary<0> Add;
typedef arithmetic_logic_binary<2> Adc;
typedef arithmetic_logic_binary<5> Sub;
typedef arithmetic_logic_binary<3> Sbb;
typedef arithmetic_logic_binary<7> Cmp;
typedef arithmetic_logic_binary<1> Or;
typedef arithmetic_logic_binary<4> And;
typedef arithmetic_logic_binary<6> Xor;
//[c]
typedef arithmetic_logic_unary<2> Not;
typedef arithmetic_logic_unary<3> Neg;
typedef arithmetic_logic_unary<0> Inc;
typedef arithmetic_logic_unary<1> Dec;
//[c]
//[of]:struct Set
struct Set : public instruction
{
	Set(r8 reg, condition _condition)
	{
		assert(_condition != NONE);

		push_octet(0, 1, 7);
		push<8>(0x90 + _condition);
		push_octet(3, 0, reg.opcode);
	}
};
//[cf]
//[c]
//[of]:struct Mov
struct Mov : public instruction
{
	template<typename PtrType>
	Mov(regEAX, ptr<PtrType> p)
	{
		push_octet(2, 4, 1);
		push<32>(p.address);
	}

	template<int w, typename PtrType>
	Mov(reg<w> dst, ptr<PtrType> p)
	{
		push<8>(0x3e);		// TODO: Why is this required?!
		push_octet(2, 1, 3);
		push_octet(0, dst.opcode, 5);
		push<32>(p.address);
	}

	template<int w>
	Mov(reg<w> dst, imm<w> src)
	{
		if(w == 32)
		{
			push_octet(2, 7, dst.opcode);
			push<32>(src.value);
		}
		else
		{
			assert(!"Not implemented yet.");
		}
	}

	template<typename PtrType>
	Mov(ptr<PtrType> p, regEAX)
	{
		push_octet(2, 4, 3);
		push<32>(p.address);
	}

	template<int w>
	Mov(reg<w> dst, reg<w> src)
	{
		if(w == 16) mode16();
		push_octet(2, 1, (w == 8 ? 2 : 3));
		push_octet(3, dst.opcode, src.opcode);
	}

};
//[cf]
//[of]:struct MovIndex
struct MovIndex : public instruction
{
	template<int w>
	MovIndex(reg<w> dst, r32 src_ptr)
	{
		if(w == 16) mode16();
		push_octet(2, 1, (w == 8) ? 2 : 3);
		push_octet(0, dst.opcode, src_ptr.opcode);
	}
};
//[cf]
//[of]:struct Ret
struct Ret : public instruction
{
	Ret()
	{
		push<8>(0xc3);
	}
};
//[cf]
//[of]:struct Jmp
struct Jmp : public instruction
{
	Jmp(std::string const& label, condition _condition = NONE) :
		label_(label), offset_(-1), condition_(_condition)
	{
		// Predetermine the size because it is needed even though we don't yet
		// know where to jump and hence haven't added any instructions.
		size_ = (condition_ == NONE) ? 5 : 6;
	}

	void set_offset(int offset)
	{
		// TODO: Hmm.. skal clear være implementeret?
//		clear();
		offset_ = offset;

		if(condition_ == NONE)
		{
			push<8>(0xe9);				// NEAR jump.
		}
		else
		{
			push_octet(0, 1, 7);
			push<8>(0x80 + condition_);	// Jcc
		}
		push<32>(offset);
	}
	
	std::vector<char> const& get_encoded() const
	{
		assert(offset_ != -1);
		return instruction::get_encoded();
	}
	
	std::string const& get_label() const
	{
		return label_;
	}
	
private:
	std::string  label_;
	int offset_;
	condition condition_;
};
//[cf]
//[of]:struct Call
struct Call : public instruction
{
	template<typename T>
	Call(T callee) : callee_address_(reinterpret_cast<int>(callee))
	{
		size_ = 5;
	}
	

	void set_address(int address)
	{
		int offset = callee_address_ - address;
		
		push_octet(3, 5, 0);
		push<32>(offset);
	}

private:
	int callee_address_;
};
//[c]
//[c]
//[c]0032131D E8 9B 84 16 00   call        @ILT+18360(_printf) (4897BDh) 
//[c]
//[c]16849b
//[c]
//[c]
//[c]
//[c]
//[cf]
//[c]
//[of]:struct Push
struct Push : public instruction
{
	Push(r16 reg)
	{
		mode16();
		push_octet(1, 2, reg.opcode);
	}

	Push(r32 reg)
	{
		push_octet(1, 2, reg.opcode);
	}
	
	Push(i16 i)
	{
		mode16();
		push_octet(1, 5, 0);
		push<16>(i.value);
	}

	Push(i32 i)
	{
		push_octet(1, 5, 0);
		push<32>(i.value);
	}
};
//[cf]
//[of]:struct Pop
struct Pop : public instruction
{
	Pop(r16 reg)
	{
		mode16();
		push_octet(1, 3, reg.opcode);
	}

	Pop(r32 reg)
	{
		push_octet(1, 3, reg.opcode);
	}
};
//[cf]
//[cf]
//[of]:class assembler
class assembler
{
public:
	assembler() : valid_(false), listsize_(0), thunk_(0) {}
	
	virtual ~assembler()
	{
		// Since we have assumed ownership of the instructions that were handed to us,
		// we must delete them.
		for(listing::iterator i = listing_.begin(); i != listing_.end(); ++i)
			delete *i;
	
		// Also, our thunk has probably been allocated as well. Wipe it.
		delete thunk_;
	}

	typedef int(CALLINGCONVENTION *FunctionPointerType)();

	int call()
	{
		// Get the function pointer and invoke.
		return get_function_pointer()();
	};

	FunctionPointerType get_function_pointer()
	{
		if(!valid_)
			assemble();
	
		void* codeptr = (void*)thunk_;

		return reinterpret_cast<FunctionPointerType>(codeptr);
	}

	void add_instruction(instruction* i) 
	{
		listing_.push_back(i);
		listsize_ += i->get_size();
		valid_ = false;
	}

	// Register a label at the current position. If the label has already been registered,
	// it will be overwritten.
	void register_label(std::string const& label)
	{
		labels_[label] = listsize_;
	}
	

private:
	void assemble()
	{
		delete thunk_;
		thunk_ = new char[listsize_];

		instruction::size_type listpos(0);

		for(listing::iterator i = listing_.begin(); i != listing_.end(); ++i)
		{
			if(Jmp* jmp = dynamic_cast<Jmp*>(*i))
			{
				// It's a jump instruction. Update the offset from here to destination.
				jmp->set_offset(static_cast<int>(labels_[jmp->get_label()] - (listpos + jmp->get_size())));
			}

			if(Call* call = dynamic_cast<Call*>(*i))
			{
				// It's a call instruction. Let it know where it is located so
				// it can determine the offset from the callee.
				void* codeptr = (void*)&thunk_[0];
				int address = reinterpret_cast<int>(codeptr) + static_cast<int>(listpos + call->get_size()); 

				call->set_address(static_cast<int>(address));
			}
			
			std::copy((*i)->get_encoded().begin(), (*i)->get_encoded().end(), thunk_ + listpos);
			listpos += (*i)->get_size();
		}
		valid_ = true;

		// Win32 specific instruction cache flushing:
		FlushInstructionCache(GetModuleHandle(NULL), (void*)thunk_, listsize_);
	}

	typedef std::vector<instruction*> listing;
	listing listing_;
	
	std::map<std::string, instruction::size_type> labels_;

	// Since there is no array implementation in the current standard, and vector<> places no
	// guarantee as to whether it will relocate everything (which we cant have with our relative calls etc.),
	// and std::auto_ptr<> doesn't support arrays, we stuck with a plain pointer here.
	char* thunk_;
	
	bool valid_;
	instruction::size_type listsize_;
};
//[cf]
//[c]
//[of]:footer

}	// namespace IA32
//[cf]
