#ifndef _PRIMITIVESFACTORY_H_
#define _PRIMITIVESFACTORY_H_

//
//	Predefs
//
#include "DLLIE.h"

#include "../ParabellumFramework/TypesC.h"
#include "../ParabellumFramework/ResourceManager.h"
#include "../ParabellumFramework/GraphicsDevice.h"
#include "../ParabellumFramework/Shader.h"
#include "../ParabellumFramework/RenderTarget.h"
#include "../ParabellumFramework/MathHelper.h"
#include "../ParabellumFramework/UnitHelper.h"

#include "RenderQuad.h"
#include "Camera.h"
#include "DirectionalLight.h"


namespace ParabellumEngine
{
	namespace Core
	{
		namespace Graphics
		{
			using namespace ParabellumFramework;
			using namespace ParabellumFramework::Graphics;
			using namespace ParabellumFramework::Resources;
			using namespace ParabellumEngine::Components;





			//
			//
			// PrimitivesFactory
			//
			//
			class XYZ_API PrimitivesFactory
			{
			private:
				PrimitivesFactory(_IN_ GraphicsDevice* pDevice);
				PrimitivesFactory(PrimitivesFactory&) = delete;

			public:
				~PrimitivesFactory();

			public:

				static PrimitivesFactory& Instance(_IN_ GraphicsDevice* pDevice);


			//
			// Methods
			//
			public:

				// generate unit sphere
				static void GenerateSphere(_IN_ GraphicsDevice* pDevice, _IN_ VertexBuffer*& pVBOut, _OUT_ IndexBuffer*& pIBOut, EUINT32 countX, EUINT32 countY, EUINT32 countZ);



			//
			// Memebers
			//
			private:

				VertexBuffer* m_vbBoxUnit;
				IndexBuffer* m_ibBoxUnit;


				VertexBuffer* m_vbCylinder;
				IndexBuffer* m_ibCylinder;

				VertexBuffer* m_vbCylinderHO2;
				IndexBuffer* m_ibCylinderHO2;

			//
			// Gets and Sets
			//
			public:

				VertexBuffer* GetBoxVertexBuffer();
				IndexBuffer* GetBoxIndexBuffer();

				VertexBuffer* GetCylinderVertexBuffer();
				IndexBuffer* GetCylinderIndexBuffer();

				VertexBuffer* GetCylinderHO2VertexBuffer();
				IndexBuffer* GetCylinderHO2IndexBuffer();

			};
		}
	}

}


#endif