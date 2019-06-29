#ifndef _DEBUGCONSOLE_H_
#define _DEBUGCONSOLE_H_

//
// libs
//
#include <cstdio>
#include <cassert>
#include <memory>
#include <unordered_map>
#include <fstream>
#include <thread>
#include <mutex>

#include "Settings.h"
#include "anonymousVariable.h"
#include "MemoryManager.h"
#include "TypesC.h"
#include "SystemCodes.h"
#include "MathHelper.h"
#include "Settings.h"
#include "Deductor.h"

namespace ParabellumFramework
{


	//
	// Simple logger
	//
	class XYZ_API DebugManager
	{
	public:

		//
		// Structs
		//
		struct Record
		{
			EUINT32			no;
			std::clock_t	time;
			char			apid[5];
			char			cpid[5];
			EUINT32			logLevel;
			char			filename[128];
			char			funcname[32];
			EUINT32			lineNo;
			char			msg[64];
		};

	public:

			static DebugManager& Instance();
			~DebugManager();		

		private:
			DebugManager();

			DebugManager(DebugManager const&) = delete;
			void operator=(DebugManager const&) = delete;

		public:

			static void Print(const char* filename, const char* funcname, long lineno, const char* input);
			static void Print(const char* filename, const char* funcname, long lineno, int input);

			static void Printnl(const char* filename, const char* funcname, long lineno, const char* input);
			static void Printnl(const char* filename, const char* funcname, long lineno, int input);

			static void Printerr(const char* filename, const char* funcname, long lineno, const char* input);
			static void Printerr(const char* filename, const char* funcname, long lineno, int input);


			// savelog in the buffer
			std::fstream& PutLog(	const char* apid, const char* cpid, EUINT8 logLevel, const char* filename, const char* funcname, int lineNo);


			void Backtrace()
			{
			}







		private:
			
			// log file
			std::fstream m_logFile;

			Record m_logBuffer[DEBUG_LOG_BUFFER_SIZE];

			// current position in buffer
			EUINT32 m_currentPosition;

			// last position which was dumped
			EUINT32 m_dumpPositionStart;
			EUINT32 m_dumpPositionEnd;

			// how many logs were saved
			EUINT32 m_recordsCounter;

			// mutex for PutLog()
			std::mutex m_mutexPutLog;

			
	};



}








#endif