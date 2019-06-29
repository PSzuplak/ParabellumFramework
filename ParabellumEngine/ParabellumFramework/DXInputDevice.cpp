#include "stdafx.h"
#include "DXInputDevice.h"


//	----------------------------------------------------
//		Constructors and destructors
//	----------------------------------------------------
ParabellumFramework::IO::DXInputDevice::DXInputDevice()
{
}

ParabellumFramework::IO::DXInputDevice::DXInputDevice(DXInputDevice&)
{
}

ParabellumFramework::IO::DXInputDevice::~DXInputDevice()
{
}


//	----------------------------------------------------
//		Methods
//	----------------------------------------------------

ParabellumFramework::EUINT32 
ParabellumFramework::IO::DXInputDevice::Initialize(void* hinstance, void* hwnd, int flag)
{
	HRESULT result;

	//
	result = DirectInput8Create((HINSTANCE)hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_dxInput, NULL);
	if (FAILED(result))
		return SystemCodes::ERROR_INIT;

	//
	result = m_dxInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
		return SystemCodes::ERROR_INIT;

	//
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
		return SystemCodes::ERROR_INIT;

	//
	result = m_keyboard->SetCooperativeLevel((HWND)hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
		return SystemCodes::ERROR_INIT;


	//
	result = m_keyboard->Acquire();
	if (FAILED(result))
		return SystemCodes::ERROR_INIT;

	//
	result = m_dxInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
		return SystemCodes::ERROR_INIT;

	//
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
		return SystemCodes::ERROR_INIT;

	//
	result = m_mouse->SetCooperativeLevel((HWND)hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
		return SystemCodes::ERROR_INIT;

	//
	result = m_mouse->Acquire();
	if (FAILED(result))
		return SystemCodes::ERROR_INIT;


	return SystemCodes::SUCCESS;
}

//
//
//	Release input EFLOAT32s
//
//
void 
ParabellumFramework::IO::DXInputDevice::Release()
{
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	if (m_dxInput)
	{
		m_dxInput->Release();
		m_dxInput = 0;
	}

}


//
//
//	Update state of input EFLOAT32s
//
//
void 
ParabellumFramework::IO::DXInputDevice::Device_Update()
{
	HRESULT result;

	result = m_keyboard->GetDeviceState(256, (LPVOID)GetAddressOfKeyboardState());


	if (FAILED(result))
	{
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			m_keyboard->Acquire();
	}

	// update mouse
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_currentMouseState);
	if (FAILED(result))
	{
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			m_mouse->Acquire();
	}

	MoveMousePosition(m_currentMouseState.lX, m_currentMouseState.lY);
}



//	----------------------------------------------------
//		Gets and Sets
//	----------------------------------------------------
void* 
ParabellumFramework::IO::DXInputDevice::GetDevice()
{
	return m_dxInput;
}

void* 
ParabellumFramework::IO::DXInputDevice::GetKeyboard()
{
	return m_keyboard;
}




ParabellumFramework::EUINT32
ParabellumFramework::IO::DXInputDevice::GetCurrentMouseState()
{
	EUINT32 result = 0;

	if (m_currentMouseState.rgbButtons[0] & 0x80)
	{
		PF_FLAG_SET(result, PFM_LEFT_DOWN);
	}
	
	if (m_currentMouseState.rgbButtons[0] ^ 0x80)
	{
		PF_FLAG_UNSET(result, PFM_LEFT_DOWN);
	}

	return result;
}

ParabellumFramework::EUINT32
ParabellumFramework::IO::DXInputDevice::GetPreviousMouseState()
{
	EUINT32 result = 0;

	if (m_previousMouseState.rgbButtons[0] & 0x80)
	{
		PF_FLAG_SET(result, PFM_LEFT_DOWN);
	}
	
	if (m_previousMouseState.rgbButtons[0] ^ 0x80)
	{
		PF_FLAG_UNSET(result, PFM_LEFT_DOWN);
	}

	return result;
}

unsigned char 
ParabellumFramework::IO::DXInputDevice::GetKeyboardASCIIState(EUINT32 key)
{
	static HKL layout = GetKeyboardLayout(0);
	unsigned short result;

	unsigned int vk = MapVirtualKeyEx(key, 1, layout);

	return result;
}




