//	------------------------------------------------------------------------------------------------
//
//	EventManager is responsible to trigger "Task" objects from internal queue.
//	It works in separate thread so all Events etc. may be redirect here
//
//	The main purpose of the class is to execute the Same trigger many times
//	F.ex. if we want to smothly resize a GUI window 
//
//	Example:
//		EventManager::m_timeStep = 40;
//		eventManager.Push(trigger, 5, 30);	//elapsedTime = 5, callCount = 30
//		//pTB will be triggered each 200 (m_timeStep*elapsedTime) miliseconds, so to acomplish its task it needs 6000 (m_timeStep*elapsedTime*callCount) miliseconds
//	------------------------------------------------------------------------------------------------

#ifndef _TASKMANAGER_H_
#define _TASKMANAGER_H_

#include <thread>
#include <mutex>
#include <queue>
#include <list>
#include <ctime>

#include "DLLIE.h"
#include "MemoryManager.h"
#include "TypesC.h"

#include "Task.h"
#include "Event.h"


namespace ParabellumFramework
{
	namespace Event
	{
		
		class XYZ_API TaskManager
		{
		
			struct QueueData
			{
				TaskBinder* m_triggerBinder;
				EUINT32 m_elapsedTime;
				EUINT32 m_callCount;

				EUINT32 m_currentStep;

				EUINT32 m_lastTimeExecuted;

				QueueData(TaskBinder* pTB, EUINT32 elapsedTime, EUINT32 callCount)
				{
					m_triggerBinder = pTB;
					m_elapsedTime = elapsedTime;
					m_callCount = callCount;
					m_currentStep = 0;
					m_lastTimeExecuted = 0;
				}
			};



		public:
			TaskManager();
			TaskManager(TaskManager& src) = delete;
			~TaskManager();

		//
		// Methods
		//
		public:
			// execute thread
			void Run();

			// stop thread
			void Stop();

			// put a TaskBinder to a queue. TaskBinder will be executed 'callCount' times between each 'elapsedTime * EventManager::m_timeStep' miliseconds	
			void Push(TaskBinder* pTB, EUINT32 elapsedTime, EUINT32 callCount);

		private:

			// clock will be updated independently of queue processing
			void Clock();

			// process task from a queue
			void Process();

		//
		// Members
		//
		private:
			
			std::thread m_threadClock;

			std::thread m_threadProcess;

			bool m_isRunning;

			std::mutex	m_mutexProcess;

			std::list<QueueData> m_tasks;
		
			std::queue<QueueData*> m_queue;

			std::clock_t m_timeLast;
			std::clock_t m_timeCurrent;
			EUINT32 m_timeCounter;

			EUINT32 m_timeStep;

		};





	}
}

#endif