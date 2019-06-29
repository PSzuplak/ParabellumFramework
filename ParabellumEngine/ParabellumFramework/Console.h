//	------------------------------------------------------------------------------------------------
//
//	Use to execute commands passing string
//
//	Example :
//
//		console.Bind< EFLOAT32 >("Set", "LightDirection", "Y");
//
//		console.Exec("Set", "LightDirection", "Y", 100.0f)
//
//	------------------------------------------------------------------------------------------------


#ifndef _CONSOLE_H_
#define _CONSOLE_H_

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
	namespace Console
	{


		// ----------------------------------------------------------------------------------------------------------------
		// 

		class CommandString
		{
		public:

		};

		// ----------------------------------------------------------------------------------------------------------------
		// 
		class Console
		{
		public:
		

		private:

		};


	}

}




#endif