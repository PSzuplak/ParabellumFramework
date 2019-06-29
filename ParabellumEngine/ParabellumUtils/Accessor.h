//****************************************************************************************************/
//
//	Filename : Accessor.h
//
//	Descritpion : 
//					Provides a special sufix interface to change the data in FloatN_M
//
//	Example :
//
//		Float32_3 sample1 = {1.0f, 3.0f, 6.0f};
//		Float32_3 sample2 = {20.0f, 40.0f, 80.0f};
//
//		sample2.yx = sample1.zx;
//
//		After this sample2 will store {20.0f, 1.0f, 80.0f}
//
//
//****************************************************************************************************/

#ifndef __PRAGMAONCE_PARABELLUMUTILS_ACCESSOR_H__
#define __PRAGMAONCE_PARABELLUMUTILS_ACCESSOR_H__

#include "Configuration.h"
#include "Fixes.h"
#include "Types.h"


namespace ParabellumUtils
{

	//
	//
	// num - number of pointers to store
	template<typename T, EUINT32 num>
	class Accessor
	{
		public:

			struct
			{
				_ARRAY_ T* data[num];
			};


		public:


		//
		// Operators
		//
		public:


		Accessor<T,num>& operator=(Accessor<T, num>& rhs)
		{
			for (EUINT32 i = 0; i < num; ++i)
			{
				*this->data[i] = *rhs.data[i];
			}

			return *this;
		}
	};


}


#define PU_ACCESSOR_DECLARE

#define PU_ACCESSOR_2_DECLARE( type, num, funcName, v0, v1) \
ParabellumUtils::Accessor<EFLOAT32, num> funcName()\
{\
	ParabellumUtils::Accessor<EFLOAT32, num> accessor;\
	accessor.data[0] = &v0;\
	accessor.data[1] = &v1;\
	return accessor;\
}

#define PU_ACCESSOR_3_DECLARE( type, num, funcName, v0, v1, v2) \
ParabellumUtils::Accessor<EFLOAT32, num> funcName()\
{\
	ParabellumUtils::Accessor<EFLOAT32, num> accessor;\
	accessor.data[0] = &v0;\
	accessor.data[1] = &v1;\
	accessor.data[2] = &v2;\
	return accessor;\
}


#define PU_ACCESSOR_4_DECLARE( type, num, funcName, v0, v1, v2, v3 ) \
ParabellumUtils::Accessor<EFLOAT32, num> funcName()\
{\
	ParabellumUtils::Accessor<EFLOAT32, num> accessor;\
	accessor.data[0] = &v0;\
	accessor.data[1] = &v1;\
	accessor.data[2] = &v2;\
	accessor.data[3] = &v3;\
	return accessor;\
}



#endif