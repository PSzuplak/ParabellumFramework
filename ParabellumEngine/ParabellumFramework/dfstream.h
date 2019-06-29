#ifndef _DFSTREAM_H_
#define _DFSTREAM_H_


#include "stdafx.h"
#include "DLLIE.h"

#include "TypesC.h"

#include <fstream>
#include <string>

#include <map>


namespace ParabellumFramework
{
	// NOT FINISHED
	// dfstream uses hash table to store strings
	// it reduces memory needed to create files and others
	// input data can also be encrypted / decrypted
	class XYZ_API dfstream
	{
	public:

	public:

		unsigned int open(char* path)
		{
			if (m_file.is_open())
			{
				m_file.open(path);
				return 0;
			}
			else
				return 1;
		}

		unsigned int close()
		{
			if (!m_file.is_open())
			{
				m_file.close();
				return 0;
			}
			else
				return 2;
		}


	//
	// operators
	//
	public:

		dfstream& operator<<(std::string rhs)
		{

			return *this;
		}


	//
	// Members
	//
	private:
		std::fstream m_file;

		std::map <EUINT32, std::string> m_lookup;

	};

}

#endif
