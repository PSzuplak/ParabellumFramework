#include "stdafx.h"
#include "ExDRVolumetricLight.h"

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

ParabellumEngine::Core::Graphics::ExDRVolumetricLight::ExDRVolumetricLight()
{
}


ParabellumEngine::Core::Graphics::ExDRVolumetricLight::~ExDRVolumetricLight()
{
}


//	---------------------------------------------------
//		Methods
//	---------------------------------------------------

void
ParabellumEngine::Core::Graphics::ExDRVolumetricLight::Initialize(
	_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager,
	EUINT32 width, EUINT32 height)
{
	// set spec
	m_screenWidth = width;
	m_screenHeight = height;

	m_SVolumetricLight = pResourceManager->Load<Shader>("Resources/Shaders/DR_VolumetricLight.psx");
	m_SVolumetricLightPolish = pResourceManager->Load<Shader>("Resources/Shaders/DR_VolumetricLightPolish.psx");

	// create RT for dvolumetric light
	m_RTVolumetricLight = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, width / 4, height / 4);

	// create RT for blured vlolumetric light
	m_RTVolumetricLightPolished = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, width / 2, height / 2);

}






void
ParabellumEngine::Core::Graphics::ExDRVolumetricLight::Draw(
	_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera, _IN_ DirectionalLight* directionalLight,
	_IN_ RenderQuad* pRenderQuad, _IN_ RenderTree* pRenderTree,
	_IN_ RenderTarget* pBufferWorldFar, _IN_ RenderTarget* pLightDepthMap)
{

	Vector4D cameraDirectionFar, cameraPositionFar, cameraNF, lightDirectionAmbient, lightPosition, lightColorIntensity, lightAmbientColor, depthMapPixelSize;

	pCamera->GetDirection(cameraDirectionFar.XYZ);
	cameraDirectionFar.W = pCamera->GetZFar();

	pCamera->GetPosition(cameraPositionFar.XYZ);
	cameraPositionFar.W = pCamera->GetZFar();

	cameraNF.Set(-500.0f, 500.0f, 0.f, 0.0f);


	lightDirectionAmbient.Set(directionalLight->GetDirection(), 0.5f);
	pCamera->GetPosition(cameraPositionFar.X, cameraPositionFar.Y, cameraPositionFar.Z);
	cameraPositionFar.W = pCamera->GetZFar();
	lightPosition.Set(0.0f, 0.0f, 0.0f, 0.0f);
	lightColorIntensity.Set(directionalLight->GetColor()->X, directionalLight->GetColor()->Y, directionalLight->GetColor()->Z, directionalLight->GetIntensity());
	lightAmbientColor.Set(0.3f, 0.3f, 0.3f, 0.5f);


	depthMapPixelSize.X = (EFLOAT32)pLightDepthMap->GetWidth();
	depthMapPixelSize.Y = (EFLOAT32)pLightDepthMap->GetHeight();

	//
	// Low resolution VL
	//

	// set render targets
	pDevice->SetRenderTargets(m_RTVolumetricLight, m_RTVolumetricLight, true);

	// set buffers
	pDevice->SetBuffers(pRenderQuad->GetVertexBuffer(), pRenderQuad->GetIndexBuffer());

	// set shader
	pDevice->SetShader(m_SVolumetricLight);

	Vector4D settings(0.0f, 0.0f, 0.0f, 0.0f);

	m_SVolumetricLight->GetParametr(0)->Set(pCamera->GetInverseView());
	m_SVolumetricLight->GetParametr(1)->Set(pCamera->GetInverseProjection());
	m_SVolumetricLight->GetParametr(2)->Set(pCamera->GetInverseViewProjection());
	m_SVolumetricLight->GetParametr(3)->Set(&cameraPositionFar);
	m_SVolumetricLight->GetParametr(4)->Set(&cameraNF);
	m_SVolumetricLight->GetParametr(5)->Set(&lightDirectionAmbient);
	m_SVolumetricLight->GetParametr(6)->Set(&lightPosition);
	m_SVolumetricLight->GetParametr(7)->Set(&lightColorIntensity);
	m_SVolumetricLight->GetParametr(8)->Set(&directionalLight->m_viewProjection[2]);
	m_SVolumetricLight->GetParametr(9)->Set(&depthMapPixelSize);

	m_SVolumetricLight->GetParametr(10)->Set(pCamera->GetCTL());
	m_SVolumetricLight->GetParametr(11)->Set(pCamera->GetCTL2CTR());
	m_SVolumetricLight->GetParametr(12)->Set(pCamera->GetCTL2CBL());

	m_SVolumetricLight->GetResource(0)->Set(pBufferWorldFar);
	m_SVolumetricLight->GetResource(1)->Set(pLightDepthMap);

	pDevice->SetSampler(0, SamplerType::POINT);
	pDevice->SetSampler(1, SamplerType::POINT);


	// render scene
	pDevice->Render();


	//
	// Polish volumetric light render target
	//

	// set render targets
	pDevice->SetRenderTargets(m_RTVolumetricLightPolished, m_RTVolumetricLightPolished, true);

	// set buffers
	pDevice->SetBuffers(pRenderQuad->GetVertexBuffer(), pRenderQuad->GetIndexBuffer());

	// set shader
	pDevice->SetShader(m_SVolumetricLightPolish);


	m_SVolumetricLightPolish->GetParametr(0)->Set(pCamera->GetProjection());
	m_SVolumetricLightPolish->GetParametr(1)->Set(&cameraDirectionFar);
	m_SVolumetricLightPolish->GetParametr(2)->Set(&lightDirectionAmbient);

	m_SVolumetricLightPolish->GetResource(0)->Set(m_RTVolumetricLight);

	// render scene
	pDevice->Render();

}