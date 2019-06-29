#include "stdafx.h"
#include "Profiler.h"

ParabellumFramework::EUINT32 ParabellumFramework::Profiler::InstancesCount = 0;
std::map<ParabellumFramework::EUINT64, ParabellumFramework::EUINT32> ParabellumFramework::Profiler::InstancesIds = {};


ParabellumFramework::Profiler::Profiler()
{
	m_nodesDepth = 0;
	m_nodesCount = 0;
	m_beginCallsCount = 0;
	m_CPUFrameAllocCount = 0;
	m_CPUTotalAllocCount = 0.0f;
}

ParabellumFramework::Profiler::~Profiler()
{
	m_nodesDepth = 0;
	m_nodesCount = 0;
	m_beginCallsCount = 0;
}



ParabellumFramework::Profiler& 
ParabellumFramework::Profiler::Instance()
{
	static Profiler Instances[PF_PROFILER_MAX_INSTANCES];

	if (InstancesCount >= PF_PROFILER_MAX_INSTANCES)
		assert(!"Assertion - Too many Profile instances in too many threads");

	EUINT32 key = GetCurrentThreadId();

	if (InstancesIds.find(key) == InstancesIds.end())
	{
		EUINT32 id = InstancesCount;
		InstancesCount++;
		InstancesIds[key] = id;

		return Instances[id];
	}
	else
	{
		return Instances[InstancesIds[key]];
	}

}



void 
ParabellumFramework::Profiler::Begin(EUINT64 key, const char* nodeName)
{
	if (m_nodesIds.find(key) == m_nodesIds.end())
	{
		m_nodesIds[key] = m_nodesCount;
		m_nodesInfo[m_nodesCount].nodeName = nodeName;
		m_nodesInfo[m_nodesCount].isEnabled = false;

		m_nodesCount++;
	}

	EUINT32 id = m_nodesIds[key];

	if (m_nodesInfo[id].isEnabled == true)
	{
		assert(!"ParabellumFramework[Profiler] : Profiler Node was not closed - in ProfileBegin()");
	}



	// if parent node it means that profiler cycle starts from the beginning
	// and we have to clear info about children
	if (m_nodesDepth == 0)
	{
		for (EUINT32 i = 0; i < m_nodesCount; ++i)
		{
			m_nodesInfo[i].timeTotal = 0;
			m_nodesInfo[i].openCount = 0;
			m_nodesInfo[i].mallocCount = 0;
			m_nodesInfo[i].freeCount = 0;
		}

		// set parent key
		m_keysOnStack[0] = key;
		m_beginCallsCount = 1;

		// clear memory rate
		m_CPUFrameAllocCount = 0;
	}
	// else set a key for childrens
	else
	{
		if (m_beginCallsCount < PF_PROFILER_MAX_INFOSTACK)
		{
			m_keysOnStack[m_beginCallsCount] = key;
			m_beginCallsCount++;
		}
		else
		{
			// Profiles stack out of memory - probably has been put in big loop
			assert(!"Profiler.cpp ProfileBegin() - Too many opened nodes");
		}
	}


	// set node
	m_nodesInfo[id].isEnabled = true;
	m_nodesInfo[id].depth = m_nodesDepth;
	m_nodesInfo[id].openCount += 1;
	m_nodesDepth++;

	// get time at the beginning of the function - this function takes a loot of cycles itself, so we have to sub to get proper value
	m_nodesInfo[id].timeStart = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

	// set key as newest
	m_currentNodeKey = key;
}


void 
ParabellumFramework::Profiler::End(EUINT64 key)
{
	long long truetime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

	m_nodesDepth--;
	
	EUINT32 id = m_nodesIds[key];
	m_nodesInfo[id].timeEnd = truetime;
	m_nodesInfo[id].isEnabled = false;

	m_nodesInfo[id].timeTotal += m_nodesInfo[id].timeEnd - m_nodesInfo[id].timeStart;
	m_nodesInfo[id].mallocCount = m_CPUFrameAllocCount;

	// check allocated memory
	if (m_CPUFrameAllocCount > 0)
	{
		#ifdef DEBUG_LOGLEVEL_ERROR
		PFDLOG("PFPM", "PROF", "[WARNING] memory dynamically allocated " << m_CPUFrameAllocCount << " times during runtime");
		#endif
	}

}