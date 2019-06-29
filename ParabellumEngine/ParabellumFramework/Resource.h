//	------------------------------------------------------------------------------------------------
//
//	Resource is a data used by ParabellumFramework managers, readed from external souces
//	
//	Example :
//		Resource image;
//		image.Load("wood.bmp");
//
//	------------------------------------------------------------------------------------------------

#ifndef _RESOURCE_H_
#define _RESOURCE_H_


//
//	Predefs
//
#include "DLLIE.h"


#include <string>

#include "TypesC.h"
#include "SystemCodes.h"
#include "MemoryManager.h"
#include "DebugManager.h"
#include "Profiler.h"
#include "Parser.h"


namespace ParabellumFramework
{
	namespace Resources
	{

		class XYZ_API Resource
		{
			friend class XYZ_API ResourceManager;

		protected:
			Resource();
			Resource(Resource& ref) = delete;
			virtual ~Resource();

		private:

			// load content from file or memory - call Parse(...) methods
			virtual EUINT32 Load(const char* filename) final;

			// release resources
			virtual EUINT32 Release() = 0;

			// Parse file or memory. Should be overloaded by a children
			virtual EUINT32 Parse(const char* filename) = 0;


			// parse file using external Parser
			template<typename ParserType, typename ClassType>
			void Parse(_IN_ Parser<ParserType, ClassType> pParser)
			{

			}


		//
		// Members
		//
		private:

			// size of resource in BYTES
			size_t m_resourceSize;		

			// pointer to resource
			
			std::string m_resourceName;
			std::wstring m_resourceNameW;
		

			// is initialized
			bool m_isInitialized;

			// is created properly and ready to use - some resources needs to be set on graphics card
			bool m_isCreated;

		public:

			// get the resource data
			virtual void GetData(void*);	

			template < typename T>
			T* Get()
			{
				return (T*)m_resource;
			}


			std::string& GetName()
			{
				return m_resourceName;
			}


			std::wstring& GetNameW()
			{
				return m_resourceNameW;
			}


			bool IsInitialized() const
			{
				return m_isInitialized;
			}

			bool IsCreated() const
			{
				return m_isCreated;
			}

			void Create()
			{
				m_isCreated = true;
			}

		//
		// operators
		//
		private:
			Resource& operator=(const Resource& right) = delete;
		};
	}
}


#endif