#include "stdafx.h"
#include "Vector3D.h"


//	-----------------------------------------------------------
//		Static common vectors
//	-----------------------------------------------------------
const ParabellumFramework::Vector3D& ParabellumFramework::Vector3D::Up()
{
	static Vector3D v3D(0.0, 1.0, 0.0);
	return v3D;
}

const ParabellumFramework::Vector3D& ParabellumFramework::Vector3D::Down()
{
	static Vector3D v3D(0.0, -1.0, 0.0);
	return v3D;
}
const ParabellumFramework::Vector3D& ParabellumFramework::Vector3D::Left()
{
	static Vector3D v3D(-1.0, 0.0, 0.0);
	return v3D;
}
const ParabellumFramework::Vector3D& ParabellumFramework::Vector3D::Right()
{
	static Vector3D v3D(1.0, 0.0, 0.0);
	return v3D;
}
const ParabellumFramework::Vector3D& ParabellumFramework::Vector3D::Forward()
{
	static Vector3D v3D(0.0, 0.0, 1.0);
	return v3D;
}
const ParabellumFramework::Vector3D& ParabellumFramework::Vector3D::Backward()
{
	static Vector3D v3D(0.0, 0.0, -1.0);
	return v3D;
}
const ParabellumFramework::Vector3D& ParabellumFramework::Vector3D::Zero()
{
	static Vector3D v3D(0.0, 0.0, 0.0);
	return v3D;
}
const ParabellumFramework::Vector3D& ParabellumFramework::Vector3D::UnitX()
{
	static Vector3D v3D(1.0, 0.0, 0.0);
	return v3D;
}
const ParabellumFramework::Vector3D& ParabellumFramework::Vector3D::UnitY()
{
	static Vector3D v3D(0.0, 1.0, 0.0);
	return v3D;
}

const ParabellumFramework::Vector3D& ParabellumFramework::Vector3D::UnitZ()
{
	static Vector3D v3D(0.0, 0.0, 1.0);
	return v3D;
}


//	-----------------------------------------------------------
//		constructors
//	-----------------------------------------------------------
ParabellumFramework::Vector3D::Vector3D()
{
}


ParabellumFramework::Vector3D::Vector3D(EFLOAT32 px, EFLOAT32 py, EFLOAT32 pz)
	: X(px), Y(py), Z(pz)
{
}

ParabellumFramework::Vector3D::Vector3D(const Vector3D& copy)
	: X(copy.X), Y(copy.Y), Z(copy.Z)
{
}


ParabellumFramework::Vector3D::~Vector3D()
{
}




//	-----------------------------------------------------------
//		Methods
//	-----------------------------------------------------------

void 
ParabellumFramework::Vector3D::Set(EFLOAT32 px, EFLOAT32 py, EFLOAT32 pz)
{
	X = px;
	Y = py;
	Z = pz;
}

void 
ParabellumFramework::Vector3D::Set(const Vector3D& src)
{
	memcpy(&this->DATA, &src.DATA, sizeof(DATA));
}


ParabellumFramework::EFLOAT32
ParabellumFramework::Vector3D::Length()
{
	return ParabellumUtils::Math::FastSqrt(X*X + Y*Y + Z*Z);
}


ParabellumFramework::EFLOAT32
ParabellumFramework::Vector3D::Length(_IN_ Vector3D& vec)
{
	return ParabellumUtils::Math::FastSqrt(vec.X*vec.X + vec.Y*vec.Y + vec.Z*vec.Z);
}

ParabellumFramework::EFLOAT32
ParabellumFramework::Vector3D::Distance(_IN_ Vector3D* vec1, _IN_ Vector3D* vec2)
{
	Vector3D sub;
	sub = (*vec1) - (*vec2);

	return sub.Length();
}


ParabellumFramework::EFLOAT32
ParabellumFramework::Vector3D::SquareLength()
{
	return (X*X + Y*Y + Z*Z);
}

ParabellumFramework::EFLOAT32
ParabellumFramework::Vector3D::SquareLength(_IN_ Vector3D& vec)
{
	return (vec.X*vec.X + vec.Y*vec.Y + vec.Z*vec.Z);
}


void
ParabellumFramework::Vector3D::Normalize()
{
	EFLOAT32 ilen = 1.0f / Length();

	X *= ilen;
	Y *= ilen;
	Z *= ilen;
}



ParabellumFramework::EFLOAT32
ParabellumFramework::Vector3D::Angle(Vector3D v1, Vector3D v2)
{
	v1.Normalize();
	v2.Normalize();

	return Dot(&v1, &v2);
}

ParabellumFramework::EFLOAT32
ParabellumFramework::Vector3D::Dot(_IN_ Vector3D* v1, _IN_ Vector3D* v2)
{
	return (v1->X * v2->X) + (v1->Y * v2->Y) + (v1->Z * v2->Z);
}



void
ParabellumFramework::Vector3D::Cross(_OUT_ Vector3D* out, _IN_ Vector3D* in1, _IN_ Vector3D* in2)
{
	float x = in1->Y * in2->Z - in2->Y * in1->Z;
	float y = in1->Z * in2->X - in2->Z * in1->X;
	float z = in1->X * in2->Y - in2->X * in1->Y;

	out->X = x;
	out->Y = y;
	out->Z = z;
}


void
ParabellumFramework::Vector3D::Reflect(_OUT_ Vector3D* pOut, _IN_ Vector3D* pIn, _IN_ Vector3D* pNormal)
{
	*pOut = *pNormal;
	*pOut *= Vector3D::Dot(pIn, pNormal) * 2.0f;
	*pOut -= *pIn;
}


void
ParabellumFramework::Vector3D::RotateX(EFLOAT32 angle)
{
	EFLOAT32 y, z;

	y = Y;
	z = Z;

	Y = y*ParabellumUtils::Math::FastCos(angle) - z*ParabellumUtils::Math::FastSin(angle);
	Z = y*ParabellumUtils::Math::FastSin(angle) + z*ParabellumUtils::Math::FastCos(angle);
}

void
ParabellumFramework::Vector3D::RotateY(EFLOAT32 angle)
{
	EFLOAT32 x, z;

	x = X;
	z = Z;

	z = z*ParabellumUtils::Math::FastCos(angle) - x*ParabellumUtils::Math::FastSin(angle);
	x = z*ParabellumUtils::Math::FastSin(angle) + x*ParabellumUtils::Math::FastCos(angle);
}

void
ParabellumFramework::Vector3D::RotateZ(EFLOAT32 angle)
{
	EFLOAT32 x, y;

	x = X;
	y = Y;

	X = x*ParabellumUtils::Math::FastCos(angle) - y*ParabellumUtils::Math::FastSin(angle);
	Y = x*ParabellumUtils::Math::FastSin(angle) + y*ParabellumUtils::Math::FastCos(angle);

}


ParabellumFramework::EFLOAT32
ParabellumFramework::Vector3D::GetMax()
{
	if (X > Y)
		if (X > Z)
			return X;
		else
			return Z;
	else
		if (Y > Z)
			return Y;
		else
			return Z;

	return 1.0f;
}


ParabellumFramework::EFLOAT32
ParabellumFramework::Vector3D::GetMin()
{
	if (X < Y)
		if (X < Z)
			return X;
		else
			return Z;
	else
		if (Y < Z)
			return Y;
		else
			return Z;

	return 1.0f;
}

//
//
//
//
//
void
ParabellumFramework::Vector3D::Get(EFLOAT32& px, EFLOAT32& py, EFLOAT32& pz)
{
	px = X;
	py = Y;
	pz = Z;
}

//
//
//
//
//
void
ParabellumFramework::Vector3D::CopyTo(EFLOAT32& x, EFLOAT32& y, EFLOAT32& z)
{
	x = X;
	y = Y;
	z = Z;
}


//
//
//
//
//
void
ParabellumFramework::Vector3D::CastToInt()
{
	X = (EFLOAT32)((int)X);
	Y = (EFLOAT32)((int)Y);
	Z = (EFLOAT32)((int)Z); 
}


//
//
//
//
//
void
ParabellumFramework::Vector3D::Trunc()
{
	X = std::trunc(100 * X) * 0.01f;
	Y = std::trunc(100 * Y) * 0.01f;
	Z = std::trunc(100 * Z) * 0.01f;
}

//	-----------------------------------------------------------
//		Common Vectors
//	-----------------------------------------------------------

void
ParabellumFramework::Vector3D::MakeOne()
{
	X = 1.0f,
	Y = 1.0f;
	Z = 1.0f;
}

void
ParabellumFramework::Vector3D::MakeZero()
{
	X = 0.0f,
	Y = 0.0f;
	Z = 0.0f;
}

void
ParabellumFramework::Vector3D::MakeUp()
{
	X = 0.0f,
	Y = 1.0f;
	Z = 0.0f;
}

void
ParabellumFramework::Vector3D::MakeDown()
{
	X = 0.0f,
	Y = -1.0f;
	Z = 0.0f;
}

void
ParabellumFramework::Vector3D::MakeLeft()
{
	X = -1.0f,
	Y = 0.0f;
	Z = 0.0f;
}

void
ParabellumFramework::Vector3D::MakeRight()
{
	X = +1.0f,
	Y = 0.0f;
	Z = 0.0f;
}

void
ParabellumFramework::Vector3D::MakeBackward()
{
	X = 0.0f,
	Y = 0.0f;
	Z = -1.0f;
}

void
ParabellumFramework::Vector3D::MakeForward()
{
	X = 0.0f,
	Y = 0.0f;
	Z = 1.0f;
}

void
ParabellumFramework::Vector3D::MakeXUnit()
{
	X = 1.0f,
	Y = 0.0f;
	Z = 0.0f;
}

void
ParabellumFramework::Vector3D::MakeYUnit()
{
	X = 0.0f,
	Y = 1.0f;
	Z = 0.0f;
}

void
ParabellumFramework::Vector3D::MakeZUnit()
{
	X = 0.0f,
	Y = 0.0f;
	Z = 1.0f;
}



std::string ParabellumFramework::Vector3D::ToString()
{
	std::string str;
	str = std::to_string(X) + "," + std::to_string(Y) + "," + std::to_string(Z);
	return str;
}


//	-----------------------------------------------------------
//		operators
//	-----------------------------------------------------------

void 
ParabellumFramework::Vector3D::operator=(const Vector3D& rhs)
{
	this->X = rhs.X;
	this->Y = rhs.Y;
	this->Z = rhs.Z;
}

void 
ParabellumFramework::Vector3D::operator=(EFLOAT32 rhs)
{
	this->X = rhs;
	this->Y = rhs;
	this->Z = rhs;
}

void ParabellumFramework::Vector3D::operator +=(const Vector3D& rhs)
{
	this->X += rhs.X;
	this->Y += rhs.Y;
	this->Z += rhs.Z;
}


void 
ParabellumFramework::Vector3D::operator +=(const EFLOAT32 rhs)
{
	this->X += rhs;
	this->Y += rhs;
	this->Z += rhs;
}

void 
ParabellumFramework::Vector3D::operator -=(const Vector3D& rhs)
{
	this->X -= rhs.X;
	this->Y -= rhs.Y;
	this->Z -= rhs.Z;
}


void 
ParabellumFramework::Vector3D::operator -=(const EFLOAT32 rhs)
{
	this->X -= rhs;
	this->Y -= rhs;
	this->Z -= rhs;
}


void ParabellumFramework::Vector3D::operator *=(const EFLOAT32 rhs)
{
	this->X *= rhs;
	this->Y *= rhs;
	this->Z *= rhs;
}

void 
ParabellumFramework::Vector3D::operator *=(const Vector3D& rhs)
{
	this->X *= rhs.X;
	this->Y *= rhs.Y;
	this->Z *= rhs.Z;
}

void 
ParabellumFramework::Vector3D::operator /=(const EFLOAT32 rhs)
{
	this->X /= rhs;
	this->Y /= rhs;
	this->Z /= rhs;
}