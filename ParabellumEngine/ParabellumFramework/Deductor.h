#ifndef _DEDUCTOR_H_
#define _DEDUCTOR_H_

#include "DLLIE.h"


namespace ParabellumFramework
{
	//
	// Deductor helps to deduct the type of value returned by operator new
	// you can do : int* variable = Deductor() = new int();
	// and do something inside operator =
	//
	class XYZ_API Deductor
	{
	public:
		template<typename T>
		T* operator=(T* ptr)
		{
			return ptr;
		}
	};

}


#endif
