#include "stdafx.h"
#include "ExDRParallaxMapping.h"

//	---------------------------------------------------
//		namespaces
//	---------------------------------------------------
using namespace ParabellumFramework;
using namespace ParabellumFramework::Graphics;
using namespace ParabellumFramework::Resources;
using namespace ParabellumEngine::Components;

//	---------------------------------------------------
//		Gets and Sets
//	---------------------------------------------------

ParabellumEngine::Core::Graphics::ExDRParallaxMapping::ExDRParallaxMapping()
{
	m_SParallaxMapping = NULLPTR;
	m_RTBuffer0 = NULLPTR;
	m_RTBuffer1 = NULLPTR;
}


ParabellumEngine::Core::Graphics::ExDRParallaxMapping::~ExDRParallaxMapping()
{
}


//	---------------------------------------------------
//		Methods
//	---------------------------------------------------

void
ParabellumEngine::Core::Graphics::ExDRParallaxMapping::Initialize(
	_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager,
	EUINT32 width, EUINT32 height)
{
	// load shadres
	m_SParallaxMapping = pResourceManager->Load<Shader>("Resources/Shaders/DR_ParallaxMapping.psx");

	// create RT for directinal light
	m_RTBuffer0 = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, width, height);

	m_RTBuffer1 = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, width, height);
}


void
ParabellumEngine::Core::Graphics::ExDRParallaxMapping::Release(_IN_ GraphicsDevice* pDevice)
{
	if (m_SParallaxMapping != NULLPTR)
	{
		pDevice->ReleaseShader(m_SParallaxMapping);
	}

	if (m_RTBuffer0 != NULLPTR)
	{
		pDevice->ReleaseRenderTarget(m_RTBuffer0);
	}

	if (m_RTBuffer1 != NULLPTR)
	{
		pDevice->ReleaseRenderTarget(m_RTBuffer1);
	}
}




void
ParabellumEngine::Core::Graphics::ExDRParallaxMapping::Draw(
	_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera,
	_IN_ RenderQuad* pRenderQuad,
	_IN_ RenderTarget* pGBuffer00, _IN_ RenderTarget* pGBuffer01, _IN_ RenderTarget* pGBuffer02)
{
	// set render targets
	pDevice->SetRenderTargets(m_RTBuffer0, m_RTBuffer1, m_RTBuffer0, true);

	// set quad
	pDevice->SetBuffers(pRenderQuad->GetVertexBuffer(), pRenderQuad->GetIndexBuffer());

	// set shader
	pDevice->SetShader(m_SParallaxMapping);

	//
	pDevice->SetSampler(0, SamplerType::POINT);
	pDevice->SetSampler(1, SamplerType::POINT);
	pDevice->SetSampler(2, SamplerType::POINT);

	Vector4D cameraPositionFar, cameraDirectionNear, irba;

	pCamera->GetPosition(cameraPositionFar.X, cameraPositionFar.Y, cameraPositionFar.Z);
	cameraPositionFar.W = pCamera->GetZFar();

	pCamera->GetDirection(cameraDirectionNear.XYZ);
	cameraDirectionNear.W = 1.0f;

	Vector4D areaProperties;
	areaProperties.Set(0.0f, 0.0f, 1.0f, 0.5f);


	m_SParallaxMapping->GetParametr(0)->Set(pCamera->GetView());
	m_SParallaxMapping->GetParametr(1)->Set(pCamera->GetProjection());
	m_SParallaxMapping->GetParametr(2)->Set(pCamera->GetViewProjection());
	m_SParallaxMapping->GetParametr(3)->Set(pCamera->GetInverseView());
	m_SParallaxMapping->GetParametr(4)->Set(pCamera->GetInverseProjection());
	m_SParallaxMapping->GetParametr(5)->Set(pCamera->GetInverseViewProjection());
	m_SParallaxMapping->GetParametr(6)->Set(&cameraPositionFar);
	m_SParallaxMapping->GetParametr(7)->Set(&cameraDirectionNear);
	m_SParallaxMapping->GetParametr(8)->Set(&areaProperties);

	m_SParallaxMapping->GetParametr(9)->Set(pCamera->GetCTL());
	m_SParallaxMapping->GetParametr(10)->Set(pCamera->GetCTL2CTR());
	m_SParallaxMapping->GetParametr(11)->Set(pCamera->GetCTL2CBL());

	m_SParallaxMapping->GetResource(0)->Set<RenderTarget>(pGBuffer00);
	m_SParallaxMapping->GetResource(1)->Set<RenderTarget>(pGBuffer01);
	m_SParallaxMapping->GetResource(2)->Set<RenderTarget>(pGBuffer02);


	pDevice->SetShaderVariables();
	pDevice->SetShaderResources();
	pDevice->RenderRAW();

}


void
ParabellumEngine::Core::Graphics::ExDRParallaxMapping::Draw(
	_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera,
	_IN_ RenderTree* pRenderTree,
	_IN_ RenderTarget* pGBuffer00, _IN_ RenderTarget* pGBuffer01, _IN_ RenderTarget* pGBuffer02)
{
	// set render targets
	pDevice->SetRenderTargets(m_RTBuffer0, m_RTBuffer1, m_RTBuffer0, true);

	// set shader
	pDevice->SetShader(m_SParallaxMapping);

	//
	pDevice->SetSampler(0, SamplerType::POINT);
	pDevice->SetSampler(1, SamplerType::POINT);
	pDevice->SetSampler(2, SamplerType::POINT);

	Vector4D cameraPositionFar, cameraDirectionNear, irba;

	pCamera->GetPosition(cameraPositionFar.X, cameraPositionFar.Y, cameraPositionFar.Z);
	cameraPositionFar.W = pCamera->GetZFar();

	pCamera->GetDirection(cameraDirectionNear.XYZ);
	cameraDirectionNear.W = 1.0f;

	Vector4D areaProperties;
	areaProperties.Set(0.0f, 0.0f, 1.0f, 0.5f);


	m_SParallaxMapping->GetParametr(0)->Set(pCamera->GetView());
	m_SParallaxMapping->GetParametr(1)->Set(pCamera->GetProjection());
	m_SParallaxMapping->GetParametr(2)->Set(pCamera->GetViewProjection());
	m_SParallaxMapping->GetParametr(3)->Set(pCamera->GetInverseView());
	m_SParallaxMapping->GetParametr(4)->Set(pCamera->GetInverseProjection());
	m_SParallaxMapping->GetParametr(5)->Set(pCamera->GetInverseViewProjection());
	m_SParallaxMapping->GetParametr(6)->Set(&cameraPositionFar);
	m_SParallaxMapping->GetParametr(7)->Set(&cameraDirectionNear);
	m_SParallaxMapping->GetParametr(8)->Set(&areaProperties);

	m_SParallaxMapping->GetParametr(9)->Set(pCamera->GetCTL());
	m_SParallaxMapping->GetParametr(10)->Set(pCamera->GetCTL2CTR());
	m_SParallaxMapping->GetParametr(11)->Set(pCamera->GetCTL2CBL());

	m_SParallaxMapping->GetResource(0)->Set<RenderTarget>(pGBuffer00);
	m_SParallaxMapping->GetResource(1)->Set<RenderTarget>(pGBuffer01);
	m_SParallaxMapping->GetResource(2)->Set<RenderTarget>(pGBuffer02);

	EUINT32 operationCounter = 0;


	// Set Resources
	//pDevice->SetShaderVariables();
	//pDevice->SetShaderResources();

	// render
	pRenderTree->Render(pDevice, m_SParallaxMapping, 8, PE_RENDER_SETTINGS_PARALLAXMAPPING, operationCounter);
	


}