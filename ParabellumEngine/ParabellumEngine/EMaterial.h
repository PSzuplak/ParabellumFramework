#ifndef _EMATERIAL_H_
#define _EMATERIAL_H_



#include "../ParabellumFramework/Texture2D.h"






namespace ParabellumEngine
{
	namespace Components
	{
		using namespace ParabellumFramework;


		struct MaterialPropertiesLight
		{
			
		};

		struct MaterialPropertiesSprite
		{

		};


		//
		//
		//
		class XYZ_API EMaterial
		{

		public:
			EMaterial();

			~EMaterial();

		private:
			EMaterial(const EMaterial&) = delete;


		private:

			// use texture as animated sprite
			EBOOL m_isTextureSprite;

			EUINT32 m_spriteHorizontalCellsCount;
			EUINT32 m_spriteVerticalCellsCount;



			// RGBA - Color
			ParabellumFramework::Graphics::Texture2D* m_T2D00;

			// RG - Normal vector ('z' always 1.0), BA - Specular power/intensity
			ParabellumFramework::Graphics::Texture2D* m_T2D01;

			// R = height factor, G - glow, B - reflection, A - emissive/refraction
			ParabellumFramework::Graphics::Texture2D* m_T2D02;

			// is bumped
			EBOOL m_isBumped;

			// is parallax
			EBOOL m_isParallax;

			// is reflects
			EBOOL m_isReflective;

			// is refracts
			EBOOL m_isRefractive;


		public:

			EBOOL IsBumped();

			EBOOL IsParallaxed();

			EBOOL IsReflective();

			EBOOL IsRefractive();

		public:

			void SetTexture00(ParabellumFramework::Graphics::Texture2D* pTexture)
			{
				m_T2D00 = pTexture;
			}

			void SetTexture01(ParabellumFramework::Graphics::Texture2D* pTexture)
			{
				m_T2D01 = pTexture;
			}

			void SetTexture02(ParabellumFramework::Graphics::Texture2D* pTexture)
			{
				m_T2D02 = pTexture;
			}

		};
	}
}
#endif