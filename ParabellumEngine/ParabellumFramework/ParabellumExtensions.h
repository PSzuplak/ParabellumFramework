//	------------------------------------------------------------------------------------------------
//
//	This has some macros which may not be portable
//	
//	
//	------------------------------------------------------------------------------------------------

#ifndef _PARABELLUMEXTENSIONS_H_
#define _PARABELLUMEXTENSIONS_H_


#include <memory>
#include <utility>
#include <functional>

#include "DLLIE.h"
#include "TypesC.h"



namespace ParabellumFramework
{
	// ----------------------------------------------------------------------------------------
	template<typename T>
	EUINT64
	AddressAsInt(T& pObject)
	{
		union uPtr
		{
			EPTRADDRESS asUINT64;
			void* address;
		};

		uPtr u;
		u.address = (void*)&pObject;

		return u.asUINT64;
	}

	// ----------------------------------------------------------------------------------------
	template<typename ReturnType, typename ClassType, typename... Args>
	void*
		AddressOf(ReturnType(ClassType::*func)(Args...))
	{
		union uPtr
		{
			ReturnType(ClassType::*f)(Args...);
			void* address;
		};

		uPtr u;
		u.f = func;

		return u.address;
	}
	// ----------------------------------------------------------------------------------------
	template<typename ReturnType, typename... Args>
	void*
	AddressOf(ReturnType(*func)(Args...))
	{
		union uPtr
		{
			ReturnType(*f)(Args...);
			void* address;
		};

		uPtr u;
		u.f = func;

		return u.address;
	}
	// ----------------------------------------------------------------------------------------
	// cast any pointer
	// Example :
	//	double ReturnDouble_IntFloat(int y, float z);
	//	Caster<double(*)(int, float), int(*)(int)> caster;
	//	caster.input = &ReturnDouble_IntFloat;
	//	int(*f0)(int) = caster.output;
	//	calling f0(12) executes ReturnDouble_IntFloat(12) - args do not matches and will probably crash the stack
	template<typename InputType, typename OutputType>
	union Caster
	{
		InputType input;
		OutputType output;
	};

	// ----------------------------------------------------------------------------------------
	// Example : auto f = DeduceCast(&ReturnInt_Int, &ReturnFloat_Float) WHERE "int ReturnInt_Int(int y);"  "float ReturnFloat_Float(float y);"
	// 'f' will be type of 'float(*)(float)
	// calling 'f(66)' executes ReturnInt_Int(66) regardles
	//
	template<typename InputType, typename OutputType>
	OutputType
	DeduceCast(InputType input, OutputType output)
	{
		Caster<InputType, OutputType> caster;
		caster.input = input;
		return caster.output;
	}


}







#endif