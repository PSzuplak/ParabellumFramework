#include "stdafx.h"
#include "ExDRSSAO.h"


//	---------------------------------------------------
//		Constructors and Destructors
//	---------------------------------------------------

ParabellumEngine::Core::Graphics::ExDRSSAO::ExDRSSAO()
{
}


ParabellumEngine::Core::Graphics::ExDRSSAO::~ExDRSSAO()
{
}


//	---------------------------------------------------
//		Methods
//	---------------------------------------------------

void
ParabellumEngine::Core::Graphics::ExDRSSAO::Initialize(
	_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager,
	EUINT32 width, EUINT32 height)
{
	// set spec
	m_screenWidth = width/2;
	m_screenHeight = height/2;

	m_intensity = 4.7f;
	m_radious = 0.040f;
	m_bias = 0.1505f;
	m_iterations = 8;
	m_angle = 0.6f;


	// load shadres
	m_ShaderSSAO = pResourceManager->Load<Shader>("Resources/Shaders/DR_SSAO.psx");

	// create RT for directinal light
	m_RTSSAO = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, width/2, height/2);

}


void
ParabellumEngine::Core::Graphics::ExDRSSAO::Release(_IN_ GraphicsDevice* pDevice)
{
	if (m_ShaderSSAO != NULLPTR)
	{
		pDevice->ReleaseShader(m_ShaderSSAO);
	}
}




void
ParabellumEngine::Core::Graphics::ExDRSSAO::Draw(
	_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera,
	_IN_ RenderQuad* pRenderQuad,
	_IN_ RenderTarget* pGBuffer00, _IN_ RenderTarget* pGBuffer01, _IN_ RenderTarget* pGBuffer02)
{
	// set render targets
	pDevice->SetRenderTargets(m_RTSSAO, m_RTSSAO, true);

	// set buffers
	pDevice->SetBuffers(pRenderQuad->GetVertexBuffer(), pRenderQuad->GetIndexBuffer());

	// set shader
	pDevice->SetShader(m_ShaderSSAO);

	Vector4D cameraPositionFar, irba;

	pCamera->GetPosition(cameraPositionFar.X, cameraPositionFar.Y, cameraPositionFar.Z);
	cameraPositionFar.W = pCamera->GetZFar();

	irba.Set(m_intensity, m_radious, m_bias, m_angle);

	// set shader parametrs
	m_ShaderSSAO->GetParametr(0)->Set(pCamera->GetInverseView());
	m_ShaderSSAO->GetParametr(1)->Set(pCamera->GetInverseView());
	m_ShaderSSAO->GetParametr(2)->Set(pCamera->GetProjection());
	m_ShaderSSAO->GetParametr(3)->Set(pCamera->GetInverseProjection());
	m_ShaderSSAO->GetParametr(4)->Set(pCamera->GetInverseViewProjection());
	m_ShaderSSAO->GetParametr(5)->Set(&cameraPositionFar);
	m_ShaderSSAO->GetParametr(6)->Set(&irba);
	m_ShaderSSAO->GetParametr(7)->Set(pCamera->GetCTL());
	m_ShaderSSAO->GetParametr(8)->Set(pCamera->GetCTL2CTR());
	m_ShaderSSAO->GetParametr(9)->Set(pCamera->GetCTL2CBL());

	m_ShaderSSAO->GetResource(0)->Set<RenderTarget>(pGBuffer00);
	m_ShaderSSAO->GetResource(1)->Set<RenderTarget>(pGBuffer01);

	pDevice->SetSampler(0, SamplerType::POINT);
	pDevice->SetSampler(1, SamplerType::POINT);

	pDevice->Render();
}







//	---------------------------------------------------
//		Gets and sets
//	---------------------------------------------------
EFLOAT32 ParabellumEngine::Core::Graphics::ExDRSSAO::GetIntensity()
{
	return m_intensity;
}

EFLOAT32 ParabellumEngine::Core::Graphics::ExDRSSAO::GetRadious()
{
	return m_radious;
}
EFLOAT32 ParabellumEngine::Core::Graphics::ExDRSSAO::GetBias()
{
	return m_bias;
}
EFLOAT32 ParabellumEngine::Core::Graphics::ExDRSSAO::GetAngle()
{
	return m_angle;
}

void ParabellumEngine::Core::Graphics::ExDRSSAO::SetIntensity(EFLOAT32 val)
{
	m_intensity = val;
}

void ParabellumEngine::Core::Graphics::ExDRSSAO::SetRadious(EFLOAT32 val)
{
	m_radious = val;
}
void ParabellumEngine::Core::Graphics::ExDRSSAO::SetBias(EFLOAT32 val)
{
	m_bias = val;
}
void ParabellumEngine::Core::Graphics::ExDRSSAO::SetAngle(EFLOAT32  val)
{
	m_angle = val;
}