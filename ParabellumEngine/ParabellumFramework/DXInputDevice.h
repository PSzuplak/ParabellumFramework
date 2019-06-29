#ifndef _DXINPUTDEVICE_H_
#define _DXINPUTDEVICE_H_


#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <stdio.h>

#include <dinput.h>

#include "SystemCodes.h"
#include "TypesC.h"
#include "InputDevice.h"

namespace ParabellumFramework
{
	namespace IO
	{
		//
		// DirectX EFLOAT32 or OpenGL deivce shoud derive from this class
		//
		class XYZ_API DXInputDevice : public InputDevice
		{

					
		public:
			DXInputDevice();
			DXInputDevice(DXInputDevice&);
			~DXInputDevice();

		public:
			EUINT32 Initialize(void* hinstance, void* hwnd, int flag);
			void Release();

			void* GetDevice();
			void* GetKeyboard();

			virtual EUINT32 GetCurrentMouseState();
			virtual EUINT32 GetPreviousMouseState();

			EUINT8 GetKeyboardASCIIState(EUINT32 key);

			

			virtual void Device_Update();

		//
		// Members
		//
		private:
			IDirectInput8* m_dxInput;
			IDirectInputDevice8* m_keyboard;
			IDirectInputDevice8* m_mouse;

		};
	}
}

#endif