#ifndef _EXDRHDR_H_
#define _EXDRHDR_H_

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
#include "Camera.h"
#include "DirectionalLight.h"




using namespace ParabellumFramework;
using namespace ParabellumFramework::Graphics;
using namespace ParabellumFramework::Resources;
using namespace ParabellumEngine::Components;

namespace ParabellumEngine
{
	namespace Core
	{
		namespace Graphics
		{

			//
			//
			// SSAO
			//
			//
			class XYZ_API ExDRHDR
			{

			public:
				ExDRHDR();
				~ExDRHDR();

			private:
				ExDRHDR(ExDRHDR&) = delete;



			public:

				void Initialize(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager, EUINT32 width, EUINT32 height);

				void Release(_IN_ GraphicsDevice* pDevice);


				void Draw(_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera,
					_IN_ RenderQuad* pRenderQuad, _IN_ RenderTarget* pGBUffer0, _IN_ RenderTarget* pBufferLight,
					EFLOAT32 averageLuminance, EFLOAT32 lowestLuminance, EFLOAT32 highestLuminance
					);



				//
				// Memebers
				//
			public:

				// screen width
				EUINT32 m_screenWidth;

				// screen height
				EUINT32 m_screenHeight;


				EFLOAT32 m_prevAvgLum;
				EFLOAT32 m_prevMinLum;
				EFLOAT32 m_prevMaxLum;



				Shader* m_ShaderHDR;
				Shader* m_SBlurVertical;


				RenderTarget* m_RTHDR;
				RenderTarget* m_RTToneMap;
				RenderTarget* m_RTToneMapBluredVertical;
				RenderTarget* m_RTToneMapBluredHorizontal;

			};
		}
	}

}


#endif