#ifndef _TEXTURECONVERTER_H_
#define _TEXTURECONVERTER_H_


//
//	Predefs
//
#include "DLLIE.h"


//
//	Includes
//
#include "DebugManager.h"

#include "TypesC.h"
#include "SystemCodes.h"
#include "GraphicsTypes.h"
#include "UnitHelper.h"

#include "ParabellumImage.h"

#include <string>

namespace ParabellumFramework
{
		class XYZ_API TextureConverter
		{

		public:
			TextureConverter();
			~TextureConverter();

		public:

			// convert BMP file to PFT
			void convertBMP2PFT(const char* in, const char* out);

			// covert 4 BMP files (diffuse, normal map, specular map, height map) tp pft
			// create OUT file with four 32bit channel. In OUR R channel i will be stored diffuse,
			// in OUT G - normal map, in OUT B - specular map, in OUT A - height map
			// shader will nee only 1 sample
			// in1, in2, in3 MUST be the same size as in0
			void convertFourBMP2PFT(const char* in0, const char* in1, const char* in2, const char* in3, const char* out);


		};

}








#endif