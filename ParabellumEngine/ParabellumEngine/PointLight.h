#ifndef _POINTLIGHT_H_
#define _POINTLIGHT_H_


#include <string>
#include <memory>

#include "../ParabellumFramework/GraphicsDevice.h"
#include "../ParabellumFramework/ResourceManager.h"
#include "../ParabellumFramework/MathHelper.h"
#include "../ParabellumFramework/RenderTarget.h"

#include "BaseLight.h"


namespace ParabellumEngine
{
	namespace Components
	{
		using namespace ParabellumFramework;
		using namespace ParabellumFramework::Resources;
		using namespace ParabellumFramework::Graphics;

		//
		// Base Light
		//
		class XYZ_API PointLight : public BaseLight
		{

		public:
			PointLight()
			{
				m_isShadowed = false;

			}


			// constructor for point light with shadows
			// resolution - size of shadow map in pixels
			PointLight(GraphicsDevice* pDevice, EUINT32 resolution)
			{
				m_isShadowed = true;

				for (EUINT32 i = 0; i < 6; ++i)
				{
					m_RTShadowMaps[i] = pDevice->CreateRenderTarget(
						GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, resolution, resolution);
				}
			}

			~PointLight()
			{

			}

		private:
			PointLight(PointLight&) = delete;



		//
		// Members
		//
		protected:

			// shadows maps for point light
			// 0 - left plane
			// 1 - right plane
			// 2 - top plane
			// 3 - bottom plane
			// 4 - near plane
			// 5 - far plane
			RenderTarget* m_RTShadowMaps[6];

			EBOOL m_isShadowed;

		//
		// Gets and sets
		//
		public:


			// just an alias for Componen3D::GetScale()->Y
			EFLOAT32 GetRayLength()
			{
				return GetScale()->Y;
			}




		};


	}
}
#endif