#ifndef _EXDRSSAO_H_
#define _EXDRSSAO_H_

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
			class XYZ_API ExDRSSAO
			{

			public:
				ExDRSSAO();
				~ExDRSSAO();

			private:
				ExDRSSAO(ExDRSSAO&) = delete;



			public:

				void Initialize(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager, EUINT32 width, EUINT32 height);

				void Release(_IN_ GraphicsDevice* pDevice);


				void Draw(_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera,
					_IN_ RenderQuad* pRenderQuad,
					_IN_ RenderTarget* pGBuffer00, _IN_ RenderTarget* pGBuffer01, _IN_ RenderTarget* pGBuffer02);



			//
			// Memebers
			//
			public:

				// screen width
				EUINT32 m_screenWidth;

				// screen height
				EUINT32 m_screenHeight;

				// intensity
				EFLOAT32 m_intensity;

				//radious
				EFLOAT32 m_radious;

				//bias - differecntce between depthsa of 2 pixels - only closets pixels should be shaded
				EFLOAT32 m_bias;

				//iterations
				EFLOAT32 m_iterations;

				// angle
				EFLOAT32 m_angle;

				Shader* m_ShaderSSAO;

				RenderTarget* m_RTSSAO;
				RenderTarget* m_RTSSAOBlured;


			//
			// Gets and Sets
			//
			public:
				EFLOAT32 GetIntensity();
				EFLOAT32 GetRadious();
				EFLOAT32 GetBias();
				EFLOAT32 GetAngle();

				void SetIntensity(EFLOAT32 val);
				void SetRadious(EFLOAT32 val);
				void SetBias(EFLOAT32 val);
				void SetAngle(EFLOAT32  val);

			};
		}
	}

}


#endif