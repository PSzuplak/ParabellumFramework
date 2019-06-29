#include "stdafx.h"
#include "BoundingVolumes.h"


//	------------------------------------------------------------
//		Constructors and destructors
//	------------------------------------------------------------
ParabellumFramework::BoundingBox::BoundingBox()
{
}


ParabellumFramework::BoundingBox::BoundingBox(BoundingBox& src)
{
	this->m_min = src.m_min;
	this->m_max = src.m_max;
}


ParabellumFramework::BoundingBox::~BoundingBox()
{
}


//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------

//
//
//	Meke box from center, width, height
//
//
ParabellumFramework::EUINT32
ParabellumFramework::BoundingBox::MakeFromPoints(_IN_ Vector3D* min, _IN_ Vector3D* max)
{
	if (min->X < max->X)
	{
		m_min.X = min->X;
		m_max.X = max->X;
	}
	else
	{
		m_min.X = max->X;
		m_max.X = min->X;
	}

	if (min->Y < max->Y)
	{
		m_min.Y = min->Y;
		m_max.Y = max->Y;
	}
	else
	{
		m_min.Y = max->Y;
		m_max.Y = min->Y;
	}

	if (min->Z < max->Z)
	{
		m_min.Z = min->Z;
		m_max.Z = max->Z;
	}
	else
	{
		m_min.Z = max->Z;
		m_max.Z = min->Z;
	}

	return 0;
}


ParabellumFramework::EUINT32
ParabellumFramework::BoundingBox::MakeFromPoints(EFLOAT32 x0, EFLOAT32 y0, EFLOAT32 z0, EFLOAT32 x1, EFLOAT32 y1, EFLOAT32 z1)
{
	Vector3D v0, v1;

	v0.Set(x0, y0, z0);
	v1.Set(x1, y1, z1);

	MakeFromPoints(&v0, &v1);

	return 0;
}

//
//
//	Meke box from points - use ONLY at loading time
//
//
ParabellumFramework::EUINT32
ParabellumFramework::BoundingBox::MakeFromPoints(_IN_ Vector3D* pPoints, EUINT32 pointsCount)
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

	MakeFromPoints(&vmin, &vmax);


	return 0;
}






//
//
//	Make box from BoundingSphere
//
//
ParabellumFramework::EUINT32
ParabellumFramework::BoundingBox::MakeFromSphere(_IN_ BoundingSphere* pSphere)
{
	Vector3D vmin, vmax;

	vmin = *pSphere->GetCenter();
	vmax = *pSphere->GetCenter();

	vmin.X -= pSphere->GetRayLength();
	vmin.Y -= pSphere->GetRayLength();
	vmin.Z -= pSphere->GetRayLength();

	vmax.X += pSphere->GetRayLength();
	vmax.Y += pSphere->GetRayLength();
	vmax.Z += pSphere->GetRayLength();

	MakeFromPoints(&vmin, &vmax);

	return SystemCodes::SUCCESS;
}


//
//
//	Make box from BoundingSphere
//
//
ParabellumFramework::EUINT32
ParabellumFramework::BoundingBox::MakeCube(EFLOAT32 length)
{
	m_min.MakeZero();

	PF_VECTOR3D_Set(m_max, length, length, length);

	return SystemCodes::SUCCESS;
}


//
//
//
//
//
ParabellumFramework::IntersectCodes
ParabellumFramework::BoundingBox::Intersects(_IN_ BoundingBox* pVolume)
{
	Vector3D rmin, rmax;
	pVolume->GetMinMax(rmin, rmax);


	if (
		m_min.X < rmin.X &&
		m_max.X > rmax.X &&
		m_min.Y < rmin.Y &&
		m_max.Y > rmax.Y &&
		m_min.Z < rmin.Z &&
		m_max.Z > rmax.Z
		)
	{
		return IntersectCodes::CONTAIN;
	}

	if(
		(m_min.X < rmax.X && m_max.X > rmin.X) &&
		(m_min.Y < rmax.Y && m_max.Y > rmin.Y) &&
		(m_min.Z < rmax.Z && m_max.Z > rmin.Z)
		)
	{
		return IntersectCodes::INTERSECT;
	}

	return IntersectCodes::DISJOINT;
}


ParabellumFramework::IntersectCodes
ParabellumFramework::BoundingBox::Intersects(_IN_ BoundingSphere* pVolume)
{
	Vector3D corners[8];
	GetCorners(corners);

	EUINT32 cornersIn = 0;

	for (EUINT32 i = 0; i < 8; ++i)
	{
		if (pVolume->Intersects(&corners[i]) != IntersectCodes::DISJOINT)
			cornersIn++;
	}

	if (cornersIn == 8)
		return IntersectCodes::CONTAIN;

	if (cornersIn != 0)
		return IntersectCodes::INTERSECT;


	return IntersectCodes::DISJOINT;
}


//
//
//
//
//
ParabellumFramework::IntersectCodes
ParabellumFramework::BoundingBox::Intersects(_IN_ Vector3D* pVolume)
{
	if ((pVolume->X >= m_min.X) && (pVolume->Y >= m_min.Y) && (pVolume->Z >= m_min.Z) &&
		(pVolume->X <= m_max.X) && (pVolume->Y <= m_max.Y) && (pVolume->Z <= m_max.Z)
		)
		return IntersectCodes::CONTAIN;

	return IntersectCodes::DISJOINT;
}




void
ParabellumFramework::BoundingBox::Merge(_IN_ BoundingBox* pVolume)
{
	Vector3D smin, smax;
	pVolume->GetMinMax(smin, smax);

	if (m_min.X > smin.X) m_min.X = smin.X;
	if (m_min.Y > smin.Y) m_min.Y = smin.Y;
	if (m_min.Z > smin.Z) m_min.Z = smin.Z;

	if (m_max.X < smax.X) m_max.X = smax.X;
	if (m_max.Y < smax.Y) m_max.Y = smax.Y;
	if (m_max.Z < smax.Z) m_max.Z = smax.Z;
}


void
ParabellumFramework::BoundingBox::Merge(_IN_ Vector3D* pVolume)
{
	if (m_min.X > pVolume->X) m_min.X = pVolume->X;
	if (m_min.Y > pVolume->Y) m_min.Y = pVolume->Y;
	if (m_min.Z > pVolume->Z) m_min.Z = pVolume->Z;

	if (m_max.X < pVolume->X) m_max.X = pVolume->X;
	if (m_max.Y < pVolume->Y) m_max.Y = pVolume->Y;
	if (m_max.Z < pVolume->Z) m_max.Z = pVolume->Z;
}






//	-----------------------------------------------------------------------------------------
//		Gets and Sets
//	-----------------------------------------------------------------------------------------


//
//
// Get cnter of the bounding box
//
//
void ParabellumFramework::BoundingBox::GetCenter(_OUT_ Vector3D* pVec)
{
	*pVec = (m_max + m_min) * 0.5f;
}


ParabellumFramework::Vector3D* 
ParabellumFramework::BoundingBox::GetMin()
{
	return &m_min;
}


ParabellumFramework::Vector3D* 
ParabellumFramework::BoundingBox::GetMax()
{
	return &m_max;
}


//
//
//
//
//
ParabellumFramework::EFLOAT32 
ParabellumFramework::BoundingBox::GetWidth()
{
	return m_max.X - m_min.X;
}


//
//
//
//
//
ParabellumFramework::EFLOAT32 
ParabellumFramework::BoundingBox::GetHeight()
{
	return m_max.Y - m_min.Y;
}


//
//
//
//
//
ParabellumFramework::EFLOAT32 
ParabellumFramework::BoundingBox::GetDepth()
{
	return m_max.Z - m_min.Z;
}


//
//
// Get 8 corners of bounding box
//
//
void 
ParabellumFramework::BoundingBox::GetCorners(_OUT_ Vector3D* pVectors)
{
	// front quad

	// 0 
	pVectors[0].X = m_min.X;
	pVectors[0].Y = m_max.Y;
	pVectors[0].Z = m_min.Z;

	// 1 
	pVectors[1].X = m_max.X;
	pVectors[1].Y = m_max.Y;
	pVectors[1].Z = m_min.Z;

	// 2 
	pVectors[2].X = m_max.X;
	pVectors[2].Y = m_min.Y;
	pVectors[2].Z = m_min.Z;

	// 3 
	pVectors[3].X = m_min.X;
	pVectors[3].Y = m_min.Y;
	pVectors[3].Z = m_min.Z;

	// back quad

	// 4
	pVectors[4].X = m_min.X;
	pVectors[4].Y = m_max.Y;
	pVectors[4].Z = m_max.Z;

	// 5 
	pVectors[5].X = m_max.X;
	pVectors[5].Y = m_max.Y;
	pVectors[5].Z = m_max.Z;

	// 6 
	pVectors[6].X = m_max.X;
	pVectors[6].Y = m_min.Y;
	pVectors[6].Z = m_max.Z;

	// 7 
	pVectors[7].X = m_min.X;
	pVectors[7].Y = m_min.Y;
	pVectors[7].Z = m_max.Z;
}






//
//
// Get min max vectors
//
//
void ParabellumFramework::BoundingBox::GetMinMax(_OUT_ Vector3D& minv, _OUT_ Vector3D& maxv)
{
	minv = m_min;
	maxv = m_max;
}