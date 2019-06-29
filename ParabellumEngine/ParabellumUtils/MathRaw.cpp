#include "MathRaw.h"


//	------------------------------------------------------------------------------------------------
ParabellumUtils::EFLOAT32
PU_FASTCALL
ParabellumUtils::Math::AsFloat(EUINT32 value)
{
	return *((EFLOAT32*)(&value));
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EFLOAT32
PU_FASTCALL
ParabellumUtils::Math::AsFloat(EINT32 value)
{
	return *((EFLOAT32*)(&value));
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EINT32
PU_FASTCALL
ParabellumUtils::Math::AsInt(EFLOAT32 value)
{
	return *((EINT32*)(&value));
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EUINT32
PU_FASTCALL
ParabellumUtils::Math::AsUInt(EFLOAT32 value)
{
	return *((EUINT32*)(&value));
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EFLOAT32
ParabellumUtils::Math::Abs(EFLOAT32 value)
{
	return abs(value);
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EFLOAT32
ParabellumUtils::Math::Fit(EFLOAT32 value, EFLOAT32 middle, EFLOAT32 end)
{
	if (value < middle)
		return end;
	return (end - value) / middle;
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EFLOAT32
ParabellumUtils::Math::FastSqrt(EFLOAT32 value)
{
	return sqrt(value);
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EFLOAT32
ParabellumUtils::Math::FastInvSqrt(EFLOAT32 number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long *)&y;                       // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);               // what the fuck?
	y = *(float *)&i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration
										   //y  = y * ( threehalfs - ( x2 * y * y ) ); 

	return y;
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EFLOAT32
ParabellumUtils::Math::FastAtan(EFLOAT32 angle)
{
	return atan(angle);
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EFLOAT32
ParabellumUtils::Math::FastTan(EFLOAT32 angle)
{
	return tan(angle);
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EFLOAT32
ParabellumUtils::Math::FastCot(EFLOAT32 angle)
{
	return cos(angle) / sin(angle);
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EFLOAT32
ParabellumUtils::Math::FastSin(EFLOAT32 angle)
{
	return sin(angle);
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EFLOAT32
ParabellumUtils::Math::FastCos(EFLOAT32 angle)
{
	return cos(angle);
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EDOUBLE
ParabellumUtils::Math::FastAtan(EDOUBLE angle)
{
	return atan(angle);
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EDOUBLE
ParabellumUtils::Math::FastTan(EDOUBLE angle)
{
	return tan(angle);
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EDOUBLE
ParabellumUtils::Math::FastCot(EDOUBLE angle)
{
	return cos(angle) / sin(angle);
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EDOUBLE
ParabellumUtils::Math::FastSin(EDOUBLE angle)
{
	return sin(angle);
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EDOUBLE
ParabellumUtils::Math::FastCos(EDOUBLE angle)
{
	return cos(angle);
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EFLOAT32
ParabellumUtils::Math::ToRadians(EFLOAT32 angle)
{
	return angle * PIO180;
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EDOUBLE
ParabellumUtils::Math::ToRadians(EDOUBLE angle)
{
	return angle * PIO180;
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EFLOAT32
ParabellumUtils::Math::ToDegrees(EFLOAT32 angle)
{
	return angle * F180OPI;
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EDOUBLE
ParabellumUtils::Math::ToDegrees(EDOUBLE angle)
{
	return angle * F180OPI;
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EINT32
ParabellumUtils::Math::RandI()
{
	return rand();
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EINT32
ParabellumUtils::Math::RandI(EINT32 min, EINT32 max)
{
	return RandI() % (max - min + 1) + min;
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EFLOAT32
ParabellumUtils::Math::RandF()
{
	return rand() / RAND_MAX;
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EFLOAT32
ParabellumUtils::Math::RandF(EFLOAT32 min, EFLOAT32 max)
{
	return min + (rand() / (RAND_MAX / (max - min)));
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EDOUBLE
ParabellumUtils::Math::RandD()
{
	EINT32 max = RandI();
	if (max == 0) max = 1;
	EINT32 val = RandI() % max;

	return (EDOUBLE)val / (EDOUBLE)max;
}
//	------------------------------------------------------------------------------------------------
ParabellumUtils::EDOUBLE
ParabellumUtils::Math::RandD(EDOUBLE min, EDOUBLE max)
{
	int cmin = (int)min;
	int cmax = (int)max;

	int restMin = (int)((min - cmin) * 100000);
	int restMax = (int)((max - cmax) * 100000);

	int randM = RandI(cmin, cmax);
	int randR = RandI(restMin, restMax);

	return ((float)randM + (float)randR / 100000.0f);
}
//	------------------------------------------------------------------------------------------------