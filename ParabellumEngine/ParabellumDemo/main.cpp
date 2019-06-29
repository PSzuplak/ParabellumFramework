#define DLLIMPORT


//
// libs
//


#include "../ParabellumFramework/DebugManager.h"

#include "../ParabellumEngine/SystemManager.h"


#include "Demo01.h"
#include "Console.h"
#include "debug_test.h"
#include "security.h"

using namespace ParabellumEngine;

#include <typeinfo>
#include <algorithm>
#include <iterator>
#include <string>
#include "../ParabellumFramework/PFString.h"

#include "../ParabellumFramework/Conversions.h"

#include "../ParabellumFramework/Serializer.h"

#include "../ParabellumUtils/Float32_4.h"

#include <type_traits>

class Fctor
{
public:

	int flag;

	Fctor()
	{
		auto lam = [&]() {flag = 5;};
	}

	void operator()(std::string& msg)
	{
		std::cout << "t1 says " << msg;
	}
};

struct XXXX
{
	int u;
};

#include <limits.h>


#define maxl 20


typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

uint8_t array[maxl];

typedef struct
{
	uint8_t a;
	uint8_t b;
	uint8_t c;
} AAA;


#include <string.h>

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

uint8_t array[maxl];

typedef struct
{
	uint32_t a;
	uint32_t b;
	uint32_t c;
} AAA;

typedef struct
{
	uint32_t a;
	uint32_t b;
	uint32_t c;
} BBB;

void foo()
{
	AAA* val = NULL;
	BBB bbb;
	uint32_t i = 0u;

	bbb.a = 123;
	bbb.b = 567;
	bbb.c = 789;

	//for(i=0; i<maxl; ++i)
	//{
	//    array[i] = i;
	//}

	memcpy(&val, &array, sizeof(array));
	//printf(" memcpy(%p, %p, %u)", &val, &array, sizeof(&array));

	//printf(" &array   = %p", array);
	//printf(" array    = %p", &array);


	//printf(" val.a = %u", val->b);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	// initialize console output
	EAPI::Console con;

	int a = -4;
	AAA* val = NULL;
	int b = -4;

	std::cout << typeid(&val).name();

	std::cout << "\n" << "  val =  " << (unsigned long)val;
	std::cout << "\n" << " &val =  " << (unsigned long)&val;

	memcpy(&val, &b, sizeof(&b));

	std::cout << "\n" << "  val =  " << (unsigned long)val;
	std::cout << "\n" << " &val =  " << (unsigned long)&val;

	while (true) {}
	//return 0;







	// system instance
	Demo01 sysInstance;

	PFDLOG_GLOBAL("MAIN", "MAIN", "START WinMain thread : " );

	System::SystemManager* systemManager;
	EINT32 result;

	systemManager = new System::SystemManager();

	if (systemManager == NULLPTR)
	{
		PFDLOG_GLOBAL("MAIN", "MAIN", "FAIL allocating memory for SystemManager");
		return SystemCodes::ERROR_BADALLOC;
	}


	//	Initialize and Execute systemManager
	result = systemManager->Initialize(&sysInstance);
	if (result == SystemCodes::SUCCESS)
	{
		systemManager->Execute();
	}
	else
	{
		Sleep(1000);
		return SystemCodes::FORCEDQUIT;
	}
	

	// Release SystemManager, memory then quit
	result = systemManager->Release();
	delete systemManager;
	systemManager = NULLPTR;

	if (result == SystemCodes::SUCCESS)
	{
		Sleep(1000);
		return 0;
	}
	else
	{
		PFDLOG_GLOBAL("MAIN", "MAIN", "ERROR releasing SystemManager :" << result);
		Sleep(1000);
		return result;
	}


	//
	// Statistics
	// 
	PFDLOG_GLOBAL("MAIN", "MAIN", "Program exited with some memory leaks");

	return 0;
}