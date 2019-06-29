//
//	Contains some group of Component3D
//


#ifndef _BASKET_H_
#define _BASKET_H_


//
// includes
//
#include "Camera.h"

#include "../ParabellumFramework/ModelPart.h"
#include "EModel.h"
#include "EModelPart.h"


#define PE_BASKET_MAXOBJECTSINBASKET 2048	// how many DIFFERENT models (unique ModelPArt ID) can be rendered
#define PE_BASKET_MAXSAMEOBJECTS 1024		// how many models with the SAME id can be rendered


using namespace ParabellumFramework;
using namespace ParabellumFramework::Resources;
using namespace ParabellumEngine::Components;


namespace ParabellumEngine
{
	namespace Core
	{
		namespace Utiles
		{

			//
			// Basket is a container for visible ModelParts. It uses advantage of unique ID (number) of loaded mesh. Each ModelPart has unique index buffer (id). 
			// Basked tells us how many types of ModelParts do we have on the screen - so we can group them and draw without repeating setting vertex buffer
			//
			class XYZ_API Basket
			{
			public:


				Basket();

				~Basket();

				// CleanUP previous frame data
				void CleanUp();

				void Add(EModel* model, _IN_ Camera* pCamera);


				//
				// Add ModelPart to PartsContainer
				// ibId - index of Index Buffer from array Model::ModelObjectsType
				//
				void Add(EModel* emodel, EModelPart* model);


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
				EUINT32 m_revision[PE_BASKET_MAXOBJECTSINBASKET];

				// how many the same IndexBuffer (with the same uniqie id) are in Camera Frustum
				EUINT32 m_objectsCounter[PE_BASKET_MAXOBJECTSINBASKET];

				// pointers to EModelPart which are in Camera Frustum. 
				EModelPart* m_refComponent3D[PE_BASKET_MAXSAMEOBJECTS][PE_BASKET_MAXOBJECTSINBASKET];

				// pointers to EModelPart which ae in Camera Frustum. 
				EModelPart* m_objects[PE_BASKET_MAXOBJECTSINBASKET];

				// contains indices to m_backward
				EUINT32 m_forward[PE_BASKET_MAXOBJECTSINBASKET];

				// contains indices to m_forward
				EUINT32 m_backward[PE_BASKET_MAXOBJECTSINBASKET];



			//
			// Gets and Sets
			//
			public:

				// how many unique objects have been picked
				EUINT32 GetPickedUniqueObjectsCount();

				// how many objects with unique index have been picked
				EUINT32 GetObjectsCountOfIndex(EUINT32 index);

				// Get object where index is uniqie ID - 
				EModelPart* GetObjectOfIndex(EUINT32 index, EUINT32 no);

			};


		}
	}
}





#endif