#include "stdafx.h"
#include "Vector2D.h"


//	-----------------------------------------------------------
//		constructors
//	-----------------------------------------------------------
ParabellumFramework::Vector2D::Vector2D()
{
}


ParabellumFramework::Vector2D::Vector2D(EFLOAT32 px, EFLOAT32 py)
	: X(px), Y(py)
{
}

ParabellumFramework::Vector2D::Vector2D(const Vector2D& copy)
	: X(copy.X), Y(copy.Y)
{
}

//	-----------------------------------------------------------
//		Methods
//	-----------------------------------------------------------



ParabellumFramework::EFLOAT32
ParabellumFramework::Vector2D::Length()
{
	return ParabellumUtils::Math::FastSqrt(X*X + Y*Y);
}

void 
ParabellumFramework::Vector2D::Normalize()
{
	EFLOAT32 ilen = 1.0f / Length();

	X *= ilen;
	Y *= ilen;
}

void 
ParabellumFramework::Vector2D::Normalize(Vector2D* v1)
{
	v1->Normalize();
}


std::string ParabellumFramework::Vector2D::ToString()
{
	std::string str;
	str = std::to_string(X) + "," + std::to_string(Y);
	return str;
}




//	-----------------------------------------------------------
//		Gets and sets
//	-----------------------------------------------------------

void 
ParabellumFramework::Vector2D::Set(EFLOAT32 px, EFLOAT32 py)
{
	X = px;
	Y = py;
}

void 
ParabellumFramework::Vector2D::Set(const Vector2D& vec)
{
	X = vec.X;
	Y = vec.Y;
}


//	-----------------------------------------------------------
//		operators
//	-----------------------------------------------------------

void
ParabellumFramework::Vector2D::operator=(Vector2D& rhs)
{
	memcpy(&this->DATA, &rhs.DATA, sizeof(DATA));
}

