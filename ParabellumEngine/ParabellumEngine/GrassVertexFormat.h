#ifndef _GRASSVERTEXFORMAT_H_
#define _GRASSVERTEXFORMAT_H_


#include "../ParabellumFramework/TypesC.h"

namespace ParabellumEngine
{
	namespace Components
	{

		struct GrassVertexFormat
		{
			ParabellumFramework::EFLOAT32 Position[3];
			ParabellumFramework::EFLOAT32 UV[2];
			ParabellumFramework::EFLOAT32 Properties[2];	// [0] - min distance from camera to draw [1] - reversed
			//ParabellumFramework::EFLOAT32 Normal[3];
			//ParabellumFramework::EFLOAT32 Tangent[3];
			//ParabellumFramework::EFLOAT32 Binormal[3];
		};

	}
}

#endif