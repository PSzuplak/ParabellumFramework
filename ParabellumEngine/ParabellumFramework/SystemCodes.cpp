#include "stdafx.h"
#include "SystemCodes.h"

const char* ParabellumFramework::GetCodeDesc(ParabellumFramework::EUINT32 codeNo)
{
	static const char* dSUCCESS								= "success";
	static const char* dUNDEFINED							= "undefined error";
	static const char* dFORCEDQUIT							= "forced to quit";
	static const char* dEXCEPTION							= "exception";
	static const char* dERROR_BADALLOC						= "bad alloc";
	static const char* dERROR_INIT							= "error during initialization";
	static const char* dERROR_OUTOFMEMORY					= "out of memory";
	static const char* dERROR_OUTOFSCOPE					= "out of scope";
	static const char* dERROR_STACKOVERFLOW					= "stack overflow";
	static const char* dERROR_TOOBIG						= "too big file";
	static const char* dERROR_DANGLINGPTR					= "dangling pointer";
	static const char* dERROR_NOFILE						= "no such file";
	static const char* dERROR_BADFILETYPE					= "bad file type";
	static const char* dERROR_MEMORY_LEAK					= "memory leak";
	static const char* dERROR_EXISTS_ALREADY				= "already exists";
	static const char* dERROR_NOT_IMPLEMENTED				= "not implemented";
	static const char* dERROR_ALREADYRUNNING				= "already running";
	

	switch (codeNo)
	{
	case SUCCESS:						return dSUCCESS; break;
	case UNDEFINED:						return dUNDEFINED; break;
	case FORCEDQUIT:					return dFORCEDQUIT; break;
	case ERROR_BADALLOC:				return dERROR_BADALLOC; break;
	case ERROR_INIT:					return dERROR_INIT; break;
	case ERROR_OUTOFMEMORYEX:			return dERROR_OUTOFMEMORY; break;
	case ERROR_OUTOFSCOPE:				return dERROR_OUTOFSCOPE; break;
	case ERROR_STACKOVERFLOW:			return dERROR_STACKOVERFLOW; break;
	case ERROR_TOOBIG:					return dERROR_TOOBIG; break;
	case ERROR_DANGLINGPTR:				return dERROR_DANGLINGPTR; break;
	case ERROR_NOFILE:					return dERROR_NOFILE; break;
	case ERROR_BADFILETYPE:				return dERROR_BADFILETYPE; break;
	case ERROR_MEMORY_LEAK:				return dERROR_MEMORY_LEAK; break;
	case ERROR_EXISTS_ALREADY:			return dERROR_EXISTS_ALREADY; break;
	case ERROR_NOT_IMPLEMENTED:			return dERROR_NOT_IMPLEMENTED; break;
	case ERROR_ALREADYRUNNING:			return dERROR_ALREADYRUNNING; break;

	default:							return dUNDEFINED; break;
	}
}