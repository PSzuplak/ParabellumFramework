#ifndef _PUIEVENTMOUSE_H_
#define _PUIEVENTMOUSE_H_


#include "PUIEvent.h"

//
//	ParabellumFramework dependieces
//
#include "../ParabellumFramework/DLLIE.h"
#include "../ParabellumFramework/TypesC.h"
#include "../ParabellumFramework/IOCodes.h"

namespace ParabellumUserInterface
{

	//
	//	Drawable PUI object
	//
	struct XYZ_API PUIEventMouse
	{
		ParabellumFramework::EFLOAT32 MouseX;
		ParabellumFramework::EFLOAT32 MouseY;
		ParabellumFramework::IO::MouseState MouseState;
	};

}

#endif