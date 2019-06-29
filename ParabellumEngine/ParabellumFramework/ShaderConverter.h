#ifndef _SHADERCONVERTER_H_
#define _SHADERCONVERTER_H_


//
//	Predefs
//
#include "DLLIE.h"


//
//	Includes
//
#include <fstream>
#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <cstdlib>
#include <stdio.h>

#include "TypesC.h"

#include "DebugManager.h"

#include <iostream>
#include <d3d11.h>

#include "MathHelper.h"

#include "GraphicsTypes.h"


namespace ParabellumFramework
{

	using namespace ParabellumFramework::Graphics;

	//
	//	Shader
	//
	class XYZ_API ShaderConverter
	{
		public:
			ShaderConverter();
			~ShaderConverter();

		private:
			ShaderConverter(const ShaderConverter&);

		public:
			// convert shader txt code to ...
			virtual EUINT32 Convert(_IN_ char* filenameIn, _IN_ char* filenameOut);


			void SaveString(std::string& str, FILE* stream)
			{
				EUINT32 len = str.length();

				fwrite(&len, sizeof(EUINT32), 1, stream);
				fwrite(str.c_str(), sizeof(char), len, stream);
			}


	};
}








#endif