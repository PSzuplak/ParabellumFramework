#include "stdafx.h"
#include "TaskManager.h"


//	------------------------------------------------------------
//		Statics
//	------------------------------------------------------------


//	------------------------------------------------------------
//		Constructors and destructors
//	------------------------------------------------------------
ParabellumFramework::Event::TaskManager::TaskManager()
{
	m_isRunning = false;

	m_timeCounter = 0;
	m_timeLast = std::clock();
	m_timeCurrent = m_timeLast;
	m_timeStep = 20;
}

ParabellumFramework::Event::TaskManager::~TaskManager()
{
	std::lock_guard<std::mutex> guard(m_mutexProcess);
	m_isRunning = false;
}


//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------

void
ParabellumFramework::Event::TaskManager::Run()
{
	std::lock_guard<std::mutex> guard(TaskManager);

	if (m_isRunning == false)
	{
		m_isRunning = true;

		m_threadProcess = std::thread{ &TaskManager::Process, this };
		m_threadProcess.detach();

		m_threadClock = std::thread{ &TaskManager::Clock, this };
		m_threadClock.detach();
	}
}


void
ParabellumFramework::Event::TaskManager::Stop()
{
	std::lock_guard<std::mutex> guard(m_mutexProcess);

	if (m_isRunning == true)
	{
		m_isRunning = false;
	}
}

void
ParabellumFramework::Event::TaskManager::Clock()
{
	while (m_isRunning == true)
	{
		m_timeCurrent = std::clock();

		if ((m_timeCurrent - m_timeLast) > m_timeStep)
		{
			m_timeCounter++;
			m_timeLast = m_timeCurrent;

			// add to the queue all tasks which needed
			std::list<QueueData>::iterator it = m_tasks.begin();

			while (it != m_tasks.end())
			{
				if (it->m_currentStep < it->m_callCount)
				{
					if ((it->m_lastTimeExecuted < m_timeCounter) && ((m_timeCounter % it->m_elapsedTime) == 0))
					{
						it->m_lastTimeExecuted = m_timeCounter;
						it->m_currentStep++;

						m_mutexProcess.lock();
						m_queue.push(&(*it));
						m_mutexProcess.unlock();
					}
				}
				else
				{
					m_tasks.erase(it++);
					continue;
				}

				it++;
			}


		} // end if ((m_timeCurrent - m_timeLast) > m_timeStep)

	} // end while (m_isRunning == true)
}

void
ParabellumFramework::Event::TaskManager::Process()
{
	while (m_isRunning == true)
	{
		std::lock_guard<std::mutex> guard(m_mutexProcess);


		if (m_queue.size() == 0)
			continue;
		
		QueueData* data = m_queue.front();
		data->m_triggerBinder->CallBinded();
		m_queue.pop();
	}
}


void
ParabellumFramework::Event::TaskManager::Push(TaskBinder* pTB, EUINT32 elapsedTime, EUINT32 callCount)
{
	std::lock_guard<std::mutex> guard(m_mutexProcess);

	QueueData qd(pTB, elapsedTime, callCount);

	m_tasks.push_front(qd);
}