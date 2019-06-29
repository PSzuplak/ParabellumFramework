#ifndef _PARTICLES_H_
#define _PARTICLES_H_


#include <string>
#include <memory>

#include "../ParabellumFramework/GraphicsDevice.h"
#include "../ParabellumFramework/VertexBuffer.h"
#include "../ParabellumFramework/IndexBuffer.h"
#include "../ParabellumFramework/ResourceManager.h"
#include "../ParabellumFramework/BoundingVolumes.h"
#include "../ParabellumFramework/BoundingFrustum.h"
#include "../ParabellumFramework/IntersectCodes.h"
#include "../ParabellumFramework/MathHelper.h"
#include "../ParabellumFramework/Model.h"


#include "Component3D.h"

#include "ParticleVertexFormat.h"


namespace ParabellumEngine
{
	namespace Components
	{
		using namespace ParabellumFramework;
		using namespace ParabellumFramework::Resources;
		using namespace ParabellumFramework::Graphics;

		//
		// Particles - for "static" particles - for objects which do not move like "static" fire
		//
		class XYZ_API Particles : public Component3D
		{
		public:

		public:
			Particles();
			~Particles();

		private:
			Particles(const Particles&) = delete;

		public:

			EUINT32 Initialize(_IN_ PFNS::Graphics::GraphicsDevice* pDevice, _IN_ PFNS::Resources::ResourceManager* pResources,
				EUINT32 particlesCount
				
				);


		//
		// Members
		//
		public:


			// skydome shader
			Shader* m_SParticle;

			Texture2D* m_T2DDiffuse;

			EUINT32 m_particlesCount;
			
			VertexBuffer* m_vertexBuffer;
			IndexBuffer* m_indexBuffer;


			EUINT32 m_timeElapsed;

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