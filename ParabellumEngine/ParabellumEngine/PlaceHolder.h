//
//	Contains list of models, light etc. 
//	It may be a part of Octree, Portals, etc.
//

#ifndef _PLACEHOLDER_H_
#define _PLACEHOLDER_H_

#define PE_PLACEHOLDER_MAXMODELS		256


#include "DLLIE.h"

#include <list>
#include <map>
#include <queue>

#include "../ParabellumFramework/SystemCodes.h"
#include "../ParabellumFramework/TypesC.h"
#include "../ParabellumFramework/Vector3D.h"

#include "Component3D.h"
#include "EModel.h"


namespace ParabellumEngine
{
	namespace Core
	{

		namespace Scene
		{
			//
			//	Contains list of models, light etc. 
			//	It may be a part of Octree, Portals, etc.
			//
			class XYZ_API PlaceHolder
			{

			//
			// Constructors 
			//
			public:
				PlaceHolder();
				PlaceHolder(PlaceHolder&) = delete;
				~PlaceHolder();


			//
			// Methods
			//
			public:

				// pComponent MUST derive from Component3D! For better performance it does NOT check class derivation or if object is already in octree!
				//template<typename T>
				//void Add(_IN_ T* pComponent)
				//{
				//}

				void Add(_IN_ ParabellumEngine::Components::EModel* pComponent)
				{
					m_objectsEModel[m_EModelsCount] = pComponent;
					m_EModelsCount++;
				}



				// clear ALL type of objects
				void ClearAll()
				{
					Clear<ParabellumEngine::Components::EModel>();
				}

				template<typename T>
				void Clear()
				{
					PF_DEBUG_ASSERT("Unknow type");
				}

				template<>
				void Clear<ParabellumEngine::Components::EModel>()
				{
					m_EModelsCount = 0;
				}



				// get count
				template<typename T>
				EUINT32 GetCount()
				{
					PF_DEBUG_ASSERT("Unknow type");
				}

				template<>
				EUINT32 GetCount<ParabellumEngine::Components::EModel>()
				{
					return m_EModelsCount;
				}



			//
			// Members
			//
			public:
				ParabellumEngine::Components::EModel* m_objectsEModel[PE_PLACEHOLDER_MAXMODELS];

				EUINT32 m_EModelsCount;

			};
		}
	}
}


#endif