#include "stdafx.h"
#include "BoundingVolumes.h"



//	------------------------------------------------------------
//		Constructors and destructors
//	------------------------------------------------------------
ParabellumFramework::BoundingSphere::BoundingSphere()
{
	m_center.MakeZero();
	m_ray = 0.0f;
}


ParabellumFramework::BoundingSphere::BoundingSphere(BoundingSphere& src)
{
	this->m_center = src.m_center;
	this->m_ray = src.m_ray;
}


ParabellumFramework::BoundingSphere::~BoundingSphere()
{
}


//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------





//
//
//	Set Center of the sphere
//
//
ParabellumFramework::EUINT32
ParabellumFramework::BoundingSphere::MakeBoundingSphere(_IN_ Vector3D* pCenter, EFLOAT32 ray)
{
	m_center = *pCenter;
	m_ray = ray;

	return SystemCodes::SUCCESS;
}




//
//
//	Set Center of the sphere
//
//
ParabellumFramework::EUINT32
ParabellumFramework::BoundingSphere::MakeBoundingSphere(EFLOAT32 px, EFLOAT32 py, EFLOAT32 pz, EFLOAT32 ray)
{
	m_center.X = px;
	m_center.Y = py;
	m_center.Z = pz;
	m_ray = ray;

	return 1;
}



//
//
//	Set Center of the sphere
//
//
ParabellumFramework::EUINT32
ParabellumFramework::BoundingSphere::MakeFromBoundingBox(_IN_ BoundingBox* pBox)
{
	pBox->GetCenter(&m_center);

	Vector3D vmax, vmin;

	vmax = *pBox->GetMax();
	vmax = vmax - m_center;

	m_ray = Vector3D::Length(vmax);

	return 0;
}


//
//
//	MAke sphere from points
//
//
ParabellumFramework::EUINT32
ParabellumFramework::BoundingSphere::MakeFromPoints(_IN_ Vector3D* pPoints, EUINT32 pointsCount)
{
	EFLOAT32 minX, minY, minZ, maxX, maxY, maxZ;
	minX = minY = minZ = +9999999.0f;
	maxX = maxY = maxZ = -9999999.0f;

	for (EUINT32 i = 0; i < pointsCount; ++i)
	{
		if (pPoints[i].X < minX) minX = pPoints[i].X;
		if (pPoints[i].Y < minY) minY = pPoints[i].Y;
		if (pPoints[i].Z < minZ) minZ = pPoints[i].Z;

		if (pPoints[i].X > maxX) maxX = pPoints[i].X;
		if (pPoints[i].Y > maxY) maxY = pPoints[i].Y;
		if (pPoints[i].Z > maxZ) maxZ = pPoints[i].Z;
	}

	Vector3D vmin(minX, minY, minZ);
	Vector3D vmax(maxX, maxY, maxZ);
	Vector3D c2m;

	m_center = (vmin + vmax) * 0.5f;
	c2m = vmax - m_center;

	m_ray = c2m.Length();

	return 0;
}





//
//
//
//
//
ParabellumFramework::IntersectCodes
ParabellumFramework::BoundingSphere::Intersects(_IN_ BoundingBox* pVolume)
{
	return pVolume->Intersects(this);
}


//
//
//
//
//
ParabellumFramework::IntersectCodes
ParabellumFramework::BoundingSphere::Intersects(_IN_ BoundingSphere* pVolume)
{
	Vector3D d;
	d = m_center - (*pVolume->GetCenter());

	if (Vector3D::Length(d) < (m_ray + pVolume->GetRayLength()))
		return IntersectCodes::INTERSECT;

	return IntersectCodes::DISJOINT;
}


//
//
//
//
//
ParabellumFramework::IntersectCodes
ParabellumFramework::BoundingSphere::Intersects(_IN_ Vector3D* pVolume)
{
	Vector3D d;
	d = (*pVolume) - m_center;

	if (Vector3D::SquareLength(d) < (m_ray*m_ray))
		return IntersectCodes::CONTAIN;

	

	return IntersectCodes::DISJOINT;
}



void
ParabellumFramework::BoundingSphere::Merge(_IN_ Vector3D* pVolume)
{
	EFLOAT32 nlen = (m_center - (*pVolume)).Length();

	if (nlen > m_ray)
		m_ray = nlen;
}




//	------------------------------------------------------------
//		Gets and Sets
//	------------------------------------------------------------



//
//
//	Set Center of the sphere
//
//
void ParabellumFramework::BoundingSphere::SetCenter(_IN_ Vector3D& center)
{
	m_center = center;
}



//
//
// Set sphere ray length
//
//
void ParabellumFramework::BoundingSphere::SetRayLength(EFLOAT32 rayLength)
{
	m_ray = rayLength;
}




//
//
//
//
//
ParabellumFramework::Vector3D* 
ParabellumFramework::BoundingSphere::GetCenter()
{
	return &m_center;
}


//
//
//
//
//
ParabellumFramework::EFLOAT32 
ParabellumFramework::BoundingSphere::GetRayLength()
{
	return m_ray;
}