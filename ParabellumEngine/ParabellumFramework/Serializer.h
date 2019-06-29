//	------------------------------------------------------------------------------------------------
//
//	Serializer
//
//
//		
//	------------------------------------------------------------------------------------------------

#ifndef _PFSERIALIZER_H_
#define _PFSERIALIZER_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <mutex>

#include "TypesC.h"


#define PF_SERIALIZE_INFO_PTR 1


#define PF_SERIALIZE_HEADER_MASK 346345

namespace ParabellumFramework
{

	namespace Serialize
	{

		struct ObjectHeader
		{
			EUINT32 headerMask = PF_SERIALIZE_HEADER_MASK;
		};


		struct MemberInfoBase
		{
			virtual EUINT64& GetOffset() = 0;
			virtual EUINT64& GetSize() = 0;
			virtual EUINT32& GetDetails() = 0;
			virtual EBYTE* GetData() = 0;
		};

		template<typename T> 
		struct MemberInfo : public MemberInfoBase
		{
			// offset (in Bytes) from the beginning of the class
			EUINT64 offset;

			// member size (in Bytes)
			EUINT64 size = sizeof(T);

			// flags - info about data type - PF_SERIALIZE_INFO_*
			EUINT32 details;

			// member data/value
			EBYTE data[sizeof(T)];


			virtual EUINT64& GetOffset() { return offset; }
			virtual EUINT64& GetSize() { return size; }
			virtual EUINT32& GetDetails() { return details; }
			virtual EBYTE* GetData() { return data; }


		};




		//
		// This class stores details about an object of some class and its members
		//
		template<typename ClassType>
		class Lager
		{
		
			//
			// Constructors and destructors
			//
			public:

				Lager(ClassType* obj)
				{
					m_ptrToObj = obj;
				}

				~Lager()
				{

				}



			//
			// Methods
			//
			public:


			template<typename T>
			void SerializeX(T& input)
			{
				MemberInfoBase* info = new MemberInfo<T>();

				info->GetOffset() = (char*)&input - (char*)m_ptrToObj;
				info->GetDetails() = 0;

				if ( std::is_pointer<T>::value )
				{
					info->GetSize() = sizeof( std::decay<T>::type );
					memcpy( (void*)info->GetData(), &input, sizeof(std::decay<T>::type) );
					info->GetDetails() |= PF_SERIALIZE_INFO_PTR;
				}
				else
				{
					info->GetSize() = sizeof(T);
					memcpy( (void*)info->GetData(), &input, sizeof(T) );
					info->GetDetails() = 0;
				}
								
				m_data.push_back(info);
			}



			template<typename T, typename... Types>
			void SerializeX(T& currentArg, Types&... args)
			{
				SerializeX(currentArg);
				SerializeX(args...);
			}


			template<typename... Types>
			void Serialize(Types&... args)
			{
				SerializeX(args...);
			}


			void ListItems()
			{
				for (auto it : m_data)
				{
					std::cout << "\n-------------------\n";
					std::cout << "   offset : " << it->GetOffset();
					std::cout << "   size   : " << it->GetSize();
				}
			}


			// transfer info stored in 'm_data' to 'output'
			void Transfer(_OUT_ ClassType* output)
			{
				for (auto it : m_data)
				{
					void* ptr = (void*)((EBYTE*)output + it->GetOffset());
					memcpy(ptr, it->GetData(), it->GetSize());
				}
			}


			//
			// Members
			//
			private:

			ClassType* m_ptrToObj;

			std::vector<MemberInfoBase*> m_data;


			//
			// Gets and Sets
			//
			public:

				std::vector<MemberInfoBase*>* GetMembers()
				{
					return *m_data;
				}

			//
			// operators
			//
			public:

				template<typename T>
				friend std::fstream& operator<< (std::fstream& stream, const Lager<T>& rhs);

		};


		template<typename T>
		std::fstream& operator<< (std::fstream& stream, const Lager<T>& rhs)
		{
			ObjectHeader header;

			// write mask
			stream << header.headerMask;

			for (auto it : rhs.m_data)
			{
				//stream << it->GetOffset() << it->GetSize() << it->GetDetails();
			
				for (EUINT32 i = 0; i < it->GetSize(); ++i)
					stream << it->GetData()[i];
			}

			stream.flush();

			return stream;
		}




	}
}






#endif