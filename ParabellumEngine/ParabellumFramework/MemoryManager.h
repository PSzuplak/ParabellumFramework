#ifndef _MEMORYMANAGER_H_
#define _MEMORYMANAGER_H_

//
//	Predefs
//
#include "DLLIE.h"
#include "Settings.h"
#include "Convenctions.h"
#include "DebugManager.h"
#include "DebugHelper.h"
#include "TypesC.h"
#include "SystemCodes.h"
#include "FlagHelper.h"

#include <cstdio>
#include <cstdlib>
#include <memory>
#include <list>

namespace
{
	//XYZ_API void* operator new(std::size_t size);
	//XYZ_API void* operator new[](std::size_t size);
}


#define PF_MEMORY_FREE			EBYTENO_1
#define PF_MEMORY_CODE			EBYTENO_10


#define PF_MEMORY_MAXCHUNKS		4096


#define PF_MEMORY_FORCESYSCALL	EBYTENO_2

#define PG_MEMORY_MAXALIGNMENT	4



// align value to bits. Used for memory alignment
#define PF_ALIGN(value, alignment) { value &= ~((1 << alignment) - 1); value += (1 << alignment); }



namespace ParabellumFramework
{
	namespace Resources
	{

		class MemoryManager;



		struct MemoryChunk
		{
			union
			{

				struct
				{
					// details about allocated memory - using PF_MEMORY_* values
					EBYTE flags;

					// how many referennces are to this memory
					EUINT16 references;

					// size of a single entity in Bytes
					EUINT32 dataSize;

					// number of entietes if we deal with an array
					EUINT16 dataCount;
				};


			};

			MemoryChunk()
			{
				flags = 0;
				references = 0;
				dataSize = 0;
				dataCount = 0;
			}
		};




		// ---------------------------------------------------------------------------------------
		// smart pointer used and managed by MemoryManager
		//
		class XYZ_API HandlerRaw
		{
			//
			// firendship
			//
			friend class MemoryManager;

		public:
			HandlerRaw();

		public:
			HandlerRaw(MemoryChunk* chunk);

			HandlerRaw(const HandlerRaw& rhs);

		public:

			virtual ~HandlerRaw();

		//
		// Members
		//
		public:
			
			MemoryChunk* m_chunk;


		//
		// operators
		//
		public:

			HandlerRaw& operator=(HandlerRaw& rhs)
			{
				this->m_chunk = rhs.m_chunk;
				m_chunk->references++;
				return *this;
			}
		};





		// ---------------------------------------------------------------------------------------
		// smart pointer used and managed by MemoryManager
		//
		template<typename T>
		class Handler : public HandlerRaw
		{
			//
			// firendship
			//
			friend class MemoryManager;

		public:
			Handler() : HandlerRaw() { }

		public:
			Handler(MemoryChunk* chunk) : HandlerRaw(chunk) { }

		public:
			Handler(const Handler<T>& rhs)
				: HandlerRaw(rhs)
			{
			}

			Handler(const HandlerRaw& rhs)
				: HandlerRaw(rhs)
			{
			}


			~Handler()
			{

			}

			//
			// operators
			//
		public:

			T* operator->()
			{
				return (T*)(m_chunk + sizeof(MemoryChunk));
			}

		};




		// ---------------------------------------------------------------------------------------
		// allocates memory faster than syscalls
		// The best results we get using the similar size of structs
		// It should not be fully replaced by new/malloc
		// Use it if you can not allocate memory on the stack or you have to save this memory in a file
		//
		class XYZ_API MemoryManager
		{
		//
		// Statics
		//
		public:

			static MemoryManager& Instance();

		private:

			// reserved memory for manager - used as heap data structure
			static char m_memory[PF_MEM_HEAP_SIZE];

			// how many bytes is reserved
			static EUINT64 m_reserved;

			// lastest position in m_memory
			static EUINT64 m_topPosition;

			// top position on the heap
			static EUINT64 m_memoryTop;

			static MemoryChunk* chunks[PF_MEMORY_MAXCHUNKS];
			static EUINT32 chunksCount;

			static std::list<MemoryChunk*> releasedChunks;


		private:
			MemoryManager();
			~MemoryManager();

		public:


			//
			//
			// alignment - alignment memory in Bytes; alignment=4 means a struct is aligned to 16bits
			//
			static PF_INLINE void* PF_FASTCALL Alloc(EUINT32 dataSize, EUINT32 dataCount, EUINT32 settings = PF_MEMORY_FORCESYSCALL, EUINT32 alignment = 0);

			template<typename T>
			static PF_INLINE
			T* 
			PF_FASTCALL
			Alloc(EUINT32 count)
			{
				return (T*)Alloc(sizeof(T), count, 0, 4);
			}

			template<typename T>
			static Handler<T> CreateHandler()
			{
				T* data = Alloc<T>(1);
				//printf("\nCreate handler with chunk = %p [%p]", (MemoryChunk*)((char*)data - sizeof(MemoryChunk)), handler.m_chunk);

				return CreateHandler<T>( (MemoryChunk*)((char*)data - sizeof(MemoryChunk)) );
			}

			// free memory allocated by Alloc. Memory is going to be freed only if there is no Handler out there.
			static void Free(void* ptr);

			// free memory allocated by Alloc. Memory is going to be freed only if there is no Handler out there.
			static void Free(MemoryChunk* pChunk);


		private:


			// mark a chunk as one to freed
			static void PrepareToFree(MemoryChunk* chunk);


			
			template<typename T>
			static Handler<T> CreateHandler(MemoryChunk* chunk)
			{
				Handler<T> handler(chunk);
				return handler;
			}



		//
		// Members
		//
		private:

			//std::vector<HandlerBase*> m_handlers;



		};





	}
}


#endif