#include "stdafx.h"
#include "ExDRHDR.h"



//	---------------------------------------------------
//		Constructors and Destructors
//	---------------------------------------------------

ParabellumEngine::Core::Graphics::ExDRHDR::ExDRHDR()
{
}


ParabellumEngine::Core::Graphics::ExDRHDR::~ExDRHDR()
{
}


//	---------------------------------------------------
//		Methods
//	---------------------------------------------------

void
ParabellumEngine::Core::Graphics::ExDRHDR::Initialize(
	_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager,
	EUINT32 width, EUINT32 height)
{
	// set spec
	m_screenWidth = width / 2;
	m_screenHeight = height / 2;


	// load shadres
	m_ShaderHDR = pResourceManager->Load<Shader>("Resources/Shaders/DR_HDR.psx");
	m_SBlurVertical = pResourceManager->Load<Shader>("Resources/Shaders/Blur_Gaussian_Vertical.psx");


	// create RT for directinal light
	m_RTHDR = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, m_screenWidth, m_screenHeight);


	m_RTToneMap = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, m_screenWidth, m_screenHeight);

	m_RTToneMapBluredVertical = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, m_screenWidth, m_screenHeight);

	m_RTToneMapBluredHorizontal = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, m_screenWidth, m_screenHeight);
}


void
ParabellumEngine::Core::Graphics::ExDRHDR::Release(_IN_ GraphicsDevice* pDevice)
{
	if (m_ShaderHDR != NULLPTR)
	{
		pDevice->ReleaseShader(m_ShaderHDR);
	}
}







void
ParabellumEngine::Core::Graphics::ExDRHDR::Draw(
	_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera,
	_IN_ RenderQuad* pRenderQuad, _IN_ RenderTarget* pGBUffer0, _IN_ RenderTarget* pBufferLight,
	EFLOAT32 averageLuminance, EFLOAT32 lowestLuminance, EFLOAT32 highestLuminance
	)
{
	// set render targets
	pDevice->SetRenderTargets(m_RTHDR, m_RTHDR, true);

	// set buffers
	pDevice->SetBuffers(pRenderQuad->GetVertexBuffer(), pRenderQuad->GetIndexBuffer());

	// set shader
	pDevice->SetShader(m_ShaderHDR);

	Vector4D cameraPositionFar, lrrr;

	pCamera->GetPosition(cameraPositionFar.X, cameraPositionFar.Y, cameraPositionFar.Z);
	cameraPositionFar.W = pCamera->GetZFar();

	//m_prevAvgLum = (m_prevAvgLum * 0.99f) + (averageLuminance * 0.01f);
	//m_prevMinLum = (m_prevMinLum * 0.99f) + (lowestLuminance  * 0.01f);
	//m_prevMaxLum = (m_prevMaxLum * 0.99f) + (highestLuminance * 0.01f);
	m_prevAvgLum = averageLuminance;
	m_prevMinLum = lowestLuminance ;
	m_prevMaxLum = highestLuminance;

	//m_prevAvgLum = averageLuminance;
	//m_prevMinLum = lowestLuminance ;
	//m_prevMaxLum = highestLuminance;

	lrrr.Set(m_prevAvgLum, m_prevMinLum, m_prevMaxLum, 1.0f);

	//printf("\n\n avg lum = %f", averageLuminance);
	//printf("\n low lum = %f", lowestLuminance);
	//printf("\n hig lum = %f", highestLuminance);

	// set shader parametrs
	m_ShaderHDR->GetParametr(0)->Set(&lrrr);


	m_ShaderHDR->GetResource(0)->Set<RenderTarget>(pGBUffer0);
	m_ShaderHDR->GetResource(1)->Set<RenderTarget>(pBufferLight);

	pDevice->SetSampler(0, SamplerType::POINT);
	pDevice->SetSampler(1, SamplerType::POINT);

	pDevice->Render();



	//
	// Blur Vertical
	/*
	pDevice->SetRenderTargets(m_RTHDRBluredVertical, m_RTHDRBluredVertical, true);

	// set shader
	pDevice->SetShader(m_SBlurVertical);

	m_SBlurVertical->GetParametr(0)->Set(&lrrr);
	m_SBlurVertical->GetResource(0)->Set<RenderTarget>(m_RTHDR);

	pDevice->Render();
	*/

}