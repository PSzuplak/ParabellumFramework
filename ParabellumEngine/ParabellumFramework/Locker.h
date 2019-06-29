//	------------------------------------------------------------------------------------------------
//
//	Use to obey a "locked" function
//
//	------------------------------------------------------------------------------------------------

#ifndef _LOCKER_H_
#define _LOCKER_H_

#include "DLLIE.h"
#include "TypesC.h"

namespace ParabellumFramework
{
	
	class XYZ_API Locker
	{

		//
		// Members
		//
		private:

			volatile bool m_isLocked;

			volatile bool m_prepareToLock;

			volatile bool m_prepareToUnlock;
		//
		// Methods
		//
		public:

			void Lock()
			{
				m_prepareToLock = true;
			}

			void Unlock()
			{
				m_prepareToUnlock = true;
			}


			bool IsLocked()
			{
				return m_isLocked;
			}
	};


	class XYZ_API LockerGuard
	{
	public:

		LockerGuard(Locker& locker)
		{

		}


	};



}

#endif