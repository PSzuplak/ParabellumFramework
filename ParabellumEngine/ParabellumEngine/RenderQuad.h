#ifndef _RENDERQUAD_H_
#define _RENDERQUAD_H_

//
//	Predefs
//
#include "DLLIE.h"

#include "../ParabellumFramework/TypesC.h"
#include "../ParabellumFramework/ResourceManager.h"
#include "../ParabellumFramework/GraphicsDevice.h"
#include "../ParabellumFramework/Shader.h"
#include "../ParabellumFramework/RenderTarget.h"
#include "../ParabellumFramework/Vector3D.h"


using namespace ParabellumFramework;
using namespace ParabellumFramework::Graphics;

namespace ParabellumEngine
{
	namespace Core
	{
		namespace Graphics
		{

			struct VertexTypeRenderQuad
			{
				EFLOAT32 position[3];
				EFLOAT32 coords[2];
			};

			//
			//
			// Contains information which are needed to know by the others engine components
			//
			//
			class XYZ_API RenderQuad
			{

			public:
				RenderQuad();
				RenderQuad(RenderQuad&) = delete;
				~RenderQuad();


			//
			// Methods
			//
			public:
				void Initialize(_IN_ GraphicsDevice* pDevice, EFLOAT32 x1, EFLOAT32 y1, EFLOAT32 x2, EFLOAT32 y2);
				void Release(_IN_ GraphicsDevice* pDevice);

			//
			// Members
			//
			private:
				VertexBuffer* m_vertexBuffer;
				IndexBuffer* m_indexBuffer;


			//
			// Gets and sets
			//
			public:

				VertexBuffer* GetVertexBuffer();
				IndexBuffer* GetIndexBuffer();


			};
		}
	}

}


#endif