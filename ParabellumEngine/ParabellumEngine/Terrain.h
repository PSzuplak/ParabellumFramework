#ifndef _TERRAIN_H_
#define _TERRAIN_H_


#include <string>
#include <memory>

#include "../ParabellumFramework/GraphicsDevice.h"
#include "../ParabellumFramework/ResourceManager.h"
#include "../ParabellumFramework/BoundingVolumes.h"
#include "../ParabellumFramework/BoundingFrustum.h"
#include "../ParabellumFramework/IntersectCodes.h"
#include "../ParabellumFramework/MathHelper.h"
#include "../ParabellumFramework/Model.h"
#include "../ParabellumFramework/Texture2D.h"

#include "Component3D.h"
#include "TerrainVertexFormat.h"
#include "TerrainCell.h"



namespace ParabellumEngine
{
	namespace Components
	{
		using namespace ParabellumFramework;
		using namespace ParabellumFramework::Resources;
		using namespace ParabellumFramework::Graphics;

		//
		// Terrain - create terrain
		//
		class XYZ_API Terrain : public Component3D
		{
		public:

			// create terrain from RAW32 heightmap
			// assumes that height and width are equal - size of image / 32bits
			Terrain(_IN_ ParabellumFramework::Graphics::GraphicsDevice* pDevice, _IN_ ParabellumFramework::Resources::ResourceManager* pResources,
					const char* fileName,
					EUINT32 slicesCountX, EUINT32 slicesCountY, EFLOAT32 distanceBetweenVertices, EFLOAT32 maxHeight
				);

			~Terrain();

		private:
			Terrain(const Terrain&) = delete;

		//
		// Members
		//
		public:

			TerrainCell* m_terrainCells;

			EUINT32 m_slicesCountX;
			EUINT32 m_slicesCountY;


			EUINT32 m_width;
			EUINT32 m_height;

		};


	}
}
#endif