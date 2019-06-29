#include "EModelPart.h"

using namespace ParabellumFramework;



//	------------------------------------------------------------
//		Constructors and destructors
//	------------------------------------------------------------
ParabellumEngine::Components::EModelPart::EModelPart(_IN_ ParabellumFramework::Resources::ResourceManager* pResourceManager, Graphics::ModelPart* pPart)
{
	m_modelPart = pPart;

	m_T2DTextures[0] = pResourceManager->Load<Texture2D>(m_modelPart->m_material.pathTextureDiffuse);
	m_T2DTextures[1] = pResourceManager->Load<Texture2D>(m_modelPart->m_material.pathTextureNormal);
	m_T2DTextures[2] = pResourceManager->Load<Texture2D>("Resources/Textures/test_light_x.pft");
	m_T2DTextures[3] = pResourceManager->Load<Texture2D>(m_modelPart->m_material.pathTextureExtenstion);

	//printf("\ndiffuse	: %s", m_modelPart->m_material.pathTextureDiffuse);
	//printf("\nnormal	: %s", m_modelPart->m_material.pathTextureNormal);
	//printf("\nlight		: %s", m_modelPart->m_material.pathTextureLight);
	//printf("\nextended  : %s", m_modelPart->m_material.pathTextureExtenstion);

	m_boundingBox = pPart->m_boundingBox;
	m_boundingSphere = pPart->m_boundingSphere;

	AddTag("EModelPart");
}	


ParabellumEngine::Components::EModelPart::~EModelPart()
{
}
