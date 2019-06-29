#ifndef _BASELIGHT_H_
#define _BASELIGHT_H_


#include <string>
#include <memory>

#include "../ParabellumFramework/GraphicsDevice.h"
#include "../ParabellumFramework/ResourceManager.h"
#include "../ParabellumFramework/MathHelper.h"

#include "Component3D.h"


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
		class XYZ_API BaseLight : public Component3D
		{
		protected:
			BaseLight();
			virtual ~BaseLight();

		private:
			BaseLight(BaseLight&) = delete;

		//
		// Members
		//
		protected:

			// intensity of the light
			EFLOAT32 m_intensity;

			// color of the light
			Vector4D m_lightColor;			

			// is this liht throwing shadows
			EBOOL m_isThrowShadows;


		//
		// gets and sets
		//
		public:
			Vector4D* GetColor();
			EFLOAT32 GetIntensity();
			EBOOL IsThrowingShadows();


			void SetColorRGBA(EUINT32 r, EUINT32 g, EUINT32 b, EUINT32 a)
			{
				m_lightColor.X = (EFLOAT32)r / 255.0f;
				m_lightColor.Y = (EFLOAT32)g / 255.0f;
				m_lightColor.Z = (EFLOAT32)b / 255.0f;
				m_lightColor.W = (EFLOAT32)a / 255.0f;
			}


		};


	}
}
#endif