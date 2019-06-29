#ifndef _FRUSTUMMANAGER_H_
#define _FRUSTUMMANAGER_H_


#include "DLLIE.h"

#include <list>

#include <thread>
#include <mutex>

#include "../ParabellumFramework/SystemCodes.h"
#include "../ParabellumFramework/TypesC.h"
#include "../ParabellumFramework/Narray.h"
#include "../ParabellumFramework/Vector3D.h"


#include "Camera.h"

#include "Octree.h"
#include "MapSector.h"
#include "SceneCart.h"

#include "RenderTree.h"


using namespace ParabellumFramework;
using namespace ParabellumEngine::Core::Scene;

namespace ParabellumEngine
{
	namespace Core
	{

		namespace Utiles
		{

			//
			// Contains models, particles, skybox and all other objects which should be drawn in the current frame
			// Used directly by ParabellumEngine Renderers
			//
			class XYZ_API FrustumOutput
			{
			public:

				FrustumOutput(EUINT32 width, EUINT32 height);

				FrustumOutput(FrustumOutput&) = delete;
				~FrustumOutput();

			//
			// Methods
			//
			public:

				void Update(
					_IN_ GraphicsDevice* pDevice, _IN_ RenderTarget* pRenderTarget,
					_IN_ MapSector* pMapSecor, _IN_ RenderTree* pRenderTree, _IN_ Camera* pCamera);

	

			public:

				// check if basket is ready to draw - if all the models are inside basket
				bool IsBasketReady(EUINT32 no);


				void Lock();

			//
			// Members
			//
			public:
				

				ParabellumEngine::Core::Utiles::SceneCart m_sceneCart[2];

				EUINT32 m_sceneCartFront;
				EUINT32 m_sceneCartBack;

				Narray<Octree*>* m_visibleOctreeNodes;

				Narray<Octree*>* m_visibleOctreeNodesInBaskets;

				volatile bool m_isCartReady[32];

				volatile bool m_threadUpdateCartsRunning;

				std::thread m_threadUpdateCarts;

				MapSector* m_mapSecor;
				RenderTree* m_RenderTree;
				Camera* m_camera;

				// if 'true' then 'Update()' will not happen
				volatile bool m_isLocked;

				// if 'true' then frustumOutup will be locked when if finish 'Update'
				volatile bool m_prepareToLock;

			//
			// Gets And sets
			//
			public:

				ParabellumEngine::Core::Utiles::SceneCart* GetSceneCart();

			};
		}
	}
}


#endif