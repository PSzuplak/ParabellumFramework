#include "stdafx.h"
#include "DebugManager.h"


ParabellumFramework::DebugManager& ParabellumFramework::DebugManager::Instance()
{
	static DebugManager instance;
	return instance;
}





ParabellumFramework::DebugManager::DebugManager()
{
	time_t rawtime;
	struct tm * timeinfo;
	char timepar[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(timepar, sizeof(timepar), "%d-%m-%Y %I-%M-%S", timeinfo);
	std::string dateStr(timepar);
	std::string filename = "debug_" + dateStr + ".log";

	m_currentPosition = 0;
	m_dumpPositionStart = 0;
	m_dumpPositionEnd = 0;

	m_logFile.open(filename, std::ios::out);

	PutLog("PFDM", "DBMN", 1, __FILE__, __func__, __LINE__) << "DebugManager Created : '" << filename <<"'";
}

ParabellumFramework::DebugManager::~DebugManager()
{
	PutLog("PFDM", "DBMN", 1, __FILE__, __func__, __LINE__) << "DebugManager Destroyed";
	m_logFile.close();
}




void 
ParabellumFramework::DebugManager::Print(const char* filename, const char* funcname, long lineno, const char* input)
{
	printf(input);
}



void 
ParabellumFramework::DebugManager::Print(const char* filename, const char* funcname, long lineno, int input)
{
	printf("%i", input);
}

void
ParabellumFramework::DebugManager::Printnl(const char* filename, const char* funcname, long lineno, const char* input)
{
	printf(input);
}



void
ParabellumFramework::DebugManager::Printnl(const char* filename, const char* funcname, long lineno, int input)
{
	printf("\n%s (%i) ", ParabellumFramework::GetCodeDesc(input), input);
}


void
ParabellumFramework::DebugManager::Printerr(const char* filename, const char* funcname, long lineno, const char* input)
{
	printf(input);
}



void
ParabellumFramework::DebugManager::Printerr(const char* filename, const char* funcname, long lineno, int input)
{
	printf("%s (%i) ", ParabellumFramework::GetCodeDesc(input), input);
}



//
// savelog in the buffer
//
std::fstream&
ParabellumFramework::DebugManager::PutLog(
	const char* apid, const char* cpid, EUINT8 logLevel,
	const char* filename, const char* funcname, int lineNo)
{
	if (m_currentPosition > DEBUG_LOG_BUFFER_SIZE - 2)
		assert(!"DebugManager buffer overflow");
	
	std::lock_guard<std::mutex> guard(m_mutexPutLog);

	EUINT32 pos = m_currentPosition++;

	m_logBuffer[pos].no = m_recordsCounter++;
	//m_logBuffer[pos].time = std::clock();
	memcpy(m_logBuffer[pos].apid, apid, sizeof(char)*4);
	memcpy(m_logBuffer[pos].cpid, cpid, sizeof(char)*4);
	//m_logBuffer[pos].logLevel = logLevel;
	//strcpy(m_logBuffer[pos].filename, filename);
	strcpy(m_logBuffer[pos].funcname, funcname);
	m_logBuffer[pos].lineNo = lineNo;
	//strcpy(m_logBuffer[pos].msg, msg);

	m_logFile
		<< "\n"
		//<< m_logBuffer[pos].no << "\t"
		//<< m_logBuffer[pos].time << "\t"
		<< m_logBuffer[pos].apid << "\t"
		<< m_logBuffer[pos].cpid << "\t"
		//<< m_logBuffer[pos].logLevel << "\t"
		//<< m_logBuffer[pos].filename << "\t"
		<< m_logBuffer[pos].funcname << "\t"
		<< m_logBuffer[pos].lineNo << "\t";

	m_logFile.flush();
	
	m_currentPosition = 0;

	return m_logFile;
}
