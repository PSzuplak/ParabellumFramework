#ifndef _SKYLIKE_H_
#define _SKYLIKE_H_


#include <string>
#include <memory>

#include "../ParabellumFramework/GraphicsDevice.h"
#include "../ParabellumFramework/ResourceManager.h"
#include "../ParabellumFramework/MathHelper.h"
#include "../ParabellumFramework/RenderTarget.h"

#include "Component3D.h"
#include "Camera.h"
#include "RenderQuad.h"
#include "PrimitivesFactory.h"

using namespace ParabellumFramework;
using namespace ParabellumFramework::Resources;
using namespace ParabellumFramework::Graphics;
using namespace ParabellumEngine::Core::Graphics;


namespace ParabellumEngine
{
	namespace Components
	{

		//
		// Base Light
		//
		class XYZ_API SkyLike : public Component3D
		{
		public:
			SkyLike(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager);
			SkyLike(SkyLike&) = delete;
			~SkyLike();




		//
		// Members
		//
		public:

			// pT2DCloud may be also a texture of planet or star
			void AddCloud(_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera, _IN_ Texture2D* pT2DCloud, Vector2D position, EFLOAT32 size);

			void CreateSkydomeTexture(_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera);


			void DrawMerged(_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera);

		//
		// Members
		//
		public:

		
			// colors used to define sky color
			Vector4D m_skyGrad[2];



			EBOOL m_isSunEnabled;

			// world position of sun - should be from (-1,-1) to (+1,+1)
			Vector2D m_sunPosition; 

			EFLOAT32 m_sunSize;

			Vector4D m_sunGradColor[2];



			RenderQuad m_renderQuad;

			RenderQuad m_placementQuad;


			// skydome model
			Model* m_MSkydome;


			RenderTarget* m_RTSun;
			RenderTarget* m_RTClouds;
			RenderTarget* m_RTMerged;
			RenderTarget* m_RTFinal;

			Shader* m_SSetBuffer;
			Shader* m_SSun;
			Shader* m_SAddCloud;
			Shader* m_SMerge;
			Shader* m_SDrawSkydome;

			Texture2D* tex;
		//
		// Gets and sets
		//
		public:





		};


	}
}
#endif