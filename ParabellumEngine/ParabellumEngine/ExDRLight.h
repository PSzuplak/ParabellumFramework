#ifndef _EXDRLIGHT_H_
#define _EXDRLIGHT_H_

//
//	Predefs
//
#include "DLLIE.h"

#include "../ParabellumFramework/TypesC.h"
#include "../ParabellumFramework/ResourceManager.h"
#include "../ParabellumFramework/GraphicsDevice.h"
#include "../ParabellumFramework/Shader.h"
#include "../ParabellumFramework/RenderTarget.h"
#include "../ParabellumFramework/Model.h"

#include "RenderQuad.h"
#include "RenderArea.h"
#include "RenderTree.h"
#include "Camera.h"

#include "DirectionalLight.h"
#include "PointLight.h"

#include "SceneCart.h"
#include "FrustumOutput.h"

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
			// Deferred Lighting
			//
			//
			class XYZ_API ExDRLight
			{
			public:
				

			public:
				ExDRLight();
				~ExDRLight();

			private:
				ExDRLight(ExDRLight&) = delete;



			public:

				void Initialize(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager, EUINT32 width, EUINT32 height);

				void Release(_IN_ GraphicsDevice* pDevice);


				void Draw(_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera,
					_IN_ RenderQuad* pRenderQuad,
					_IN_ RenderTarget* pGBuffer00, _IN_ RenderTarget* pGBuffer01, _IN_ RenderTarget* pGBuffer02);


				// draw ShadowMap for Directional Light
				void DrawDirectionalLightSolidShadow(
					_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera, _IN_ Utiles::FrustumOutput* pFrustumOutput);



				//
				// Draw Point lights
				void DrawPointLights(
					_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera, _IN_ RenderQuad* pRenderQuad,
					_IN_ RenderTarget* pGBuffer00, _IN_ RenderTarget* pGBuffer01, _IN_ RenderTarget* pGBuffer02,
					_IN_ PointLight* pPointLights, EUINT32 pointLightsCount
					);




			private:



			//
			// Memebers
			//
			public:

				EUINT32 m_screenWidth;
				EUINT32 m_screenHeight;

				DirectionalLight m_directionalLight;

				// shader to perform ONLY ambient lighting - the area is fully covered
				Shader* m_SDirectionalLightBasic;

				Shader* m_SDirectionalLight;


				Shader* m_SShadowMap_Solid;
				Shader* m_SShadowMap_Transparent;

				Shader* m_SBlurLightMap;

				// RT for lightmap
				// R - light color, RESERVED (Float4 as Float)
				// G - X,Y components of reflected light direction by normal (each X,Y as Float16)
				// B - Z component of eflected light direction by normal, RESERVED (Z and RESERVED as Float16)
				// A - RESERVED
				RenderTarget* m_RTLightMapSolid;

				RenderTarget* m_RTLightMapSolidBlured;

				// array of pointer to Shadow Maps
				RenderTarget** m_ARTShadowDepth;

				// number of shadow maps for solid objects - number of m_ARTShadowDepth
				EUINT32 m_solidShadowMapsCount;


				// model for point light
				Model* m_MPointLight;


				Shader* m_SPointLight;


				RenderTarget* m_RTPointLightSolid;





				EUINT32 m_frameCounter;

				//
				// Settgins
				// 

				// contains number of frame when the ShadowMap should be created
				EUINT32 m_settingsCSMUpdateCounter[9];




				///////////////////////////////////////////////////////////////////////////////
				// GI
				//Shader* m_SShadowMap;
				Shader* m_SFindUVs;
				RenderTarget* m_RTShadowMap;
				RenderTarget* m_RTUVs;
			};
		}
	}

}


#endif