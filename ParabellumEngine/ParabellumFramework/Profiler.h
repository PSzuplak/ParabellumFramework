//	------------------------------------------------------------------------------------------------
//
//	Profiler
//
//	Use PROFILER_BEGIN( NODENAME ) and PROFILER_END( NODENAME ) to obey a code to profile
//	Whenever one of these two macros are being called, Profiler will dump data from all registered 'ProfilerUnits'
//	See 'ProfilerUnit.h' for details
//
//	Settings are stored in ProfilerSettings.h
//
//
//	------------------------------------------------------------------------------------------------

#ifndef _PROFILER_H_
#define _PROFILER_H_


#include <cstdio>
#include <cassert>
#include <memory>
#include <unordered_map>
#include <fstream>
#include <chrono>
#include <map>
#include <thread>
#include <mutex>

#include "MemoryManager.h"
#include "SystemCodes.h"
#include "MathHelper.h"
#include "Settings.h"
#include "Deductor.h"
#include "DebugManager.h"
#include "MemoryManager.h"
#include "Settings.h"


#ifdef PF_PROFILER_ENABLED
	#define PROFILER_BEGIN( NODENAME )	{ ParabellumFramework::Profiler::Instance().Begin(C$(NODENAME), NODENAME);}
	#define PROFILER_END( NODENAME )	{ ParabellumFramework::Profiler::Instance().End(C$(NODENAME));}
#else
	#define PROFILER_BEGIN( NODENAME )
	#define PROFILER_END( NODENAME )
#endif


// Memory profiler
#ifdef DEBUG_MEMORY
	//#define new ParabellumFramework::Profiler::Instance().Deduct() = new
	//#define new ParabellumFramework::DeductorNew(__FILE__, __func__, __LINE__) = new 

	//#define delete ParabellumFramework::DeductorDelete(__FILE__, __func__, __LINE__) =
#endif




namespace ParabellumFramework
{

	//
	//
	//
	class XYZ_API DeductorNew
	{
		unsigned int m_lineNo; 
		const char* m_filename; 
		const char* m_funcname;
	public:
		DeductorNew(const char* filename, const char* funcname, unsigned int lineNo) { m_filename = filename; m_funcname = funcname; m_lineNo = lineNo; }

		template<typename T>
		T* operator=(T* ptr)
		{
			PFDLOG("PFDM", "MEMO", "%MEMORY new [" << (EADDR)ptr << "] : " << typeid(T).name() << " in ("<< m_funcname  <<" : " << m_lineNo <<") " << m_filename);		
			ParabellumFramework::Profiler::Instance().IncCPUMemoryRate( sizeof(T) );

			return ptr;
		}
	};


	//
	//
	//
	class XYZ_API DeductorDelete
	{
		unsigned int m_lineNo;
		const char* m_filename;
		const char* m_funcname;
	public:

		DeductorDelete(const char* filename, const char* funcname, unsigned int lineNo) { m_filename = filename; m_funcname = funcname; m_lineNo = lineNo; }

		template<typename T>
		void operator=(T* ptr)
		{
			PFDLOG("PFDM", "MEMO", "%MEMORY delete " << (EADDR)ptr << " : type = " << typeid(T).name() << " in (" << m_funcname << " : " << m_lineNo << ") " << m_filename);
			ParabellumFramework::DebugManager::Instance().Flush();
			ParabellumFramework::Profiler::Instance().IncCPUMemoryRate(sizeof(T));

			delete ptr;
		}
	};



	//
	// Profiler - simple slow multithreaded profiler - allow to track opened branches for profiling. Do NOT use inside big loops
	//
	class XYZ_API Profiler
	{
	private:

		typedef struct
		{
			// time ProfilerBegin started
			long long timeStart;

			// time ProfilerBegin ended
			long long timeEnd;

			// branch can be opened few times - in loop fex. so we have to cunt total time
			long long timeTotal;

			// name of the node
			std::string nodeName;

			// node/children depth
			EUINT32 depth;

			// if node with similar node name was started withount ending
			EBOOL isEnabled;

			// how many times opened by parent
			EUINT32 openCount;


			// allocating memory counter
			EUINT32 mallocCount;

			// freeing memory counter
			EUINT32 freeCount;

		} ProfileInfo;

	private:

		//
		// static members
		//

		// solve Thread Id to array index
		static std::map<EUINT64, EUINT32> InstancesIds;

		static EUINT32 InstancesCount;

	public:

		static Profiler& Instance();


	public:
		Profiler();
		~Profiler();

		Profiler(Profiler const&) = delete;
		void operator=(Profiler const&) = delete;

	public:

		// begine profiling block - there shoud be only 1 main block - all others should be inside main block
		// Do NOT call directly if You do know what You do - macro PROFILER_BEGIN( NODENAME ) should be called instead
		inline void Begin(EUINT64 key, const char* nodeName);

		// end profiling block
		inline void End(EUINT64 key);

		void GetInfo(EUINT32 no, std::string& out)
		{
			out = "";

			for (EUINT32 j = 0; j < m_nodesInfo[no].depth; ++j)
				out += " ";

			out += " " + m_nodesInfo[no].nodeName;
			out += "(" + m_nodesInfo[no].timeTotal;
			out += ")";
			//out += " ot=%i" + m_nodesInfo[no].openCount;
			//out += " ma=%i" + m_nodesInfo[no].mallocCount;
			//out += " fr=%i" + m_nodesInfo[no].freeCount;
		}


		//
		// Get sample info
		//
		void GetTexty(std::string& str)
		{
			str = "";
			
			for (EUINT32 i = 0; i < m_nodesCount; ++i)
			{
				str += " \n";
				for (EUINT32 j = 0; j < m_nodesInfo[i].depth; ++j)
					str += "  ";

				str += " " + m_nodesInfo[i].nodeName;
				str += " ( " + m_nodesInfo[i].timeTotal + std::string(")");
				//str += " ot = " + m_nodesInfo[i].openCount;
				//str += " ma = " + m_nodesInfo[i].mallocCount;
				//str += " fr = " + m_nodesInfo[i].freeCount;
			}

		}


		void IncCPUMemoryRate(EUINT32 val)
		{
			m_CPUFrameAllocCount += val;
			m_CPUTotalAllocCount += val;
		}


	private:



		// info 
		ProfileInfo m_nodesInfo[PF_PROFILER_MAX_NODES];

		// depth in profiler nodes
		EUINT32 m_nodesDepth;

		// how many nodes are opened
		EUINT32 m_nodesCount;

		// hash string key - id in 
		std::map<EUINT64, EUINT32> m_nodesIds;

		// how many PROFILE_BEGIN are opened
		EUINT32 m_beginCallsCount;

		// which PROFILE_BEGIN are opened (stores hash-key for used by ProfileBegin func)
		EUINT64 m_keysOnStack[PF_PROFILER_MAX_INFOSTACK];

		// current node (hash key of node name)
		EUINT64 m_currentNodeKey;

		// how much memeory was allocated from ProbieBegin(FIRST) to ProfileEnd
		EUINT32 m_CPUFrameAllocCount;

		// how many memory was allocated in total
		EUINT32 m_CPUTotalAllocCount;

	};
}









#endif