//****************************************************************************************************/
//
//	Filename : Float32_2.h
//
//	Descritpion : 2 dimension vector of 32bit floats
//
//					
//
//
//****************************************************************************************************/

#ifndef __PRAGMAONCE_PARABELLUMUTILS_FLOAT32_2_H__
#define __PRAGMAONCE_PARABELLUMUTILS_FLOAT32_2_H__

#include "Convenctions.h"
#include "Configuration.h"
#include "Fixes.h"
#include "Types.h"
#include "MathRaw.h"


namespace ParabellumUtils
{

	// 4 dimension vector of 32bit floats
	class Float32_2
	{
	public:

		union
		{
			struct
			{
				EFLOAT32 X;
				EFLOAT32 Y;
			};

			// 16 Bytes variable - for shaders parametrs purpose
			struct
			{
				EFLOAT32 data[2];
			} DATA;

		};


		Float32_2();
		Float32_2(EFLOAT32 px, EFLOAT32 py);
		Float32_2(const Float32_2& copy);

		void Set(EFLOAT32 px, EFLOAT32 py);
		void Set(const Float32_2& vec);

		EFLOAT32 Length();

		void Normalize();
		static void Normalize(Float32_2* v1);


	public:

		PU_INLINE Float32_2& operator=(Float32_2& rhs);


		PU_INLINE void operator +=(Float32_2& rhs)
		{
			this->X += rhs.X;
			this->Y += rhs.Y;
		}

		PU_INLINE void operator +=(EFLOAT32 rhs)
		{
			this->X += rhs;
			this->Y += rhs;
		}

		PU_INLINE void operator -=(Float32_2& rhs)
		{
			this->X -= rhs.X;
			this->Y -= rhs.Y;
		}

		PU_INLINE void operator -=(EFLOAT32 rhs)
		{
			this->X -= rhs;
			this->Y -= rhs;
		}

		PU_INLINE void operator *=(Float32_2& rhs)
		{
			this->X *= rhs.X;
			this->Y *= rhs.Y;
		}

		PU_INLINE void operator *=(EFLOAT32 rhs)
		{
			this->X *= rhs;
			this->Y *= rhs;
		}

		//
		// Operators left-right
		//
		PU_INLINE friend Float32_2 operator+(Float32_2& lhs, Float32_2& rhs)
		{
			return Float32_2(lhs.X + rhs.X, lhs.Y + rhs.Y);
		}

		PU_INLINE friend Float32_2 operator-(Float32_2& lhs, Float32_2& rhs)
		{
			return Float32_2(lhs.X - rhs.X, lhs.Y - rhs.Y);
		}

		PU_INLINE friend Float32_2 operator*(Float32_2& lhs, Float32_2& rhs)
		{
			return Float32_2(lhs.X * rhs.X, lhs.Y * rhs.Y);
		}

		PU_INLINE friend Float32_2 operator*(Float32_2& lhs, EFLOAT32 rhs)
		{
			return Float32_2(lhs.X * rhs, lhs.Y * rhs);
		}

		PU_INLINE friend Float32_2 operator/(Float32_2& lhs, EFLOAT32 rhs)
		{
			return Float32_2(lhs.X / rhs, lhs.Y / rhs);
		}


	};

}



#endif