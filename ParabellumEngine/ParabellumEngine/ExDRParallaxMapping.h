#ifndef _EXDRPARALLAXMAPPING_H_
#define _EXDRPARALLAXMAPPING_H_

//
//	Predefs
//
#include "DLLIE.h"

#include "../ParabellumFramework/TypesC.h"
#include "../ParabellumFramework/ResourceManager.h"
#include "../ParabellumFramework/GraphicsDevice.h"
#include "../ParabellumFramework/Shader.h"
#include "../ParabellumFramework/RenderTarget.h"

#include "RenderQuad.h"
#include "RenderArea.h"
#include "RenderTree.h"

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
			// Parallax Mapping
			//
			//
			class XYZ_API ExDRParallaxMapping
			{

			public:
				ExDRParallaxMapping();
				~ExDRParallaxMapping();

			private:
				ExDRParallaxMapping(ExDRParallaxMapping&) = delete;



			public:

				void Initialize(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager, EUINT32 width, EUINT32 height);

				void Release(_IN_ GraphicsDevice* pDevice);


				void Draw(
					_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera,
					_IN_ RenderQuad* pRenderQuad,
					_IN_ RenderTarget* pGBuffer00, _IN_ RenderTarget* pGBuffer01, _IN_ RenderTarget* pGBuffer02);

				void Draw(
					_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera,
					_IN_ RenderTree* pRenderTree,
					_IN_ RenderTarget* pGBuffer00, _IN_ RenderTarget* pGBuffer01, _IN_ RenderTarget* pGBuffer02);

				//
				// Memebers
				//
			public:


				Shader* m_SParallaxMapping;

				RenderTarget* m_RTBuffer0;
				RenderTarget* m_RTBuffer1;
			};
		}
	}

}


#endif