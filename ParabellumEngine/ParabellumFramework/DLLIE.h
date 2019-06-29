#ifndef _DLLIE_H_
#define _DLLIE_H_

#include "SystemSettings.h"

#ifdef DLLIMPORT
	#ifdef PF_COMPILER_TYPE_VISUALSTUDIO
		#define XYZ_API __declspec(dllimport)
	#else
		#define XYZ_API
	#endif
#else
	#ifdef PF_COMPILER_TYPE_VISUALSTUDIO
		#define XYZ_API __declspec(dllexport)
	#else
		#define XYZ_API
	#endif
#endif //DLLIMPORT


#endif //_DLLIE_H_