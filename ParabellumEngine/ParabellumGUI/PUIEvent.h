#ifndef _PUIEVENT_H_
#define _PUIEVENT_H_


//
//	dependieces
//
#include "../ParabellumFramework/DLLIE.h"

namespace ParabellumUserInterface
{

	//
	//	abstract event
	//
	struct XYZ_API PUIEventq
	{
	public:
		PUIEventq()
		{

		}

		PUIEventq(const PUIEventq&) = delete;

		virtual ~PUIEventq()
		{
		}
	};

}

#endif