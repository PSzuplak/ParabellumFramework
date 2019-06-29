//****************************************************************************************************/
//
//	Filename : MathRaw.h
//
//	Descritpion : Provides math function for internal C++ types (int, float, double, etc.)
//
//					
//
//
//****************************************************************************************************/

#ifndef __PRAGMAONCE_PARABELLUMUTILS_MATH_H__
#define __PRAGMAONCE_PARABELLUMUTILS_MATH_H__

#include "DLLIE.h"
#include "Configuration.h"
#include "Convenctions.h"
#include "Fixes.h"
#include "Types.h"

#include <cmath>


//	----------------------------------------------------
//	Macros
//	----------------------------------------------------


#define PU_MATH_EPS			EFEPSILON

// because C++ standard does not allow to put floating point numbers into templates
// PF_MATH_EVAL is being used to convert float to int
#define PU_MATH_EVAL		1000000.0f

// how many steps 
#define PU_MATH_TRIGAPROX	8

// treat 'value' as type of 'type'
#define PU_MATH_AS(type, value) *((type*)(&value))

// return constant value of sinus
#define PU_MATH_SIN(rad) (ParabellumUtils::Math::ForceConst<ParabellumUtils::EUINT32, SinConstEval(rad, 12)>::value / PU_MATH_EVAL)

// return constant value of cosinus
#define PU_MATH_COS(rad) (ParabellumUtils::Math::ForceConst<ParabellumUtils::EUINT32, CosConstEval(rad, 12)>::value / PU_MATH_EVAL)

// return constant value of sinus
#define PU_MATH_SINCOS(rad) (PU_MATH_SIN(rad) * PU_MATH_COS(rad) )

// return constant value of tangens
#define PU_MATH_TAN(rad) (PU_MATH_SIN(rad) / PU_MATH_COS(rad) )

// return constant value of tangens
#define PU_MATH_COT(rad) (PU_MATH_COS(rad) / PU_MATH_SIN(rad) )







namespace ParabellumUtils
{

	namespace Math
	{

		const ParabellumUtils::EFLOAT32 PI_SQR = 9.86960440109f;
		const ParabellumUtils::EFLOAT32 PI2 = 6.28318530718f;
		const ParabellumUtils::EFLOAT32 PI = 3.14159265359f;
		const ParabellumUtils::EFLOAT32 PIO2 = 1.57079632679f;
		const ParabellumUtils::EFLOAT32 PIO3 = 1.0471975512f;
		const ParabellumUtils::EFLOAT32 PIO4 = 0.78539816339f;
		const ParabellumUtils::EFLOAT32 PIO180 = 0.01745329251f;	// PI/180
		const ParabellumUtils::EFLOAT32 F180OPI = 57.2957795131f;	// 180/PI


		PU_INLINE EFLOAT32 PU_FASTCALL AsFloat(EUINT32 value);
		PU_INLINE EFLOAT32 PU_FASTCALL AsFloat(EINT32 value);
		PU_INLINE EINT32 PU_FASTCALL AsInt(EFLOAT32 value);
		PU_INLINE EUINT32 PU_FASTCALL AsUInt(EFLOAT32 value);

		// TOutput and 
		template<typename TOutput, typename TInput>
		PU_INLINE XYZ_API TOutput PU_FASTCALL
			As(TInput value)
		{
			return *((TOutput*)(&value));
		}

		PU_INLINE XYZ_API EFLOAT32 Abs(EFLOAT32 value);

		template<typename T>
		PU_INLINE XYZ_API T Clamp(T in, T min, T max)
		{
			if (in < min)
				return min;
			else if (in > max)
				return max;

			return in;
		}



		//	------------------------------------------------------------------------------------
		//		trygonometric stuff
		//	------------------------------------------------------------------------------------

		constexpr EUINT32 Factorial(EUINT32 value)
		{
			return ((value <= 1) ? 1 : (value * Factorial(value - 1)));
		}


		constexpr EFLOAT32 Pow(EFLOAT32 value, EUINT32 exponent, EFLOAT32 sum)
		{
			return (exponent ? Pow(value, exponent - 1, sum * value) : sum);
		}

		constexpr EFLOAT32 Pow(EFLOAT32 value, EUINT32 exponent)
		{
			return Pow(value, exponent, 1);
		}


		constexpr EFLOAT32 SinConst(EFLOAT32 x, EINT32 n)
		{
			return (n < 1 ? 0 : (Math::Pow(-1, n + 1) * Math::Pow(x, 2 * n - 1) / Math::Factorial(2 * n - 1)) + SinConst(x, n - 1));
		}

		constexpr EUINT32 SinConstEval(EFLOAT32 x, EINT32 n)
		{
			return (EUINT32)(SinConst(x, n) * PU_MATH_EVAL);
		}

		constexpr EFLOAT32 CosConst(EFLOAT32 x, EINT32 n)
		{
			return (n < 1 ? 1 : (Math::Pow(-1, n) * Math::Pow(x, 2 * n) / Math::Factorial(2 * n)) + CosConst(x, n - 1));
		}

		constexpr EUINT32 CosConstEval(EFLOAT32 x, EINT32 n)
		{
			return (EUINT32)(CosConst(x, n) * PU_MATH_EVAL);
		}




		// return 'end' if 'value' is lower tan 'middle',
		// return '(end - value) / middle' if value is bigger than 'middle'
		PU_INLINE XYZ_API  EFLOAT32 Fit(EFLOAT32 value, EFLOAT32 middle, EFLOAT32 end);

		PU_INLINE XYZ_API  EFLOAT32 FastSqrt(EFLOAT32 value);
		PU_INLINE XYZ_API  EFLOAT32 FastInvSqrt(EFLOAT32 value);

		PU_INLINE XYZ_API  EFLOAT32 FastAtan(EFLOAT32 angle);
		PU_INLINE XYZ_API  EFLOAT32 FastTan(EFLOAT32 angle);
		PU_INLINE XYZ_API  EFLOAT32 FastCot(EFLOAT32 angle);
		PU_INLINE XYZ_API  EFLOAT32 FastSin(EFLOAT32 angle);
		PU_INLINE XYZ_API  EFLOAT32 FastCos(EFLOAT32 angle);

		PU_INLINE XYZ_API  EDOUBLE FastAtan(EDOUBLE angle);
		PU_INLINE XYZ_API  EDOUBLE FastTan(EDOUBLE angle);
		PU_INLINE XYZ_API  EDOUBLE FastCot(EDOUBLE angle);
		PU_INLINE XYZ_API  EDOUBLE FastSin(EDOUBLE angle);
		PU_INLINE XYZ_API  EDOUBLE FastCos(EDOUBLE angle);

		PU_INLINE XYZ_API  EFLOAT32 ToRadians(EFLOAT32 angle);
		PU_INLINE XYZ_API  EDOUBLE ToRadians(EDOUBLE angle);

		PU_INLINE XYZ_API  EFLOAT32 ToDegrees(EFLOAT32 angle);
		PU_INLINE XYZ_API  EDOUBLE ToDegrees(EDOUBLE angle);



		//
		// Searching min/max
		//



		// min for 2 values
		template < typename T>
		PU_INLINE  T Min(T in0, T in1)
		{
			if (in0 < in1)
				return in0;
			else
				return in1;
		}

		// max for 2 values
		template < typename T>
		PU_INLINE  T Max(T in0, T in1)
		{
			if (in0 > in1)
				return in0;
			else
				return in1;
		}

		// min for 3 values
		template < typename T>
		PU_INLINE  T Min(T in0, T in1, T in2)
		{
			T val;
			val = Min(in0, in1);
			return Min(val, in2);
		}

		// max for 3 values
		template < typename T>
		PU_INLINE  T Max(T in0, T in1, T in2)
		{
			T val;
			val = Max(in0, in1);
			return Max(val, in2);
		}


		PU_INLINE XYZ_API EINT32 RandI();
		PU_INLINE XYZ_API EINT32 RandI(EINT32 min, EINT32 max);
		PU_INLINE XYZ_API EFLOAT32 RandF();
		PU_INLINE XYZ_API EFLOAT32 RandF(EFLOAT32 min, EFLOAT32 max);
		PU_INLINE XYZ_API EDOUBLE RandD();
		PU_INLINE XYZ_API EDOUBLE RandD(EDOUBLE min, EDOUBLE max);



	}

}



#endif