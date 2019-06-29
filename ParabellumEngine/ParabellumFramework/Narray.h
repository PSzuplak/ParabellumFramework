#ifndef _NARRAY_H_
#define _NARRAY_H_

#include "DLLIE.h"
#include "../ParabellumFramework/TypesC.h"

namespace ParabellumFramework
{
	template <typename T> 
	class XYZ_API Narray
	{

	public:
		Narray(EUINT32 size = 1024)
		{
			m_size = size;
			m_data = new T[size];

			m_count = 0;
		}

		Narray(Narray&) = delete;

		~Narray()
		{
			delete[] m_data;
		}


	//
	// Methods
	//
	public:

		void Add(T tobj)
		{
			m_data[m_count] = tobj;
			m_count++;
		}

		void Clear()
		{
			m_count = 0;
		}


		EUINT32 GetCount()
		{
			return m_count;
		}

		EUINT32 GetSize()
		{
			return m_size;
		}

		T& GetItem(EUINT32 index)
		{
			return m_data[index];
		}

	//
	// Members
	//
	private:

		T* m_data;
		EUINT32 m_count;
		EUINT32 m_size;

	//
	// Operators
	//
	public:

		T& operator[](unsigned int index) { return m_data[index]; }


	};
}





#endif