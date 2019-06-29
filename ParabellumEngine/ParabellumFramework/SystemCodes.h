#ifndef _SYSTEMCODES_H_
#define _SYSTEMCODES_H_

//
//	Predefs
//
#include "DLLIE.h"
#include "TypesC.h"

namespace ParabellumFramework
{
	enum SystemCodes : ParabellumFramework::EUINT32
	{		
		//
		// Common function return codes used by ParabellumFramework
		//
		SUCCESS = 1,
		UNDEFINED = 2,
		FORCEDQUIT,
		ERROR_BADALLOC,
		ERROR_INIT,
		ERROR_OUTOFMEMORYEX,
		ERROR_OUTOFSCOPE,
		ERROR_STACKOVERFLOW,
		ERROR_TOOBIG,
		ERROR_DANGLINGPTR,			// dangling pointer or null pointer
		ERROR_NOFILE,				// can not find file
		ERROR_BADFILETYPE,			// bad filetype
		ERROR_MEMORY_LEAK,
		ERROR_EXISTS_ALREADY,
		ERROR_NOT_EXIST,
		ERROR_NOT_IMPLEMENTED,
		ERROR_ALREADYRUNNING,
		//
		// Graphics codes
		//

	};

	// return code description
	const char* GetCodeDesc(ParabellumFramework::EUINT32 codeNo);


	//
	// System signals
	//
	enum class SystemSignal : ParabellumFramework::EUINT32
	{
		SIGABRT = 6,		//6 	Terminate (core dump) 	Process abort signal
		SIGALRM = 14,		//14 	Terminate 	Alarm clock
		SIGBUS = 1001,		//n/a 	Terminate (core dump) 	Access to an undefined portion of a memory object.
		SIGCHLD = 1002,		//n/a 	Ignore 	Child process terminated, stopped, or continued.
		SIGCONT = 1003,		//n/a 	Continue 	Continue executing, if stopped.
		SIGFPE = 1004,		//n/a 	Terminate (core dump) 	Erroneous arithmetic operation.
		SIGHUP = 1,			//1 	Terminate 	Hangup.
		SIGILL = 1005,		//n/a 	Terminate (core dump) 	Illegal instruction.
		SIGINT = 2,			//2 	Terminate 	Terminal interrupt signal.
		SIGKILL = 9,		//9 	Terminate 	Kill (cannot be caught or ignored).
		SIGPIPE = 1006,		//n/a 	Terminate 	Write on a pipe with no one to read it.
		SIGPOLL = 1007,		//n/a 	Terminate 	Pollable event.
		SIGPROF = 1008,		//n/a 	Terminate 	Profiling timer expired.
		SIGQUIT = 3,		//3 	Terminate (core dump) 	Terminal quit signal.
		SIGSEGV = 1009,		//n/a 	Terminate (core dump) 	Invalid memory reference.
		SIGSTOP = 1010,		//n/a 	Stop 	Stop executing (cannot be caught or ignored).
		SIGSYS = 1011,		//n/a 	Terminate (core dump) 	Bad system call.
		SIGTERM = 15,		//15 	Terminate 	Termination signal.
		SIGTRAP = 1012,		//n/a 	Terminate (core dump) 	Trace/breakpoint trap.
		SIGTSTP = 1013,		//n/a 	Stop 	Terminal stop signal.
		SIGTTIN = 1014,		//n/a 	Stop 	Background process attempting read.
		SIGTTOU = 1015,		//n/a 	Stop 	Background process attempting write.
		SIGUSR1 = 1016,		//n/a 	Terminate 	User-defined signal 1.
		SIGUSR2 = 1017,		//n/a 	Terminate 	User-defined signal 2.
		SIGURG = 1018,		//n/a 	Ignore 	High bandwidth data is available at a socket.
		SIGVTALRM = 1019,	//n/a 	Terminate 	Virtual timer expired.
		SIGXCPU = 1020,		//n/a 	Terminate (core dump) 	CPU time limit exceeded.
		SIGXFSZ = 1020,		//n/a 	Terminate (core dump) 	File size limit exceeded
	};







}








#endif