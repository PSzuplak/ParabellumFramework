//****************************************************************************************************/
//
//	Filename : MathRaw.h
//
//	Descritpion : Provides math function for internal C++ types (int, float, double, etc.)
//
//					
//
//
//****************************************************************************************************/

#ifndef __PRAGMAONCE_PARABELLUMUTILS_CONVENCTIONS_H__
#define __PRAGMAONCE_PARABELLUMUTILS_CONVENCTIONS_H__

#include "Configuration.h"
#include "Fixes.h"
#include "Types.h"


// force to use C++ Move except compiler RVO - NOT IMPLEMENTED - checked once and looks like it decreases speed
//#define PF_FORCE_MOVE

#define PU_FASTCALL __fastcall 
#define PU_INLINE __forceinline

#endif

