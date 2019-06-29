//
// This container is used to store unique values 
//
//
//

#ifndef _DECK_H_
#define _DECK_H_

#define PF_DECK_MAXSIZE 4096


#include "DLLIE.h"
#include "../ParabellumFramework/TypesC.h"

#include <stdio.h>

namespace ParabellumFramework
{

	//
	// This container is used to store unique values. For example if we "Add" these numbers : 1,2,2,4,4,5,7,7,7,1 then
	// Deck stores values : 1,2,4,5,7 and we are able to iterate only on these values, 
	// so f.ex we are not forced to repeat operations for the same value
	//
	class XYZ_API Deck
	{
	public:
		Deck()
		{
			m_uniqueObjectsCount = 0;

			for (EUINT32 i = 0; i < PF_DECK_MAXSIZE; ++i)
			{
				m_revision[i] = 0;
				m_objectsCounter[i] = 0;
			}
		}

		Deck(Deck&) = delete;

		~Deck()
		{
		}


	//
	// Methods
	//
	public:

		void CleanUp()
		{
			for (EUINT32 i = 0; i < m_uniqueObjectsCount; ++i)
			{
				m_objectsCounter[m_backward[m_forward[i]]] = 0;
				m_revision[m_forward[i]] = 0;
			}

			m_uniqueObjectsCount = 0;
		}

		void Add(EUINT32 no)
		{
			EUINT32 ibId = no;

			if (m_revision[ibId] == 0)
			{
				m_revision[ibId] = 1;

				m_forward[m_uniqueObjectsCount] = ibId;
				m_backward[ibId] = m_uniqueObjectsCount;

				m_objectsCounter[m_backward[ibId]] = 1;

				m_uniqueObjectsCount++;
			}
			else
			{
				m_objectsCounter[m_backward[ibId]]++;

				m_revision[ibId]++;
			}

		}


		// check if the deck contains specific number
		bool Contains(EUINT32 no)
		{
			for (EUINT32 i = 0; i < m_uniqueObjectsCount; ++i)
			{
				if (m_forward[i] == no)
					return true;
			}

			return false;
		}


		EUINT32 GetForward(EUINT32 no)
		{
			return m_forward[no];
		}





		void Print()
		{
			for (EUINT32 i = 0; i < m_uniqueObjectsCount; ++i)
			{
				printf("%u, ", m_forward[i]);
			}
		}

	//
	// Members
	//
	public:

		// how many unique objects were Added to PartsContainer during this Frame
		// each IndexBuffer has unique id - if it was NOT added before in this fram
		// then variable m_uniqueObjectsCount is NOT increased.
		// It is being increased only when IndexBuffer is added FIRST time
		EUINT32 m_uniqueObjectsCount;


		// whenever IndexBuffer is added it gets unique identifier which is greater by one than previous IndexBuffer
		// m_revision is an "infinity" array which can be interpreted that each of its index is unique id of IndexBuffer
		// so if IndexBuffer unique id is 123, and m_revision[123] == 1, then we can say that IndexBuffer with unique 123
		// is inside Camera Frustum
		// m_revision[id] is increased each time IndexBuffer with the same ID is added
		EUINT32 m_revision[PF_DECK_MAXSIZE];

		// how many the same IndexBuffer (with the same uniqie id) are in Camera Frustum
		EUINT32 m_objectsCounter[PF_DECK_MAXSIZE];

		// contains indices to m_backward
		EUINT32 m_forward[PF_DECK_MAXSIZE];

		// contains indices to m_forward
		EUINT32 m_backward[PF_DECK_MAXSIZE];






	//
	// Operators
	//
	public:

		//EUINT32 operator[](unsigned int index) { return m_data[index]; }

	//
	// Gets And Sets
	//
	public:

		EUINT32 GetUniqueObjectsCount()
		{
			return m_uniqueObjectsCount;
		}


	};
}





#endif