#include "stdafx.h"
#include "Volume.h"



//	------------------------------------------------------------
//		Constructors and destructors
//	------------------------------------------------------------


ParabellumFramework::Volume::Volume()
{
	m_verticesCount = 0;
	m_vertices = NULLPTR;
	m_indicesCount = 0;
	m_indices = NULLPTR;
	m_inverse.MakeIdentity();
}


ParabellumFramework::Volume::Volume(_IN_ _ARRAY_ Vector3D* vertices, EUINT32 verticesCount, _IN_ _ARRAY_ EUINT32* indices, EUINT32 indicesCount)
{
	m_verticesCount = 0;
	m_vertices = NULLPTR;
	m_indicesCount = 0;
	m_indices= NULLPTR;
	m_inverse.MakeIdentity();

	Initialize(vertices, verticesCount, indices, indicesCount);
}

ParabellumFramework::Volume::~Volume()
{
	if(m_vertices != NULLPTR)
		delete[] m_vertices;

	if (m_indices != NULLPTR)
		delete[] m_indices;

	printf("\n~Volume()");
}


//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------


void 
ParabellumFramework::Volume::Initialize(_IN_ _ARRAY_ Vector3D* vertices, EUINT32 verticesCount, _IN_ _ARRAY_ EUINT32* indices, EUINT32 indicesCount)
{
	if (m_vertices != NULLPTR)
		return;

	m_vertices = new Vector3D[verticesCount];

	if (m_vertices == NULLPTR)
		return;

	m_indices = new EUINT32[indicesCount];

	if (m_indices == NULLPTR)
	{
		delete[] m_vertices;
		return;
	}


	memcpy(m_vertices, vertices, sizeof(Vector3D) * verticesCount);
	memcpy(m_indices, indices, sizeof(EUINT32) * indicesCount);

	m_verticesCount = verticesCount;
	m_indicesCount = indicesCount;
}


void
ParabellumFramework::Volume::Release()
{
	if (m_vertices != NULLPTR)
		delete[] m_vertices;

	if (m_indices != NULLPTR)
		delete[] m_indices;
}


ParabellumFramework::IntersectCodes 
ParabellumFramework::Volume::Intersects(_IN_ Volume* pMesh)
{
	return IntersectCodes::DISJOINT;
}

void 
ParabellumFramework::Volume::TransformOriginal(_IN_ Matrix4x4* pTransform)
{
	for (EUINT32 i = 0; i < m_verticesCount; ++i)
	{
		m_vertices[i] = (*pTransform) * (m_inverse * m_vertices[i]);
	}

	m_inverse = Matrix4x4::Invert(*pTransform);
}


ParabellumFramework::Vector3D*
ParabellumFramework::Volume::GetVertex(EUINT32 vertexNo)
{
	return &m_vertices[ vertexNo ];
}

ParabellumFramework::EUINT32 
ParabellumFramework::Volume::GetVerticesCount()
{
	return m_verticesCount;
}

void
ParabellumFramework::Volume::GetFace(EUINT32 triangleNo, _OUT_ Triangle& triangle)
{
	triangle.a = m_vertices[ m_indices[triangleNo * 3 + 0] ];
	triangle.b = m_vertices[ m_indices[triangleNo * 3 + 1] ];
	triangle.c = m_vertices[ m_indices[triangleNo * 3 + 2] ];
}
