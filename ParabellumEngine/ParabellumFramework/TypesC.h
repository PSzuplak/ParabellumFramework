#ifndef _TYPESC_H_
#define _TYPESC_H_

#include "SystemSettings.h"
#include "FlagHelper.h"
#include "../ParabellumUtils/Fixes.h"

// shortcut for PArabellumFramework namespace
#define PFNS	ParabellumFramework


namespace ParabellumFramework
{

	typedef void EVOID;

	typedef bool EBOOL;

	typedef float EFLOAT32;
	typedef EFLOAT32 EFLOAT;

	typedef double EDOUBLE;
	typedef double EFLOAT64;

	typedef unsigned char EBYTE;

	typedef unsigned int EUINT;
	typedef unsigned char EUINT8;
	typedef short int EINT16;
	typedef int EINT;
	typedef int EINT32;
	typedef unsigned short int EUINT16;
	typedef unsigned int EUINT32;
	typedef unsigned long long EUINT64;

	typedef signed char ESINT8;
	typedef signed short int ESINT16;
	typedef signed int ESINT32;
	typedef signed int ESINT32;
	typedef signed long long ESINT64;

	// flags




	// 8 bit strings
	typedef char* ESTR;
	typedef const char* ECSTR;

	// pointers
	typedef void* EADDR32;			// 32 bit adress
	typedef void* EADDR64;			// 64 bit address
	typedef EADDR64 EADDR;
	typedef EADDR64 EPTR;
	typedef EUINT64 EPTRADDRESS;	// integer which size matches to a pointer

	// size of pointer
	typedef EUINT32 EADDRSIZE;


	// 128 bit Data - basic type must be a basic type of C++, because there are specializations for them
	template<typename BasicType>
	struct EVAR128
	{
		union
		{
			char data[16];
		};
	};

	template<>
	struct EVAR128<int>
	{
		union
		{
			char data[16];
		};

		EVAR128<int>& operator+(EVAR128<int>& rhs)
		{
			return *this;
		}
	};




	//	Force structs - force constrexp to be one ForceUINT64<Foo(4)>::value
	//	do not know why but assembly sometimes shows that there is a jump to conexpr function...
	template<typename T, T constValue>
	struct ForceConst
	{
		enum : T
		{
			value = constValue;
		};
	};



}

#endif