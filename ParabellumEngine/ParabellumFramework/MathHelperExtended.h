//--------------------------------------------------------------------------------
//
//	Contains some extenstions for basic MatHelper functions
//	The idea is to provide calculations for all ParabellumFramework basic math structs
//
//--------------------------------------------------------------------------------

#ifndef _EXTMATHHELPER_H_
#define _EXTMATHHELPER_H_

//
//	Predefs
//
#include "DLLIE.h"

//
//
//
#include "TypesC.h"
#include "MathHelper.h"
#include "UnitHelper.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "IntersectCodes.h"



namespace ParabellumFramework
{
	namespace Math
	{

		//	------------------------------------------------------------------------------------
		//		Space "tricks"
		//	------------------------------------------------------------------------------------



		//	------------------------------------------------------------------------------------
		//		
		//	------------------------------------------------------------------------------------

		// Ratio

		// return ratio (0.0f to 1.0f) of input between 2 values - minv and maxv
		// 'input' must be greater than minv and lower than maxv
		// Example :
		//		Ratio( Clamp(input, minv, maxv), minv, maxv );
		PF_INLINE XYZ_API EFLOAT32 PF_FASTCALL Ratio(_IN_ EFLOAT32 input, _IN_ EFLOAT32 minv, EFLOAT32 maxv);

		// returns a ratio of vector lengths : Length(input - minv) / Length(maxv - minv);
		// first 'input' must be projected on Vector(maxv - minv)
		// Example :
		//		Ratio( Projection(input - minv, maxv - minv), minv, maxv  );
		PF_INLINE XYZ_API EFLOAT32 PF_FASTCALL Ratio(_IN_ Vector2D& input, _IN_ Vector2D& minv, _IN_ Vector2D& maxv);

		// returns a ratio of vector lengths : Length(input - minv) / Length(maxv - minv);
		// first 'input' must be projected on Vector(maxv - minv)
		// Example :
		//		Ratio( Projection(input - minv, maxv - minv), minv, maxv  );
		PF_INLINE XYZ_API EFLOAT32 PF_FASTCALL Ratio(_IN_ Vector3D& input, _IN_ Vector3D& minv, _IN_ Vector3D& maxv);


		// 
		// val1 and val2 must lie on a ray definied by (begin,end)
		// also begin < val0 < val1 < end ; Length(begin-begin) < Length(val0-begin) < Length(val1-begin) < Length(end-begin)
		// Example: used to get the ratio for mouse movement ; 
		//		where begin is projected XY coordinates of the begining of a vector3D
		//		where end is projected XY coordinates of the end of a vector3D
		//		val0 may be mouse coordiantes from the previous frame
		//		val1 may be mouse coordiantes from the current frame
		PF_INLINE XYZ_API EFLOAT32 PF_FASTCALL Ratio(_IN_ Vector2D& begin, _IN_ Vector2D& val0, _IN_ Vector2D& val1, _IN_ Vector2D& end);



		// Sign - returns +1.0f if val is greater thatn 0.0f and returns -1.0f if val is lower than 0.0f

		PF_INLINE XYZ_API EINT PF_FASTCALL Sign(_IN_ EINT val);
		PF_INLINE XYZ_API EFLOAT32 PF_FASTCALL Sign(_IN_ EFLOAT32 val);
		PF_INLINE XYZ_API Vector2D PF_FASTCALL Sign(_IN_ Vector2D& val);
		PF_INLINE XYZ_API Vector3D PF_FASTCALL Sign(_IN_ Vector3D& val);
		PF_INLINE XYZ_API Vector4D PF_FASTCALL Sign(_IN_ Vector4D& val);

		// Cross

		PF_INLINE XYZ_API Vector3D PF_FASTCALL Cross(_IN_ Vector3D& lhs, _IN_ Vector3D& rhs);

		// Dot

		PF_INLINE XYZ_API EFLOAT32 PF_FASTCALL Dot(_IN_ Vector2D& lhs, _IN_ Vector2D& rhs);
		PF_INLINE XYZ_API EFLOAT32 PF_FASTCALL Dot(_IN_ Vector3D& lhs, _IN_ Vector3D& rhs);


		// Angle

		PF_INLINE XYZ_API EFLOAT32 PF_FASTCALL Angle(_IN_ Vector2D lhs, _IN_ Vector2D rhs);
		PF_INLINE XYZ_API EFLOAT32 PF_FASTCALL Angle(_IN_ Vector3D lhs, _IN_ Vector3D rhs);

		// Length

		PF_INLINE XYZ_API EFLOAT32 PF_FASTCALL Length(_IN_ Vector2D& pIn);
		PF_INLINE XYZ_API EFLOAT32 PF_FASTCALL Length(_IN_ Vector3D& pIn);
		
		// Normalized

		PF_INLINE XYZ_API Vector2D PF_FASTCALL Normalize(_IN_ Vector2D& pIn);
		PF_INLINE XYZ_API Vector3D PF_FASTCALL Normalize(_IN_ Vector3D& pIn);

		// Projections

		PF_INLINE XYZ_API Vector2D PF_FASTCALL Projection(_IN_ Vector2D& lhs, _IN_ Vector2D& rhs);
		PF_INLINE XYZ_API Vector3D PF_FASTCALL Projection(_IN_ Vector3D& lhs, _IN_ Vector3D& rhs);

		// Lerp

		PF_INLINE XYZ_API EFLOAT32 PF_FASTCALL Lerp(_IN_ EFLOAT32 v0, _IN_ EFLOAT32 v1, EFLOAT32 s);
		PF_INLINE XYZ_API Vector2D PF_FASTCALL Lerp(_IN_ Vector2D& v0, _IN_ Vector2D& v1, EFLOAT32 s);
		PF_INLINE XYZ_API Vector3D PF_FASTCALL Lerp(_IN_ Vector3D& v0, _IN_ Vector3D& v1, EFLOAT32 s);
		PF_INLINE XYZ_API Vector4D PF_FASTCALL Lerp(_IN_ Vector4D& v0, _IN_ Vector4D& v1, EFLOAT32 s);

		// Clamp

		PF_INLINE XYZ_API EFLOAT32 PF_FASTCALL Clamp(_IN_ EFLOAT32 input, _IN_ EFLOAT32 minv, EFLOAT32 maxv);
		PF_INLINE XYZ_API Vector2D PF_FASTCALL Clamp(_IN_ Vector2D& input, _IN_ EFLOAT32 minv, EFLOAT32 maxv);
		PF_INLINE XYZ_API Vector3D PF_FASTCALL Clamp(_IN_ Vector3D& input, _IN_ EFLOAT32 minv, EFLOAT32 maxv);
		PF_INLINE XYZ_API Vector4D PF_FASTCALL Clamp(_IN_ Vector4D& input, _IN_ EFLOAT32 minv, EFLOAT32 maxv);

		// Reflect

		PF_INLINE XYZ_API Vector2D PF_FASTCALL Reflect(_IN_ Vector2D& input, _IN_ Vector2D& normal);
		PF_INLINE XYZ_API Vector3D PF_FASTCALL Reflect(_IN_ Vector3D& input, _IN_ Vector3D& normal);

		// Rotation

		PF_INLINE XYZ_API Vector3D PF_FASTCALL RotateX(_IN_ Vector3D& input, EFLOAT32 angle);
		PF_INLINE XYZ_API Vector3D PF_FASTCALL RotateY(_IN_ Vector3D& input, EFLOAT32 angle);
		PF_INLINE XYZ_API Vector3D PF_FASTCALL RotateZ(_IN_ Vector3D& input, EFLOAT32 angle);


		// calculate field of 2D triangle using Herons equation
		PF_INLINE XYZ_API ParabellumFramework::EFLOAT32 Area(Vector2D& p0, Vector2D& p1, Vector2D&  p2);

		// calculate field of 3D triangle using Herons equation
		PF_INLINE XYZ_API ParabellumFramework::EFLOAT32 Area(Vector3D& p0, Vector3D& p1, Vector3D&  p2);

		// get values from 'in1' and 'in2' and save in 'out1' and 'out2'
		// result will be : out1.X < out2.X and out1.Y < out2
		PF_INLINE XYZ_API void Sort(Vector2D in1, Vector2D in2, Vector2D& out1, Vector2D& out2);
	
		// find a point which lies on a 'ray' and has a shortest distance between the ray and the 'pPoint'
		PF_INLINE XYZ_API void NearestPointOnRay(_IN_ Vector3D* pRayBegin, _IN_ Vector3D* pRayDirection, _IN_ Vector3D* pPoint, _OUT_ Vector3D* pOnRay);


		//
		// Calculate distance between different objects
		//

		// return a distance between two vertices
		PF_INLINE XYZ_API ParabellumFramework::EFLOAT32 Distance(_IN_ Vector3D* pVec1, _IN_ Vector3D* pVec2);

		// return distance from 's' to a line form vectors p0 and p1
		PF_INLINE XYZ_API ParabellumFramework::EFLOAT32 Distance(Vector2D& p0, Vector2D& p1, Vector2D& s);

		// return distance from 'pPoint' to a line represend by parametric equation begin + t*direction
		PF_INLINE XYZ_API ParabellumFramework::EFLOAT32 Distance(_IN_ Vector3D* pRayBegin, _IN_ Vector3D* pRayDirection, _IN_ Vector3D* pPoint);

		// return distance from 's' to the Vector (p0, p1)
		// which means if 's' MUST lie inside rectangle (p0,p1) - return -1.0f if not
		PF_INLINE XYZ_API ParabellumFramework::EFLOAT32 DistanceFixed(Vector2D& p0, Vector2D& p1, Vector2D& s);


		// return a distance between a plane definied by (a normal vector 'planeNormal' and a distance from it planeDistance) and a 3D point 'pPoint'
		PF_INLINE XYZ_API ParabellumFramework::EFLOAT32 Distance(_IN_ Vector3D* planeNormal, EFLOAT32 planeDistance, _IN_ Vector3D* pPoint);


		//
		// Calculate intersection between different objects
		//

		// calculate parametrs 't' of a ray parametric equation 'v = v0 + t*direction'
		// then the intersection points can be calculated using :
		// 'POINT1 = *pRayBegin + (*pRayDirection) * t1;'
		// 'POINT2 = *pRayBegin + (*pRayDirection) * t2;'
		PF_INLINE XYZ_API IntersectCodes Intersects(
			_IN_ Vector3D* pRayBegin, _IN_ Vector3D* pRayDirection, _IN_ Vector3D* pSphereCenter, EFLOAT32 sphereRay,
			_OUT_ EFLOAT32& t1, _OUT_ EFLOAT32& t2);

		// calculate points of intersection with a ray and a sphere and store them in 'pIntersectionPoint1' and 'pIntersectionPoint2'
		// if there is one point (ray tangential to a sphere border) the funcion returns IntersectCodes::INTERSECT
		// if there are 2 solutions the function returns IntersectCodes::CONTAIN
		PF_INLINE XYZ_API IntersectCodes Intersects(
			_IN_ Vector3D* pRayBegin, _IN_ Vector3D* pRayDirection, _IN_ Vector3D* pSphereCenter, EFLOAT32 sphereRay, 
			_OUT_ Vector3D* pIntersectionPoint1, _OUT_ Vector3D* pIntersectionPoint2);


		// check if a Ray (pRayBegin, pRayDirection) intersect with a Triangle (pV0, pV1, V2)
		// calculate intersection point 'pIP' which lies inside the triangle
		PF_INLINE XYZ_API IntersectCodes Intersects(
			_IN_ Vector3D* pRayBegin, _IN_ Vector3D* pRayDirection,
			_IN_ Vector3D* pV0, _IN_ Vector3D* pV1, _IN_ Vector3D* pV2,
			_OUT_ Vector3D* pIP
			);






		//
		// Given vector 'world' 
		//
		//inline XYZ_API ParabellumFramework::EFLOAT32 Displace(_IN_ Vector3D* begin, _IN_ Vector3D* end, _IN_ Vector2D screen, _OUT_ Vector3D* output)



		//	------------------------------------------------------------------------------------
		//		ScreenSpace operations
		//	------------------------------------------------------------------------------------






	}

}

#endif