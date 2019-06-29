#include "stdafx.h"
#include "BoundingFrustum.h"


//	-------------------------------------------------
//		Constructors and destrucors
//	-------------------------------------------------

ParabellumFramework::BoundingFrustum::BoundingFrustum()
{

}



ParabellumFramework::BoundingFrustum::BoundingFrustum(BoundingFrustum&)
{


}



ParabellumFramework::BoundingFrustum::~BoundingFrustum()
{




}


//	-------------------------------------------------
//		methods
//	-------------------------------------------------



ParabellumFramework::Plane* 
ParabellumFramework::BoundingFrustum::GetPlane(EUINT32 no)
{
	return &m_planes[no];
}

//
//
//	Create bounding volume from ViewProjection Matrix
//
//
void ParabellumFramework::BoundingFrustum::MakeFromMatrix(_IN_ Matrix4x4* viewProjection)
{
	// left plane
	m_planes[0].a = viewProjection->m41 + viewProjection->m11;
	m_planes[0].b = viewProjection->m42 + viewProjection->m12;
	m_planes[0].c = viewProjection->m43 + viewProjection->m13;
	m_planes[0].d = viewProjection->m44 + viewProjection->m14;
	// right plane
	m_planes[1].a = viewProjection->m41 - viewProjection->m11;
	m_planes[1].b = viewProjection->m42 - viewProjection->m12;
	m_planes[1].c = viewProjection->m43 - viewProjection->m13;
	m_planes[1].d = viewProjection->m44 - viewProjection->m14;
	// top plane
	m_planes[2].a = viewProjection->m41 - viewProjection->m21;
	m_planes[2].b = viewProjection->m42 - viewProjection->m22;
	m_planes[2].c = viewProjection->m43 - viewProjection->m23;
	m_planes[2].d = viewProjection->m44 - viewProjection->m24;
	// bottom plane
	m_planes[3].a = viewProjection->m41 + viewProjection->m21;
	m_planes[3].b = viewProjection->m42 + viewProjection->m22;
	m_planes[3].c = viewProjection->m43 + viewProjection->m23;
	m_planes[3].d = viewProjection->m44 + viewProjection->m24;
	// near plane
	m_planes[4].a = viewProjection->m41 + viewProjection->m31;
	m_planes[4].b = viewProjection->m42 + viewProjection->m32;
	m_planes[4].c = viewProjection->m43 + viewProjection->m33;
	m_planes[4].d = viewProjection->m44 + viewProjection->m34;
	// far plane
	m_planes[5].a = viewProjection->m41 - viewProjection->m31;
	m_planes[5].b = viewProjection->m42 - viewProjection->m32;
	m_planes[5].c = viewProjection->m43 - viewProjection->m33;
	m_planes[5].d = viewProjection->m44 - viewProjection->m34;

	// normalize planes
	m_planes[0].Normalize();
	m_planes[1].Normalize();
	m_planes[2].Normalize();
	m_planes[3].Normalize();
	m_planes[4].Normalize();
	m_planes[5].Normalize();

	CalculateCorners();
}








//
//
//	Check intersects with BoundingBox
//
//
ParabellumFramework::IntersectCodes
ParabellumFramework::BoundingFrustum::Intersects(_IN_ BoundingBox* pVolume)
{
	Vector3D corners[8];
	pVolume->GetCorners(corners);

	int counterIn = 0;


	//
	// next check "bounding sphere"
	EFLOAT32 rayLen = ( (*pVolume->GetMax()) - (*pVolume->GetMin()) ).Length() * 0.5f;

	Vector3D center;
	pVolume->GetCenter(&center);

	for (EUINT32 i = 0; i<6; ++i)
	{
		if (m_planes[i].GetDistance(&center) < -rayLen)
		{
			return IntersectCodes::DISJOINT;
		}
	}


	//
	// Check each corner of boundingbox
	for (EUINT32 i = 0; i<6; ++i)
	{
		int counter = 8;

		for (int n = 0; n < 8; n++)
		{
			if (m_planes[i].GetSquareDistance(&corners[n]) < 0.0f)
			{
				counter--;
			}
		}

		if (counter == 0)
			return IntersectCodes::DISJOINT;

		if (counter == 8)
			counterIn++;
	}


	if (counterIn == 8)
		return IntersectCodes::CONTAIN;


	return IntersectCodes::INTERSECT;
}


//
//
//	Check intersects with BoundingSphere
//
//
ParabellumFramework::IntersectCodes
ParabellumFramework::BoundingFrustum::Intersects(_IN_ BoundingSphere* pVolume)
{
	IntersectCodes result = IntersectCodes::DISJOINT;

	EUINT32 planesIn = 0;


	for (EUINT32 i = 0; i<6; ++i)
	{
		if (m_planes[i].GetDistance(pVolume->GetCenter()) < -pVolume->GetRayLength())
		{
			return IntersectCodes::DISJOINT;
		}

		planesIn++;
	}

	return IntersectCodes::INTERSECT;
}


//
//
//	Check intersects with point
//
//
ParabellumFramework::IntersectCodes
ParabellumFramework::BoundingFrustum::Intersects(_IN_ Vector3D* pVolume)
{
	for (EUINT32 i = 0; i<6; ++i)
	{
		if (m_planes[i].GetSquareDistance(pVolume) < 0.0f)
		{
			return IntersectCodes::DISJOINT;
		}
	}

	return IntersectCodes::INTERSECT;
}



//
//
//	Calculate 8 corners of the frustum
//
//
void 
ParabellumFramework::BoundingFrustum::CalculateCorners()
{
	// near top left
	Plane::Intersect(&m_planes[2], &m_planes[0], &m_planes[4], &m_corners[0]);
	// near top right
	Plane::Intersect(&m_planes[2], &m_planes[1], &m_planes[4], &m_corners[1]);
	// near bottom right
	Plane::Intersect(&m_planes[3], &m_planes[1], &m_planes[4], &m_corners[2]);
	// near bottom left
	Plane::Intersect(&m_planes[3], &m_planes[0], &m_planes[4], &m_corners[3]);
	// far top left
	Plane::Intersect(&m_planes[2], &m_planes[0], &m_planes[5], &m_corners[4]);
	// far top right
	Plane::Intersect(&m_planes[2], &m_planes[1], &m_planes[5], &m_corners[5]);
	// far bottom right
	Plane::Intersect(&m_planes[3], &m_planes[1], &m_planes[5], &m_corners[6]);
	// far bottom left
	Plane::Intersect(&m_planes[3], &m_planes[0], &m_planes[5], &m_corners[7]);
}





ParabellumFramework::EFLOAT32
ParabellumFramework::BoundingFrustum::GetSquareDistance(EUINT32 planeNo, _IN_ Vector3D* pVolume)
{
	return m_planes[planeNo].GetSquareDistance(pVolume);
}


ParabellumFramework::EUINT32
ParabellumFramework::BoundingFrustum::TransformWorldToView(_IN_ Vector3D* pWorldPosition, _OUT_ Vector3D* pViewPosition)
{
	return 0;
}


//	-------------------------------------------------
//		Gets and sets
//	-------------------------------------------------


//
//
//	Get 8 corners of the frustum
//
//
void 
ParabellumFramework::BoundingFrustum::GetCorners(_OUT_ Vector3D* pVectorArray)
{
	memcpy(pVectorArray, m_corners, sizeof(Vector3D) * 8);
}



//
//
//	Get 8 corners of the frustum
//
//
void 
ParabellumFramework::BoundingFrustum::GetSpliceCorners(
	_OUT_ Vector3D* pVectorArray, EFLOAT32 minRatio, EFLOAT32 maxRatio)
{
	Vector3D corners[8];
	Vector3D btl2ftl, btr2ftr, bbr2fbr, bbl2fbl;

	GetCorners(corners);
	
	btl2ftl = corners[4] - corners[0];
	btr2ftr = corners[5] - corners[1];
	bbr2fbr = corners[6] - corners[2];
	bbl2fbl = corners[7] - corners[3];

	pVectorArray[0] = corners[0] + btl2ftl*minRatio;
	pVectorArray[1] = corners[1] + btr2ftr*minRatio;
	pVectorArray[2] = corners[2] + bbr2fbr*minRatio;
	pVectorArray[3] = corners[3] + bbl2fbl*minRatio;

	pVectorArray[4] = corners[0] + btl2ftl*maxRatio;
	pVectorArray[5] = corners[1] + btr2ftr*maxRatio;
	pVectorArray[6] = corners[2] + bbr2fbr*maxRatio;
	pVectorArray[7] = corners[3] + bbl2fbl*maxRatio;

}