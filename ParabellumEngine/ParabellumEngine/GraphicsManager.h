#ifndef _GRAPHICSMANAGER_H_
#define _GRAPHICSMANAGER_H_


//
//	Predefs
//
#include "DLLIE.h"

#define WIN32_LEAN_AND_MEAN

//
//	Includes
//

#include <windows.h>

#include "../ParabellumFramework/TypesC.h"
#include "../ParabellumFramework/SystemCodes.h"

#include "../ParabellumFramework/GraphicsDevice.h"
#include "../ParabellumFramework/DeviceDirectX11.h"

#include "SystemInstance.h"

namespace ParabellumEngine
{
	namespace System
	{
			using namespace ParabellumFramework::Graphics;

			//
			//	GraphicsManager
			//
		 class XYZ_API GraphicsManager
			{
			public:
				GraphicsManager();
				GraphicsManager(const GraphicsManager&);
				~GraphicsManager();

				EUINT32 Initialize(HWND hwnd, EUINT32 screenWidth, EUINT32 screenHeight, EUINT32 settings);
				EUINT32 Release();
				EUINT32 Update(_IO_ SystemInstance* pSystemInstance);


				ParabellumFramework::Graphics::GraphicsDevice* GetDevice();

			private:
				ParabellumFramework::Graphics::GraphicsDevice* m_graphicsDevice;


				bool m_isVSync;		// is vsync enabled

				float m_zFar;		// near and far(depth) planes
				float m_zNear;
			};
	}
}








#endif