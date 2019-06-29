#include "stdafx.h"
#include "InputManager.h"

//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumEngine::System::InputManager::InputManager()
{
	m_positionPressedKeys = 0;
}

ParabellumEngine::System::InputManager::InputManager(const InputManager& ref)
{

}

ParabellumEngine::System::InputManager::~InputManager()
{

}


//	----------------------------------------------------
//		Methods
//	----------------------------------------------------
ParabellumFramework::EUINT32
ParabellumEngine::System::InputManager::Initialize(void* hinstance, void* hwnd, EUINT32 flag)
{
	int result;

	// ceate EFLOAT32s
	m_device = new IO::DXInputDevice();
	result = m_device->Initialize(hinstance, hwnd, 0);

	if (result != SystemCodes::SUCCESS)
		return SystemCodes::ERROR_INIT;


	return SystemCodes::SUCCESS;
}

EUINT32 
ParabellumEngine::System::InputManager::Release()
{
	if (m_device != NULLPTR)
	{
		m_device->Release();
		delete m_device;
		m_device = NULLPTR;
	}

	return SystemCodes::SUCCESS;
}


EUINT32 
ParabellumEngine::System::InputManager::Update()
{
	m_device->Update();

	return SystemCodes::SUCCESS;
}



//	----------------------------------------------------
//		IO State
//	----------------------------------------------------


ParabellumFramework::EUINT8 
ParabellumEngine::System::InputManager::IsKeyDown(EUINT32 keyCode)
{
	return m_device->GetCurrentKeyboardState(keyCode);
}

ParabellumFramework::EUINT8
ParabellumEngine::System::InputManager::WasKeyDown(EUINT32 keyCode)
{
	return m_device->GetCurrentKeyboardState(keyCode);
}


//	----------------------------------------------------
//		Gets and Sets
//	----------------------------------------------------
ParabellumFramework::EINT32 
ParabellumEngine::System::InputManager::GetMouseX()
{
	return m_device->GetMouseX();
}

ParabellumFramework::EINT32
ParabellumEngine::System::InputManager::GetMouseY()
{
	return m_device->GetMouseY();
}

IO::InputDevice* ParabellumEngine::System::InputManager::GetDevice()
{
	return m_device;
}
