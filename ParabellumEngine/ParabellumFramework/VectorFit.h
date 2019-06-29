#ifndef _VECTRORFIT_H_
#define _VECTRORFIT_H_

//
//	Predefs
//
#include "DLLIE.h"
#include "MemorySettings.h"
#include "SystemSettings.h"
#include "Convenctions.h"
#include "TypesC.h"
#include "MathHelper.h"


#define OFFSET_X	0
#define OFFSET_Y	0
#define OFFSET_Z	0
#define OFFSET_W	0

namespace ParabellumFramework
{

	template<unsigned int ArgsCount>
	class VectorFit
	{
	public:
		EFLOAT32* m_args[ArgsCount];


		PF_INLINE friend VectorFit operator+(VectorFit& lhs, VectorFit& rhs)
		{
			VectorFit<ArgsCount> result;

			for (EUINT32 i = 0; i < ArgsCount; ++i)
			{
				result.*m_args[i] = lhs.*m_args[i] + rhs.*m_args[i];
			}

			return result;
		}

	};

}





#endif