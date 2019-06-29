#ifndef _UNITHELPER_H_
#define _UNITHELPER_H_

//
//	Predefs
//
#include "DLLIE.h"

//
//
//

#include "TypesC.h"
#include "MathHelper.h"
#include "Vector3D.h"
#include "Vector4D.h"


//	----------------------------------------------------
//	Optimized inline version of some functions
//	----------------------------------------------------

// covert four 8-bit component to 32bit
#define C4U8TOU32(OUTU32, IN0U8, IN1U8, IN2U8, IN3U8) (result = ((EUINT32)IN0U8 << 24) | ((EUINT32)IN1U8 << 16) | ((EUINT32)IN2U8 << 8) | (EUINT32)IN3U8)







namespace ParabellumFramework
{
	namespace Math
	{
		//
		// Some help to deal with different spaces - also color spaces
		//
		class XYZ_API UnitHelper
		{
		private:
			UnitHelper();
			UnitHelper(UnitHelper&) = delete;
			~UnitHelper();

		public:
			

			//
			// 'in' contains 4 components - 8bit per each (unsigned short int)
			// this takes each of this component, div it by 255 (nomralized 0.0f to 1.0f) and save to Vector4D
			static void U32toV4N(EUINT32 in, Vector4D& out);

			// convert Vector4D with normalized components to integer
			// each vector component will be transformed to range 0 to 255, shift and save in 'out'
			static void V4NtoU32(Vector4D& in, EUINT32& out);


			static EUINT32 U8toU32(EBYTE r, EBYTE g, EBYTE b, EBYTE a);


			// treat each 'out' as float bits reproesentation and save them in 'input'
			static void U4toV4(_OUT_ Vector4D* input, _IN_ EUINT32* outX, _IN_ EUINT32* outY, _IN_ EUINT32* outZ, _IN_ EUINT32* outW);

			// treat each component of 'input' as float bits reproesentation and save them in 'out'
			static void V4toU4(_IN_ Vector4D* input, _OUT_ EUINT32* outX, _OUT_ EUINT32*outY, _OUT_ EUINT32* outZ, _OUT_ EUINT32* outW);




			static void ScreenSpaceToUv(Vector2D ss, Vector2D& uv);


			static void UVToScreenSpace(Vector2D uv, Vector2D& ss);



			//static float HueToRGB(float v1, float v2, float vH) 
			static EFLOAT32 HUEtoRGB(Vector3D& hue);

			// change HSL color space stored in 'hsl' param to RGB and save it in 'rgb'
			// hsl.X (hue) must be in degrees (0 to 360)
			static void HSLtoRGB(Vector3D& hsl, Vector3D& rgb);

			


			inline static void CartesianToPolar2D(Vector2D& cart, Vector2D& polar);

			inline static void SphericalToCartesian(
				_IN_ EFLOAT32 ax, _IN_ EFLOAT32 ay, _IN_ EFLOAT32 az,
				_OUT_ EFLOAT32& cx, _OUT_ EFLOAT32& cy, _OUT_ EFLOAT32& cz);

		};


	}

}

#endif