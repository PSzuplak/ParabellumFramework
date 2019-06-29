#ifndef _DEBUGHELPER_H_
#define _DEBUGHELPER_H_


#include "Settings.h"
#include "DebugManager.h"
#include <cassert>
#include <string>


#ifdef DEBUG_SETUP_DEBUGGING


	// assert
	#define PF_DEBUG_ASSERT(condition)	assert(condition)
	
	
	// force crash
	#define FORCE_CRASH(msg) {char _crashmsg[] = msg; _crashmsg[strlen(msg)-1] = *(char*)0;}
	
	
	// printf
	#ifdef DEBUG_SHOW_DETAILS
	
		
		#ifdef DEBUG_LOGLEVEL_1
		#define PFDPRINT(input)				ParabellumFramework::DebugManager::Instance().Print(__FILE__, __func__, __LINE__, input);
		#define PFDPRINTNL(input)			ParabellumFramework::DebugManager::Instance().Printnl(__FILE__, __func__, __LINE__, input);
		#define PFDPRINTERR(input)			ParabellumFramework::DebugManager::Instance().Printerr(__FILE__, __func__, __LINE__, input);
		#define PFDLOG_GLOBAL(APID, CTID, msg)	{ ParabellumFramework::DebugManager::Instance().PutLog(APID, CTID, 0, __FILE__, __func__, __LINE__) << msg; }
		#define PFDLOG(APID, CTID, msg)		{ DebugManager::Instance().PutLog(APID, CTID, 0, __FILE__, __func__, __LINE__) << "[" << (EPTR)this << "] :" << msg; }
		#else
		#define PFDPRINT(input)				
		#define PFDPRINTNL(input)			
		#define PFDPRINTERR(input)			
		#define PFDLOG_GLOBAL(APID, CTID, msg)	
		#define PFDLOG(APID, CTID, msg)
		#endif
	
		#ifdef DEBUG_LOGLEVEL_1
		#define PFDLOG1(APID, CTID, msg) PFDLOG(APID, CTID, msg)
		#else
		#define PFDLOG1(APID, CTID, msg)
		#endif
		
		#ifdef DEBUG_LOGLEVEL_2
		#define PFDLOG2(APID, CTID, msg) PFDLOG(APID, CTID, msg)
		#else
		#define PFDLOG2(APID, CTID, msg)
		#endif
		
		#ifdef DEBUG_LOGLEVEL_3
		#define PFDLOG3(APID, CTID, msg) PFDLOG(APID, CTID, msg)
		#else
		#define PFDLOG3(APID, CTID, msg)
		#endif
		
		#ifdef DEBUG_LOGLEVEL_4
		#define PFDLOG4(APID, CTID, msg) PFDLOG(APID, CTID, msg)
		#else
		#define PFDLOG4(APID, CTID, msg)
		#endif
	
		#else
			#define PFDPRINT(input)			
			#define PFDPRINTNL(input)		
			#define PFDPRINTERR(input)		
			#define PFDLOG(APID, CTID, msg)
		#endif
	#else
		#define PFDDPRINT(str)
		#define PFDPRINTNL(str)
		#define	PFDPRINT(str)
	#endif


// use this macro if you do some experiments and thecode may not work
#define PF_DEBUG_EXPERIMENT(info) PFDPRINTNL(info)





namespace ParabellumFramework
{

	class DebugHelper
	{
	public:


		template<typename T> static std::string GetVal(T val)		{ return ""; }
		template<> static std::string GetVal(int val)				{ return std::to_string(val); }
		template<> static std::string GetVal(unsigned int val)		{ return std::to_string(val); }
		template<> static std::string GetVal(long val)				{ return std::to_string(val); }
		template<> static std::string GetVal(long long val)			{ return std::to_string(val); }
		template<> static std::string GetVal(char val)				{ return std::to_string(val); }
		template<> static std::string GetVal(unsigned char val)		{ return std::to_string(val); }
		template<> static std::string GetVal(float val)				{ return std::to_string(val); }
		template<> static std::string GetVal(double val)			{ return std::to_string(val); }
		template<> static std::string GetVal(long double val)		{ return std::to_string(val); }

		static std::string ArgumentsInfo()
		{
			return "";
		}

		template<typename T, typename... Args>
		static std::string ArgumentsInfo(T first, Args... args)
		{
			std::string out;

			out += std::string(typeid(T).name()) + std::string("  ") + GetVal(first) + std::string(", ");
			out += ArgumentsInfo(args...);

			return std::move(out);
		}
	};


}





	





#endif