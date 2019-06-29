#include "stdafx.h"
#include "Plane.h"


//	-----------------------------------------------------------
//		constructors
//	-----------------------------------------------------------
ParabellumFramework::Plane::Plane()
{
}


ParabellumFramework::Plane::Plane(const Plane& copy)
{
}


ParabellumFramework::Plane::~Plane()
{
}


//
//
//	Plane normalization
//
//
void 
ParabellumFramework::Plane::Normalize()
{
	m_V4D.W /= m_V4D.XYZ.Length();
	m_V4D.XYZ.Normalize();
}



//
//
//	Plane normalization
//
//
void 
ParabellumFramework::Plane::MakeFromPoints(_IN_ Vector3D& v1, _IN_ Vector3D& v2, _IN_ Vector3D& v3)
{
	//D3DXPlaneFromPoints(&m_DXPlane, &v1.DXVector3, &v2.DXVector3, &v3.DXVector3);
}



//
//
//	Get point intersection
//
//
void ParabellumFramework::Plane::Intersect(
	_IN_ Plane* Plane1, _IN_ Plane* Plane2, _IN_ Plane* Plane3,
	_OUT_ Vector3D* pOut)
{
	Vector3D n1, n2, n3;

	Plane1->GetNormal(&n1);
	Plane2->GetNormal(&n2);
	Plane3->GetNormal(&n3);

	Vector3D c23, c31, c12;

	Vector3D::Cross(&c23, &n2, &n3);

	float denom = Vector3D::Dot(&n1, &c23);


	if (fabsf(denom) < 0.00001f)
		return;



	Vector3D::Cross(&c23, &n2, &n3);
	Vector3D::Cross(&c31, &n3, &n1);
	Vector3D::Cross(&c12, &n1, &n2);

	c23 *= Plane1->d;
	c31 *= Plane2->d;
	c12 *= Plane3->d;

	*pOut = c23 + c31 + c12;
	*pOut /= -denom;
}


//	-------------------------------------------------
//		Gets and sets
//	-------------------------------------------------


void 
ParabellumFramework::Plane::GetPlane(_OUT_ Vector4D* pOut)
{
	*pOut = m_V4D;
}

//
//
//	get normal vector
//
//
void 
ParabellumFramework::Plane::GetNormal(_OUT_ Vector3D* pOut)
{
	pOut->X = a;
	pOut->Y = b;
	pOut->Z = c;
}



//
//
//	Get distance from the point 0
//
//
ParabellumFramework::EFLOAT32
ParabellumFramework::Plane::GetPlaneDistance()
{
	return d;
}





//
//
//	Get distance from the point
//
//
ParabellumFramework::EFLOAT32 
ParabellumFramework::Plane::GetDistance(_IN_ Vector3D* pVector)
{
	EFLOAT result;
	result = (pVector->X * a) + (pVector->Y * b) + (pVector->Z * c) + d;
	result /= ParabellumUtils::Math::FastSqrt(a*a + b*b + c*c);

	return result;
}


//
//
//	Get square distance from the point
//
//
ParabellumFramework::EFLOAT32
ParabellumFramework::Plane::GetSquareDistance(_IN_ Vector3D* pVector)
{
	return (pVector->X * a) + (pVector->Y * b) + (pVector->Z * c) + d;
}