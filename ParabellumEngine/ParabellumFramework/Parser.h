#ifndef _PARSER_H_
#define _PARSER_H_

//
//	Predefs
//
#include "DLLIE.h"

#include <string>
#include <map>
#include <new>
#include <memory>

#include "DebugManager.h"
#include "TypesC.h"
#include "SystemCodes.h"
#include "Settings.h"



namespace ParabellumFramework
{
	namespace Resources
	{

		class XYZ_API ParserBase
		{
		};


		template<typename ReturnType, typename ClassType, typename... Args>
		class XYZ_API Parser : public ParserBase
		{
		};


	}
}


#endif