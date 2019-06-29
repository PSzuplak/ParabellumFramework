#ifndef _MEMORYSETTINGS_H_
#define _MEMORYSETTINGS_H_

#include "SystemSettings.h"


//#define PF_ALLIGNMEMORY


#ifdef PF_COMPILER_TYPE_VISUALSTUDIO2015
	#ifdef PF_ALLIGNMEMORY
		#define PF_ALLIGN(bits) __declspec(align(bits))
	#else
		#define PF_ALLIGN(bits)
	#endif
#else
	#define PF_ALLIGN(bits)
#endif



#endif