#ifndef _SYSTEMINSTANCE_H_
#define _SYSTEMINSTANCE_H_

//
//	Predefs
//
#include "DLLIE.h"

#include <stdio.h>

#include "../ParabellumFramework/ResourceManager.h"
#include "../ParabellumFramework/GraphicsDevice.h"
#include "../ParabellumFramework/InputDevice.h"
//#include "Timer.h"

using namespace ParabellumFramework;
using namespace ParabellumFramework::Resources;
using namespace ParabellumFramework::Graphics;
using namespace ParabellumFramework::IO;

namespace ParabellumEngine
{
	namespace System
	{
	 class XYZ_API SystemInstance
		{
		private:
			ResourceManager* m_resourceManager;
			GraphicsDevice* m_graphicsDevice;
			InputDevice* m_inputDevice;
			//Timer* m_time;

		public:
			SystemInstance() {}
			SystemInstance(SystemInstance&) {}
			~SystemInstance() {}

			virtual void Initialize() = 0;
			virtual void Release() = 0;
			virtual void Draw() = 0;
			virtual void Update() = 0;

			virtual EUINT32 Initialize(
				_IO_ ResourceManager* pResource, _IO_ GraphicsDevice* pGraphics, _IN_ InputDevice* pInput) final
			{
				m_resourceManager = pResource;
				m_graphicsDevice = pGraphics;
				m_inputDevice = pInput;
				//m_time = pTime;

				Initialize();

				return SystemCodes::SUCCESS;
			}






		protected:
			ResourceManager* GetResourceManager()
			{
				return m_resourceManager;
			}

			GraphicsDevice*  GetGraphicsDevice()
			{
				return m_graphicsDevice;
			}
			
			InputDevice*  GetInputDevice()
			{
				return m_inputDevice;
			}
			/*
			Timer* GetTime()
			{
				return m_time;
			}
			*/
		};
	}
}


#endif