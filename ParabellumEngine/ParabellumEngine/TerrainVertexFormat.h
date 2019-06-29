#ifndef _TERRAINVERTEXFORMAT_H_
#define _TERRAINVERTEXFORMAT_H_


#include "../ParabellumFramework/TypesC.h"

namespace ParabellumEngine
{
	namespace Components
	{

		struct TerrainVertexFormat
		{
			ParabellumFramework::EFLOAT32 Position[3];
			ParabellumFramework::EFLOAT32 AbsoluteUV[2];
			ParabellumFramework::EFLOAT32 RelativeUV[2];		//XY- uvs, Z- if 0.5 then RUV.X is swapped, W - if 0.5f then RUV.y is swapped
			ParabellumFramework::EFLOAT32 Normal[3];
			ParabellumFramework::EFLOAT32 Tangent[3];
			ParabellumFramework::EFLOAT32 Binormal[3];
		};

	}
}

#endif