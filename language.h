//[of]:Copyright statement
//[c](C) Copyright 2005, 2012 Kristian Dupont Knudsen. MIT license -- see LICENSE.md for details
//[c]This file is most easily read with Code Browser: http://tibleiz.net/code-browser/
//[cf]
//[of]:header
#pragma once

#include <stack>
#include "assembler.h"
#include <boost/function.hpp>
#include <boost/type_traits/function_traits.hpp>

#pragma warning(disable: 4355)

namespace rtasm
{

// using a namespace in a header file is note pretty but since the using statement is 
// present within the rtcg namespace, it's somewhat okay..
using namespace IA32;
//using namespace boost;
//[cf]
//[c]
//[of]:class scope_proxy
class scope_proxy
{
public:
	enum scope_type
	{
		PLAIN,
		IF,
		ELSE,
		WHILE,
	};


	scope_proxy(assembler* asmb) : asm_(asmb), scope_type_(PLAIN) {}

	assembler* get_assembler() { return asm_; }
	
	void tag(scope_type _scope_type, std::string const& label_1, std::string const& label_2)
	{
		scope_type_ = _scope_type;
		label_1_ = label_1;
		label_2_ = label_2;
	}
	
	scope_type get_scope_type() const { return scope_type_; }
	std::string const& get_label_1() const { return label_1_; }
	std::string const& get_label_2() const { return label_2_; }
	
private:
	assembler* asm_;
	scope_type scope_type_;
	std::string label_1_;
	std::string label_2_;
};
//[cf]
//[of]:class environment
class environment
{
public:
	environment() : next_scope_type_(scope_proxy::PLAIN) {}

	assembler* get_assembler() { return scopes_.top()->get_assembler(); }
	
	void push_scope(scope_proxy* scope)
	{
		scope->tag(next_scope_type_, next_label_1_, next_label_2_);
		scopes_.push(scope);
		
		next_label_1_ = "";
		next_label_2_ = "";
	}
	
	void pop_scope()
	{
		scope_proxy* oldscope = scopes_.top();
		scopes_.pop();

		if(oldscope->get_scope_type() == scope_proxy::IF)
		{
			// If this was an "if" scope, we'll register begin by jumping to the "endif" label,
			// and then register both the endif and the else label.

			std::string else_label = oldscope->get_label_1();
			current_endif_label_ = oldscope->get_label_2();
			
			get_assembler()->add_instruction(new Jmp(current_endif_label_));
			
			get_assembler()->register_label(current_endif_label_);
			get_assembler()->register_label(else_label);
		}
		else if(oldscope->get_scope_type() == scope_proxy::ELSE)
		{
			// It was an "else" scope. Move the "endif" label down to the end of this scope then.
			
			current_endif_label_ = oldscope->get_label_1();
			
			get_assembler()->register_label(current_endif_label_);
		}
	}

	// Tells the environment that the proceeding scope is an if scope and may we
	// please have an else label from it.
	std::string create_if_scope()
	{
		next_scope_type_ = scope_proxy::IF;
		next_label_1_ = create_label();		// "else" label
		next_label_2_ = create_label();		// "endif" label
		return next_label_1_;
	}

	std::string create_else_scope()
	{
		next_scope_type_ = scope_proxy::ELSE;
		next_label_1_ = current_endif_label_;	// The "endif" label moves along to the end of the else scope.
		return next_label_1_;
	}

private:
	std::stack<scope_proxy*> scopes_;
	
	scope_proxy::scope_type next_scope_type_;
	
	std::string next_label_1_;
	std::string next_label_2_;

	std::string current_endif_label_;
	
	std::string create_label()
	{
		static int counter(0);
		std::string result = "env_generated_label_" + counter++;
		return result;
	}
};
//[cf]
//[of]:environment g_environment;
environment g_environment;
//[cf]
//[c]
//[of]:class functor_base
template<typename Signature>
class functor_base
{
public:
	typedef typename boost::function_traits<Signature>::result_type ResultType;


	Signature* get_function_get_ptr()
	{
		assembler* a = g_environment.get_assembler();
		return reinterpret_cast<Signature*>(a->get_function_pointer());
	}

protected:

//[of]:	class variable_d
	template<typename T>
	class variable_d
	{
	public:
		variable_d() : var_(new T(0)), ownership_(true)
		{
		}
		
		virtual ~variable_d()
		{
//			if(ownership_)
//				delete var_;
		}
		
		explicit variable_d(T const& var) : var_(new T(var)), ownership_(true)
		{
			assembler* a = g_environment.get_assembler();

			// Initialization is required because we don't know
			// if this function has been called before.
			
			a->add_instruction(new Mov(EAX, i32(var)));
			a->add_instruction(new Mov(get_ptr(), EAX));
		}
		
		// Copy constructor. Make a dynamic copy.
		variable_d(variable_d<T>& var) : var_(new T(*(var.var_))), ownership_(true)
		{
			assembler* a = g_environment.get_assembler();

			a->add_instruction(new Mov(EAX, var.get_ptr()));
			a->add_instruction(new Mov(get_ptr(), EAX));
		}
	
		ptr<T> get_ptr()
		{
			return ptr<T>(var_);
		}

		variable_d<T>& operator = (variable_d<T>& v)
		{
			assembler* a = g_environment.get_assembler();

			a->add_instruction(new Mov(EAX, v.get_ptr()));
			a->add_instruction(new Mov(get_ptr(), EAX));
			return *this;
		}

		// Used for parameters. Assign to the value currently present in EAX.
		void from_EAX()
		{
			assembler* a = g_environment.get_assembler();
			a->add_instruction(new Mov(get_ptr(), EAX));
		}


	private:
		T* var_;
		bool ownership_;
	};
//[cf]
//[of]:	class bool_d \: public variable_d<bool>
	class bool_d : public variable_d<bool>
	{
	public:
		bool_d() {}
		explicit bool_d(bool const& var) : variable_d<bool>(var) {}
		bool_d(bool_d& var) : variable_d<bool>(var) {}


		bool_d operator == (bool_d& v)
		{
			assembler* a = g_environment.get_assembler();
			bool_d temporary;

			a->add_instruction(new Mov(EAX, get_ptr()));
			a->add_instruction(new Mov(EBX, v.get_ptr()));
			a->add_instruction(new Cmp(EAX, EBX));
			a->add_instruction(new Mov(EAX, imm<32>(0)));
			a->add_instruction(new Set(AL, EQUAL));
						
			a->add_instruction(new Mov(temporary.get_ptr(), EAX));
			return temporary;
		}



	};
//[cf]
//[of]:	class int_d \: public variable_d<int>
	class int_d : public variable_d<int>
	{
	public:
		int_d() {}
		explicit int_d(int const& var) : variable_d<int>(var) {}
		int_d(int_d& var) : variable_d<int>(var) {}
	

		int_d& operator += (i32 const& i)
		{
			assembler* a = g_environment.get_assembler();
			a->add_instruction(new Mov(EAX, get_ptr()));
			a->add_instruction(new Add(EAX, i));
			a->add_instruction(new Mov(get_ptr(), EAX));
			return *this;
		}

		int_d& operator += (int_d& v)
		{
			assembler* a = g_environment.get_assembler();
			a->add_instruction(new Mov(EAX, get_ptr()));
			a->add_instruction(new Mov(EBX, v.get_ptr()));
			a->add_instruction(new Add(EAX, EBX));
			a->add_instruction(new Mov(get_ptr(), EAX));
			return *this;
		}

		int_d& operator -= (i32 const& i)
		{
			assembler* a = g_environment.get_assembler();
			a->add_instruction(new Mov(EAX, get_ptr()));
			a->add_instruction(new Sub(EAX, i.value));
			a->add_instruction(new Mov(get_ptr(), EAX));
			return *this;
		}

		int_d& operator -= (int_d& v)
		{
			assembler* a = g_environment.get_assembler();
			a->add_instruction(new Mov(EAX, get_ptr()));
			a->add_instruction(new Mov(EBX, v.get_ptr()));
			a->add_instruction(new Sub(EAX, EBX));
			a->add_instruction(new Mov(get_ptr(), EAX));
			return *this;
		}

		int_d operator + (int_d& v)
		{
			assembler* a = g_environment.get_assembler();
			int_d temporary;
			a->add_instruction(new Mov(EAX, get_ptr()));
			a->add_instruction(new Mov(EBX, v.get_ptr()));
			a->add_instruction(new Add(EAX, EBX));
			a->add_instruction(new Mov(temporary.get_ptr(), EAX));
			return temporary;
		}

		int_d operator - (int_d& v)
		{
			assembler* a = g_environment.get_assembler();
			int_d temporary;
			a->add_instruction(new Mov(EAX, get_ptr()));
			a->add_instruction(new Mov(EBX, v.get_ptr()));
			a->add_instruction(new Sub(EAX, EBX));
			a->add_instruction(new Mov(temporary.get_ptr(), EAX));
			return temporary;
		}

		bool_d operator == (int_d& v)
		{
			assembler* a = g_environment.get_assembler();
			bool_d temporary;

			a->add_instruction(new Mov(EAX, get_ptr()));
			a->add_instruction(new Mov(EBX, v.get_ptr()));
			a->add_instruction(new Cmp(EAX, EBX));
			a->add_instruction(new Mov(EAX, imm<32>(0)));
			a->add_instruction(new Set(AL, EQUAL));
						
			a->add_instruction(new Mov(temporary.get_ptr(), EAX));
			return temporary;
		}

		bool_d operator == (i32 const& i)
		{
			assembler* a = g_environment.get_assembler();
			bool_d temporary;

			a->add_instruction(new Mov(EAX, get_ptr()));
			a->add_instruction(new Mov(EBX, i));
			a->add_instruction(new Cmp(EAX, EBX));
			a->add_instruction(new Mov(EAX, imm<32>(0)));
			a->add_instruction(new Set(AL, EQUAL));
						
			a->add_instruction(new Mov(temporary.get_ptr(), EAX));
			return temporary;
		}

		bool_d operator != (int_d& v)
		{
			assembler* a = g_environment.get_assembler();
			bool_d temporary;

			a->add_instruction(new Mov(EAX, get_ptr()));
			a->add_instruction(new Mov(EBX, v.get_ptr()));
			a->add_instruction(new Cmp(EAX, EBX));
			a->add_instruction(new Mov(EAX, imm<32>(0)));
			a->add_instruction(new Set(AL, NOT_EQUAL));
						
			a->add_instruction(new Mov(temporary.get_ptr(), EAX));
			return temporary;
		}

		bool_d operator != (i32 const& i)
		{
			assembler* a = g_environment.get_assembler();
			bool_d temporary;

			a->add_instruction(new Mov(EAX, get_ptr()));
			a->add_instruction(new Mov(EBX, i));
			a->add_instruction(new Cmp(EAX, EBX));
			a->add_instruction(new Mov(EAX, imm<32>(0)));
			a->add_instruction(new Set(AL, NOT_EQUAL));
						
			a->add_instruction(new Mov(temporary.get_ptr(), EAX));
			return temporary;
		}
	};
//[cf]

//[of]:	class scope \: public scope_proxy
//[c]Hvordan skal scope fungere?
//[c] - Den skal kende alle variable, og hvilke registre de befinder sig i..
//[c] - Den skal instansieres med parent scopets variable og registre
//[c] - Den skal holde styr på temporaries og registrere disse hos sin functor, der så kan tilbyde hukommelsesplads til dem (om nødvendigt).
//[c] - Hvad med bools? Men skal kunne optimere bools væk fra variable.
//[c]
	class scope : public scope_proxy
	{
	public:
		scope(functor_base<Signature>* owner) : scope_proxy(&owner->asm_), owner_(owner)
		{
			g_environment.push_scope(this);
		};

		~scope()
		{
			g_environment.pop_scope();
		}
	
		template<typename T>
		void register_variable(variable_d<T>& var)
		{
		}

	private:
		functor_base<Signature>* owner_;
	};
	
	// Allow scopes to access our internal assembler.
	friend class scope;
	
//[cf]

	// Parameter container. Currently, only ints are allowed .
	std::vector<int_d*> param_;

	// The outermost scope ensures that we have an environment right away.
	scope outer_scope_;

public:
	functor_base() : outer_scope_(this)
	{		
		assembler* a = g_environment.get_assembler();
		
		// Add code for function entry.
		// Read arguments from the stack etc.
		a->add_instruction(new Push(EDI));
		a->add_instruction(new Push(ESI));
		a->add_instruction(new Push(EBX));

		a->add_instruction(new Push(EBP));

		a->add_instruction(new Mov(EBX, ESP));
		a->add_instruction(new Add(EBX, 20));

		// The following code stores every parameter as a dynamic variable.
		// The type system is broken here because we don't know the types of the actual
		// parameters. Hence, we just store dynamic ints for now which in effect
		// means that you can only use ints as parameters presently.
		for(int i = 0; i != boost::function_traits<Signature>::arity; ++i)
		{
			int_d* parameter = new int_d;

			a->add_instruction(new MovIndex(EAX, EBX));
			parameter->from_EAX();
			a->add_instruction(new Add(EBX, 4));

			// Parameters are pushed on the stack in reverse order.
			param_.push_back(parameter);
		}
	}
	
	virtual ~functor_base() {}

	void if_d(bool_d& condition)
	{
		assembler* a = g_environment.get_assembler();

		a->add_instruction(new Mov(EAX, condition.get_ptr()));
		a->add_instruction(new Cmp(EAX, i32(0)));

		std::string else_label = g_environment.create_if_scope();
		a->add_instruction(new Jmp(else_label, EQUAL));
	}

	void else_d()
	{
		std::string endif_label = g_environment.create_else_scope();
	}


	void return_d(ResultType i)
	{
		assembler* a = g_environment.get_assembler();
		a->add_instruction(new Mov(EAX, i32(i)));

		call_return();
	}
	
	void return_d(variable_d<ResultType>& var)
	{
		assembler* a = g_environment.get_assembler();
		a->add_instruction(new Mov(EAX, var.get_ptr()));

		call_return();
	}

private:
	assembler asm_;

	void call_return()
	{
		assembler* a = g_environment.get_assembler();

		a->add_instruction(new Pop(EBP));
		a->add_instruction(new Pop(EBX));
		a->add_instruction(new Pop(ESI));
		a->add_instruction(new Pop(EDI));
		a->add_instruction(new Ret());		
	}
};
//[cf]
//[of]:class functor
template<typename Signature>
class functor : public boost::function<Signature>
{
public:
	functor(functor_base<Signature>& base)
	{
		*((boost::function<Signature>*)this) = base.get_function_get_ptr();
	}
};

//[cf]
//[c]
//[of]:footer
}	// namespace rtc
//[cf]
