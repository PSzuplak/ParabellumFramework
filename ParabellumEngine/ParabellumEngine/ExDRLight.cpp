#include "stdafx.h"
#include "ExDRLight.h"

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


//	-----------------------------------------------------------------------------------------------------------------------------
ParabellumEngine::Core::Graphics::ExDRLight::ExDRLight()
{
	m_frameCounter = 0;

	m_settingsCSMUpdateCounter[0] = 1;
	m_settingsCSMUpdateCounter[1] = 1;
	m_settingsCSMUpdateCounter[2] = 1;
	m_settingsCSMUpdateCounter[3] = 1;
	m_settingsCSMUpdateCounter[4] = 1;
	m_settingsCSMUpdateCounter[5] = 1;
	m_settingsCSMUpdateCounter[6] = 1;
	m_settingsCSMUpdateCounter[7] = 1;
	m_settingsCSMUpdateCounter[8] = 1;
}

//	-----------------------------------------------------------------------------------------------------------------------------
ParabellumEngine::Core::Graphics::ExDRLight::~ExDRLight()
{
}


//	---------------------------------------------------
//		Methods
//	---------------------------------------------------


//	-----------------------------------------------------------------------------------------------------------------------------
void 
ParabellumEngine::Core::Graphics::ExDRLight::Initialize(
	_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager,
	EUINT32 width, EUINT32 height)
{
	
	Vector3D dir, color;
	EFLOAT32 intensity;

	dir.Set(0.2f, -0.4f, -0.4001f);
	dir.Normalize();
	color.Set(0.9f, 0.9f, 0.9f);
	intensity = 0.9f;

	m_directionalLight.Initialize(dir, color, intensity);

	// set spec
	m_screenWidth = width;
	m_screenHeight = height;


	// load shadres
	m_SDirectionalLightBasic = pResourceManager->Load<Shader>("Resources/Shaders/DR_DirectionalLightBasic.psx");
	m_SDirectionalLight = pResourceManager->Load<Shader>("Resources/Shaders/DR_DirectionalLight.psx");

	//m_SShadowMap = pResourceManager->Load<Shader>("Resources/Shaders/CreateDepthBuffer.psx");

	m_SBlurLightMap = pResourceManager->Load<Shader>("Resources/Shaders/DR_Light_BlurLightMap.psx");

	// create RT for directinal light
	m_RTLightMapSolid = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, m_screenWidth, m_screenHeight);

	m_RTLightMapSolidBlured = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, m_screenWidth, m_screenHeight);

	//
	// Shadow map
	m_solidShadowMapsCount = 4;

	m_ARTShadowDepth = new RenderTarget*[m_solidShadowMapsCount];
	//@
	m_ARTShadowDepth[0] = pDevice->CreateRenderTarget(GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, 1024, 1024);
	m_ARTShadowDepth[1] = pDevice->CreateRenderTarget(GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, 1024, 1024);
	m_ARTShadowDepth[2] = pDevice->CreateRenderTarget(GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, 1024, 1024);
	m_ARTShadowDepth[3] = pDevice->CreateRenderTarget(GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, 1024, 1024);


	//
	// Point light settings
	m_MPointLight = pResourceManager->Load<Model>("Resources/Models/Sphere_16x8.pfm");

	m_SPointLight = pResourceManager->Load<Shader>("Resources/Shaders/DR_PointLight.psx");

	m_RTPointLightSolid = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, m_screenWidth, m_screenHeight);



	/////////////////////////////////////////////////////////
	// load shadres
	m_SShadowMap_Solid = pResourceManager->Load<Shader>("Resources/Shaders/DR_GI_ShadowMap_Solid.psx");
	m_SShadowMap_Transparent = pResourceManager->Load<Shader>("Resources/Shaders/DR_GI_ShadowMap_Transparent.psx");


	// create RT for directinal light
	m_RTShadowMap = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, 128, 128);

	m_RTUVs = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, 128, 128);
}

//	-----------------------------------------------------------------------------------------------------------------------------
void 
ParabellumEngine::Core::Graphics::ExDRLight::Release(_IN_ GraphicsDevice* pDevice)
{
	/*
	if (m_SDirectionalLight != NULLPTR)
		pDevice->ReleaseShader(m_SDirectionalLight);

	if (m_SDirectionalLight != NULLPTR)
		pDevice->ReleaseShader(m_SShadowMap);
	*/

	delete[] m_ARTShadowDepth;
}



//	-----------------------------------------------------------------------------------------------------------------------------
void 
ParabellumEngine::Core::Graphics::ExDRLight::Draw(
	_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera,
	_IN_ RenderQuad* pRenderQuad,
	_IN_ RenderTarget* pGBuffer00, _IN_ RenderTarget* pGBuffer01, _IN_ RenderTarget* pGBuffer02)
{
	Vector4D cameraPositionFar, lightDirectionAmbient, lightPosition, lightColorIntensity, lightAmbientColor;
	Vector3D light00Corners[8], light01Corners[8], light02Corners[8], light03Corners[8];
	Vector4D light00_plane_top, light00_plane_left, light00_plane_near;
	Vector4D light01_plane_top, light01_plane_left, light01_plane_near;
	Vector4D light02_plane_top, light02_plane_left, light02_plane_near;
	Vector4D light03_plane_top, light03_plane_left, light03_plane_near;

	m_directionalLight.GetFrustum(0)->GetCorners(light00Corners);
	m_directionalLight.GetFrustum(1)->GetCorners(light01Corners);
	m_directionalLight.GetFrustum(2)->GetCorners(light02Corners);
	m_directionalLight.GetFrustum(3)->GetCorners(light03Corners);

	light00_plane_top = m_directionalLight.GetFrustum(0)->GetPlane(2)->m_V4D;
	light00_plane_left = m_directionalLight.GetFrustum(0)->GetPlane(0)->m_V4D;
	light00_plane_near = m_directionalLight.GetFrustum(0)->GetPlane(4)->m_V4D;

	light01_plane_top = m_directionalLight.GetFrustum( 1)->GetPlane(2)->m_V4D;
	light01_plane_left = m_directionalLight.GetFrustum(1)->GetPlane(0)->m_V4D;
	light01_plane_near = m_directionalLight.GetFrustum(1)->GetPlane(4)->m_V4D;

	light02_plane_top = m_directionalLight.GetFrustum( 2)->GetPlane(2)->m_V4D;
	light02_plane_left = m_directionalLight.GetFrustum(2)->GetPlane(0)->m_V4D;
	light02_plane_near = m_directionalLight.GetFrustum(2)->GetPlane(4)->m_V4D;

	light03_plane_top = m_directionalLight.GetFrustum( 3)->GetPlane(2)->m_V4D;
	light03_plane_left = m_directionalLight.GetFrustum(3)->GetPlane(0)->m_V4D;
	light03_plane_near = m_directionalLight.GetFrustum(3)->GetPlane(4)->m_V4D;

	lightDirectionAmbient.Set(m_directionalLight.GetDirection(), 0.5f);
	pCamera->GetPosition(cameraPositionFar.X, cameraPositionFar.Y, cameraPositionFar.Z);
	cameraPositionFar.W = pCamera->GetZFar();
	lightPosition.Set(0.0f, 0.0f, 0.0f, 0.0f);
	lightColorIntensity.Set(m_directionalLight.GetColor()->X, m_directionalLight.GetColor()->Y, m_directionalLight.GetColor()->Z, m_directionalLight.GetIntensity());
	lightAmbientColor.Set(0.2f, 0.2f, 0.2f, 0.5f);
	

	//0000000000000000000000000000000000000000000000000000000000
	Vector4D lightBoxes[4];

	lightBoxes[0].X = (light00Corners[1] - light00Corners[0]).Length();
	lightBoxes[0].Y = (light00Corners[3] - light00Corners[0]).Length();

	lightBoxes[1].X = (light01Corners[1] - light01Corners[0]).Length();
	lightBoxes[1].Y = (light01Corners[3] - light01Corners[0]).Length();

	lightBoxes[2].X = (light02Corners[1] - light02Corners[0]).Length();
	lightBoxes[2].Y = (light02Corners[3] - light02Corners[0]).Length();

	lightBoxes[3].X = (light03Corners[1] - light03Corners[0]).Length();
	lightBoxes[3].Y = (light03Corners[3] - light03Corners[0]).Length();

	//0000000000000000000000000000000000000000000000000000000000
	Vector4D cameraSlice00_min, cameraSlice00_max;

	cameraSlice00_min.XYZ = *m_directionalLight.GetCSMBox(0)->GetMin();
	cameraSlice00_max.XYZ = *m_directionalLight.GetCSMBox(0)->GetMax();


	//0000000000000000000000000000000000000000000000000000000000
	Vector3D point;

	point = light00Corners[6];


	Vector3D pointView = m_directionalLight.m_view[0] * point;

	EFLOAT32 dist = Math::Distance(&m_directionalLight.GetFrustum(0)->GetPlane(4)->m_V4D.XYZ, m_directionalLight.GetFrustum(0)->GetPlane(4)->m_V4D.W, &point);


	//printf("\nMAcerz = %f", pointView.Z);
	//printf("\nDist = %f", dist);
	//Sleep(99999);


	// set render targets
	pDevice->SetRenderTargets(m_RTLightMapSolid, m_RTLightMapSolid, true);

	// set buffers
	pDevice->SetBuffers(pRenderQuad->GetVertexBuffer(), pRenderQuad->GetIndexBuffer());

	// set shader
	pDevice->SetShader(m_SDirectionalLight);

	m_SDirectionalLight->GetParametr(0)->Set(&cameraPositionFar);
	m_SDirectionalLight->GetParametr(1)->Set(&lightDirectionAmbient);
	m_SDirectionalLight->GetParametr(2)->Set(&lightPosition);
	m_SDirectionalLight->GetParametr(3)->Set(&lightColorIntensity);
	m_SDirectionalLight->GetParametr(4)->Set(&lightAmbientColor);
	m_SDirectionalLight->GetParametr(5)->Set(pCamera->GetCTL());
	m_SDirectionalLight->GetParametr(6)->Set(pCamera->GetCTL2CTR());
	m_SDirectionalLight->GetParametr(7)->Set(pCamera->GetCTL2CBL());

	m_SDirectionalLight->GetParametr(8)->Set(&cameraSlice00_min);
	m_SDirectionalLight->GetParametr(9)->Set(&cameraSlice00_max);

	m_SDirectionalLight->GetParametr(10)->Set(&lightBoxes[0]);
	m_SDirectionalLight->GetParametr(11)->Set(&lightBoxes[1]);
	m_SDirectionalLight->GetParametr(12)->Set(&lightBoxes[2]);
	m_SDirectionalLight->GetParametr(13)->Set(&lightBoxes[3]);
	m_SDirectionalLight->GetParametr(14)->Set(&light00_plane_top);
	m_SDirectionalLight->GetParametr(15)->Set(&light00_plane_left);
	m_SDirectionalLight->GetParametr(16)->Set(&light00_plane_near);
	m_SDirectionalLight->GetParametr(17)->Set(&light01_plane_top);
	m_SDirectionalLight->GetParametr(18)->Set(&light01_plane_left);
	m_SDirectionalLight->GetParametr(19)->Set(&light01_plane_near);
	m_SDirectionalLight->GetParametr(20)->Set(&light02_plane_top);
	m_SDirectionalLight->GetParametr(21)->Set(&light02_plane_left);
	m_SDirectionalLight->GetParametr(22)->Set(&light02_plane_near);
	m_SDirectionalLight->GetParametr(23)->Set(&light03_plane_top);
	m_SDirectionalLight->GetParametr(24)->Set(&light03_plane_left);
	m_SDirectionalLight->GetParametr(25)->Set(&light03_plane_near);




	m_SDirectionalLight->GetResource(0)->Set<RenderTarget>(pGBuffer00);
	m_SDirectionalLight->GetResource(1)->Set<RenderTarget>(pGBuffer01);
	m_SDirectionalLight->GetResource(2)->Set<RenderTarget>(pGBuffer02);
	m_SDirectionalLight->GetResource(3)->Set<RenderTarget>(m_ARTShadowDepth[0]);
	m_SDirectionalLight->GetResource(4)->Set<RenderTarget>(m_ARTShadowDepth[1]);
	m_SDirectionalLight->GetResource(5)->Set<RenderTarget>(m_ARTShadowDepth[2]);
	m_SDirectionalLight->GetResource(6)->Set<RenderTarget>(m_ARTShadowDepth[3]);

	pDevice->SetSampler(0, SamplerType::POINT);
	pDevice->SetSampler(1, SamplerType::POINT);
	pDevice->SetSampler(2, SamplerType::POINT);
	pDevice->SetSampler(3, SamplerType::POINT);
	pDevice->SetSampler(4, SamplerType::POINT);
	pDevice->SetSampler(5, SamplerType::POINT);
	pDevice->SetSampler(6, SamplerType::POINT);


	pDevice->SetShaderResources();
	pDevice->SetShaderVariables();
	pDevice->RenderRAW();


	//
	// Blur lightmap
	//
	//BlurLightMap(pDevice, pCamera, pRenderQuad);

}
//	-----------------------------------------------------------------------------------------------------------------------------
void 
ParabellumEngine::Core::Graphics::ExDRLight::DrawDirectionalLightSolidShadow(
	_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera, _IN_ Utiles::FrustumOutput* pFrustumOutput)
{
	// inc frame counter only if CSM is 0
	m_frameCounter++;

	Vector4D lightDir, cameraProperties, frustumProperties;

	cameraProperties.X = pCamera->GetZNear();
	cameraProperties.Y = pCamera->GetZFar();
	m_directionalLight.CalculateLight3DData(pCamera);

	for (EUINT32 i = 0; i < m_solidShadowMapsCount; ++i)
		m_directionalLight.CreateLightViewProjectionMatrix(i);


	lightDir = *m_directionalLight.GetDirection();



	Vector3D light00Corners[8];
	m_directionalLight.GetFrustum(0)->GetCorners(light00Corners);

	Vector4D light00_plane_top, light00_plane_left, light00_plane_near;


	frustumProperties.X = (light00Corners[1] - light00Corners[0]).Length();
	frustumProperties.Y = frustumProperties.X;


	pDevice->RasterizerSetMode(PF_GI_RS_CULL_COUNTERCLOCKWISE);

	// set shader
	pDevice->SetShader(m_SShadowMap_Solid);


	Utiles::SceneCart* pSceneCart = pFrustumOutput->GetSceneCart();

	for (EUINT32 CSMNo = 0; CSMNo < m_solidShadowMapsCount; CSMNo++)
	{

		// skip drawing shadow map
		//if ((m_frameCounter % m_settingsCSMUpdateCounter[CSMNo]) != 0)
		//	return;

		m_SShadowMap_Solid->SPARAMETR("g_cameraView") = m_directionalLight.m_view[CSMNo];
		m_SShadowMap_Solid->SPARAMETR("g_cameraProjection") = m_directionalLight.m_projection[CSMNo];

		m_SShadowMap_Solid->SPARAMETR("g_lightPlaneTop") = m_directionalLight.GetFrustum(CSMNo)->GetPlane(2)->m_V4D;
		m_SShadowMap_Solid->SPARAMETR("g_lightPlaneLeft") = m_directionalLight.GetFrustum(CSMNo)->GetPlane(0)->m_V4D;
		m_SShadowMap_Solid->SPARAMETR("g_lightPlaneNear") = m_directionalLight.GetFrustum(CSMNo)->GetPlane(4)->m_V4D;
		m_SShadowMap_Solid->SPARAMETR("g_lightFrustumProperties") = frustumProperties;

		// set render targets
		pDevice->SetRenderTargets(m_ARTShadowDepth[CSMNo], m_ARTShadowDepth[CSMNo], true);

		EUINT32 q = 0; // current basket to render

		while (q < pSceneCart->GetBasketsCount())
		{
			for (EUINT32 i = 0; i < pSceneCart->m_containers[q]->GetPickedUniqueObjectsCount(); ++i)
			{
				//PFDLOG("PFGI", "SHCL", " @@@@ ODPALAMY SIE !");		
				pDevice->SetVertexBuffer(pSceneCart->m_containers[q]->m_objects[i]->GetVertexBuffer());
				pDevice->SetIndexBuffer(pSceneCart->m_containers[q]->m_objects[i]->GetIndexBuffer());

				m_SShadowMap_Solid->GetResource(0)->Set<Texture2D>(pSceneCart->m_containers[q]->m_objects[i]->GetDiffuseMap());

				// set resources
				pDevice->SetShaderResources();


				for (EUINT32 n = 0; n < pSceneCart->m_containers[q]->GetObjectsCountOfIndex(i); ++n)
				{
					//*m_SShadowMap->GetParametrsBuffer<Matrix4x4>(0, 0) = *pSceneCart->m_containers[q]->GetObjectOfIndex(i, n)->GetWorldMatrix();	
					m_SShadowMap_Solid->SetParametrsBuffer(0, pSceneCart->m_containers[q]->GetObjectOfIndex(i, n)->GetWorldMatrix());

					pDevice->Render(PF_GI_RENDERING_UPDATEVARIABLES | PF_GI_RENDERING_UPDATERESOURCES);
				}
			}

			q++;

		} // end while 'q'

	}

	// set rasterizer back to draw all faces
	pDevice->RasterizerSetMode(PF_GI_RS_CULL_NONE);
}



//	-----------------------------------------------------------------------------------------------------------------------------
void 
ParabellumEngine::Core::Graphics::ExDRLight::DrawPointLights(
	_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera, _IN_ RenderQuad* pRenderQuad,
	_IN_ RenderTarget* pGBuffer00, _IN_ RenderTarget* pGBuffer01, _IN_ RenderTarget* pGBuffer02,
	_IN_ PointLight* pPointLights, EUINT32 pointLightsCount
)
{
	Vector4D cameraPositionFar;
	EUINT32 operationCounter = 0;
	Vector3D corners[8];

	Vector4D plPositionRay, plColor;

	pCamera->GetPosition(cameraPositionFar.X, cameraPositionFar.Y, cameraPositionFar.Z);
	cameraPositionFar.W = pCamera->GetZFar();


	// set rasterizer back to draw all faces
	pDevice->RasterizerSetMode(PF_GI_RS_CULL_COUNTERCLOCKWISE);
	//pDevice->TurnAlphaBlending(true);
	pDevice->TurnDepthBuffer(false);

	pDevice->BlendSetState(PF_GI_BLEND_COLOR_ADD);


	// set render targets
	pDevice->SetRenderTargets(m_RTPointLightSolid, m_RTPointLightSolid, true);
	//pDevice->SetRenderTargets(m_RTLightMapSolid, m_RTLightMapSolid, false);

	// set buffers
	pDevice->SetBuffers(m_MPointLight->GetVertexBuffer(0), m_MPointLight->GetIndexBuffer(0));

	// set shader
	pDevice->SetShader(m_SPointLight, ShaderFlagSet::NO_SAMPLERS);

	m_SPointLight->SetParametrsBuffer(0, pCamera->GetViewProjection());
	m_SPointLight->SetParametrsBuffer(1, &cameraPositionFar);
	m_SPointLight->SetParametrsBuffer(2, pCamera->GetCTL());
	m_SPointLight->SetParametrsBuffer(3, pCamera->GetCTL2CTR());
	m_SPointLight->SetParametrsBuffer(4, pCamera->GetCTL2CBL());
	
	
	m_SPointLight->GetResource(0)->Set<RenderTarget>(pGBuffer00);
	m_SPointLight->GetResource(1)->Set<RenderTarget>(pGBuffer01);

	pDevice->SetShaderResources();

	for (EUINT32 i = 0; i < pointLightsCount; ++i)
	{
		pPointLights[i].GetPosition(plPositionRay.XYZ);
		plPositionRay.W = pPointLights[i].GetRayLength();
		plColor.Set(1.0f, 0.0f, 0.0f, 1.0f);

		
		m_SPointLight->SetParametrsBuffer(5, &plPositionRay);
		m_SPointLight->SetParametrsBuffer(6, pPointLights[i].GetColor());
		
		pDevice->Render(PF_GI_RENDERING_UPDATEVARIABLES | PF_GI_RENDERING_UPDATERESOURCES);
	}

	pDevice->RasterizerSetMode(PF_GI_RS_CULL_NONE);
	pDevice->TurnAlphaBlending(false);
	pDevice->TurnDepthBuffer(true);
}

//	-----------------------------------------------------------------------------------------------------------------------------

//	-----------------------------------------------------------------------------------------------------------------------------
