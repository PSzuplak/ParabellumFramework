#include "SkyLike.h"



//	------------------------------------------------------------
//		Constructors and destructors
//	------------------------------------------------------------
ParabellumEngine::Components::SkyLike::SkyLike(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager)
{

	m_RTSun = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, 1024, 1024);

	m_RTClouds = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, 1024, 4* 1024);

	m_RTMerged = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, 1024, 1024);

	m_RTFinal = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, 1024, 1024);

	m_SSetBuffer = pResourceManager->Load<Shader>("Resources/Shaders/SetBuffer.psx");
	m_SSun = pResourceManager->Load<Shader>("Resources/Shaders/SkyLike_GenerateSun.psx");
	m_SAddCloud = pResourceManager->Load<Shader>("Resources/Shaders/SkyLike_AddCloud.psx");
	m_SMerge = pResourceManager->Load<Shader>("Resources/Shaders/SkyLike_Merge.psx");
	m_SDrawSkydome = pResourceManager->Load<Shader>("Resources/Shaders/SkyLike_DrawSkydome.psx");

	tex = pResourceManager->Load<Texture2D>("Resources/Textures/cloud_test.pft");

	m_MSkydome = pResourceManager->Load<Model>("Resources/Models/Skydome/Skydome_01.pfm");

	m_renderQuad.Initialize(pDevice, -1.0f, 1.0f, 1.0f, -1.0f);
	m_placementQuad.Initialize(pDevice, 0.0f, 0.0f, 1.0f, -1.0f);

	//////////////////////////////////////////////////////////////////////
	//m_skyGrad[0].Set(0.1640625f, 0.66796875f, 1.0f, 0.5f);
	//m_skyGrad[1].Set(1.0f, 1.0f, 1.0f, 0.5f);

	m_skyGrad[0].Set(0.6f, 0.3f, 0.0f, 0.9f);
	m_skyGrad[1].Set(0.6f, 0.3f, 0.0f, 0.9f);

	m_isSunEnabled = true;
	m_sunPosition.Set(-0.3f, 0.3f);
	m_sunSize = 0.1f;

	m_sunGradColor[0].Set(0.6f, 0.3f, 0.0f, 0.9f);
	m_sunGradColor[1].Set(1.0f, 0.45f, 0.0f, 0.8f);
	
}


ParabellumEngine::Components::SkyLike::~SkyLike()
{
}

//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------




void
ParabellumEngine::Components::SkyLike::AddCloud(_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera, _IN_ Texture2D* pT2DCloud, Vector2D position, EFLOAT32 size)
{
	// set alphachannel on 1 first
	Vector4D cloudInitColor(0.0f, 0.0f, 0.0f, 0.0f);

	pDevice->SetRenderTargets(m_RTClouds, m_RTClouds, true);
	pDevice->SetShader(m_SSetBuffer);
	pDevice->SetBuffers(m_renderQuad.GetVertexBuffer(), m_renderQuad.GetIndexBuffer());
	m_SSetBuffer->GetParametr(0)->Set(&cloudInitColor);
	pDevice->Render(PF_GI_RENDERING_UPDATEVARIABLES);


	//
	//
	EFLOAT32 factorRT = (EFLOAT32)m_RTClouds->GetWidth() / (EFLOAT32)m_RTClouds->GetHeight();
	EFLOAT32 factorTex = (EFLOAT32)pT2DCloud->GetWidth() / (EFLOAT32)pT2DCloud->GetHeight();

	EFLOAT32 sizel = size;

	EFLOAT32 sizex = sizel * factorTex;// *factorRT * 0.5f;
	EFLOAT32 sizey = sizel;

	Vector4D cloudPos;
	cloudPos.Set(position.X, position.Y, sizex, sizey);

	pDevice->TurnAlphaBlending(true);
	pDevice->TurnDepthBuffer(false);

	pDevice->SetRenderTargets(m_RTClouds, m_RTClouds, false);

	pDevice->SetShader(m_SAddCloud);


	pDevice->SetBuffers(m_placementQuad.GetVertexBuffer(), m_placementQuad.GetIndexBuffer());


	m_SAddCloud->GetParametr(0)->Set(&cloudPos);

	m_SAddCloud->GetResource(0)->Set<Texture2D>(tex);



	pDevice->Render(PF_GI_RENDERING_UPDATEVARIABLES | PF_GI_RENDERING_UPDATERESOURCES);


	pDevice->TurnAlphaBlending(false);
	pDevice->TurnDepthBuffer(true);
}


void 
ParabellumEngine::Components::SkyLike::CreateSkydomeTexture(_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera)
{

	pDevice->SetSampler(0, SamplerType::ANISOTROPIC);
	pDevice->SetSampler(1, SamplerType::ANISOTROPIC);
	pDevice->SetSampler(2, SamplerType::ANISOTROPIC);
	pDevice->SetSampler(3, SamplerType::ANISOTROPIC);

	//
	// Draw Sun
	//
	pDevice->SetRenderTargets(m_RTSun, m_RTSun, true);


	pDevice->SetShader(m_SSun);

	//
	pDevice->SetBuffers(m_renderQuad.GetVertexBuffer(), m_renderQuad.GetIndexBuffer());

	Vector4D sunPositionSize;

	sunPositionSize.XY = m_sunPosition;
	sunPositionSize.Z = m_sunSize;

	m_SSun->GetParametr(0)->Set(&m_skyGrad[0]);
	m_SSun->GetParametr(1)->Set(&m_skyGrad[1]);
	m_SSun->GetParametr(2)->Set(&sunPositionSize);
	m_SSun->GetParametr(3)->Set(&m_sunGradColor[0]);
	m_SSun->GetParametr(4)->Set(&m_sunGradColor[1]);

	pDevice->Render(PF_GI_RENDERING_UPDATEVARIABLES);




	//
	// Merge
	//

	pDevice->TurnAlphaBlending(true);
	pDevice->TurnDepthBuffer(false);


	pDevice->SetRenderTargets(m_RTMerged, m_RTMerged, true);

	pDevice->SetShader(m_SMerge);


	pDevice->SetBuffers(m_renderQuad.GetVertexBuffer(), m_renderQuad.GetIndexBuffer());

	Vector4D cloudPos;
	cloudPos.Set(0.15f, 0.50f, 1, 1);
	m_SMerge->GetParametr(0)->Set(&cloudPos);

	m_SMerge->GetResource(0)->Set(m_RTSun);
	m_SMerge->GetResource(1)->Set(m_RTClouds);


	pDevice->Render(PF_GI_RENDERING_UPDATEVARIABLES | PF_GI_RENDERING_UPDATERESOURCES);



	pDevice->TurnAlphaBlending(false);
	pDevice->TurnDepthBuffer(true);




}



void 
ParabellumEngine::Components::SkyLike::DrawMerged(_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera)
{
	pDevice->SetRenderTargets(m_RTFinal, m_RTFinal, true);
	pDevice->SetShader(m_SDrawSkydome);
	
	//pDevice->SetBuffers(m_MSkydome->GetVertexBuffer(0), m_MSkydome->GetIndexBuffer(0));
	pDevice->SetBuffers(PrimitivesFactory::Instance(pDevice).GetCylinderVertexBuffer(), PrimitivesFactory::Instance(pDevice).GetCylinderIndexBuffer());
	

	Matrix4x4 world, worldViewIT;
	Vector3D scale;
	EFLOAT32 scaleFactor = pCamera->GetZFar() * 0.8f;

	scale.Set(scaleFactor, scaleFactor, scaleFactor);
	world.MakeScaleTranslation(pCamera->GetPosition(), &scale);

	Matrix4x4::Transponse(Matrix4x4::Invert((*pCamera->GetView())) * world, worldViewIT);

	Vector4D cameraPositionFar, materialProperties;

	pCamera->GetPosition(cameraPositionFar.X, cameraPositionFar.Y, cameraPositionFar.Z);
	cameraPositionFar.W = pCamera->GetZFar();

	m_SDrawSkydome->GetParametr(0)->Set(pCamera->GetView());
	m_SDrawSkydome->GetParametr(1)->Set(pCamera->GetProjection());
	m_SDrawSkydome->GetParametr(2)->Set(&world);
	m_SDrawSkydome->GetParametr(3)->Set(&worldViewIT);
	m_SDrawSkydome->GetParametr(4)->Set(&cameraPositionFar);

	m_SDrawSkydome->GetResource(0)->Set(m_RTMerged);
	//m_SDrawSkydome->GetResource(0)->Set(m_RTClouds);
	

	pDevice->Render(PF_GI_RENDERING_UPDATEVARIABLES | PF_GI_RENDERING_UPDATERESOURCES);
}






//	------------------------------------------------------------
//		Gets and Sets
//	------------------------------------------------------------

