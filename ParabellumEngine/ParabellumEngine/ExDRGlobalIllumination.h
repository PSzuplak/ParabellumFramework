#ifndef _EXDRGLOBALILLUMINATION_H_
#define _EXDRGLOBALILLUMINATION_H_

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
			// Global Illumination
			//
			//
			class XYZ_API ExDRGlobalIllumination
			{

			public:
				ExDRGlobalIllumination();
				~ExDRGlobalIllumination();

			private:
				ExDRGlobalIllumination(ExDRGlobalIllumination&) = delete;



			public:

				void Initialize(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager, EUINT32 width, EUINT32 height);

				void Release(_IN_ GraphicsDevice* pDevice);


				void Draw(
					_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera,
					_IN_ RenderQuad* pRenderQuad,
					_IN_ RenderTarget* pBufferWorldZ, _IN_ RenderTarget* pBufferDiffuse, _IN_ RenderTarget* pBufferNormalH);


				//
				// Memebers
				//
			public:

				Shader* m_SShadowMap;
				Shader* m_SFindUVs;
				RenderTarget* m_RTShadowMap;
				RenderTarget* m_RTUVs;
			};
		}
	}

}


#endif