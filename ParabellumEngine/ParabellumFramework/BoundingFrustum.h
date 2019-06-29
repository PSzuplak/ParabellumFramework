#ifndef _BOUNDINGFRUSTUM_H_
#define _BOUNDINGFRUSTUM_H_

#include "TypesC.h"
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "Plane.h"
#include "BoundingVolumes.h"
#include "IntersectCodes.h"


namespace ParabellumFramework
{

	class XYZ_API BoundingFrustum
	{


	//
	// Constructors and destructors
	//
	public:
		BoundingFrustum();
		BoundingFrustum(BoundingFrustum&);
		~BoundingFrustum();






		//
		//	Methods
		//
	public:

		void MakeFromMatrix(_IN_ Matrix4x4* viewProjection);

		// check intersection with BoundingBox
		IntersectCodes Intersects(_IN_ BoundingBox* pVolume);

		// check intersection with BoundingSphere
		IntersectCodes Intersects(_IN_ BoundingSphere* pVolume);

		// check intersection with point
		IntersectCodes Intersects(_IN_ Vector3D* pVolume);


		// get squared distance from the plane no 'planeNo'
		// 0 - left plane
		// 1 - right plane
		// 2 - top plane
		// 3 - bottom plane
		// 4 - near plane
		// 5 - far plane
		EFLOAT32 GetSquareDistance(EUINT32 planeNo, _IN_ Vector3D* pVolume);


		// transform pWorldPosition to View coorinates using frustum
		EUINT32 TransformWorldToView(_IN_ Vector3D* pWorldPosition, _OUT_ Vector3D* pViewPosition);






	private:
		void CalculateCorners();



	//
	// Members
	//
	public:

		// 6 planes which describe frustum
		// 0 - left plane
		// 1 - right plane
		// 2 - top plane
		// 3 - bottom plane
		// 4 - near plane
		// 5 - far plane
		Plane m_planes[6];

		Vector3D m_corners[8];			// array for frustum corners

	//
	//	Gets and set
	//
	public:


		// 0 - left plane
		// 1 - right plane
		// 2 - top plane
		// 3 - bottom plane
		// 4 - near plane
		// 5 - far plane
		Plane* GetPlane(EUINT32 no);

		// get corners of frustum
		// 0 - near plane top left
		// 1 - near plane top right
		// 2 - near plane bottom right
		// 3 - near plane bottom left
		// 4 - far plane top left
		// 5 - far plane top right
		// 6 - far plane bottom right
		// 7 - far plane bottom left
		void GetCorners(_OUT_ Vector3D* pVectorArray);		


		// get corners which calls to some splice of the frustum. 
		void GetSpliceCorners(_OUT_ Vector3D* pVectorArray, EFLOAT32 minRatio, EFLOAT32 maxRatio);

	};

}

#endif