#ifndef _TERRAINCELL_H_
#define _TERRAINCELL_H_


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




namespace ParabellumEngine
{
	namespace Components
	{
		using namespace ParabellumFramework;
		using namespace ParabellumFramework::Resources;
		using namespace ParabellumFramework::Graphics;

		//
		// TerrainCell - can be created only inside Terrain
		//
		class XYZ_API TerrainCell : public Component3D
		{
			//
			// Friendship
			//
			friend class Terrain;

		public:
			TerrainCell();
			~TerrainCell();

		private:
			TerrainCell(const TerrainCell&) = delete;




		public:

			// intiialize TerrainCell with Heightmap stored in pTexture 
			// parametrs begin and width/height describes which slice of the texture should be consider
			EUINT32 Initialize(
				_IN_ ParabellumFramework::Graphics::GraphicsDevice* pDevice,
				_IN_ Texture2D* pTexture, EFLOAT32 distanceBetweenVertices, EFLOAT32 maxHeight,
				EUINT32 beginX = 0, EUINT32 beginY = 0, EUINT32 width = 0, EUINT32 height = 0
				);


			// intiialize TerrainCell with Heightmap stored in memory
			// parametrs begin and width/height describes which slice of the texture should be consider
			// optimizeDSlope - vertice will be removed if its positon differs less than optimizeDSlope from its neighbours 
			// promiseOptimization - all neighbours will be replace by one Quad - 75% optimization, but may cause artifacts - should be VERY small!
			EUINT32 Initialize(
				_IN_ ParabellumFramework::Graphics::GraphicsDevice* pDevice,
				_IN_ EFLOAT32* heightmap, EUINT32 heightmapWidth, EUINT32 heightmapHeight,
				EFLOAT32 distanceBetweenVertices, EFLOAT32 maxHeight,
				EUINT32 sliceBeginX, EUINT32 sliceBeginY, EUINT32 sliceWidth, EUINT32 sliceHeight,
				EFLOAT32 optimizedSlope
				);


		//
		// Members
		//
		public:

			VertexBuffer* m_vertexBuffer;

			IndexBuffer* m_indexBuffer;	


			//TerrainVertexFormat* m_vertices;
			//EUINT32* m_indices;

			//EUINT32 m_verticesCount;
			//EUINT32 m_indicesCount;

			EUINT32 m_vertexCountX;
			EUINT32 m_vertexCountZ;


			EUINT32* m_lodLevels;

			EFLOAT32 m_startHeight;			// value that will be add to finaly height (from heightmap)


			EFLOAT32 m_minHeight;
			EFLOAT32 m_maxHeight;

			EFLOAT32* m_heights;				// array of size m_verticesCount which store terrain heights

			EFLOAT32 m_distanceBetweenVertices;	// distance between 2 neighbour vertices


			Texture2D* m_T2Dtex;

		//
		// Get and Sets
		//
		public:
		
			ParabellumFramework::Graphics::VertexBuffer*
				ParabellumEngine::Components::TerrainCell::GetVertexBuffer();


			ParabellumFramework::Graphics::IndexBuffer*
				ParabellumEngine::Components::TerrainCell::GetIndexBuffer();

		};


	}
}
#endif