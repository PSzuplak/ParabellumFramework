#include "stdafx.h"
#include "GraphicsManager.h"


//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumEngine::System::GraphicsManager::GraphicsManager()
{
	m_graphicsDevice = NULLPTR;
	m_isVSync = false;
	m_zFar = 1000.0f;
	m_zNear = 0.1f;
}

ParabellumEngine::System::GraphicsManager::GraphicsManager(const GraphicsManager& ref)
{

}

ParabellumEngine::System::GraphicsManager::~GraphicsManager()
{

}


//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------

ParabellumFramework::EUINT32 
ParabellumEngine::System::GraphicsManager::Initialize(HWND hwnd, EUINT32 screenWidth, EUINT32 screenHeight, EUINT32 settings)
{
	EUINT32 result = SystemCodes::SUCCESS;

	m_graphicsDevice = new DeviceDirectX11();

	if (m_graphicsDevice == NULLPTR)
		return SystemCodes::ERROR_BADALLOC;
	
	result = m_graphicsDevice->Initialize(screenWidth, screenHeight, hwnd, settings);

	if (result != SystemCodes::SUCCESS)
	{
		return result;
	}
	
	return SystemCodes::SUCCESS;
}


ParabellumFramework::EUINT32 
ParabellumEngine::System::GraphicsManager::Release()
{
	EUINT32 result = SystemCodes::SUCCESS;

	if (m_graphicsDevice != NULLPTR)
	{
		result = m_graphicsDevice->Release();
		delete m_graphicsDevice;
		m_graphicsDevice = NULLPTR;
	}

	return SystemCodes::SUCCESS;
}



ParabellumFramework::EUINT32 
ParabellumEngine::System::GraphicsManager::Update(_IO_ SystemInstance* pSystemInstance)
{
	EUINT32 result;

	result = m_graphicsDevice->Update();
	
	pSystemInstance->Draw();

	m_graphicsDevice->SwapBuffers();


	return result;
}


ParabellumFramework::Graphics::GraphicsDevice* 
ParabellumEngine::System::GraphicsManager::GetDevice()
{
	return m_graphicsDevice;
}