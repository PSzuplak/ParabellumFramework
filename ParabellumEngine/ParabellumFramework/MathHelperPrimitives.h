//--------------------------------------------------------------------------------
//
//	Contains functions working on primitives : 
//		Plane, Triangle, BoundingBox, BoundingSphere, Ray, Volume etc.
//	
//
//--------------------------------------------------------------------------------

#ifndef _MATHHELPERPRIMITIVES_H_
#define _MATHHELPERPRIMITIVES_H_


//
//	Predefs
//
#include "DLLIE.h"

//
//
//
#include "TypesC.h"
#include "MathHelper.h"
#include "MathHelperExtended.h"

#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "BoundingVolumes.h"
#include "BoundingFrustum.h"
#include "Ray.h"
#include "Plane.h"
#include "Triangle.h"
#include "Volume.h"


namespace ParabellumFramework
{
	namespace Math
	{

		//
		// To check a distance
		//
	
		// return a distance between a point and a border of a bounding box
		inline XYZ_API ParabellumFramework::EFLOAT32 Distance(_IN_ Vector3D* pVec, _IN_ BoundingBox* pBoundingBox);

		// return a distance from a 'point' to a 'ray'
		inline XYZ_API ParabellumFramework::EFLOAT32 Distance(_IN_ Ray* pRay, _IN_ Vector3D* pPoint);

		// return a distance from a 'point' to a 'ray'
		// save in 'pOnRay' the nearest point to 'pPoint' lying on the 'ray'
		inline XYZ_API ParabellumFramework::EFLOAT32 Distance(_IN_ Ray* pRay, _IN_ Vector3D* pPoint, _OUT_ Vector3D* pOnRay);


		// return a distance from a 'Ray' to a border of a 'bounding sphere'
		inline XYZ_API ParabellumFramework::EFLOAT32 Distance(_IN_ Ray* pRay, _IN_ BoundingSphere* pBoundingsphere);

		// return a distance from a 'bounding spere' border to an another bounding sphere border
		// if the bounding spheres intersects it returns negative number
		inline XYZ_API ParabellumFramework::EFLOAT32 Distance(_IN_ BoundingSphere* pBoundingsphere1, _IN_ BoundingSphere* pBoundingsphere2);

		// return a distance from a 'bounding spere' to a nearest face of a 'bounding box'
		inline XYZ_API ParabellumFramework::EFLOAT32 Distance(_IN_ BoundingSphere* pBoundingsphere, _IN_ BoundingBox* pBoundingBox);

		//
		// To check an intersection
		//

		XYZ_API IntersectCodes Intersects(_IN_ Vector3D* pVol1, _IN_ BoundingSphere* pVol2);
		XYZ_API IntersectCodes Intersects(_IN_ Vector3D* pVol1, _IN_ BoundingBox* pVol2);

		XYZ_API IntersectCodes Intersects(_IN_ Plane* pVol1, _IN_ BoundingSphere* pVol2);
		XYZ_API IntersectCodes Intersects(_IN_ Plane* pVol1, _IN_ BoundingBox* pVol2);
		XYZ_API IntersectCodes Intersects(_IN_ Plane* pVol1, _IN_ Triangle* pVol2);

		XYZ_API IntersectCodes Intersects(_IN_ BoundingSphere* pVol1, _IN_ BoundingSphere* pVol2);
		XYZ_API IntersectCodes Intersects(_IN_ BoundingSphere* pVol1, _IN_ BoundingBox* pVol2);
		XYZ_API IntersectCodes Intersects(_IN_ BoundingSphere* pVol1, _IN_ Triangle* pVol2);

		XYZ_API IntersectCodes Intersects(_IN_ BoundingBox* pVol1, _IN_ BoundingBox* pVol2);

		// check if a 'ray intersects with a 'sphere'
		XYZ_API IntersectCodes Intersects(_IN_ Ray* pRay, _IN_ BoundingSphere* pBoundingSphere);

		// check if a 'ray; intersects with a 'sphere' and return two points of this intersection
		XYZ_API IntersectCodes Intersects(_IN_ Ray* pRay, _IN_ BoundingSphere* pBoundingSphere, _OUT_ Vector3D* pIntersectionPointIn, _OUT_ Vector3D* pIntersectionPointOut);

		// check if a 'ray intersects with a 'BoundingBox'
		XYZ_API IntersectCodes Intersects(_IN_ Ray* pRay, _IN_ BoundingBox* pBoundingBox);

		XYZ_API IntersectCodes Intersects(_IN_ Ray* pVol1, _IN_ BoundingBox* pVol2);
		XYZ_API IntersectCodes Intersects(_IN_ Ray* pVol1, _IN_ Triangle* pVol2);
		XYZ_API IntersectCodes Intersects(_IN_ Ray* pVol1, _IN_ Plane* pVol2);


		// check if point 'pV' lies inside convex 'pConvex'
		XYZ_API IntersectCodes Intersects(_IN_ _ARRAY_ Vector3D* pConvex1, EUINT32 countConvex1, _IN_ _ARRAY_ Vector3D* pConvex2, EUINT32 countConvex2);


		XYZ_API IntersectCodes Intersects(_IN_ Volume* pVol1, _IN_ Vector3D* pVol2);
		XYZ_API IntersectCodes Intersects(_IN_ Volume* pVol1, _IN_ BoundingSphere* pVol2);
		XYZ_API IntersectCodes Intersects(_IN_ Volume* pVol1, _IN_ BoundingBox* pVol2);
		XYZ_API IntersectCodes Intersects(_IN_ Volume* pVol1, _IN_ Ray* pVol2);


		//
		// Collection
		// 

		class XYZ_API Collection
		{
		public:

			// common part of 2 rectangles
			static void Common(_IN_ Vector4D* pVol0, _IN_ Vector4D* pVol1, _OUT_ Vector4D* pVolOut);

			// common part of 2 boxes
			static void Common(_IN_ BoundingBox* pVol0, _IN_ BoundingBox* pVol1, _OUT_ BoundingBox* pVolOut);



		};




	}
}



#endif