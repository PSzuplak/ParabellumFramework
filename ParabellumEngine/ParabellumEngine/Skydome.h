#ifndef _SKYDOME_H_
#define _SKYDOME_H_


#include <string>
#include <memory>

#include "../ParabellumFramework/GraphicsDevice.h"
#include "../ParabellumFramework/ResourceManager.h"
#include "../ParabellumFramework/BoundingVolumes.h"
#include "../ParabellumFramework/BoundingFrustum.h"
#include "../ParabellumFramework/IntersectCodes.h"
#include "../ParabellumFramework/MathHelper.h"
#include "../ParabellumFramework/Model.h"


#include "Component3D.h"




namespace ParabellumEngine
{
	namespace Components
	{
		using namespace ParabellumFramework;
		using namespace ParabellumFramework::Resources;
		using namespace ParabellumFramework::Graphics;

		//
		//
		//
		class XYZ_API Skydome : public Component3D
		{
		public:

		public:
			Skydome();
			~Skydome();

		private:
			Skydome(const Skydome&) = delete;

		public:

			EUINT32 Initialize(_IN_ PFNS::Graphics::GraphicsDevice* pDevice, _IN_ PFNS::Resources::ResourceManager* pResources);


		//
		// Members
		//
		private:

			// skydome model
			Model* m_MSkydome;

			// skydome shader
			Shader* m_SSkydome;

			Texture2D* m_T2DSkydome;


		//
		// Gets and Sets
		//
		public:

			
			Model* GetModel();

			Shader* GetShader();

			Texture2D* GetTexture();

		};


	}
}
#endif