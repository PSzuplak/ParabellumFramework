#include "stdafx.h"
#include "ExDRGlobalIllumination.h"

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

ParabellumEngine::Core::Graphics::ExDRGlobalIllumination::ExDRGlobalIllumination()
{
	m_SShadowMap = NULLPTR;
	m_SFindUVs = NULLPTR;
	m_RTShadowMap = NULLPTR;
	m_RTUVs = NULLPTR;
}


ParabellumEngine::Core::Graphics::ExDRGlobalIllumination::~ExDRGlobalIllumination()
{
}


//	---------------------------------------------------
//		Methods
//	---------------------------------------------------

void
ParabellumEngine::Core::Graphics::ExDRGlobalIllumination::Initialize(
	_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager,
	EUINT32 width, EUINT32 height)
{
	// load shadres
	m_SShadowMap = pResourceManager->Load<Shader>("Resources/Shaders/DR_GI_ShadowMap.psx");

	//m_SFindUVs = pResourceManager->Load<Shader>("Resources/Shaders/DR_GI_FindUVs.psx");

	// create RT for directinal light
	m_RTShadowMap = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, 128, 128);

	m_RTUVs = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, width, height);
}


void
ParabellumEngine::Core::Graphics::ExDRGlobalIllumination::Release(_IN_ GraphicsDevice* pDevice)
{
	if (m_SFindUVs != NULLPTR)
	{
		pDevice->ReleaseShader(m_SFindUVs);
	}

	if (m_RTShadowMap != NULLPTR)
	{
		pDevice->ReleaseRenderTarget(m_RTShadowMap);
	}

}




void
ParabellumEngine::Core::Graphics::ExDRGlobalIllumination::Draw(
	_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera,
	_IN_ RenderQuad* pRenderQuad,
	_IN_ RenderTarget* pBufferWorldZ, _IN_ RenderTarget* pBufferDiffuse, _IN_ RenderTarget* pBufferNormalH)
{
	//// set render targets
	//pDevice->SetRenderTargets(m_RTBuffer0, m_RTBuffer1, m_RTBuffer0, true);
	//
	//// set buffers
	//pDevice->SetBuffers(pRenderQuad->m_vertexBuffer, pRenderQuad->m_indexBuffer);
	//
	//// set shader
	//pDevice->SetShader(m_SParallaxMapping);
	//
	//Vector4D cameraPositionFar, cameraDirectionNear, irba;
	//
	//pCamera->GetPosition(cameraPositionFar.X, cameraPositionFar.Y, cameraPositionFar.Z);
	//cameraPositionFar.W = pCamera->GetZFar();
	//
	//pCamera->GetDirection(cameraDirectionNear.XYZ);
	//cameraDirectionNear.W = 1.0f;
	//
	////irba.Set(m_intensity, m_radious, m_bias, m_angle);
	//
	//
	//m_SParallaxMapping->GetParametr(0)->Set(pCamera->GetView());
	//m_SParallaxMapping->GetParametr(1)->Set(pCamera->GetProjection());
	//m_SParallaxMapping->GetParametr(2)->Set(pCamera->GetViewProjection());
	//m_SParallaxMapping->GetParametr(3)->Set(pCamera->GetInverseView());
	//m_SParallaxMapping->GetParametr(4)->Set(pCamera->GetInverseProjection());
	//m_SParallaxMapping->GetParametr(5)->Set(pCamera->GetInverseViewProjection());
	//m_SParallaxMapping->GetParametr(6)->Set(&cameraPositionFar);
	//m_SParallaxMapping->GetParametr(7)->Set(&cameraDirectionNear);
	//m_SParallaxMapping->GetParametr(8)->Set(&cameraDirectionNear);
	//
	//m_SParallaxMapping->GetResource(0)->Set<RenderTarget>(pBufferWorldZ);
	//m_SParallaxMapping->GetResource(1)->Set<RenderTarget>(pBufferDiffuse);
	//m_SParallaxMapping->GetResource(2)->Set<RenderTarget>(pBufferNormalH);
	//
	//pDevice->SetSampler(0, SamplerType::POINT);
	//pDevice->SetSampler(1, SamplerType::POINT);
	//pDevice->SetSampler(2, SamplerType::POINT);
	//
	//pDevice->Render();
}



