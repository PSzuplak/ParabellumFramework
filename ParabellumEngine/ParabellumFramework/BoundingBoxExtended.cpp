#include "stdafx.h"
#include "BoundingVolumes.h"


//	------------------------------------------------------------
//		Constructors and destructors
//	------------------------------------------------------------
ParabellumFramework::OrientedBoundingBox::OrientedBoundingBox()
{
}


ParabellumFramework::OrientedBoundingBox::OrientedBoundingBox(OrientedBoundingBox& src)
{
	this->m_points[0] = src.m_points[0];
	this->m_points[1] = src.m_points[1];
	this->m_points[2] = src.m_points[2];
	this->m_points[3] = src.m_points[3];
	this->m_points[4] = src.m_points[4];
	this->m_points[5] = src.m_points[5];
	this->m_points[6] = src.m_points[6];
	this->m_points[7] = src.m_points[7];
}


ParabellumFramework::OrientedBoundingBox::~OrientedBoundingBox()
{
}


//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------


void 
ParabellumFramework::OrientedBoundingBox::Make(_IN_ BoundingBox* pBox, _IN_ Vector3D* scale, _IN_ Vector3D* rotation, _IN_ Vector3D* position)
{
	Vector3D points[8];
	Vector3D cp, cs, cr;

	pBox->GetCorners(m_points);

	for (EUINT32 i = 0; i < 8; ++i)
	{
		m_points[i] *= *scale;

		m_points[i].RotateX(rotation->X);
		m_points[i].RotateY(rotation->Y);
		m_points[i].RotateZ(rotation->Z);

		m_points[i] += *position;
	}
}

void
ParabellumFramework::OrientedBoundingBox::Make(_IN_ Vector3D* min, _IN_ Vector3D* max, _IN_ Matrix4x4* pWorld)
{
	// front quad

	// 0 
	m_points[0].X = min->X;
	m_points[0].Y = max->Y;
	m_points[0].Z = min->Z;

	// 1 
	m_points[1].X = max->X;
	m_points[1].Y = max->Y;
	m_points[1].Z = min->Z;

	// 2 
	m_points[2].X = max->X;
	m_points[2].Y = min->Y;
	m_points[2].Z = min->Z;

	// 3 
	m_points[3].X = min->X;
	m_points[3].Y = min->Y;
	m_points[3].Z = min->Z;

	// back quad

	// 4
	m_points[4].X = min->X;
	m_points[4].Y = max->Y;
	m_points[4].Z = max->Z;

	// 5 
	m_points[5].X = max->X;
	m_points[5].Y = max->Y;
	m_points[5].Z = max->Z;

	// 6 
	m_points[6].X = max->X;
	m_points[6].Y = min->Y;
	m_points[6].Z = max->Z;

	// 7 
	m_points[7].X = min->X;
	m_points[7].Y = min->Y;
	m_points[7].Z = max->Z;


	for (EUINT32 i = 0; i < 8; ++i)
	{
		m_points[i] = (*pWorld) * m_points[i];
	}
}






//	------------------------------------------------------------
//		Gets and sets
//	------------------------------------------------------------
void 
ParabellumFramework::OrientedBoundingBox::GetCorners(_OUT_ Vector3D* pVectors)
{
	memcpy(pVectors, m_points, 8 * sizeof(Vector3D));
}


ParabellumFramework::Vector3D* 
ParabellumFramework::OrientedBoundingBox::GetCorners()
{
	return m_points;
}