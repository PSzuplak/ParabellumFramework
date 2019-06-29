#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <string>
#include <vector>

#include "DLLIE.h"
#include "../ParabellumFramework/TypesC.h"
#include "../ParabellumFramework/MathHelper.h"
#include "ComponentSettings.h"

using namespace ParabellumFramework;

namespace ParabellumEngine
{

	namespace Components
	{

		class XYZ_API ComponentTag
		{
		public:
			ComponentTag(char* tag);
			~ComponentTag();

			bool Is(char* tag);
			bool Is(_IN_ ComponentTag* pCT);


		public:
			EUINT64 m_hash;
			std::string m_tag;

		};


		//
		//
		// Basic part of the system.
		//
		//
		class XYZ_API Component
		{
		private:
			static EUINT32 ComponentConstructorCalls;
			
			static EPTR PointersToComponents[PF_COMPONENT_MAXCOUNT];
	
		// static functions
		public:
			static EUINT32 GetComponentsCount();

			template<typename T>
			static T* GetComponentById(EUINT32 id)
			{
				return (T*)PointersToComponents[id];
			}

		public:
			Component();
			Component(Component&);
			virtual ~Component();


		public:
			virtual void Initialize() = 0;
			virtual void Release() = 0;

			virtual void Draw() = 0;
			virtual void Update() = 0;

		//
		// Methods
		//
		protected:

			void AddTag(char* tag);


		//
		// Members
		//
		private:

			EUINT32	m_id; // unique Id of the component

			std::vector<ComponentTag> m_tags;
			

		//
		// Gets and Sets
		//
		public:

			// retunr unique Id of the Component
			EUINT32 GetId();


		};
	}
}


#endif