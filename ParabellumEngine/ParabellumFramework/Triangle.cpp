#include "stdafx.h"
#include "Triangle.h"


//	------------------------------------------------------------
//		Constructors and destructors
//	------------------------------------------------------------
ParabellumFramework::Triangle::Triangle()
{
}

ParabellumFramework::Triangle::Triangle(const Triangle& src)
{
	this->a = src.a;
	this->b = src.b;
	this->c = src.c;
}


ParabellumFramework::Triangle::~Triangle()
{
}


//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------

void
ParabellumFramework::Triangle::GetMiddle(_OUT_ Vector3D& out)
{
	out = (a+b+c);
	out /= 3.0f;
}

void 
ParabellumFramework::Triangle::GetNormal(_OUT_ Vector3D& out)
{
	Vector3D u, v;

	GetU(u);
	GetV(v);

	Vector3D::Cross(&out, &u, &v);
	out.Normalize();
}


ParabellumFramework::EFLOAT32
ParabellumFramework::Triangle::Area()
{
	return Math::Area(a, b, c);
}




//	------------------------------------------------------------
//		Gets And Sets
//	------------------------------------------------------------


void
ParabellumFramework::Triangle::GetU(_OUT_ Vector3D& out)
{
	out = vertices[1] - vertices[0];
	out.Normalize();
}


void
ParabellumFramework::Triangle::GetV(_OUT_ Vector3D& out)
{
	out = vertices[2] - vertices[0];
	out.Normalize();
}