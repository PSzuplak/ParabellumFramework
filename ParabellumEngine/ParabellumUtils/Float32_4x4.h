//****************************************************************************************************/
//
//	Filename : Float32_4x4.h
//
//	Descritpion : defines a 4x4 matrix
//
//					
//
//
//****************************************************************************************************/

#ifndef __PRAGMAONCE_PARABELLUMUTILS_FLOAT32_4x4_H__
#define __PRAGMAONCE_PARABELLUMUTILS_FLOAT32_4x4_H__

#include "Configuration.h"
#include "Fixes.h"
#include "Types.h"

#include "Float32_4.h"

namespace ParabellumUtils
{

	class Float32_4x4
	{

		//
		//	Data
		//
		public:

			union
			{
				// 512bit DATA - for putting as shader parametr
				struct
				{
					EFLOAT32 m_fields[4][4];
				} DATA;


				// 1D array
				struct
				{
					EFLOAT32 m_matrix1D[16];
				};

				struct
				{
					EFLOAT32 m_matrix[4][4];
				};

				// for SIMD
				#ifdef PARABELLUMUTILS_CONFIGURATION_SSE
				struct
				{
					__m128 m1, m2, m3, m4;
				};

				struct
				{
					__m128 m_row[4];
				};
				#endif

				//
				struct
				{
					EFLOAT32 m11, m12, m13, m14;
					EFLOAT32 m21, m22, m23, m24;
					EFLOAT32 m31, m32, m33, m34;
					EFLOAT32 m41, m42, m43, m44;
				};


				// Vector4D
				struct
				{
					Float32_4 row0;
					Float32_4 row1;
					Float32_4 row2;
					Float32_4 row3;
				};

			};

	};
}

#endif