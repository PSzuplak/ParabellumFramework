#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_


//
//	Predefs
//
#include "DLLIE.h"

#define INPUT_KEYSSEQUENCESIZE 64
#define INPUT_KEYSTATUSSIZE 256

//
//	Includes
//

#include "../ParabellumFramework/TypesC.h"
#include "../ParabellumFramework/SystemCodes.h"

#include "SystemInstance.h"
#include "../ParabellumFramework/InputDevice.h"
#include "../ParabellumFramework/DXInputDevice.h"


namespace ParabellumEngine
{
	namespace System
	{
		//
		//	GraphicsManager
		//
		class XYZ_API InputManager
		{

		public:
			InputManager();
			InputManager(const InputManager&);
			~InputManager();

		public:

			EUINT32 Initialize(void* hinstance, void* hwnd, EUINT32 flag);
			EUINT32 Release();
			EUINT32 Update();

			EUINT8 IsKeyDown(EUINT32 keyCode);
			EUINT8 WasKeyDown(EUINT32 keyCode);

		private:
			EINT32 m_mousePositionX;
			EINT32 m_mousePositionY;

			IO::InputDevice* m_device;

			// buffor for pressed keys
			EUINT8 m_pressedKeys[512];

			// position in pressedKeys
			EUINT32 m_positionPressedKeys;


		//
		//	Get and Set
		//
		public:
			EINT32 GetMouseX();
			EINT32 GetMouseY();

			IO::InputDevice* GetDevice();
		};


	}
}








#endif