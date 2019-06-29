//	------------------------------------------------------------------------------------------------
//
//	Contains some helper functions for scripting, console f.ex. call a function using string	
//
//	------------------------------------------------------------------------------------------------




#ifndef _FUNCTIONHELPER_H_
#define _FUNCTIONHELPER_H_


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



	class Funcy
	{
		//
		// Internal Types
		//
		private:

		class ArgFieldBase
		{

		};

		template <typename T>
		class ArgField : public ArgFieldBase
		{
			public:

				ArgField(T data) : m_data(data)
				{

				}

			public:
				T m_data;
		};

		//
		// Methods
		//
		private:

		template<typename T>
		push(T& t)
		{
			m_register.push_back( ArgField<T>(t) );
		}

		public:
		

		//
		// Memebers
		//
		std::vector< ArgFieldBase* > m_register;

	};




}






#endif