#ifndef _EMODELPART_H_
#define _EMODELPART_H_

#include "../ParabellumFramework/ResourceManager.h"
#include "../ParabellumFramework/BoundingVolumes.h"
#include "../ParabellumFramework/BoundingFrustum.h"
#include "../ParabellumFramework/IntersectCodes.h"
#include "../ParabellumFramework/MathHelper.h"
#include "../ParabellumFramework/Model.h"
#include "../ParabellumFramework/ModelPart.h"
#include "../ParabellumFramework/Texture2D.h"

#include "Component3D.h"
#include "EMaterial.h"

namespace ParabellumEngine
{
	namespace Components
	{
		using namespace ParabellumFramework;
		using namespace ParabellumFramework::Graphics;

		//
		// Wraper on ModelPart - each model part should be an instance of Component3D. ParabellumFramework does not provide such
		// option, so it should be implemented in engine. NOTE that ModelPart can be ANYTHING (mesh, light etc)
		//
		class XYZ_API EModelPart : public Component3D
		{
		public:
			Vector4D boxStartEnd;

			EFLOAT32 boxDepth;

		public:
			EModelPart(_IN_ ParabellumFramework::Resources::ResourceManager* pResourceManager, Graphics::ModelPart* pPart);
			EModelPart(const EModelPart&) = delete;
			virtual ~EModelPart();
			
		public:
			ModelPart* m_modelPart;	
			EMaterial m_meterial;

			Texture2D* m_T2DTextures[4];


		public:

			inline VertexBuffer* GetVertexBuffer()
			{
				return (VertexBuffer*)Model::GetModelObject(m_modelPart->m_vbIndex);
			}

			inline IndexBuffer* GetIndexBuffer()
			{
				return (IndexBuffer*)Model::GetModelObject(m_modelPart->m_ibIndex);
			}
		

			inline Graphics::ModelPart* GetModelPart()
			{
				return m_modelPart;
			}

			inline ParabellumFramework::Graphics::Texture2D* GetDiffuseMap()
			{
				return m_T2DTextures[0];
			}

			inline ParabellumFramework::Graphics::Texture2D* GetNormalMap()
			{
				return m_T2DTextures[1];
			}

			inline ParabellumFramework::Graphics::Texture2D* GetTGEMap()
			{
				return m_T2DTextures[2];
			}

			inline ParabellumFramework::Graphics::Texture2D* GetHeightMap()
			{
				return m_T2DTextures[3];
			}

		};
	}
}
#endif