#include "FrustumOutput.h"



//	------------------------------------------------------
//	Constructors and destructors
//	------------------------------------------------------

ParabellumEngine::Core::Utiles::FrustumOutput::FrustumOutput(EUINT32 width, EUINT32 height)
	: m_threadUpdateCarts{}
{
	m_sceneCartFront = 0;
	m_sceneCartBack = 1;

	m_sceneCart[m_sceneCartFront].CleanUp();
	m_sceneCart[m_sceneCartBack].CleanUp();

	m_visibleOctreeNodes = new Narray<Octree*>(1024);
	m_visibleOctreeNodesInBaskets = new Narray<Octree*>[m_sceneCart[m_sceneCartFront].GetBasketsCount()];

	for (EUINT32 i = 0; i < m_sceneCart[m_sceneCartFront].GetBasketsCount(); ++i)
		m_isCartReady[i] = false;

	//m_threadUpdateCarts = std::thread{ &FrustumOutput::UpdateCarts, this };
	//m_threadUpdateCarts.detach();
}

ParabellumEngine::Core::Utiles::FrustumOutput::~FrustumOutput()
{
	delete m_visibleOctreeNodes;
	delete[] m_visibleOctreeNodesInBaskets;
}


//	------------------------------------------------------
//	Methods
//	------------------------------------------------------

void
ParabellumEngine::Core::Utiles::FrustumOutput::Update(
	_IN_ GraphicsDevice* pDevice, _IN_ RenderTarget* pRenderTarget,
	_IN_ MapSector* pMapSecor, _IN_ RenderTree* pRenderTree, _IN_ Camera* pCamera)
{

	if (m_isLocked == true)
		return;

	// replace pointers
	m_mapSecor = pMapSecor;
	m_RenderTree = pRenderTree;
	m_camera = pCamera;


	for (EUINT32 i = 0; i < m_sceneCart[m_sceneCartFront].GetBasketsCount(); ++i)
		m_isCartReady[i] = false;

	// get all visible octree nodes
	m_visibleOctreeNodes->Clear();
	m_mapSecor->GetNodes(m_visibleOctreeNodes, m_camera);

	m_sceneCart[m_sceneCartFront].CleanUp();

	for (EUINT32 i = 0; i < m_visibleOctreeNodes->GetCount(); ++i)
	{
		EUINT32 index = m_sceneCart[m_sceneCartFront].GetContainerIndexBasedOnDistance(m_camera->GetPosition(), &m_visibleOctreeNodes->GetItem(i)->m_center);
		m_visibleOctreeNodesInBaskets[index].Add(m_visibleOctreeNodes->GetItem(i));
	}


	for (EUINT32 i = 0; i < m_sceneCart[m_sceneCartFront].GetBasketsCount(); ++i)
	{
		if (m_isCartReady[i] == true)
			continue;

		for (EUINT32 j = 0; j < m_visibleOctreeNodesInBaskets[i].GetCount(); ++j)
		{
			m_sceneCart[m_sceneCartFront].Add(m_visibleOctreeNodesInBaskets[i].GetItem(j)->m_placeHolder.m_objectsEModel, m_visibleOctreeNodesInBaskets[i].GetItem(j)->m_placeHolder.m_EModelsCount, i, pCamera);
		}

		m_isCartReady[i] = true;
		m_visibleOctreeNodesInBaskets[i].Clear();

	}

	EUINT32 nextcart = m_sceneCartBack;
	m_sceneCartBack = m_sceneCartFront;
	m_sceneCartFront = nextcart;


	if (m_prepareToLock == true)
	{
		m_prepareToLock = false;
		m_isLocked = true;
	}

}


bool
ParabellumEngine::Core::Utiles::FrustumOutput::IsBasketReady(EUINT32 no)
{
	return m_isCartReady[no];
}

void
ParabellumEngine::Core::Utiles::FrustumOutput::Lock()
{
	m_prepareToLock = true;
}


//	------------------------------------------------------
//	Gets and sets
//	------------------------------------------------------

ParabellumEngine::Core::Utiles::SceneCart* 
ParabellumEngine::Core::Utiles::FrustumOutput::GetSceneCart()
{
	return &m_sceneCart[m_sceneCartBack];
}
