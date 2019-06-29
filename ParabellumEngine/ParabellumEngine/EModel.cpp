#include "EModel.h"

using namespace ParabellumFramework;



//	------------------------------------------------------------
//		Constructors and destructors
//	------------------------------------------------------------
ParabellumEngine::Components::EModel::EModel(_IN_ ParabellumFramework::Resources::ResourceManager* pResourceManager, ParabellumFramework::Graphics::Model* pModel)
{
	m_models = NULLPTR;

	Initialize(pResourceManager, pModel);

	AddTag("EModel");
}



ParabellumEngine::Components::EModel::~EModel()
{
	for (EUINT32 i = 0; i < m_modelsCount; ++i)
		delete m_models[i];

	delete[] m_models;
}


//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------

ParabellumFramework::EUINT32
ParabellumEngine::Components::EModel::Initialize(_IN_ ParabellumFramework::Resources::ResourceManager* pResourceManager, ParabellumFramework::Graphics::Model* pModel)
{
	// load model if NOT loaded and create Components for it
	if (m_models == NULLPTR)
	{
		m_modelsCount = pModel->GetModelPartsCount();
		m_models = new EModelPart*[m_modelsCount];

		// merge bboxes
		m_boundingBox = pModel->m_boundingBox;
		m_boundingSphere.MakeFromBoundingBox(&m_boundingBox);

		// set parent
		for (EUINT32 i = 0; i < m_modelsCount; ++i)
		{
			m_models[i] = new EModelPart(pResourceManager, pModel->GetModelPart(i));
			m_models[i]->SetParent(this);
		}
	


		return SystemCodes::SUCCESS;
	}

	return SystemCodes::ERROR_EXISTS_ALREADY;
}



//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------
ParabellumFramework::Graphics::Texture2D* 
ParabellumEngine::Components::EModel::GetDiffuseMap()
{
	return m_t2d;
}