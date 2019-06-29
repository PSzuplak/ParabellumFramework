#include "stdafx.h"
#include "SystemManager.h"


//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumEngine::System::SystemManager::SystemManager()
{
	m_graphics = NULLPTR;
	m_input = NULLPTR;
	m_isFullscreen = false;
	m_skipUpdateCycle = 3;
}

ParabellumEngine::System::SystemManager::SystemManager(const SystemManager& ref)
{

}

ParabellumEngine::System::SystemManager::~SystemManager()
{

}


//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------
ParabellumFramework::EUINT32
ParabellumEngine::System::SystemManager::Initialize(_IO_ SystemInstance* pSysInstance)
{
	EUINT32 screenWidth, screenHeight;
	EUINT32 result = SystemCodes::SUCCESS;
	EUINT32 graphicsSettings = 0;

	// default values for screen width and height
	screenHeight = 0;
	screenWidth = 0;


	// initialize window
	InitializeWindow(screenWidth, screenHeight, 0);

	// initialize input manager
	m_input = new InputManager();

	if (m_input != NULLPTR)
	{
		result = m_input->Initialize(m_hinstance, m_hwnd, 0);

		if (result != SystemCodes::SUCCESS)
			return SystemCodes::ERROR_INIT;
	}
	else
	{
		return SystemCodes::ERROR_BADALLOC;
	}


	// initialize graphics manager
	m_graphics = new GraphicsManager();

	if (m_graphics != NULLPTR)
	{
		result = m_graphics->Initialize(m_hwnd, screenWidth, screenHeight, graphicsSettings);

		if (result != SystemCodes::SUCCESS)
			return SystemCodes::ERROR_INIT;
	}
	else
	{
		return SystemCodes::ERROR_BADALLOC;
	}


	// create resource manager
	m_resourceManager = new ResourceManager();
	result = m_resourceManager->Initialize(m_graphics->GetDevice());

	if (result != SystemCodes::SUCCESS)
	{
		return result;
	}

	//
	// set system instance
	m_sysInstance = pSysInstance;
	result = m_sysInstance->Initialize(m_resourceManager, m_graphics->GetDevice(), m_input->GetDevice());//), m_time);


	if (result != SystemCodes::SUCCESS)
	{
		SystemCodes::SUCCESS;
	}

	//
	//
	m_threadUpdateSystemInstance = std::thread{ &SystemManager::UpdateSystemInstance, this };
	m_threadUpdateSystemInstance.detach();

	return SystemCodes::SUCCESS;
}


//
//
//
ParabellumFramework::EUINT32
ParabellumEngine::System::SystemManager::Release()
{
	EUINT32 result = SystemCodes::SUCCESS;

	// release System Instance
	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PFGI", "SYSM", "Releasing SystemInstance");
	#endif

	m_sysInstance->Release();

	// release graphics manager
	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PFGI", "SYSM", "Releasing Graphics");
	#endif

	if (m_graphics != NULLPTR)
	{
		result = m_graphics->Release();
		delete m_graphics;
		m_graphics = NULLPTR;

		if (result != SystemCodes::SUCCESS) return result;
	}


	// release input manager
	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PFGI", "SYSM", "Releasing Input");
	#endif

	if (m_input != NULLPTR)
	{
		result = m_input->Release();
		delete m_input;
		m_input = NULLPTR;

		if (result != SystemCodes::SUCCESS) return result;
	}

	// release resource manager
	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PFGI", "SYSM", "Releasing ResourceManager");
	#endif

	if (m_resourceManager != NULLPTR)
	{
		result = m_resourceManager->Release();
		delete m_resourceManager;
		m_resourceManager = NULLPTR;

		if (result != SystemCodes::SUCCESS) return result;
	}


	return SystemCodes::SUCCESS;
}



//
//
//
ParabellumFramework::EUINT32
ParabellumEngine::System::SystemManager::Execute()
{
	MSG msg;
	EUINT32 result = SystemCodes::SUCCESS;
	bool quit = false;
	EUINT32 counterUpdateCycle = 0;

	// initialize message struct
	ZeroMemory(&msg, sizeof(MSG));

	while (!quit)
	{
		PROFILER_BEGIN("MainLoop");

		//	process the message
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Message : Quit
		if (msg.message == WM_QUIT)
		{
			quit = true;
		}
		// update system status
		else
		{

			// update input deice only if 
			PROFILER_BEGIN("Update");
			result = Update();		
			PROFILER_END("Update");

			if (result != SystemCodes::SUCCESS) quit = true;


			// draw frame
			PROFILER_BEGIN("Draw");
			result = Draw();	
			PROFILER_END("Draw");

			if (result != SystemCodes::SUCCESS) quit = true;
		}

		PROFILER_END("MainLoop");

		//Profiler::Instance().PrintS();
	}

	return result;
}

ParabellumFramework::EUINT32
ParabellumEngine::System::SystemManager::Draw()
{
	EUINT32 result = 0;

	//	update graphics manager
	result = m_graphics->Update(m_sysInstance);

	if (result != SystemCodes::SUCCESS)
	{
		return result;
	}

	return SystemCodes::SUCCESS;
}

ParabellumFramework::EUINT32
ParabellumEngine::System::SystemManager::Update()
{
	EUINT32 result = 0;
	
	// updade input device
	m_input->Update();

	// update input manager and check key value
	if (m_input->IsKeyDown(DIK_ESCAPE))
	{
		return SystemCodes::FORCEDQUIT;
	}
	
	return SystemCodes::SUCCESS;
}


ParabellumFramework::EUINT32
ParabellumEngine::System::SystemManager::UpdateSystemInstance()
{
	while (1)
	{
		m_sysInstance->Update();
	}

	return SystemCodes::SUCCESS;
}

//
//
//
LRESULT CALLBACK ParabellumEngine::System::SystemManager::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}



//
//
//
ParabellumFramework::EUINT32
ParabellumEngine::System::SystemManager::InitializeWindow(EUINT32& width, EUINT32& height, EUINT32 settings)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;

	int positionX, positionY;	// position of top left corner of the window

	//	pointer to this object
	ApplicationHandle = this;

	//	get instance
	m_hinstance = GetModuleHandle(NULL);

	//	set name
	m_applicationName = SYSTEM_NAME;


	//	set parametr of window class XYZ_API
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// register the window
	RegisterClassEx(&wc);

	//	get the resolution
	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);

	if (m_isFullscreen == true)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsHeight = (unsigned long)height;
		dmScreenSettings.dmPelsWidth = (unsigned long)width;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		positionX = positionY = 0;
	}
	else
	{
		width = 1680;
		height = 1050;

		positionX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		positionY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}

	//	create winapi window
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		positionX, positionY, width, height, NULL, NULL, m_hinstance, NULL);

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
	ShowCursor(false);

	return SystemCodes::SUCCESS;
}



//
//
//
ParabellumFramework::EUINT32
ParabellumEngine::System::SystemManager::ReleaseWindow()
{
	EUINT32 result = 0;

	// show cursor if hidden
	ShowCursor(true);

	// exit from the fullscreen mode
	if (m_isFullscreen)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// release winapi window
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	ApplicationHandle = NULL;

	return SystemCodes::SUCCESS;
}





//
//
//
LRESULT CALLBACK ParabellumEngine::System::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}