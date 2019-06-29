#include "Float32_2.h"


//	-----------------------------------------------------------
//		constructors
//	-----------------------------------------------------------
ParabellumUtils::Float32_2::Float32_2()
{
}


ParabellumUtils::Float32_2::Float32_2(EFLOAT32 px, EFLOAT32 py)
	: X(px), Y(py)
{
}

ParabellumUtils::Float32_2::Float32_2(const Float32_2& copy)
	: X(copy.X), Y(copy.Y)
{
}

//	-----------------------------------------------------------
//		Methods
//	-----------------------------------------------------------



ParabellumUtils::EFLOAT32
ParabellumUtils::Float32_2::Length()
{
	return ParabellumUtils::Math::FastSqrt(X*X + Y*Y);
}

void
ParabellumUtils::Float32_2::Normalize()
{
	EFLOAT32 ilen = 1.0f / Length();

	X *= ilen;
	Y *= ilen;
}

void
ParabellumUtils::Float32_2::Normalize(Float32_2* v1)
{
	v1->Normalize();
}



//	-----------------------------------------------------------
//		Gets and sets
//	-----------------------------------------------------------

void
ParabellumUtils::Float32_2::Set(EFLOAT32 px, EFLOAT32 py)
{
	X = px;
	Y = py;
}

void
ParabellumUtils::Float32_2::Set(const Float32_2& vec)
{
	X = vec.X;
	Y = vec.Y;
}


//	-----------------------------------------------------------
//		operators
//	-----------------------------------------------------------

ParabellumUtils::Float32_2&
ParabellumUtils::Float32_2::operator=(Float32_2& rhs)
{
	X = rhs.X;
	Y = rhs.Y;

	return *this;
}

