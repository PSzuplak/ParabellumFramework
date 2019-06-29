//	------------------------------------------------------------------------------------------------
//
//	Maintaining events
//
//
//
// Example :
//
//
//		class InputDevice
//		{
//			Event::Event<char> EventPressedKey;
//			
//			void PressKey_N()
//			{
//				EventPressedKey( 'N' );
//			}
//			
//		};
//		
//
//
//		class Reciever1 : public Event::EventHandler
//		{
//		public:
//				void Test1(char t) { printf("Receiver1::Test1 - pressed %c", t); }
//				void Test2(char t) { printf("Receiver1::Test2 - pressed %c", t); }
//		};
//		
//
//		main(){ ...
//
//		InputDevice id;
//		Reciever1 e1a;
//
//		e1a.Subscribe( id.EventPressedKey,&Reciever1::Test1);
//		
//		... whenever inputDevice calls EventPressedKey( XXX ), e1a calls automatically e1a.Test1( XXX )
//		
//		
//		
//		
//	------------------------------------------------------------------------------------------------


#ifndef _EVENT_H_
#define _EVENT_H_


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
#include "Task.h"
#include "GraphicsDevice.h"
#include "DebugHelper.h"

namespace ParabellumFramework
{
	namespace Event
	{

		class EventHandler;

		
		// ----------------------------------------------------------------------------------------------------------------
		// this is virtual kind of class only for polymorphism purposes
		//
		class XYZ_API EventBase 
		{
		public:
			virtual ~EventBase()
			{

			}
		};

		// ----------------------------------------------------------------------------------------------------------------
		// This may be 'Subscribe'd by an EventHandler object. 
		// Whenever EventHandler execute an Event, the Event will execute all 'Subscribe'd functions
		//
		template<typename... Args>
		class XYZ_API Event : public EventBase
		{		
			// friendship - only EventHandler should be able to add events
			friend class XYZ_API ParabellumFramework::Event::EventHandler;


			//
			// Constructors and Destructors
			//
			public:

			Event()
			{
				PFDLOG("EVNT", "EVNT", "Event::Event() : [" << std::addressof(this) << "]");
			}

			virtual ~Event()
			{
				for (auto it : m_register)
					delete it;
			}

		private:

			// add function which will be triggered for this Event
			template<typename ReturnType, typename ClassType>
			void Add(ClassType* t, ReturnType(ClassType::*func)(Args... args))
			{
				PFDLOG("EVNT", "EVNT", "Event::Add(" << typeid(ClassType).name() << ", " << typeid(ReturnType).name() << "(" << typeid(ClassType).name() << "::[" << std::addressof(func) << "])  ( Args )" );
				TaskBase<Args...>* i = new Task <ReturnType, ClassType, Args...>(t, func);
				m_register.push_back(i);
			}

		public:

			void Call(Args... args)
			{
				PFDLOG("EVNT", "EVNT", "Event::Call( " << DebugHelper::ArgumentsInfo(args...) << " )" );
				for (auto it : m_register)
					it->Call(args...);
			}

			void CallParallel(Args... args)
			{
				PFDLOG("EVNT", "EVNT", "Event::CallParallel( " << DebugHelper::ArgumentsInfo(args...) << " )");
				std::thread thread = std::thread{ &Event::Call, this, args... };
				thread.detach();
			}

		//
		// Members
		//
		private:
			std::vector< TaskBase<Args...>* > m_register;

		//
		// Operators
		//
		public:
			void operator()(Args... args)
			{
				Call(args...);
			}
		};


		// ----------------------------------------------------------------------------------------------------------------
		//
		//
		class XYZ_API EventHandler
		{

		//
		// Methods
		//
		public:

			virtual ~EventHandler()
			{

			}

			template<typename ReturnType, typename ClassType, typename... Args>
			void Subscribe(Event<Args...>& ev, ReturnType(ClassType::*func)(Args...) ) 
			{
				PFDLOG("EVNT", "EVNT", "Event::Subscribe( [" << std::addressof(ev) << "] , " << typeid(ClassType).name() << ", " << typeid(ReturnType).name() << "(" << typeid(ClassType).name() << "::[" << std::addressof(func) << "])  ( Args )");
				ev.Add<ReturnType, ClassType>((ClassType*)this, func);
				m_events.push_back(&ev);
			}

		//
		// Members
		//
		private:
			
			std::vector<EventBase*> m_events;

			EUINT32 reserved0;
			EUINT32 reserved1;
			
		}; // end class
		


		// ----------------------------------------------------------------------------------------------------------------
		// Some global functions to handle the events
		//


	}
}

#endif