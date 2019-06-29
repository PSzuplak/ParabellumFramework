#ifndef _EMODEL_H_
#define _EMODEL_H_

#include "../ParabellumFramework/ResourceManager.h"
#include "../ParabellumFramework/BoundingVolumes.h"
#include "../ParabellumFramework/BoundingFrustum.h"
#include "../ParabellumFramework/IntersectCodes.h"
#include "../ParabellumFramework/MathHelper.h"
#include "../ParabellumFramework/Model.h"


#include "Component3D.h"

#include "EModelPart.h"


#include <string>

namespace ParabellumEngine
{
	namespace Components
	{
		using namespace ParabellumFramework;

		//
		// ROOT of modelparts
		//
		class XYZ_API EModel : public Component3D
		{
		public:
			EModel(_IN_ ParabellumFramework::Resources::ResourceManager* pResourceManager, ParabellumFramework::Graphics::Model* pModel);
			virtual ~EModel();

		private:
			EModel(const EModel&) = delete;


		public:
			
			EUINT32 Initialize(_IN_ ParabellumFramework::Resources::ResourceManager* pResourceManager, ParabellumFramework::Graphics::Model* pModel);


		private:

			// Array of models parts 
			EModelPart** m_models;
			EUINT32 m_modelsCount;

		public:

			ParabellumFramework::Graphics::Texture2D* m_t2d;
			ParabellumFramework::Graphics::Texture2D* m_t2n;
			ParabellumFramework::Graphics::Texture2D* m_t2s;
			ParabellumFramework::Graphics::Texture2D* m_t2e;

			bool m_isReflection;
			bool m_isParallax;
			bool m_isRefract;

		private:

		//
		// Gets and Sets
		//
		public:
			ParabellumFramework::Graphics::Texture2D* GetDiffuseMap();

			// Get ModelPart - "no" must be lower than GetModelsCount()
			EModelPart* GetModel(EUINT32 no)
			{
				return m_models[no];
			}

			// how many ModelParts are in this model
			inline EUINT32 GetModelsCount()
			{
				return m_modelsCount;
			}

		};
	}
}
#endif