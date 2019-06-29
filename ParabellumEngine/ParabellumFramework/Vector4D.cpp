#include "stdafx.h"
#include "Vector4D.h"



//	-----------------------------------------------------------
//		constructors
//	-----------------------------------------------------------
ParabellumFramework::Vector4D::Vector4D()
{
}


ParabellumFramework::Vector4D::Vector4D(EFLOAT32 px, EFLOAT32 py, EFLOAT32 pz, EFLOAT32 pw)
	: X(px), Y(py), Z(pz), W(pw)
{
}


ParabellumFramework::Vector4D::Vector4D(const Vector4D& copy)
	: X(copy.X), Y(copy.Y), Z(copy.Z), W(copy.W)
{
}

ParabellumFramework::Vector4D::~Vector4D()
{
}

//	-----------------------------------------------------------
//		Methods
//	-----------------------------------------------------------


ParabellumFramework::EFLOAT32
ParabellumFramework::Vector4D::Sum()
{
	return (X + Y + Z + W);
}


ParabellumFramework::EFLOAT32
ParabellumFramework::Vector4D::Length()
{
	return ParabellumUtils::Math::FastSqrt(X*X + Y*Y + Z*Z);
}

void
ParabellumFramework::Vector4D::Normalize()
{
	EFLOAT32 length = Length();

	X /= length;
	Y /= length;
	Z /= length;
	W /= length;
}



ParabellumFramework::EFLOAT32 
ParabellumFramework::Vector4D::Angle(Vector4D* v1, Vector4D* v2)
{
	return Dot(v1, v2) / (v1->Length() * v2->Length());
}

ParabellumFramework::EFLOAT32 
ParabellumFramework::Vector4D::Dot(_IN_ Vector4D* v1, _IN_ Vector4D* v2)
{
	return (v1->X * v2->X) + (v1->Y * v2->Y) + (v1->Z * v2->Z);
}

void
ParabellumFramework::Vector4D::Cross(_IN_ Vector4D* in1, _IN_ Vector4D* in2)
{
	float x = in1->Y * in2->Z - in2->Y * in1->Z;
	float y = in1->Z * in2->X - in2->Z * in1->X;
	float z = in1->X * in2->Y - in2->X * in1->Y;

	this->X = x;
	this->Y = y;
	this->Z = z;
}

void
ParabellumFramework::Vector4D::Cross(_IN_ Vector4D* in1, _IN_ Vector4D* in2, _OUT_ Vector4D* out)
{
	float x = in1->Y * in2->Z - in2->Y * in1->Z;
	float y = in1->Z * in2->X - in2->Z * in1->X;
	float z = in1->X * in2->Y - in2->X * in1->Y;

	out->X = x;
	out->Y = y;
	out->Z = z;
}

void
ParabellumFramework::Vector4D::Min(_IN_ Vector4D* in1, _IN_ Vector4D* in2, _OUT_ Vector4D* out)
{
	#ifdef PF_MATH_SIMD 
		out->m128 = _mm_min_ps(in1->m128, in2->m128);
	#else
		out->m128 = _mm_min_ps(in1->m128, in2->m128);
	#endif
}

void
ParabellumFramework::Vector4D::Max(_IN_ Vector4D* in1, _IN_ Vector4D* in2, _OUT_ Vector4D* out)
{
	#ifdef PF_MATH_SIMD 
		out->m128 = _mm_max_ps(in1->m128, in2->m128);
	#else
		out->m128 = _mm_max_ps(in1->m128, in2->m128);
	#endif
}


ParabellumFramework::Vector4D*
ParabellumFramework::Vector4D::CreateVector4D(EFLOAT32 px, EFLOAT32 py, EFLOAT32 pz, EFLOAT32 pw)
{
	return new Vector4D(px, py, pz, pw);
}




//	-----------------------------------------------------------
//		Common Vectors
//	-----------------------------------------------------------

void
ParabellumFramework::Vector4D::MakeOne()
{
	X = 1.0f,
	Y = 1.0f;
	Z = 1.0f;
	W = 1.0f;
}

void
ParabellumFramework::Vector4D::MakeZero()
{
	X = 0.0f,
	Y = 0.0f;
	Z = 0.0f;
	W = 1.0f;
}

void
ParabellumFramework::Vector4D::MakeUp()
{
	X = 0.0f,
	Y = 1.0f;
	Z = 0.0f;
	W = 1.0f;
}

void
ParabellumFramework::Vector4D::MakeDown()
{
	X = 0.0f,
	Y = -1.0f;
	Z = 0.0f;
	W = 1.0f;
}

void
ParabellumFramework::Vector4D::MakeLeft()
{
	X = -1.0f,
	Y = 0.0f;
	Z = 0.0f;
	W = 1.0f;
}

void
ParabellumFramework::Vector4D::MakeRight()
{
	X = +1.0f,
	Y = 0.0f;
	Z = 0.0f;
	W = 1.0f;
}

void
ParabellumFramework::Vector4D::MakeBackward()
{
	X = 0.0f,
	Y = 0.0f;
	Z = -1.0f;
	W = 1.0f;
}

void
ParabellumFramework::Vector4D::MakeForward()
{
	X = 0.0f,
	Y = 0.0f;
	Z = 1.0f;
	W = 1.0f;
}

void
ParabellumFramework::Vector4D::MakeXUnit()
{
	X = 1.0f,
	Y = 0.0f;
	Z = 0.0f;
	W = 1.0f;
}

void
ParabellumFramework::Vector4D::MakeYUnit()
{
	X = 0.0f,
	Y = 1.0f;
	Z = 0.0f;
	W = 1.0f;
}

void
ParabellumFramework::Vector4D::MakeZUnit()
{
	X = 0.0f,
	Y = 0.0f;
	Z = 1.0f;
	W = 1.0f;
}


//	-----------------------------------------------------------
//		Gets and Sets
//	-----------------------------------------------------------


void
ParabellumFramework::Vector4D::Set(EFLOAT32 px, EFLOAT32 py, EFLOAT32 pz, EFLOAT32 pw)
{
	X = px;
	Y = py;
	Z = pz;
	W = pw;
}


void
ParabellumFramework::Vector4D::Set(const Vector2D& src, EFLOAT32 pz, EFLOAT32 pw)
{
	this->X = src.X;
	this->Y = src.Y;
	this->Z = pz;
	this->W = pw;
}

void ParabellumFramework::Vector4D::Set(EFLOAT32 px, const Vector2D& src, EFLOAT32 pw)
{
	this->X = px;
	this->Y = src.X;
	this->Z = src.Y;
	this->W = pw;
}

void ParabellumFramework::Vector4D::Set(EFLOAT32 px, EFLOAT32 py, const Vector2D& src)
{
	this->X = px;
	this->Y = py;
	this->Z = src.X;
	this->W = src.Y;
}


void ParabellumFramework::Vector4D::Set(const Vector2D& src0, const Vector2D& src1)
{
	this->X = src0.X;
	this->Y = src0.Y;
	this->Z = src1.X;
	this->W = src1.Y;
}


void
ParabellumFramework::Vector4D::Set(const Vector3D& src, EFLOAT32 pw)
{
	this->X = src.X;
	this->Y = src.Y;
	this->Z = src.Z;
	this->W = pw;
}

void
ParabellumFramework::Vector4D::Set(EFLOAT32 px, const Vector3D& src)
{
	this->X = px;
	this->Y = src.X;
	this->Z = src.Y;
	this->W = src.Z;
}

void
ParabellumFramework::Vector4D::Set(const Vector4D& src)
{
	memcpy(&this->DATA, &src.DATA, sizeof(DATA));
}


void
ParabellumFramework::Vector4D::Set(const Vector2D* src, EFLOAT32 pz, EFLOAT32 pw)
{
	this->X = src->X;
	this->Y = src->Y;
	this->Z = pz;
	this->W = pw;
}

void ParabellumFramework::Vector4D::Set(EFLOAT32 px, const Vector2D* src, EFLOAT32 pw)
{
	this->X = px;
	this->Y = src->X;
	this->Z = src->Y;
	this->W = pw;
}

void ParabellumFramework::Vector4D::Set(EFLOAT32 px, EFLOAT32 py, const Vector2D* src)
{
	this->X = px;
	this->Y = py;
	this->Z = src->X;
	this->W = src->Y;
}


void ParabellumFramework::Vector4D::Set(const Vector2D* src0, const Vector2D* src1)
{
	this->X = src0->X;
	this->Y = src0->Y;
	this->Z = src1->X;
	this->W = src1->Y;
}


void
ParabellumFramework::Vector4D::Set(const Vector3D* src, EFLOAT32 pw)
{
	this->X = src->X;
	this->Y = src->Y;
	this->Z = src->Z;
	this->W = pw;
}

void
ParabellumFramework::Vector4D::Set(EFLOAT32 px, const Vector3D* src)
{
	this->X = px;
	this->Y = src->X;
	this->Z = src->Y;
	this->W = src->Z;
}

void
ParabellumFramework::Vector4D::Set(const Vector4D* src)
{
	memcpy(&this->DATA, &src->DATA, sizeof(DATA));
}





//	------------------------------------------------------------------------------------
//		Global Vector4D functions
//	------------------------------------------------------------------------------------


ParabellumFramework::Vector4D 
PF_FASTCALL 
ParabellumFramework::V4DXUnit()
{
	return Vector4D(1.0f, 0.0f, 0.0f, 1.0f);
}

ParabellumFramework::Vector4D
PF_FASTCALL
ParabellumFramework::V4DYUnit()
{
	return Vector4D(0.0f, 1.0f, 0.0f, 1.0f);
}

ParabellumFramework::Vector4D
PF_FASTCALL
ParabellumFramework::V4DZUnit()
{
	return Vector4D(0.0f, 0.0f, 1.0f, 1.0f);
}