#ifndef _COMMONMATERIALTYPE_H_
#define _COMMONMATERIALTYPE_H_

#include <list>

#include "TypesC.h"
#include "Vector3D.h"
#include "Texture2D.h"

#include <string>

namespace ParabellumFramework
{
	//
	// Common used Material
	//
	struct CommonMaterialType
	{
		EUINT32 properties01[16];
		EFLOAT32 properties02[16];


		char pathTextureDiffuse[256];
		char pathTextureNormal[256];		// path to normal and lightproperties
		char pathTextureLight[256];			// path to normal and lightproperties
		char pathTextureExtenstion[256];	// path to texture with extension properties		

		CommonMaterialType()
		{
		}

		~CommonMaterialType()
		{
		}
	};



}

#endif