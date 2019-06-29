//	------------------------------------------------------------------------------------------------
//
//	Task is used to simplify work with Events, Timers etc.
//	Basically it is kind a wrapper on std::function with some extensions
//
//	Example :
//		int ReturnInt_IntInt(int p0, int p1);
//		auto task1 = Event::CreateTask(&ReturnInt_IntInt, 115, 611);
//		task1->CallBinded();	// print "115,661"
//		task1->Bind(123, 446);
//		task1->Call(789, 753);	// print "789,753"
//		task1->CallBinded();	// print "123,446"
//
//	------------------------------------------------------------------------------------------------

#ifndef _TASK_H_
#define _TASK_H_

#include <thread>
#include <mutex>
#include <list>
#include <map>
#include <memory>
#include <functional>

#include "DLLIE.h"
#include "MemoryManager.h"
#include "TypesC.h"
#include "ParabellumExtensions.h"

namespace ParabellumFramework
{
	namespace Event
	{

		// ----------------------------------------------------------------------------------------------------------------
		class TaskBinder
		{
		public:

			// execute Task with previously binded (TaskBase::Bind) parametrs
			virtual void CallBinded() = 0;
		};



		// ----------------------------------------------------------------------------------------------------------------
		// Stores pointer to a function triggered by an Event
		// this is virtual kind of class only for polymorphism purposes
		template<class... Args>
		class TaskBase : public TaskBinder
		{
		public:
			virtual ~TaskBase() { }

			virtual void Call(Args... args) = 0;

			virtual void CallParallel(Args... args)
			{
				std::thread thread = std::thread{ &TaskBase::Call, this, args... };
				thread.detach();
			}

			// bind arguments with the Task
			virtual void Bind(Args... args) = 0;

			virtual void CallBinded()
			{

			}
		};

		// ----------------------------------------------------------------------------------------------------------------
		// Stores pointer on class method - binded with an object; triggered by an Event
		//
		template<typename ReturnType, typename ClassType, typename... Args>
		class Task : public TaskBase<Args...>
		{
		public:

			Task(ClassType* t, ReturnType(ClassType::*f)(Args...))
			{
				m_pObject = t;
				m_pFunction = f;
			}

			Task(ClassType* t, ReturnType(ClassType::*f)(Args...), Args... args)
			{
				m_pObject = t;
				m_pFunction = f;
				Bind(args...);
			}

			~Task()
			{
			}

			virtual void Call(Args... args)
			{
				(m_pObject->*m_pFunction)(args...);
			}


			virtual void Bind(Args... args)
			{
				m_pFunctionBinded = std::bind(m_pFunction, m_pObject, args...);
			}

			virtual void CallBinded()
			{
				m_pFunctionBinded();
			}

		//
		// Members
		//
		private:
			ClassType* m_pObject;
			ReturnType(ClassType::*m_pFunction)(Args...);
			std::function<ReturnType()> m_pFunctionBinded;
		};

		// ----------------------------------------------------------------------------------------------------------------
		// Specialization to store pointer to an external function
		//
		template<typename ReturnType, typename... Args>
		class Task<ReturnType, void, Args...> : public TaskBase<Args...>
		{
		public:

			Task(ReturnType(*f)(Args...))
			{
				m_pFunction = f;
			}

			Task(ReturnType(*f)(Args...), Args... args)
			{
				m_pFunction = f;
				Bind(args...);
			}

			~Task()
			{
			}

			void Call(Args... args)
			{
				m_pFunction(args...);
			}

			
			virtual void Bind(Args... args)
			{
				m_pFunctionBinded = std::bind(m_pFunction, args...);
			}

			virtual void CallBinded()
			{
				m_pFunctionBinded();
			}


		//
		// Members
		//
		private:
			std::function<ReturnType(Args...)> m_pFunction;
			std::function<ReturnType()> m_pFunctionBinded;
		};




		// ----------------------------------------------------------------------------------------------------------------
		// helpe functions


		// return a pointer to a Task definied by an object and its method
		template<typename ReturnType, typename ClassType, typename... Args>
		Task <ReturnType, ClassType, Args...>*
		CreateTask( ClassType* pObj, ReturnType(ClassType::*pMethod)(Args...) )
		{
			Task <ReturnType, ClassType, Args...>* task0 = new Task <ReturnType, ClassType, Args...>(pObj, pMethod);
			return task0;
		}
		
		// return a pointer to a Task definied by an object and its method and automatically bind 'args'
		template<typename ReturnType, typename ClassType, typename... Args>
		Task <ReturnType, ClassType, Args...>*
		CreateTask(ClassType* pObj, ReturnType(ClassType::*pMethod)(Args...), Args... args)
		{
			Task <ReturnType, ClassType, Args...>* task0 = new Task <ReturnType, ClassType, Args...>(pObj, pMethod, args...);
			return task0;
		}

		// return a pointer to a Task
		template<typename ReturnType, typename... Args>
		Task <ReturnType, void, Args...>*
		CreateTask(ReturnType(*func)(Args...))
		{
			Task <ReturnType, void, Args...>* task0 = new Task <ReturnType, void, Args...>(func);
			return task0;
		}

		// return a pointer to a Task and automatically bind 'args'
		template<typename ReturnType, typename... Args>
		Task <ReturnType, void, Args...>*
		CreateTask(ReturnType(*func)(Args...), Args... args)
		{
			Task <ReturnType, void, Args...>* task0 = new Task <ReturnType, void, Args...>(func, args...);
			return task0;
		}




	}
}


#endif