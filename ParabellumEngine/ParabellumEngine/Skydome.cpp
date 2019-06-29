#include "Skydome.h"



//	------------------------------------------------------------
//		Statics
//	------------------------------------------------------------



//	------------------------------------------------------------
//		Constructors and destructors
//	------------------------------------------------------------
ParabellumEngine::Components::Skydome::Skydome()
{
}


ParabellumEngine::Components::Skydome::~Skydome()
{
}

//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------

ParabellumFramework::EUINT32
ParabellumEngine::Components::Skydome::Initialize(
	_IN_ PFNS::Graphics::GraphicsDevice* pDevice,
	_IN_ PFNS::Resources::ResourceManager* pResources
	)
{
	m_MSkydome = pResources->Load<Model>("Resources/Models/Skydome/Skydome_01.pfm");

	m_SSkydome = pResources->Load<Shader>("Resources/Shaders/ShaderSkydome.psx");

	m_T2DSkydome = pResources->Load<Texture2D>("Resources/Textures/SkyDome.pft");

	return SystemCodes::SUCCESS;
}



//	------------------------------------------------------------
//		Gets Sets
//	------------------------------------------------------------


ParabellumFramework::Graphics::Model* 
ParabellumEngine::Components::Skydome::GetModel()
{
	return m_MSkydome;
}


ParabellumFramework::Graphics::Shader* 
ParabellumEngine::Components::Skydome::GetShader()
{
	return m_SSkydome;
}


ParabellumFramework::Graphics::Texture2D* 
ParabellumEngine::Components::Skydome::GetTexture()
{
	return m_T2DSkydome;
}