#ifndef _PARTICLEVERTEXFORMAT_H_
#define _PARTICLEVERTEXFORMAT_H_


#include "../ParabellumFramework/TypesC.h"

namespace ParabellumEngine
{
	namespace Components
	{

		struct ParticleVertexFormat
		{
			ParabellumFramework::EFLOAT32 Position[3];
			ParabellumFramework::EFLOAT32 UV[2];
			ParabellumFramework::EFLOAT32 Normal[3];
			ParabellumFramework::EFLOAT32 REVERSED;
			ParabellumFramework::EFLOAT32 DeltaDirection[3];
			ParabellumFramework::EFLOAT32 DeltaRotation[3];
			ParabellumFramework::EFLOAT32 Properties[3];
			ParabellumFramework::EFLOAT32 Sprite[3];			// X - row counts, Y - cols count
		};

	}
}

#endif