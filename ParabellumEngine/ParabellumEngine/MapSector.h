#ifndef _MAPSECTOR_H_
#define _MAPSECTOR_H_


#include "DLLIE.h"

#include <list>
#include <thread>

#include "../ParabellumFramework/SystemCodes.h"
#include "../ParabellumFramework/TypesC.h"
#include "../ParabellumFramework/Vector3D.h"


#include "Octree.h"


using namespace ParabellumFramework;



namespace ParabellumEngine
{
	namespace Core
	{

		namespace Scene
		{
			//
			//
			// Sector/Part of the sandbox world
			//
			//
			class XYZ_API MapSector
			{
			public:

				// create Map sector
				// vmin and vmax set the MapSector size (2 vertices of bounding box)
				MapSector(Vector3D& vmin, Vector3D& vmax);

				MapSector(MapSector&) = delete;
				~MapSector();


			public:

				// Add component to the map sector
				template<typename T>
				void Add(_IN_ T* pComponent)
				{
					m_octree.Add<T>(pComponent);
				}


				void AddEModel(EModel* pEModel)
				{
					m_octree.AddEModel(pEModel);
				}



				void GetNodes(
					ParabellumFramework::Narray<Octree*>* nodes,
					_IN_ Camera* pCamera
					)
				{
					m_octree.GetNodes(nodes, pCamera);
				}


			//
			// Members
			//
			public:

				// octree which contains properly components
				Octree m_octree;

			};
		}
	}
}


#endif