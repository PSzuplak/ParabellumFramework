#ifndef _SCENECART_H_
#define _SCENECART_H_


//
// includes
//
#include "Camera.h"
#include "Basket.h"



#define PF_SCENECART_BASKETSCOUNT 7

namespace ParabellumEngine
{
	namespace Core
	{
		namespace Utiles
		{

			// SceneCart - manages objects visible by camera. The idea is to draw visible models most efficiently.
			//
			// Details :
			// Camera frustum may be divided onto few parts. Each of this part may contain some models.  That is the SceneCart purpose
			// SceneCart constains few Baskets - one for every "part" of frustum
			// SceneCart is for better performance - we can draw 1st part first and take advantage of Z-Buffer - no need to do expensive sorting
			// Parts number have to be choosed wisely (not too many), otherwise You lose bonuses from PartsContainer class (limited calls to Graphics Card)
			// SceneCart automatically put models level of detail depends of data like distance from camera etc.
			// 
			class XYZ_API SceneCart
			{


			public:

				// partsCount - 
				SceneCart()
				{
					m_containers = new Basket*[PF_SCENECART_BASKETSCOUNT];

					for (EUINT32 i = 0; i < PF_SCENECART_BASKETSCOUNT; ++i)
					{
						m_containers[i] = new Basket();
					}

					m_LODDistances = new EFLOAT32[PF_SCENECART_BASKETSCOUNT];
					m_LODDistances[0] = 10.0f;
					m_LODDistances[1] = 50.0f;
					m_LODDistances[2] = 120.0f;
					m_LODDistances[4] = 280.0f;
					m_LODDistances[5] = 600.0f;
					m_LODDistances[6] = 1000.0f;

					m_basketsCount = PF_SCENECART_BASKETSCOUNT;
				}

				~SceneCart()
				{
					for (EUINT32 i = 0; i < m_basketsCount; ++i)
					{
						delete m_containers[i];
					}
					delete [] m_containers;

					delete[] m_LODDistances;

					m_basketsCount = 0;
				}



			public:

				EUINT32 GetBasketsCount()
				{
					return m_basketsCount;
				}



				// Get m_containers index based on the distance
				EUINT32 GetContainerIndexBasedOnDistance(Vector3D* cameraPosition, Vector3D* targetPoint)
				{
					EUINT32 index;
					EFLOAT32 distance;

					PF_VECTOR3D_Distance(cameraPosition, targetPoint, distance);

					if (distance < m_LODDistances[0]) index = 0;
					else if (distance < m_LODDistances[1]) index = 1;
					else if (distance < m_LODDistances[2]) index = 2;
					else if (distance < m_LODDistances[3]) index = 3;
					else if (distance < m_LODDistances[4]) index = 4;
					else index = 0;

					return index;
				}


				// apEModels - array of pointer to EModel objects
				// emodelsCount - number of elements in apEModels
				// partsContainerIndex - index in m_containers
				void Add(_IN_ EModel** apEModels, EUINT32 emodelsCount, EUINT32 partsContainerIndex, _IN_ Camera* pCamera)
				{
					for (EUINT32 i = 0; i < emodelsCount; ++i)
						m_containers[partsContainerIndex]->Add(apEModels[i], pCamera);
				}



				void CleanUp()
				{
					for (EUINT32 i = 0; i < m_basketsCount; ++i)
						m_containers[i]->CleanUp();
				}


			public:


				Basket** m_containers;

				EUINT32 m_basketsCount;


				// array of distances which determines where (to which m_objects) model will be added
				EFLOAT32* m_LODDistances;



				EFLOAT32 GetBasketDistance(EUINT32 basketNo)
				{
					return m_LODDistances[basketNo];
				}

			};
		}
	}
}



#endif