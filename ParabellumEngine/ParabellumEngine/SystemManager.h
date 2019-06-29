#ifndef _SYSTEMMANAGER_H_
#define _SYSTEMMANAGER_H_


//
//	Predefs
//

#define WIN32_LEAN_AND_MEAN

#define SYSTEM_NAME L"Parabellum Engine v1"


#include "DLLIE.h"

//
//	Includes
//

#include <windows.h>
#include <thread>

#include "../ParabellumFramework/TypesC.h"
#include "../ParabellumFramework/SystemCodes.h"

#include "../ParabellumFramework/DebugManager.h"
#include "../ParabellumFramework/Profiler.h"

#include "../ParabellumFramework/MemoryManager.h"
#include "../ParabellumFramework/ResourceManager.h"
#include "GraphicsManager.h"
#include "InputManager.h"
#include "SystemInstance.h"

using namespace ParabellumFramework;
using namespace ParabellumFramework::Graphics;
using namespace ParabellumFramework::IO;

namespace ParabellumEngine
{
	namespace System
	{
		//
		//	SystemManager
		//
	 class XYZ_API SystemManager
		{
			public:
				SystemManager();
				SystemManager(const SystemManager&);
				~SystemManager();

				EUINT32 Initialize(_IO_ SystemInstance* pSysInstance);


				EUINT32 Release();

				EUINT32 Execute();


				// update graphics manager and call SystemInstance->Draw
				EUINT32 Draw();

				// Update Input Manager
				EUINT32 Update();
		
			private:

				// run m_sysInstance->Update() as a separate thread
				EUINT32 UpdateSystemInstance();
			
			public:
				LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

			private:
				EUINT32 InitializeWindow(EUINT32& width, EUINT32& height, EUINT32 settings);
				EUINT32 ReleaseWindow();

			


			private:
				LPCWSTR m_applicationName;
				HINSTANCE m_hinstance;
				HWND m_hwnd;

				Resources::ResourceManager* m_resourceManager;
				GraphicsManager* m_graphics;
				InputManager* m_input;
				SystemInstance* m_sysInstance;

				bool m_isFullscreen;

				EUINT32 m_skipUpdateCycle;		// how many update call should be skipped in the loop

				std::thread m_threadDraw;
				std::thread m_threadUpdateSystemInstance;


				volatile bool m_isExecuteRunning;


		};

		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


		static SystemManager* ApplicationHandle = 0;
	}
}








#endif