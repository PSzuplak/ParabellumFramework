#include "stdafx.h"
#include "MathHelperExtended.h"

//	------------------------------------------------------------------------------------------------
ParabellumFramework::EFLOAT32
PF_FASTCALL
ParabellumFramework::Math::Ratio(_IN_ EFLOAT32 input, _IN_ EFLOAT32 minv, EFLOAT32 maxv)
{
	return (input - minv) / (maxv - minv);
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::EFLOAT32
PF_FASTCALL
ParabellumFramework::Math::Ratio(_IN_ Vector2D& input, _IN_ Vector2D& minv, _IN_ Vector2D& maxv)
{
	return Length(input - minv) / Length(maxv - minv);
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::EFLOAT32
PF_FASTCALL
ParabellumFramework::Math::Ratio(_IN_ Vector3D& input, _IN_ Vector3D& minv, _IN_ Vector3D& maxv)
{
	return Length(input - minv) / Length(maxv - minv);
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::EFLOAT32
PF_FASTCALL
ParabellumFramework::Math::Ratio(_IN_ Vector2D& begin, _IN_ Vector2D& val0, _IN_ Vector2D& val1, _IN_ Vector2D& end)
{
	EFLOAT32 len;
	len = Length(end - begin);

	return ( (Length(val1 - begin) / len) - (Length(val0 - begin) / len) );
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::EINT 
PF_FASTCALL 
ParabellumFramework::Math::Sign(_IN_ EINT val)
{
	return val < 0 ? -1 : 1;
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::EFLOAT32 
PF_FASTCALL 
ParabellumFramework::Math::Sign(_IN_ EFLOAT32 val)
{
	return std::signbit(val) ? 1.0f : -1.0f;
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::Vector2D
PF_FASTCALL
ParabellumFramework::Math::Sign(_IN_ Vector2D& val)
{
	return Vector2D( Sign(val.X), Sign(val.Y) );
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::Vector3D
PF_FASTCALL
ParabellumFramework::Math::Sign(_IN_ Vector3D& val)
{
	return Vector3D(Sign(val.X), Sign(val.Y), Sign(val.Z));
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::Vector4D
PF_FASTCALL
ParabellumFramework::Math::Sign(_IN_ Vector4D& val)
{
	return Vector4D(Sign(val.X), Sign(val.Y), Sign(val.Z), Sign(val.W));
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::Vector3D
PF_FASTCALL
ParabellumFramework::Math::Cross(_IN_ Vector3D& lhs, _IN_ Vector3D& rhs)
{
	float x = lhs.Y * rhs.Z - rhs.Y * lhs.Z;
	float y = lhs.Z * rhs.X - rhs.Z * lhs.X;
	float z = lhs.X * rhs.Y - rhs.X * lhs.Y;

	return Vector3D(x, y, z);
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::EFLOAT32 
PF_FASTCALL
ParabellumFramework::Math::Dot(_IN_ Vector2D& lhs, _IN_ Vector2D& rhs)
{
	return (lhs.X * rhs.X) + (lhs.Y * rhs.Y);
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::EFLOAT32
PF_FASTCALL
ParabellumFramework::Math::Dot(_IN_ Vector3D& lhs, _IN_ Vector3D& rhs)
{
	return (lhs.X * rhs.X) + (lhs.Y * rhs.Y) + (lhs.Z * rhs.Z);
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::EFLOAT32
PF_FASTCALL
ParabellumFramework::Math::Angle(_IN_ Vector2D lhs, _IN_ Vector2D rhs)
{
	lhs.Normalize();
	rhs.Normalize();

	return Dot(lhs, rhs);
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::EFLOAT32
PF_FASTCALL
ParabellumFramework::Math::Angle(_IN_ Vector3D lhs, _IN_ Vector3D rhs)
{
	lhs.Normalize();
	rhs.Normalize();

	return Dot(lhs, rhs);
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::EFLOAT32
PF_FASTCALL 
ParabellumFramework::Math::Length(_IN_ Vector2D& pIn)
{
	return ParabellumUtils::Math::FastSqrt(pIn.X*pIn.X + pIn.Y*pIn.Y);
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::EFLOAT32
PF_FASTCALL
ParabellumFramework::Math::Length(_IN_ Vector3D& pIn)
{
	return ParabellumUtils::Math::FastSqrt(pIn.X*pIn.X + pIn.Y*pIn.Y + pIn.Z*pIn.Z);
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::Vector2D
PF_FASTCALL
ParabellumFramework::Math::Normalize(_IN_ Vector2D& pIn)
{
	return pIn/Length(pIn);
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::Vector3D
PF_FASTCALL
ParabellumFramework::Math::Normalize(_IN_ Vector3D& pIn)
{
	return pIn / Length(pIn);
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::Vector2D
PF_FASTCALL
ParabellumFramework::Math::Projection(_IN_ Vector2D& lhs, _IN_ Vector2D& rhs)
{
	EFLOAT32 len = Math::Length(rhs);
	return rhs * (Dot(lhs, rhs) / (len*len));
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::Vector3D
PF_FASTCALL
ParabellumFramework::Math::Projection(_IN_ Vector3D& lhs, _IN_ Vector3D& rhs)
{
	EFLOAT32 len = Math::Length(rhs);
	return rhs * (Dot(lhs, rhs) / (len*len));
}
//	------------------------------------------------------------------------------------------------
//	------------------------------------------------------------------------------------------------
ParabellumFramework::EFLOAT32
PF_FASTCALL
ParabellumFramework::Math::Lerp(_IN_ EFLOAT32 v0, _IN_ EFLOAT32 v1, EFLOAT32 s)
{
	return v0 + (v1 - v0) * s;
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::Vector2D
PF_FASTCALL
ParabellumFramework::Math::Lerp(_IN_ Vector2D& v0, _IN_ Vector2D& v1, EFLOAT32 s)
{
	return v0 + (v1 - v0) * s;
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::Vector3D
PF_FASTCALL
ParabellumFramework::Math::Lerp(_IN_ Vector3D& v0, _IN_ Vector3D& v1, EFLOAT32 s)
{
	return v0 + (v1 - v0) * s;
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::Vector4D
PF_FASTCALL
ParabellumFramework::Math::Lerp(_IN_ Vector4D& v0, _IN_ Vector4D& v1, EFLOAT32 s)
{
	return v0 + (v1 - v0) * s;
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::EFLOAT32 
PF_FASTCALL 
ParabellumFramework::Math::Clamp(_IN_ EFLOAT32 input, _IN_ EFLOAT32 minv, EFLOAT32 maxv)
{
	if (input < minv)
		return minv;
	else if (input > maxv)
		return maxv;

	return input;
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::Vector2D 
PF_FASTCALL 
ParabellumFramework::Math::Clamp(_IN_ Vector2D& input, _IN_ EFLOAT32 minv, EFLOAT32 maxv)
{
	return Vector2D( 
		Math::Clamp(input.X, minv, maxv), 
		Math::Clamp(input.Y, minv, maxv) 
		);
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::Vector3D 
PF_FASTCALL 
ParabellumFramework::Math::Clamp(_IN_ Vector3D& input, _IN_ EFLOAT32 minv, EFLOAT32 maxv)
{
	return Vector3D(
		Math::Clamp(input.X, minv, maxv),
		Math::Clamp(input.Y, minv, maxv),
		Math::Clamp(input.Z, minv, maxv)
		);
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::Vector4D 
PF_FASTCALL 
ParabellumFramework::Math::Clamp(_IN_ Vector4D& input, _IN_ EFLOAT32 minv, EFLOAT32 maxv)
{
	return Vector4D(
		Math::Clamp(input.X, minv, maxv),
		Math::Clamp(input.Y, minv, maxv),
		Math::Clamp(input.Z, minv, maxv),
		Math::Clamp(input.W, minv, maxv)
		);
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::Vector2D 
PF_FASTCALL 
ParabellumFramework::Math::Reflect(_IN_ Vector2D& input, _IN_ Vector2D& normal)
{
	Vector2D result;

	result = normal;
	result *= Math::Dot(input, normal) * 2.0f;
	result -= normal;

	return result;
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::Vector3D 
PF_FASTCALL 
ParabellumFramework::Math::Reflect(_IN_ Vector3D& input, _IN_ Vector3D& normal)
{
	Vector3D result;

	result = normal;
	result *= Math::Dot(input, normal) * 2.0f;
	result -= normal;

	return result;
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::Vector3D
PF_FASTCALL 
ParabellumFramework::Math::RotateX(_IN_ Vector3D& input, EFLOAT32 angle)
{
	return Vector3D( 
		input.X, 
		input.Y*ParabellumUtils::Math::FastCos(angle) - input.Z*ParabellumUtils::Math::FastSin(angle),
		input.Y*ParabellumUtils::Math::FastSin(angle) + input.Z*ParabellumUtils::Math::FastCos(angle));
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::Vector3D
PF_FASTCALL 
ParabellumFramework::Math::RotateY(_IN_ Vector3D& input, EFLOAT32 angle)
{
	return Vector3D(
		input.Z*ParabellumUtils::Math::FastSin(angle) + input.X*ParabellumUtils::Math::FastCos(angle),
		input.Y, 
		input.Z*ParabellumUtils::Math::FastCos(angle) - input.X*ParabellumUtils::Math::FastSin(angle));
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::Vector3D
PF_FASTCALL 
ParabellumFramework::Math::RotateZ(_IN_ Vector3D& input, EFLOAT32 angle)
{
	return Vector3D(
		input.X*ParabellumUtils::Math::FastCos(angle) - input.Y*ParabellumUtils::Math::FastSin(angle),
		input.X*ParabellumUtils::Math::FastSin(angle) + input.Y*ParabellumUtils::Math::FastCos(angle),
		input.Z);
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::EFLOAT32 
ParabellumFramework::Math::Area(Vector2D& p0, Vector2D& p1, Vector2D&  p2)
{
	Vector2D p0p1, p0p2, p1p2;
	EFLOAT32 a, b, c, s;

	p0p1 = p1 - p0;
	p0p2 = p2 - p0;
	p1p2 = p2 - p1;

	a = p0p1.Length();
	b = p0p2.Length();
	c = p1p2.Length();

	s = (a + b + c) * 0.5f;

	return FastSqrt( s*(s-a)*(s-b)*(s-c) );
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::EFLOAT32
ParabellumFramework::Math::Area(Vector3D& p0, Vector3D& p1, Vector3D&  p2)
{
	Vector3D p0p1, p0p2, p1p2;
	EFLOAT32 a, b, c, s;

	p0p1 = p1 - p0;
	p0p2 = p2 - p0;
	p1p2 = p2 - p1;

	a = p0p1.Length();
	b = p0p2.Length();
	c = p1p2.Length();

	s = (a + b + c) * 0.5f;

	return FastSqrt( s*(s - a)*(s - b)*(s - c) );
}
//	------------------------------------------------------------------------------------------------
void 
ParabellumFramework::Math::Sort(Vector2D in1, Vector2D in2, Vector2D& out1, Vector2D& out2)
{
	if (in1.X < in2.X)
	{
		out1.X = in1.X;
		out2.X = in2.X;
	}
	else
	{
		out1.X = in2.X;
		out2.X = in1.X;
	}
	// y
	if (in1.Y < in2.Y)
	{
		out1.Y = in1.Y;
		out2.Y = in2.Y;
	}
	else
	{
		out1.Y = in2.Y;
		out2.Y = in1.Y;
	}
}
//	------------------------------------------------------------------------------------------------
void 
ParabellumFramework::Math::NearestPointOnRay(_IN_ Vector3D* pRayBegin, _IN_ Vector3D* pRayDirection, _IN_ Vector3D* pPoint, _OUT_ Vector3D* pOnRay)
{
	Vector3D sub, cross_sub_vd, hhh;
	EFLOAT32 square, t;

	sub = (*pPoint) - (*pRayBegin);
	square = pRayDirection->X*pRayDirection->X + pRayDirection->Y*pRayDirection->Y + pRayDirection->Z*pRayDirection->Z;

	// calculate length of a vector to the nearest point
	t = Vector3D::Dot(&sub, pRayDirection) / square;
	*pOnRay = (*pRayBegin) + ((*pRayDirection) * t);
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::EFLOAT32
ParabellumFramework::Math::Distance(_IN_ Vector3D* pVec1, _IN_ Vector3D* pVec2)
{
	Vector3D sub;
	sub = (*pVec1) - (*pVec2);

	return sub.Length();
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::EFLOAT32
ParabellumFramework::Math::Distance(Vector2D& p0, Vector2D& p1, Vector2D& s)
{
	Vector2D p0p1, fs;
	EFLOAT32 area;

	p0p1 = p1 - p0;

	area = Area(p0, p1, s);

	return (area * 2.0f) / p0p1.Length();
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::EFLOAT32
ParabellumFramework::Math::Distance(_IN_ Vector3D* pRayBegin, _IN_ Vector3D* pRayDirection, _IN_ Vector3D* pPoint)
{
	Vector3D sub, cross_sub_vd;

	sub = (*pRayBegin) - (*pPoint);
	Vector3D::Cross(&cross_sub_vd, &sub, pRayDirection);

	return cross_sub_vd.Length() / pRayDirection->Length();

}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::EFLOAT32
ParabellumFramework::Math::DistanceFixed(Vector2D& p0, Vector2D& p1, Vector2D& s)
{
	Vector2D p0p1, fs, middle;
	EFLOAT32 area, h, lp0, lp1, ray;

	p0p1 = p1 - p0;
	middle = (p0 + p1) * 0.5f;
	ray = (middle - p1).Length();

	if ((s - middle).Length() > ray)
		return -1.0f;

	area = Area(p0, p1, s);
	h = (area * 2.0f) / p0p1.Length();

	return h;
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::EFLOAT32
ParabellumFramework::Math::Distance(_IN_ Vector3D* planeNormal, EFLOAT32 planeDistance, _IN_ Vector3D* pPoint)
{
	return (abs(planeNormal->X * pPoint->X + planeNormal->Y * pPoint->Y + planeNormal->Z * pPoint->Z + planeDistance))
		/ sqrt(planeNormal->X*planeNormal->X + planeNormal->Y*planeNormal->Y + planeNormal->Z*planeNormal->Z);
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::IntersectCodes
ParabellumFramework::Math::Intersects(
	_IN_ Vector3D* pRayBegin, _IN_ Vector3D* pRayDirection, _IN_ Vector3D* pSphereCenter, EFLOAT32 sphereRay,
	_OUT_ EFLOAT32& t1, _OUT_ EFLOAT32& t2)
{
	EFLOAT32 a, b, c, delta;

	a = (pRayDirection->X * pRayDirection->X) + (pRayDirection->Y * pRayDirection->Y) + (pRayDirection->Z * pRayDirection->Z);

	b = 2 * ((pRayDirection->X)*(pRayBegin->X - pSphereCenter->X) + (pRayDirection->Y)*(pRayBegin->Y - pSphereCenter->Y) + (pRayDirection->Z)*(pRayBegin->Z - pSphereCenter->Z));
	c = pSphereCenter->X*pSphereCenter->X + pSphereCenter->Y*pSphereCenter->Y + pSphereCenter->Z*pSphereCenter->Z +
		pRayBegin->X*pRayBegin->X + pRayBegin->Y*pRayBegin->Y + pRayBegin->Z*pRayBegin->Z -
		2 * (pSphereCenter->X*pRayBegin->X + pSphereCenter->Y*pRayBegin->Y + pSphereCenter->Z*pRayBegin->Z) -
		sphereRay*sphereRay;

	delta = ParabellumUtils::Math::FastSqrt(b*b - 4 * a*c);

	if (delta < 0.0f)
		return IntersectCodes::DISJOINT;

	t1 = (-b - delta) / (2 * a);
	t2 = (-b + delta) / (2 * a);

	if (EEQ_ZERO(delta))
		return IntersectCodes::INTERSECT;

	return IntersectCodes::CONTAIN;
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::IntersectCodes
ParabellumFramework::Math::Intersects(
	_IN_ Vector3D* pRayBegin, _IN_ Vector3D* pRayDirection, _IN_ Vector3D* pSphereCenter, EFLOAT32 sphereRay,
	_OUT_ Vector3D* pIntersectionPoint1, _OUT_ Vector3D* pIntersectionPoint2)
{
	IntersectCodes result;
	EFLOAT32 t1, t2;

	result = Intersects(pRayBegin, pRayDirection, pSphereCenter, sphereRay, t1, t2);
	*pIntersectionPoint1 = *pRayBegin + (*pRayDirection) * t1;
	*pIntersectionPoint2 = *pRayBegin + (*pRayDirection) * t2;

	return result;
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::IntersectCodes
ParabellumFramework::Math::Intersects(
	_IN_ Vector3D* pRayBegin, _IN_ Vector3D* pRayDirection,
	_IN_ Vector3D* pV0, _IN_ Vector3D* pV1, _IN_ Vector3D* pV2,
	_OUT_ Vector3D* pIP
	)
{
	Vector3D middle = (*pV0) + (*pV1) + (*pV2);
	middle *= 0.3333f;

	return IntersectCodes::DISJOINT;
}

