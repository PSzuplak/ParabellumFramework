//	------------------------------------------------------------------------------------------------
//
//	
//
//	
//
//	
//	
//	
//	
//	
//	
//	
//
//	------------------------------------------------------------------------------------------------


#ifndef _TIMER_H_
#define _TIMER_H_


#include <thread>
#include <mutex>
#include <list>
#include <map>
#include <memory>
#include <functional>

#include "DLLIE.h"
#include "MemoryManager.h"
#include "TypesC.h"
#include "Task.h"
#include "Event.h"


namespace ParabellumFramework
{
	namespace Event
	{

		//
		// Simple separate timer which will trigger Event::Event whenever time passes and start over again
		//
		class XYZ_API Timer
		{
		public:
			Timer(EUINT32 timeout)
			{
				m_timeout = timeout;
			}

			Timer(Timer&) = delete;
			
			virtual ~Timer()
			{ 
			}


		//
		// Methods
		//
		public:

			void Run()
			{
				if (m_isRunning == false)
				{
					m_threadUpdate = std::thread{ &Timer::Update, this };
					m_threadUpdate.detach();
					m_isRunning = true;
				}
			}

			void Stop()
			{
				m_isRunning = false;
			}


		private:

			void Update()
			{
				while (m_isRunning)
				{
					
				}
			}

		//
		// Members
		//
		private:

			EUINT32 m_timeout;

			std::thread m_threadUpdate;

			bool m_isRunning;

		//
		// Events
		//
		public:

			Event<int> EventOnTimer;
		};


	}
}

#endif