#include "Basket.h"



//	------------------------------------------------------
//	Constructors and destructors
//	------------------------------------------------------

ParabellumEngine::Core::Utiles::Basket::Basket()
{
	m_uniqueObjectsCount = 0;

	for (int i = 0; i < PE_BASKET_MAXOBJECTSINBASKET; ++i)
	{
		m_revision[i] = 0;
		m_objectsCounter[i] = 0;
	}
}

ParabellumEngine::Core::Utiles::Basket::~Basket()
{

}


//	------------------------------------------------------
//	Methods
//	------------------------------------------------------



void 
ParabellumEngine::Core::Utiles::Basket::CleanUp()
{
	for (int i = 0; i < m_uniqueObjectsCount; ++i)
	{
		m_objectsCounter[m_backward[m_forward[i]]] = 0;
		m_revision[m_forward[i]] = 0;
	}

	m_uniqueObjectsCount = 0;
}

void 
ParabellumEngine::Core::Utiles::Basket::Add(EModel* model, _IN_ Camera* pCamera)
{
	for (EUINT32 i = 0; i < model->GetModelsCount(); ++i)
	{
		//if (pCamera->Intersects(model->GetModel(i)->GetTransformedBoundingSphere()) == IntersectCodes::DISJOINT)
		//	continue;

		Add(model, model->GetModel(i));
	}
}

void 
ParabellumEngine::Core::Utiles::Basket::Add(EModel* emodel, EModelPart* model)
{
	EUINT32 ibId = model->GetModelPart()->m_ibIndex;

	if (m_revision[ibId] == 0)
	{
		m_revision[ibId] = 1;

		m_forward[m_uniqueObjectsCount] = ibId;
		m_backward[ibId] = m_uniqueObjectsCount;

		m_objectsCounter[m_backward[ibId]] = 1;
		m_objects[m_uniqueObjectsCount] = model;
		m_refComponent3D[m_backward[ibId]][0] = model;

		m_uniqueObjectsCount++;
	}
	else
	{
		m_refComponent3D[m_backward[ibId]][m_objectsCounter[m_backward[ibId]]] = model;
		m_objects[m_uniqueObjectsCount] = model;
		m_objectsCounter[m_backward[ibId]]++;

		m_revision[ibId]++;
	}

}






//	------------------------------------------------------
//	Gets and sets
//	------------------------------------------------------

ParabellumFramework::EUINT32
ParabellumEngine::Core::Utiles::Basket::GetPickedUniqueObjectsCount()
{
	return m_uniqueObjectsCount;
}

ParabellumFramework::EUINT32
ParabellumEngine::Core::Utiles::Basket::GetObjectsCountOfIndex(EUINT32 index)
{
	EUINT32 forward = m_forward[index];

	return m_objectsCounter[m_backward[forward]];
}

ParabellumEngine::Components::EModelPart*
ParabellumEngine::Core::Utiles::Basket::GetObjectOfIndex(EUINT32 index, EUINT32 no)
{
	EUINT32 forward = m_forward[index];

	return m_refComponent3D[m_backward[forward]][no];
}