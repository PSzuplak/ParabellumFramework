#ifndef _EXDRVOLUMETRICLIGHT_H_
#define _EXDRVOLUMETRICLIGHT_H_

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
			// SSAO
			//
			//
			class XYZ_API ExDRVolumetricLight
			{

			public:
				ExDRVolumetricLight();
				~ExDRVolumetricLight();

			private:
				ExDRVolumetricLight(ExDRVolumetricLight&) = delete;



			public:

				void Initialize(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager, EUINT32 width, EUINT32 height);

				void Release(_IN_ GraphicsDevice* pDevice);


				void Draw(_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera, _IN_ DirectionalLight* directionalLight,
					_IN_ RenderQuad* pRenderQuad, _IN_ RenderTree* pRenderTree,
					_IN_ RenderTarget* pBufferWorldFar, _IN_ RenderTarget* pLightDepthMap);



				//
				// Memebers
				//
			public:

				// screen width
				EUINT32 m_screenWidth;

				// screen height
				EUINT32 m_screenHeight;

				Shader* m_SVolumetricLight;

				Shader* m_SVolumetricLightPolish;

				RenderTarget* m_RTVolumetricLight;

				// blured, depth comparsion etc.
				RenderTarget* m_RTVolumetricLightPolished;

			};
		}
	}

}


#endif