#include "stdafx.h"
#include "MemoryManager.h"



//	--------------------------------------------------------------------
//		HandlerRaw
//	--------------------------------------------------------------------

ParabellumFramework::Resources::HandlerRaw::HandlerRaw()
{
	m_chunk = nullptr;
}

ParabellumFramework::Resources::HandlerRaw::HandlerRaw(MemoryChunk* chunk)
{
	m_chunk = chunk;
}

ParabellumFramework::Resources::HandlerRaw::HandlerRaw(const HandlerRaw& rhs)
{
	this->m_chunk = rhs.m_chunk;
	m_chunk->references++;
}


ParabellumFramework::Resources::HandlerRaw::~HandlerRaw()
{
	m_chunk->references--;

	if (m_chunk->references == 0)
		MemoryManager::Free(m_chunk);
}


//	--------------------------------------------------------------------
//		Handler
//	--------------------------------------------------------------------


//	--------------------------------------------------------------------
//		Statics
//	--------------------------------------------------------------------

char ParabellumFramework::Resources::MemoryManager::m_memory[] = { 0 };
ParabellumFramework::EUINT64 ParabellumFramework::Resources::MemoryManager::m_topPosition = 0;
ParabellumFramework::EUINT64 ParabellumFramework::Resources::MemoryManager::m_memoryTop = 0;
ParabellumFramework::Resources::MemoryChunk* ParabellumFramework::Resources::MemoryManager::chunks[];
ParabellumFramework::EUINT32 ParabellumFramework::Resources::MemoryManager::chunksCount = 0;
std::list<ParabellumFramework::Resources::MemoryChunk*> ParabellumFramework::Resources::MemoryManager::releasedChunks;

//void* ParabellumFramework::operator new(std::size_t sz)
//void* operator new(std::size_t size)
//{
//	return ParabellumFramework::Resources::MemoryManager::Alloc(size);
//}


//void* operator new[](size_t size) 
//{
//	std::printf("\noperator new[](size_t size) ", size);
//	return ::new char[size];
//}


ParabellumFramework::Resources::MemoryManager& 
ParabellumFramework::Resources::MemoryManager::Instance()
{
	static MemoryManager instance;
	return instance;
}





ParabellumFramework::Resources::MemoryManager::MemoryManager()
{
}

ParabellumFramework::Resources::MemoryManager::~MemoryManager()
{
}



void*
ParabellumFramework::Resources::MemoryManager::Alloc(EUINT32 dataSize, EUINT32 dataCount, EUINT32 settings, EUINT32 alignment)
{
	void* result;
	EUINT64 addr = m_memoryTop;
	MemoryChunk** chunk = &chunks[chunksCount];

	// check if we have reached the heap size
	if (m_memoryTop + dataSize * dataCount + sizeof(MemoryChunk) > PF_MEM_HEAP_SIZE)
	{
		// do syscall
		if (settings & PF_MEMORY_FORCESYSCALL)
			return malloc(dataSize * dataCount);
		// look for a free chunk 
		else
		{
			return NULLPTR;
		}
	}


	// check if there is a freed chunk
	for(MemoryChunk* it : releasedChunks)
	{
		if (it->references == 0)
		{
			EINT32 diff = (it->dataSize * it->dataCount) - dataSize*dataCount;

			// allocate memory under the released chunk only if size of that memory is equal
			if(diff == 0)
			{

				//printf("\n found free chunk = %p", it);
				//printf("\n size of a  chunk = %u", (it->dataSize * it->dataCount));
				//printf("\n diff = %i", diff);

				it->flags = 0;
				it->references = 1;
				it->dataSize = dataSize;
				it->dataCount = dataCount;

				result = (void*)((char*)it + sizeof(MemoryChunk));


				// if there is more space add extra chunk after this one and mark it as free
				//if (diff >= (sizeof(MemoryChunk) + 16 + PG_MEMORY_MAXALIGNMENT))
				//{
				//	MemoryChunk* extraChunk = (MemoryChunk*)((char*)result + dataSize*dataCount);
				//
				//	extraChunk->flags = PF_MEMORY_FREE;
				//	extraChunk->references = 0;
				//	extraChunk->dataSize = diff;
				//	extraChunk->dataCount = 1;
				//
				//	//releasedChunks.push_back(extraChunk);
				//
				//	printf("\n create extra chunk at = %p", extraChunk);
				//}
				

				releasedChunks.remove(it);

				return result;
			}

		}
	
	
	}


	// fit to alignment
	PF_ALIGN(addr, alignment);
	addr += ( sizeof(MemoryChunk) % (1 << alignment) );

	// set chunk data
	(*chunk) = (MemoryChunk*)&m_memory[addr];
	(*chunk)->references = 1;
	(*chunk)->dataSize = dataSize;
	(*chunk)->dataCount = dataCount;

	result = &m_memory[addr + sizeof(MemoryChunk)];

	addr += sizeof(MemoryChunk) + dataCount * dataSize;
	m_memoryTop = addr;

	chunksCount++;

	//PFDLOG_GLOBAL("PMEM", "MMAN", "[Memory] Allocated at : " << (EUINT64)result << "; alignment = " << (1 << alignment) << ";  "<< (EUINT64)result<<"%" << (1 << alignment) << " = " << ((EUINT64)(result)) % (1 << alignment));
	//printf("\n result chunk = %p", (*chunk));
	return result;
}




void 
ParabellumFramework::Resources::MemoryManager::Free(void* ptr)
{
	MemoryChunk* chunk = (MemoryChunk*) ( (char*)ptr - sizeof(chunk) );
	Free(chunk);
}


void 
ParabellumFramework::Resources::MemoryManager::Free(MemoryChunk* pChunk)
{
	//printf("\nFree( %p )", pChunk);
	PrepareToFree(pChunk);
}



void 
ParabellumFramework::Resources::MemoryManager::PrepareToFree(MemoryChunk* chunk)
{
	releasedChunks.push_back(chunk);
}
