#ifndef _GRASS_H_
#define _GRASS_H_


#include "../ParabellumFramework/MathHelper.h"
#include "../ParabellumFramework/GraphicsDevice.h"
#include "../ParabellumFramework/VertexBuffer.h"
#include "../ParabellumFramework/IndexBuffer.h"
#include "../ParabellumFramework/Texture2D.h"

#include "Component3D.h"
#include "GrassVertexFormat.h"

namespace ParabellumEngine
{
	namespace Components
	{
		using namespace ParabellumFramework;
		using namespace ParabellumFramework::Graphics;
		//
		//
		//
		class XYZ_API Grass : public Component3D
		{
		public:

		public:
			Grass()
			{

			}

			~Grass()
			{

			}

		private:
			Grass(const Grass&) = delete;


		public:

			void GenerateGrass(_IN_ GraphicsDevice* pDevice, EFLOAT32 sizeX, EFLOAT32 sizeZ, EUINT32 elementsCountInRow, EFLOAT32 minHeight, EFLOAT32 maxHeight, EFLOAT32 curveMin, EFLOAT32 curveMax)
			{
				EFLOAT32 dx = sizeX / (EFLOAT32)elementsCountInRow;
				EFLOAT32 dz = sizeZ / (EFLOAT32)elementsCountInRow;

				

				EFLOAT32 posX = 0.0f;
				EFLOAT32 posZ = 0.0f;

				EUINT32 verticesCountY = 10;

				EUINT32 verticesCount = elementsCountInRow * elementsCountInRow * verticesCountY;
				GrassVertexFormat* vertices = new GrassVertexFormat[verticesCount];

				EUINT32 indicesCount = elementsCountInRow * elementsCountInRow * 8 * 3;
				EUINT32* indices = new EUINT32[indicesCount];

		
				//
				// set vertices
				//
				EUINT32 elemCounter = 0;

				EUINT32 lodCounter = 1;	// 1 because distance to grass is always positive


				for (EUINT32 i = 0; i < verticesCount; i += verticesCountY)
				{
					// generate curve
					EFLOAT32 curvePower = ParabellumUtils::Math::RandF(curveMin, curveMax);

					EFLOAT32 curveDirectionX = ParabellumUtils::Math::RandF(0.0f, 2.0f) - 1.0f;
					EFLOAT32 curveDirectionZ = ParabellumUtils::Math::RandF(0.0f, 2.0f) - 1.0f;

					curveDirectionX *= 0.2f;
					curveDirectionZ *= 0.2f;

					EFLOAT32 dh = ParabellumUtils::Math::RandF(minHeight, maxHeight) / (float)verticesCountY;

					EFLOAT32 gx = ParabellumUtils::Math::FastSin(ParabellumUtils::Math::RandF(1.0f, 10.0f)) / 20.0f;
					EFLOAT32 gz = ParabellumUtils::Math::FastSin(ParabellumUtils::Math::RandF(1.0f, 10.0f)) / 20.0f;

					EUINT32 j = 0;
					for (j = 0; j < verticesCountY; j +=2)
					{
						
						vertices[i + j + 0].UV[0] = 0.0f;
						vertices[i + j + 0].UV[1] = (float)j / (float)(verticesCountY-1);
						vertices[i + j + 0].Position[0] = gx + posX - (dx / 4) + vertices[i + j + 0].UV[1] * vertices[i + j + 0].UV[1] * curveDirectionX;
						vertices[i + j + 0].Position[1] = dh * j;
						vertices[i + j + 0].Position[2] = gz + posZ + vertices[i + j + 0].UV[1] * vertices[i + j + 0].UV[1] * curveDirectionZ;

						vertices[i + j + 0].Properties[0] = (float)lodCounter;
						vertices[i + j + 0].Properties[1] = ParabellumUtils::Math::RandF(0.25f, 0.8f);

						vertices[i + j + 1].UV[0] = 1.0f;
						vertices[i + j + 1].UV[1] = (float)j / (float)(verticesCountY - 1);
						vertices[i + j + 1].Position[0] = posX + (dx / 4) + vertices[i + j + 1].UV[1] * vertices[i + j + 1].UV[1] * curveDirectionX;
						vertices[i + j + 1].Position[1] = dh * j;
						vertices[i + j + 1].Position[2] = posZ + vertices[i + j + 1].UV[1] * vertices[i + j + 1].UV[1] * curveDirectionZ;

						vertices[i + j + 1].Properties[0] = (float)lodCounter;
						vertices[i + j + 1].Properties[1] = ParabellumUtils::Math::RandF(0.25f, 0.8f);

					}


					elemCounter++;

					if (elemCounter % elementsCountInRow == 0)
					{
						posX = 0.0f;
						posZ += dz;
					}

					posX += dx;

					lodCounter++;

					if (lodCounter > 8) lodCounter = 1;
				}


				//
				// set indices
				//
				EUINT32 ind = 0;

				for (EUINT32 i = 0; i < verticesCount; i += 10)
				{
					// 0
					indices[ind++] = i + 0;
					indices[ind++] = i + 1;
					indices[ind++] = i + 2;

					// 1
					indices[ind++] = i + 1;
					indices[ind++] = i + 3;
					indices[ind++] = i + 2;

					// 2
					indices[ind++] = i + 2;
					indices[ind++] = i + 3;
					indices[ind++] = i + 4;

					// 3
					indices[ind++] = i + 3;
					indices[ind++] = i + 5;
					indices[ind++] = i + 4;

					// 4
					indices[ind++] = i + 4;
					indices[ind++] = i + 5;
					indices[ind++] = i + 6;

					// 5
					indices[ind++] = i + 5;
					indices[ind++] = i + 7;
					indices[ind++] = i + 6;

					// 6
					indices[ind++] = i + 6;
					indices[ind++] = i + 7;
					indices[ind++] = i + 8;

					// 7
					indices[ind++] = i + 7;
					indices[ind++] = i + 9;
					indices[ind++] = i + 8;
				}



				m_vertexBuffer = pDevice->CreateVertexBuffer(vertices, sizeof(GrassVertexFormat), verticesCount);
				m_indexBuffer = pDevice->CreateIndexBuffer(indices, indicesCount);



				// clear
				delete[] vertices;
				delete[] indices;

			}


		//
		// Members
		//
		public:

			VertexBuffer* m_vertexBuffer;
			IndexBuffer* m_indexBuffer;

		};
	}
}
#endif