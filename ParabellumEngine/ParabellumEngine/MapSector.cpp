#include "MapSector.h"



//	------------------------------------------------------
//	Constructors and destructors
//	------------------------------------------------------

ParabellumEngine::Core::Scene::MapSector::MapSector(Vector3D& vmin, Vector3D& vmax)
{
	// setup octree
	m_octree.Initialize(vmin, vmax);
}

ParabellumEngine::Core::Scene::MapSector::~MapSector()
{
	m_octree.Release();
}


//	------------------------------------------------------
//	Methods
//	------------------------------------------------------