#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>

namespace EAPI
{
	class Console
	{
	private:

		FILE* m_output;

	public:

		Console()
		{
			//COORD t;
			//t.X = 128;
			//t.Y = 4096;

			AllocConsole();
			AttachConsole(GetCurrentProcessId());
			//SetConsoleScreenBufferSize(GetConsoleWindow(), t);

			// CON - console
			m_output= freopen("CON", "w", stdout);
		}

		void Print(char* str)
		{
			printf(str);
		}

		void Printnl(char* str)
		{
			printf("\n%s", str);
		}

	};
}


#endif