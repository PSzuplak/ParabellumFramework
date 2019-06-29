#include "DeferredRenderer.h"




//	------------------------------------------------------------
//		Constructors and destructors
//	------------------------------------------------------------
ParabellumEngine::Core::Graphics::DeferredRenderer::DeferredRenderer()
	: m_threadGBufferSolidGenerateMipMaps{}
{
	m_frameRate = 0;
	m_frameCounter = 0;
	m_frameRateLast = 100;
	m_timeBegin = std::clock();
	m_timeEnd = std::clock();

	ccc = new Vector4D[256 * 256];
	xxxxdrawcalls = 0;
	//
	// Settings
	m_settingsRenderTreeFrameSkip = 4;
}


ParabellumEngine::Core::Graphics::DeferredRenderer::~DeferredRenderer()
{
	delete m_terrainz;
}

//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------




ParabellumFramework::EUINT32
ParabellumEngine::Core::Graphics::DeferredRenderer::Initialize(
	_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager, EUINT32 width, EUINT32 height)
{
	m_renderTree.Initialize(pDevice, 5);


	//
	//m_solidObjects = std::shared_ptr<PartsContainer>( new PartsContainer() );
	//m_solidObjects->CleanUp();

	/*
	for (int i = 0; i < m_solidObjects->m_uniqueObjectsCount; ++i)
	{
		EUINT32 index = m_solidObjects->m_forward[i];
		printf("\n unique object = %i ; size = %i", index, m_solidObjects->m_objectsCounter[i]);
	}
	*/



	m_screenWidth = width;
	m_screenHeight = height;

	m_fullScreenQuad.Initialize(pDevice, -1.0f, 1.0f, 1.0f, -1.0f);


	//m_renderArea.Initialize(pDevice, 64, 64);



	//create render targets
	m_RTGBufferSolid0 = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, width, height);

	m_RTGBufferSolid1 = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, width, height);

	m_RTGBufferSolid2 = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, width, height);

	//m_RTGBufferSolid3 = pDevice->CreateRenderTarget(
	//	GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, width, height);



	m_RTGBufferSolidBack0 = pDevice->CreateRenderTarget(GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, width/2, height/2);
	m_RTGBufferSolidBack1 = pDevice->CreateRenderTarget(GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, width/2, height/2);
	m_RTGBufferSolidBack2 = pDevice->CreateRenderTarget(GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, width/2, height/2);
	m_RTGBufferSolidBack3 = pDevice->CreateRenderTarget(GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, width/2, height/2);


	//
	//
	m_RTMergeGBufferLightMap = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, width, height);

	//
	//
	m_RTMergeGBufferWithEffects = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, width, height);


	// mipmaps
	//m_RTGBufferSolidMipMaps[0] = pDevice->CreateRenderTarget(GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, 1024, 1024);
	m_RTGBufferSolidMipMaps[0] = pDevice->CreateRenderTarget(GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, 512, 512);
	m_RTGBufferSolidMipMaps[1] = pDevice->CreateRenderTarget(GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, 256, 256);
	m_RTGBufferSolidMipMaps[2] = pDevice->CreateRenderTarget(GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, 128, 128);
	m_RTGBufferSolidMipMaps[3] = pDevice->CreateRenderTarget(GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, 64, 64);
	m_RTGBufferSolidMipMaps[4] = pDevice->CreateRenderTarget(GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, 32, 32);
	m_RTGBufferSolidMipMaps[5] = pDevice->CreateRenderTarget(GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, 16, 16);
	m_RTGBufferSolidMipMaps[6] = pDevice->CreateRenderTarget(GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, 8, 8);
	m_RTGBufferSolidMipMaps[7] = pDevice->CreateRenderTarget(GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, 4, 4);

	m_RTGBufferProperties = pDevice->CreateRenderTarget(GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, 32, 32);
	m_RTGBufferPropertiesHisogram = pDevice->CreateRenderTarget(GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, 32, 32);


	// RTs for particles
	m_RTGBufferParticlesFront0 = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, width, height);

	m_RTGBufferParticlesFront1 = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, width, height);

	m_RTGBufferParticlesBack0 = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, width, height);

	m_RTParticleLightMap = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, width, height);


	// merges
	m_RTGBufferMergeBasics = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, width, height);


	
	//
	// create shaders
	m_ShaderCreateGBufferSolid = pResourceManager->Load<Shader>("Resources/Shaders/DR_CreateGBufferSolid_Default.psx");
	m_ShaderCreateGBufferSolidFar = pResourceManager->Load<Shader>("Resources/Shaders/DR_CreateGBufferSolid_Far.psx");
	m_ShaderCreateGBufferSolidInstanced2 = pResourceManager->Load<Shader>("Resources/Shaders/DR_CreateGBufferSolid_Instanced2.psx");
	m_ShaderCreateGBufferSolidInstanced8 = pResourceManager->Load<Shader>("Resources/Shaders/DR_CreateGBufferSolid_Instanced8.psx");

	m_ShaderCreateGBufferSolid_GPUCalc = pResourceManager->Load<Shader>("Resources/Shaders/DR_CreateGBufferSolid_GPUCalc.psx");
	m_ShaderCreateGBufferSolid_GPUCalcI8 = pResourceManager->Load<Shader>("Resources/Shaders/DR_CreateGBufferSolid_GPUCalcI8.psx");


	m_SMergeGBufferSolidLightMap = pResourceManager->Load<Shader>("Resources/Shaders/DR_MergeGBufferLight.psx");


	m_ShaderDisplayQuad = pResourceManager->Load<Shader>("Resources/Shaders/DisplayQuad.psx");

	m_SMergeSolidNParticles = pResourceManager->Load<Shader>("Resources/Shaders/DR_MergeSolidNParticles.psx");

	m_SParticles = pResourceManager->Load<Shader>("Resources/Shaders/DR_CreateGBufferParticles.psx");

	m_SMergeGBufferWithEffects = pResourceManager->Load<Shader>("Resources/Shaders/DR_MergeForTree.psx");

	m_SShrinkMaterialProperties = pResourceManager->Load<Shader>("Resources/Shaders/ShaderShrinkMaterial.psx");

	m_SCopyGBufferProperties = pResourceManager->Load<Shader>("Resources/Shaders/ShaderCopyGBufferProperties.psx");

	//
	// initialize extensions
	m_ExDRLight.Initialize(pDevice, pResourceManager, width, height);
	m_ExDRSSAO.Initialize(pDevice, pResourceManager, width, height);
	m_ExDRHDR.Initialize(pDevice, pResourceManager, width, height);
	m_ExDRParallaxMapping.Initialize(pDevice, pResourceManager, width, height);
	m_ExDRVolumetricLight.Initialize(pDevice, pResourceManager, width, height);


	m_isFeatureParallaxMaping = false;

	//
	//
	//
	m_grass.GenerateGrass(pDevice, 5.0f, 5.0f, 50, 0.1f, 0.2f, 0.5f, 1.0f);
	m_ShaderDrawGrass = pResourceManager->Load<Shader>("Resources/Shaders/DR_CreateGBufferGrass.psx");


	//
	//
	//
	//m_terrain.Initialize(pDevice, pResourceManager->Load<Texture2D>("Resources/Textures/test1.pft"), 1.0f, 10.0f, 0, 0, 255, 255);


	

	m_ShaderDrawTerrain = pResourceManager->Load<Shader>("Resources/Shaders/DR_CreateGBufferTerrain.psx");

	//m_terrain.Initialize(pDevice, pResourceManager->Load<Texture2D>("Resources/Textures/test1.pft"), 1.0f, 10.0f, 0, 0, 255, 255);
	//m_terrainCells.push_back(&m_terrain);
	//m_terrainz = new Terrain(pDevice, pResourceManager, "Resources/Terrain/heightmap_01/Raw32.r32", 8, 8, 1.0f, 100.0f);


	//
	// Particles

	m_particleSmoke.Initialize(pDevice, pResourceManager, 200);

	//TEX01 = pResourceManager->Load<Texture2D>("Resources/Textures/smoke.pft");

	SDrawOctree = pResourceManager->Load<Shader>("Resources/Shaders/DrawOctreeBox.psx");

	m_SParticlesDirectionalLight = pResourceManager->Load<Shader>("Resources/Shaders/DR_ParticleDirectionalLight.psx");


	//
	// Settings
	// 

	m_settingsMaxParallaxDepth = 10.0f;



	//
	// Threads
	//
	//m_threadGBufferSolidGenerateMipMaps = std::thread{ &DeferredRenderer::PIZDA, this };
	//m_threadGBufferSolidGenerateMipMaps.detach();

	return 0;
}

ParabellumFramework::EUINT32
ParabellumEngine::Core::Graphics::DeferredRenderer::Release()
{
	return 0;
}


//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------


//	-----------------------------------------------------------------------------------------------------------------------------
void ParabellumEngine::Core::Graphics::DeferredRenderer::PrepareToDraw(_IN_ GraphicsDevice* pDevice)
{
	m_frameCounter++;
	m_frameRate++;



	m_timeEnd = std::clock();

	

	if (m_timeEnd - m_timeBegin > 1000)
	{
		//printf("\n");
		m_frameRateLast = m_frameRate;

		m_timeBegin = std::clock();
		printf("\n framerate : %u", m_frameRate);
		m_frameRate = 0;	

		//m_settingsRenderTreeFrameSkip = (m_frameRateLast >> 2) + 1;

		printf("\pDevice->m_debugDrawCall = %u (%u/%u)", xxxxdrawcalls, xxxxdrawcallsA, xxxxdrawcallsB);


		//printf("\n");
	}




}

//	-----------------------------------------------------------------------------------------------------------------------------
void ParabellumEngine::Core::Graphics::DeferredRenderer::PrepareAndSetGBufferSolid(_IN_ GraphicsDevice* pDevice)
{
	// set render targets
	pDevice->SetRenderTargets(m_RTGBufferSolid0, m_RTGBufferSolid1, m_RTGBufferSolid2, m_RTGBufferSolid0, true);
}



//	-----------------------------------------------------------------------------------------------------------------------------
void ParabellumEngine::Core::Graphics::DeferredRenderer::DrawSolidPhase1(
	_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera,
	_IN_ Utiles::FrustumOutput* pFrustumOutput
	)
{
	PROFILER_BEGIN("Draw Solid Front");
	//printf("\nDraw Solid Front Begin");
	// set render targets
	pDevice->SetRenderTargets(m_RTGBufferSolid0, m_RTGBufferSolid1, m_RTGBufferSolid2, m_RTGBufferSolid0, false);

	
	//pDevice->RasterizerSetMode(PF_GI_RS_CULL_NONE);
	pDevice->RasterizerSetMode(PF_GI_RS_CULL_CLOCKWISE);
	//pDevice->RasterizerSetMode(PF_GI_CULL_NONE | PF_GI_MODE_WIREFRAME);
	//pDevice->RasterizerSetMode(PF_GI_CULL_CLOCKWISE | PF_GI_MODE_WIREFRAME);

	//pDevice->TurnAlphaBlending(true);
	//Matrix4x4 worldView[8], worldViewIT[8];
	//
	//for (EUINT32 m = 0; m < 8; ++m)
	//{
	//	PF_MATRIX4X4_MakeIdentity(worldView[m]);
	//	PF_MATRIX4X4_MakeIdentity(worldViewIT[m]);
	//}

	Shader* usedShader = m_ShaderCreateGBufferSolid;


	Vector4D cameraPositionFar, materialProperties, objectProperties;

	pCamera->GetPosition(cameraPositionFar.X, cameraPositionFar.Y, cameraPositionFar.Z);
	cameraPositionFar.W = pCamera->GetZFar();
	
	objectProperties.Set(0.0f, 0.0f, 0.0f, 0.0f);

	// call SetShader here to setup samplers
	pDevice->SetShader(m_ShaderCreateGBufferSolid);
	
	xxxxdrawcalls = 0;
	xxxxdrawcallsA = 0;
	xxxxdrawcallsB = 0;

	Utiles::SceneCart* pSceneCart = pFrustumOutput->GetSceneCart();

	//
	//
	m_sliceNo++;

	if (m_sliceNo == m_sliceCount)
	{
		m_renderTree.SwapDeck();
		m_renderTree.ClearDeck();
		m_renderTree.AddToDeck(0);
		m_sliceNo = 0;
	}

	

	EFLOAT32 delta = 2.0f / m_sliceCount;
	EFLOAT32 pox1 = m_sliceNo * delta - 1.0f;
	EFLOAT32 pox2 = m_sliceNo * delta + delta - 1.0f;

	//printf("\nslice = %f -> %f", pox1, pox2);


	EUINT32 q = 0; // current basket to render

	while (q < pSceneCart->GetBasketsCount() )
	{
		// check if basket is ready
		if (pFrustumOutput->IsBasketReady(q) == false)
		{
			continue;
		}

		for (EUINT32 i = 0; i < pSceneCart->m_containers[q]->GetPickedUniqueObjectsCount(); ++i)
		{
			EUINT32 counter = 0;

			//
			// Draw instances of 8
			/*
			if (pSceneCart->m_containers[q]->GetObjectsCountOfIndex(i) - counter >= 8)
			{
				pDevice->SetVertexBuffer(pSceneCart->m_containers[q]->m_objects[i]->GetVertexBufferInstanced8());
				pDevice->SetIndexBuffer(pSceneCart->m_containers[q]->m_objects[i]->GetIndexBufferInstanced8());

				pDevice->SetShader(m_ShaderCreateGBufferSolidInstanced8, ShaderFlagSet::NO_SAMPLERS);

				m_ShaderCreateGBufferSolidInstanced8->SetParametrsBuffer(0, pCamera->GetView());
				m_ShaderCreateGBufferSolidInstanced8->SetParametrsBuffer(1, pCamera->GetProjection());
				m_ShaderCreateGBufferSolidInstanced8->SetParametrsBuffer(3, &cameraPositionFar);

				m_ShaderCreateGBufferSolidInstanced8->GetResource(0)->Set<Texture2D>(pSceneCart->m_containers[q]->m_objects[i]->GetDiffuseMap());
				m_ShaderCreateGBufferSolidInstanced8->GetResource(1)->Set<Texture2D>(pSceneCart->m_containers[q]->m_objects[i]->GetNormalMap());
				m_ShaderCreateGBufferSolidInstanced8->GetResource(2)->Set<Texture2D>(pSceneCart->m_containers[q]->m_objects[i]->GetTGEMap());
				m_ShaderCreateGBufferSolidInstanced8->GetResource(3)->Set<Texture2D>(pSceneCart->m_containers[q]->m_objects[i]->GetHeightMap());

				pDevice->SamplerSetDefault(0, SamplerType::ANISOTROPIC);
				pDevice->SamplerSetDefault(1, SamplerType::ANISOTROPIC);
				pDevice->SamplerSetDefault(2, SamplerType::ANISOTROPIC);
				pDevice->SamplerSetDefault(3, SamplerType::ANISOTROPIC);

				//m_ShaderCreateGBufferSolidInstanced8->Parametrs<"g_TDiffuseMap"_hash>() = pSceneCart->m_containers[q]->m_objects[i]->GetDiffuseMap();
				//m_ShaderCreateGBufferSolidInstanced8->Parametrs<"g_TNSMap"_hash>() = pSceneCart->m_containers[q]->m_objects[i]->GetDiffuseMap();
				//m_ShaderCreateGBufferSolidInstanced8->Parametrs<"g_TTGEMap"_hash>() = pSceneCart->m_containers[q]->m_objects[i]->GetDiffuseMap();
				//m_ShaderCreateGBufferSolidInstanced8->Parametrs<"g_THGRRMap"_hash>() = pSceneCart->m_containers[q]->m_objects[i]->GetDiffuseMap();

				// set resources
				pDevice->SetShaderResources();

				for (EUINT32 n = counter; n < pSceneCart->m_containers[q]->GetObjectsCountOfIndex(i) - 7; n += 8, counter += 8)
				{
					*m_ShaderCreateGBufferSolidInstanced8->GetParametrsBuffer<Matrix4x4>(0, 2) = *pSceneCart->m_containers[q]->GetObjectOfIndex(i, n + 0)->GetWorldMatrix();
					*m_ShaderCreateGBufferSolidInstanced8->GetParametrsBuffer<Matrix4x4>(0, 3) = *pSceneCart->m_containers[q]->GetObjectOfIndex(i, n + 1)->GetWorldMatrix();
					*m_ShaderCreateGBufferSolidInstanced8->GetParametrsBuffer<Matrix4x4>(0, 4) = *pSceneCart->m_containers[q]->GetObjectOfIndex(i, n + 2)->GetWorldMatrix();
					*m_ShaderCreateGBufferSolidInstanced8->GetParametrsBuffer<Matrix4x4>(0, 5) = *pSceneCart->m_containers[q]->GetObjectOfIndex(i, n + 3)->GetWorldMatrix();
					*m_ShaderCreateGBufferSolidInstanced8->GetParametrsBuffer<Matrix4x4>(0, 6) = *pSceneCart->m_containers[q]->GetObjectOfIndex(i, n + 4)->GetWorldMatrix();
					*m_ShaderCreateGBufferSolidInstanced8->GetParametrsBuffer<Matrix4x4>(0, 7) = *pSceneCart->m_containers[q]->GetObjectOfIndex(i, n + 5)->GetWorldMatrix();
					*m_ShaderCreateGBufferSolidInstanced8->GetParametrsBuffer<Matrix4x4>(0, 8) = *pSceneCart->m_containers[q]->GetObjectOfIndex(i, n + 6)->GetWorldMatrix();
					*m_ShaderCreateGBufferSolidInstanced8->GetParametrsBuffer<Matrix4x4>(0, 9) = *pSceneCart->m_containers[q]->GetObjectOfIndex(i, n + 7)->GetWorldMatrix();

					pDevice->Render(PF_GI_RENDERING_UPDATEVARIABLES);

					// update debug data
					m_debugDrawCallsSolid++;
					m_debugTotalDrawedTriangles += pSceneCart->m_containers[q]->m_objects[i]->GetIndexBufferInstanced8()->GetIndicesCount() / 3;
				}

			}
			*/

			if (q < 2)
				usedShader = m_ShaderCreateGBufferSolid;
			else
				usedShader = m_ShaderCreateGBufferSolidFar;

			//
			// Draw instances of 1	
			pDevice->SetShader(usedShader, ShaderFlagSet::NO_SAMPLERS);

			usedShader->SPARAMETR("g_cameraView") = *pCamera->GetView();
			usedShader->SPARAMETR("g_cameraProjection") = *pCamera->GetProjection();
			usedShader->SPARAMETR("g_cameraPositionFar") = cameraPositionFar;
			
			// set samplers
			pDevice->SetSampler(0, SamplerType::ANISOTROPIC);
			pDevice->SetSampler(1, SamplerType::ANISOTROPIC);
			pDevice->SetSampler(2, SamplerType::ANISOTROPIC);
			pDevice->SetSampler(3, SamplerType::ANISOTROPIC);

			if (pSceneCart->m_containers[q]->GetObjectsCountOfIndex(i) - counter >= 1)
			{
				pDevice->SetVertexBuffer(pSceneCart->m_containers[q]->m_objects[i]->GetVertexBuffer());
				pDevice->SetIndexBuffer(pSceneCart->m_containers[q]->m_objects[i]->GetIndexBuffer());
		
				usedShader->GetResource(0)->Set<Texture2D>(pSceneCart->m_containers[q]->m_objects[i]->GetDiffuseMap());
				usedShader->GetResource(1)->Set<Texture2D>(pSceneCart->m_containers[q]->m_objects[i]->GetNormalMap());
				usedShader->GetResource(2)->Set<Texture2D>(pSceneCart->m_containers[q]->m_objects[i]->GetTGEMap());
				usedShader->GetResource(3)->Set<Texture2D>(pSceneCart->m_containers[q]->m_objects[i]->GetHeightMap());

				// set the same textures for each object, so only variables will be updated
				pDevice->SetShaderResources();
				 
				for (EUINT32 n = counter; n < pSceneCart->m_containers[q]->GetObjectsCountOfIndex(i); n += 1, counter += 1)
				{
					EUINT32 id = pSceneCart->m_containers[q]->GetObjectOfIndex(i, n)->GetId();

					//if(m_renderTree.m_isDeckReady == true)
					//	if (m_renderTree.m_deck.Contains(id) == false)
					//	{
					//		continue;
					//	}

					//printf("x %u ", id);

					Vector4D ao;
					EFLOAT32 ad;

					pCamera->ToScreenPosition(pSceneCart->m_containers[q]->GetObjectOfIndex(i, n)->GetTransformedBoundingBox(), &ao, &ad);
					

					//if ( (ao.X > -0.75f) && (ao.Z < 0.75f) )
					//	if (ao.X < pox1 || ao.X > pox2)
					//		continue;



					//if (m_renderTree.Covers(ao, ad) == true )
					//{
					//	continue;
					//}


					//if (pCamera->Intersects(pSceneCart->m_containers[q]->GetObjectOfIndex(i, n)->GetTransformedBoundingSphere()) == IntersectCodes::DISJOINT)
					//	continue;

					

					
					usedShader->GetResource(0)->Set<Texture2D>(pSceneCart->m_containers[q]->m_objects[i]->GetDiffuseMap());
					usedShader->GetResource(1)->Set<Texture2D>(pSceneCart->m_containers[q]->m_objects[i]->GetNormalMap());
					usedShader->GetResource(2)->Set<Texture2D>(pSceneCart->m_containers[q]->m_objects[i]->GetTGEMap());
					usedShader->GetResource(3)->Set<Texture2D>(pSceneCart->m_containers[q]->m_objects[i]->GetHeightMap());
					pDevice->SetShaderResources();

					objectProperties.X =  *((EFLOAT32*)(&id));
					//objectProperties.X = (EFLOAT32)id;
					usedShader->SPARAMETR("g_objectProperties") = objectProperties;

					usedShader->SPARAMETR("g_world") = *pSceneCart->m_containers[q]->GetObjectOfIndex(i, n)->GetWorldMatrix();

					pDevice->SetShaderVariables();
					pDevice->RenderRAW();


					xxxxdrawcalls++;
					xxxxdrawcallsA++;
				}

			}
			


			/*
			pDevice->TurnDepthBuffer(true);
			pDevice->RasterizerSetMode(PF_GI_RS_CULL_NONE | PF_GI_RS_MODE_WIREFRAME);
			pDevice->SetVertexBuffer(PrimitivesFactory::Instance(pDevice).GetBoxVertexBuffer());
			pDevice->SetIndexBuffer(PrimitivesFactory::Instance(pDevice).GetBoxIndexBuffer());
			
			usedShader->GetResource(0)->Set<Texture2D>(pSceneCart->m_containers[q]->m_objects[i]->GetDiffuseMap());
			usedShader->GetResource(1)->Set<Texture2D>(pSceneCart->m_containers[q]->m_objects[i]->GetNormalMap());
			usedShader->GetResource(2)->Set<Texture2D>(pSceneCart->m_containers[q]->m_objects[i]->GetTGEMap());
			usedShader->GetResource(3)->Set<Texture2D>(pSceneCart->m_containers[q]->m_objects[i]->GetHeightMap());
			
			// set resources
			pDevice->SetShaderResources();
				
			for (EUINT32 n = 0; n < pSceneCart->m_containers[q]->GetObjectsCountOfIndex(i); n += 1)
			{
				BoundingBox bbox;
				Vector3D center, vscal, vmin, vmax, nmin, nmax;
				Matrix4x4 translate, scale, world;
			
				bbox = *pSceneCart->m_containers[q]->GetObjectOfIndex(i, n)->GetTransformedBoundingBox();
				bbox.GetMinMax(vmin, vmax);
				center = (vmax + vmin) * 0.5f;
				vscal = vmax - vmin;
				translate.MakeTranslation(&center);
				scale.MakeScale(&vscal);
				world = translate * scale;
			
				//usedShader->SPARAMETR("g_world") = *pSceneCart->m_containers[q]->GetObjectOfIndex(i, n)->GetWorldMatrix();		
				usedShader->SPARAMETR("g_world") = world;

				pDevice->SetShaderVariables();
				pDevice->RenderRAW();
			}
			
			pDevice->TurnDepthBuffer(true);
			pDevice->RasterizerSetMode(PF_GI_RS_CULL_COUNTERCLOCKWISE | PF_GI_RS_MODE_SOLID);
			*/

		} // end for 'i'


		// take the next backet
		q++;

	} // end for 'q'
	

	//
	// draw deck
	if(1 == 2)
	{
		//printf("\n\n Draw deck : ");

		pDevice->SetShader(m_ShaderCreateGBufferSolid, ShaderFlagSet::NO_SAMPLERS);

		m_ShaderCreateGBufferSolid->SPARAMETR("g_cameraView") = *pCamera->GetView();
		m_ShaderCreateGBufferSolid->SPARAMETR("g_cameraProjection") = *pCamera->GetProjection();
		m_ShaderCreateGBufferSolid->SPARAMETR("g_cameraPositionFar") = cameraPositionFar;

		for (EUINT32 i = 0; i < m_renderTree.GetDeckBack()->GetUniqueObjectsCount(); ++i)
		{
			// id=0 is invalid - we must skip it
			if (m_renderTree.GetDeckBack()->GetForward(i) == 0)
				continue;


			//printf("%u ,", m_renderTree.m_deck.GetForward(i));

			EModelPart* modelek = Component::GetComponentById<EModelPart>(m_renderTree.GetDeckBack()->GetForward(i));


			pDevice->SetVertexBuffer(modelek->GetVertexBuffer());
			pDevice->SetIndexBuffer(modelek->GetIndexBuffer());


			m_ShaderCreateGBufferSolid->GetResource(0)->Set<Texture2D>(modelek->GetDiffuseMap());
			m_ShaderCreateGBufferSolid->GetResource(1)->Set<Texture2D>(modelek->GetNormalMap());
			m_ShaderCreateGBufferSolid->GetResource(2)->Set<Texture2D>(modelek->GetTGEMap());
			m_ShaderCreateGBufferSolid->GetResource(3)->Set<Texture2D>(modelek->GetHeightMap());
			pDevice->SetShaderResources();


			EUINT32 id = modelek->GetId();

			objectProperties.X = (EFLOAT32)id;
			m_ShaderCreateGBufferSolid->SPARAMETR("g_objectProperties") = objectProperties;

			m_ShaderCreateGBufferSolid->SPARAMETR("g_world") = *modelek->GetWorldMatrix();

			pDevice->SetShaderVariables();
			pDevice->RenderRAW();


			xxxxdrawcalls++;
			xxxxdrawcallsB++;
		}
	}


	pDevice->RasterizerSetMode(PF_GI_RS_CULL_NONE);

	cam = pCamera;

	//printf("\nDraw Solid Front End = %u", xxxxdrawcalls);
	PROFILER_END("Draw Solid Front");
}






//	-----------------------------------------------------------------------------------------------------------------------------
//
//
//
void ParabellumEngine::Core::Graphics::DeferredRenderer::DrawSolidBack(
	_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera)
{
	/*
	//
	// Back
	//
	pDevice->RasterizerSetMode(PF_GI_RS_CULL_COUNTERCLOCKWISE);
	pDevice->StencilSetState(PF_GI_STENCIL_GREATER);


	pDevice->SetRenderTargets(m_RTGBufferSolidBack0, m_RTGBufferSolidBack1, m_RTGBufferSolidBack2, m_RTGBufferSolidBack3, m_RTGBufferSolidBack0, true);


	// set shader
	pDevice->SetShader(m_ShaderCreateGBufferSolid);




	//pDevice->TurnAlphaBlending(true);

	//for (EUINT32 i = 0; i < m_solidParts.size(); ++i)
	for (EUINT32 i = 0; i < m_solidObjects->GetPickedUniqueObjectsCount(); ++i)
	{

		for (EUINT32 n = 0; n < m_solidObjects->GetObjectsCountOfIndex(i); ++n)
		{
			pDevice->SetShader(m_ShaderCreateGBufferSolid);


			pDevice->SetVertexBuffer(m_solidObjects->m_objects[i]->GetVertexBuffer());
			pDevice->SetIndexBuffer(m_solidObjects->m_objects[i]->GetIndexBuffer());

			Matrix4x4 worldViewIT;

			//world = *objects->m_objects[i]->GetWorldMatrix();

			Matrix4x4::Transponse(Matrix4x4::Invert(*m_solidObjects->GetObjectOfIndex(i, n)->GetWorldMatrix() * (*pCamera->GetView())), worldViewIT);

			Vector4D cameraPositionFar, materialProperties;

			pCamera->GetPosition(cameraPositionFar.X, cameraPositionFar.Y, cameraPositionFar.Z);
			cameraPositionFar.W = pCamera->GetZFar();

			m_ShaderCreateGBufferSolid->GetParametr(0)->Set(pCamera->GetView());
			m_ShaderCreateGBufferSolid->GetParametr(1)->Set(pCamera->GetProjection());
			m_ShaderCreateGBufferSolid->GetParametr(2)->Set(m_solidObjects->GetObjectOfIndex(i, n)->GetWorldMatrix());
			m_ShaderCreateGBufferSolid->GetParametr(3)->Set(&worldViewIT);
			m_ShaderCreateGBufferSolid->GetParametr(4)->Set(&cameraPositionFar);
			m_ShaderCreateGBufferSolid->GetParametr(5)->Set(m_ExDRLight.m_directionalLight.GetDirection());

			m_ShaderCreateGBufferSolid->GetResource(0)->Set<Texture2D>(m_solidObjects->m_objects[i]->GetDiffuseMap());
			m_ShaderCreateGBufferSolid->GetResource(1)->Set<Texture2D>(m_solidObjects->m_objects[i]->GetHeightMap());
			m_ShaderCreateGBufferSolid->GetResource(2)->Set<Texture2D>(m_solidObjects->m_objects[i]->GetHeightMap());

			pDevice->Render();
		}
	}


	pDevice->RasterizerSetMode(PF_GI_RS_CULL_NONE);
	pDevice->StencilSetState(PF_GI_STENCIL_LESS);

	cam = pCamera;

	*/


	//////////////////////////////////	

}



//	-----------------------------------------------------------------------------------------------------------------------------
void 
ParabellumEngine::Core::Graphics::DeferredRenderer::DrawTerrainCells(
	_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera)
{

	// set shader
	pDevice->SetShader(m_ShaderCreateGBufferSolid);

	//pDevice->RasterizerSetMode(PF_GI_CULL_NONE | PF_GI_MODE_WIREFRAME);

	for (EUINT32 i = 0; i < 2; ++i)
	{
		pDevice->SetBuffers(m_terrainz->m_terrainCells[i].GetVertexBuffer(), m_terrainz->m_terrainCells[i].GetIndexBuffer());
		pDevice->SetShader(m_ShaderDrawTerrain);


		Matrix4x4 world, worldViewIT;
		world.MakeIdentity();

		world = *m_terrainz->m_terrainCells[i].GetWorldMatrix();

		Matrix4x4::Transponse(Matrix4x4::Invert((*pCamera->GetView())) * world, worldViewIT);

		Vector4D cameraPositionFar, materialProperties;

		pCamera->GetPosition(cameraPositionFar.X, cameraPositionFar.Y, cameraPositionFar.Z);
		cameraPositionFar.W = pCamera->GetZFar();

		m_ShaderDrawTerrain->GetParametr(0)->Set(pCamera->GetView());
		m_ShaderDrawTerrain->GetParametr(1)->Set(pCamera->GetProjection());
		m_ShaderDrawTerrain->GetParametr(2)->Set(&world);
		m_ShaderDrawTerrain->GetParametr(3)->Set(&worldViewIT);
		m_ShaderDrawTerrain->GetParametr(4)->Set(&cameraPositionFar);
		m_ShaderDrawTerrain->GetParametr(5)->Set(m_ExDRLight.m_directionalLight.GetDirection());

		m_ShaderDrawTerrain->GetResource(0)->Set<Texture2D>(m_terrainCells[0]->m_T2Dtex);

		//pResourceManager->Load<Texture2D>("Resources/Terrain/heightmap_01/map_x0y0.pft")

		pDevice->Render(PF_GI_RENDERING_UPDATEVARIABLES | PF_GI_RENDERING_UPDATERESOURCES);
	}



}
//	-----------------------------------------------------------------------------------------------------------------------------
void ParabellumEngine::Core::Graphics::DeferredRenderer::DrawExtensions(
	_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera, _IN_ Utiles::FrustumOutput* pFrustumOutput)
{
	// draw extensions
	PROFILER_BEGIN("Deferred Extensions");


	// Draw Parallax Mapping first because parallaxed GBuffer must be used in others shaders
	PROFILER_BEGIN("Parallax Mapping");
	m_ExDRParallaxMapping.Draw(pDevice, pCamera, &m_fullScreenQuad, m_RTGBufferSolid0, m_RTGBufferSolid1, m_RTGBufferSolid2);
	//m_ExDRParallaxMapping.Draw(pDevice, pCamera, &m_renderArea, m_RTGBufferSolid0, m_RTGBufferSolid1, m_RTGBufferSolid3);
	//m_ExDRParallaxMapping.Draw(pDevice, pCamera, &m_renderTree, m_RTGBufferSolid0, m_RTGBufferSolid1, m_RTGBufferSolid2);
	PROFILER_END("Parallax Mapping");

	PROFILER_BEGIN("Shadows");
	m_ExDRLight.DrawDirectionalLightSolidShadow(pDevice, pCamera, pFrustumOutput);
	PROFILER_END("Shadows");

	PROFILER_BEGIN("Deferred Lighting");
	//m_ExDRLight.DrawSolidFull(pDevice, pCamera, &m_renderArea, m_RTGBufferSolid0, m_RTGBufferSolid1, m_RTGBufferSolid2, m_RTGBufferSolidBack0);
	//m_ExDRLight.DrawSolidFull(pDevice, pCamera, &m_renderTree, m_RTGBufferSolid0, m_RTGBufferSolid1, m_RTGBufferSolid2, m_RTGBufferSolidBack0);
	//m_ExDRLight.Draw(pDevice, pCamera, &m_fullScreenQuad, m_RTGBufferSolid0, m_RTGBufferSolid1, m_RTGBufferSolid2);

	//m_ExDRLight.Draw(pDevice, pCamera, &m_fullScreenQuad, m_ExDRParallaxMapping.m_RTBuffer0, m_ExDRParallaxMapping.m_RTBuffer1, m_RTGBufferSolid2);
	m_ExDRLight.Draw(pDevice, pCamera, &m_fullScreenQuad, m_RTGBufferSolid0, m_RTGBufferSolid1, m_RTGBufferSolid2);
	PROFILER_END("Deferred Lighting");

	PROFILER_BEGIN("GI");
	//m_ExDRLight.DrawGI(pDevice, pCamera, &m_fullScreenQuad, m_RTGBufferSolid0, m_RTGBufferSolid1, m_RTGBufferSolid2);
	PROFILER_END("GI");



	//PointLight xxx[5*5];
	//
	//for (int ix = 0; ix < 5; ix++)
	//	for (int iy = 0; iy < 5; iy++)
	//	{
	//		xxx[ix + iy * 5].SetPosition(ix*5.0f, 3.0f, iy*5.0f);
	//		xxx[ix + iy * 5].SetScale(5.0f, 5.0f, 5.0f);
	//
	//		xxx[ix + iy * 5].SetColorRGBA(0, 0, 255, 255);
	//
	//		if(ix % 2 == 0)
	//			xxx[ix + iy * 5].SetColorRGBA(255, 0, 0, 255);
	//
	//		if (iy % 2 == 0)
	//			xxx[ix + iy * 5].SetColorRGBA(0, 255, 0, 255);
	//	}


	//xxx[0].SetPosition(20.0f, 10.0f, 20.0f);
	//xxx[0].SetScale(25.0f, 25.0f, 25.0f);
	//xxx[0].SetColorRGBA(255, 0, 0, 255);
	//
	//xxx[1].SetPosition(35.0f, 10.0f, 35.0f);
	//xxx[1].SetScale(25.0f, 25.0f, 25.0f);
	//xxx[1].SetColorRGBA(0, 0, 255, 255);


	PROFILER_BEGIN("Deferred PointLights");
 	//m_ExDRLight.DrawPointLights(pDevice, pCamera, &m_fullScreenQuad, m_ExDRParallaxMapping.m_RTBuffer0, m_ExDRParallaxMapping.m_RTBuffer1, m_RTGBufferSolid2, xxx, 5*5);
	PROFILER_END("Deferred PointLights");

	PROFILER_BEGIN("SSAO");
	if (m_isFeatureParallaxMaping == true)
	{
		//m_ExDRSSAO.Draw(pDevice, pCamera, &m_fullScreenQuad, m_ExDRParallaxMapping.m_RTBuffer0, m_ExDRParallaxMapping.m_RTBuffer1, m_RTGBufferSolid2);
	}
	else
	{
		//m_ExDRSSAO.Draw(pDevice, pCamera, &m_fullScreenQuad, m_RTGBufferSolid0, m_ExDRParallaxMapping.m_RTBuffer1, m_RTGBufferSolid2);
	}
	PROFILER_END("SSAO"); 
	

	PROFILER_BEGIN("Volumetric Light");
	m_ExDRVolumetricLight.Draw(pDevice, pCamera, &m_ExDRLight.m_directionalLight, &m_fullScreenQuad, &m_renderTree, m_RTGBufferSolid0, m_ExDRLight.m_ARTShadowDepth[2]);
	PROFILER_END("Volumetric Light");
	

	PROFILER_BEGIN("HDR");
	if (m_isFeatureParallaxMaping == true)
	{
		//m_ExDRHDR.Draw(pDevice, pCamera, &m_fullScreenQuad, m_ExDRParallaxMapping.m_RTBuffer0, m_RTMergeGBufferLightMap,
		//	m_renderTree.Properties.averageLuminance, m_renderTree.Properties.luminanceMin, m_renderTree.Properties.luminanceMax);
	}
	else
	{
		//m_ExDRHDR.Draw(pDevice, pCamera, &m_fullScreenQuad, m_RTGBufferSolid0, m_RTMergeGBufferLightMap,
		//	m_renderTree.Properties.averageLuminance, m_renderTree.Properties.luminanceMin, m_renderTree.Properties.luminanceMax);
	}
	PROFILER_END("HDR");


	PROFILER_END("Deferred Extensions");


}



//	-----------------------------------------------------------------------------------------------------------------------------
void
ParabellumEngine::Core::Graphics::DeferredRenderer::MergeSolidNParticles(
	_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera)
{
	Vector4D settings;
	settings.X = 0.0f;
	settings.Y = 0.0f;
	settings.Z = 0.0f;
	settings.W = 0.0f;

	pDevice->SetRenderTargets(m_RTGBufferMergeBasics, m_RTGBufferMergeBasics, true);

	pDevice->SetShader(m_SMergeSolidNParticles);

	pDevice->SetBuffers(m_fullScreenQuad.GetVertexBuffer(), m_fullScreenQuad.GetIndexBuffer());

	(*m_SMergeSolidNParticles)(C$("g_settings"))->Set(&settings);

	m_SMergeSolidNParticles->GetResource(0)->Set(m_RTGBufferSolid0);
	m_SMergeSolidNParticles->GetResource(1)->Set(m_RTGBufferSolid1);
	m_SMergeSolidNParticles->GetResource(2)->Set(m_RTGBufferParticlesFront0);
	m_SMergeSolidNParticles->GetResource(3)->Set(m_RTGBufferParticlesFront1);
	m_SMergeSolidNParticles->GetResource(4)->Set(m_RTGBufferParticlesBack0);

	pDevice->Render(PF_GI_RENDERING_UPDATEVARIABLES | PF_GI_RENDERING_UPDATERESOURCES);
}

//	-----------------------------------------------------------------------------------------------------------------------------
void ParabellumEngine::Core::Graphics::DeferredRenderer::MergeGBufferSolidWithLightMap(
	_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera)
{

	Vector4D properties(0.0f, 0.0f, 0.0f, 0.0f);


	pDevice->SetRenderTargets(m_RTMergeGBufferLightMap, m_RTMergeGBufferLightMap, true);
	pDevice->SetBuffers(m_fullScreenQuad.GetVertexBuffer(), m_fullScreenQuad.GetIndexBuffer());

	pDevice->SetShader(m_SMergeGBufferSolidLightMap);

	m_SMergeGBufferSolidLightMap->GetParametr(0)->Set(&properties);
	
	if (m_isFeatureParallaxMaping == true)
		m_SMergeGBufferSolidLightMap->GetResource(0)->Set(m_ExDRParallaxMapping.m_RTBuffer0);
	else
		m_SMergeGBufferSolidLightMap->GetResource(0)->Set(m_RTGBufferSolid0);

	m_SMergeGBufferSolidLightMap->GetResource(1)->Set(m_ExDRLight.m_RTLightMapSolid);
	m_SMergeGBufferSolidLightMap->GetResource(2)->Set(m_ExDRSSAO.m_RTSSAO);
	m_SMergeGBufferSolidLightMap->GetResource(3)->Set(m_ExDRVolumetricLight.m_RTVolumetricLight);

	// set anisotropic filtering for better quality
	pDevice->SetSampler(0, SamplerType::POINT);
	pDevice->SetSampler(1, SamplerType::ANISOTROPIC);
	pDevice->SetSampler(2, SamplerType::ANISOTROPIC);
	pDevice->SetSampler(3, SamplerType::ANISOTROPIC);

	pDevice->SetShaderVariables();
	pDevice->SetShaderResources();
	pDevice->RenderRAW();
}


//	-----------------------------------------------------------------------------------------------------------------------------
void
ParabellumEngine::Core::Graphics::DeferredRenderer::GBufferSolidGenerateMipMaps(_IN_ GraphicsDevice* pDevice)
{
	//PROFILER_BEGIN("Solid Generate MipMaps");

	// if all slicess processed - clear it
	

	PropertiesRenderTree properties[32 * 32]; // the screen and RenderTree wil be divided onto 32x32 parts

	Vector4D settings(0.0f, 0.0f, 0.0f, 0.0f);
	Vector4D rtProperties(0.0f, 0.0f, 0.0f, 0.0f);

	//
	// Generate mip map from m_RTGBufferSolid0
	rtProperties.X = m_RTGBufferSolid1->GetWidth() * 0.5f;
	rtProperties.Y = m_RTGBufferSolid1->GetHeight() * 0.5f;
	rtProperties.Z = 1.0f / m_RTGBufferSolidMipMaps[0]->GetWidth()  * 0.5f;
	rtProperties.W = 1.0f / m_RTGBufferSolidMipMaps[0]->GetHeight()  * 0.5f;

	pDevice->SetRenderTargets(m_RTGBufferSolidMipMaps[0], m_RTGBufferSolidMipMaps[0], true);

	pDevice->SetShader(m_SShrinkMaterialProperties, ShaderFlagSet::NO_SAMPLERS);

	pDevice->SetVertexBuffer(m_fullScreenQuad.GetVertexBuffer());
	pDevice->SetIndexBuffer(m_fullScreenQuad.GetIndexBuffer());

	m_SShrinkMaterialProperties->GetParametr(0)->Set(&rtProperties);
	m_SShrinkMaterialProperties->GetResource(0)->Set<RenderTarget>(m_RTGBufferSolid0);

	pDevice->SetSampler(0, SamplerType::POINT);

	pDevice->SetShaderResources();
	pDevice->SetShaderVariables();
	pDevice->RenderRAW();


	//
	// find pixels which are not covered by any model
	static Vector4D  blindData[512 * 512];

	static EUINT32 blindPrev = 1;
	static EUINT32 blindCurrent = 0;

	//for (EUINT32 indx = 0; indx < 512 * 512; ++indx)
	//	blindData[512 * 512].MakeZero();

	pDevice->GetResourceData512x512(m_RTGBufferSolidMipMaps[0], blindData);

	//pDevice->GetResourceData512x512(m_sliceNo * m_sliceSize, 0, 16, 512, m_RTGBufferSolidMipMaps[0], blindData);

	//pDevice->GetResourceData512x512(m_sliceSize * m_sliceNo, 0, 64, 512, m_RTGBufferSolidMipMaps[0], blindData);

	//pDevice->GetResourceData512x512(0, 0, 512, 512, m_RTGBufferSolidMipMaps[0], blindData);

	for (EUINT32 indx = 0; indx < 512 * 512; ++indx)
	{
		if((EUINT32)blindData[indx].X > 0)
			m_renderTree.AddToDeck( (EUINT32)blindData[indx].X );


		EUINT32 ix = indx % 512;
		EUINT32 iy = indx / 512;
		
		EUINT32 sx = ix / 16;
		EUINT32 sy = iy / 16;
		
		if (blindData[indx].W < properties[sx + sy * 32].minDepth)
		{
			properties[sx + sy * 32].minDepth = blindData[indx].W;
			properties[sx + sy * 32].maxDepth = blindData[indx].W;
		}
		
		// blind spot
		if (properties[sx + sy * 32].isBlindSpot == false)
			if ((EUINT32)blindData[indx].X == 0)
			{
				properties[sx + sy * 32].isBlindSpot = true;
			}
	}


	//printf("\nilosc obiektow = %u \n :   ", m_renderTree.m_deck.GetUniqueObjectsCount());
	//m_renderTree.m_deck.Print();

	//
	// setup render Tree
	for (EUINT32 i = 0; i < 32 * 32; ++i)
	{

		// get color
		Vector4D color(0.0f, 0.0f, 0.0f, 0.0f);

		//EUINT32 uColor = *((EUINT32*)&data[i].X);
		EUINT32 uColor = 0;

		color.X = (EFLOAT32)((uColor >> 24)) / 255.0f;
		color.Y = (EFLOAT32)((uColor >> 16) & 0x000000FF) / 255.0f;
		color.Z = (EFLOAT32)((uColor >> 8) & 0x000000FF) / 255.0f;
		color.W = (EFLOAT32)(uColor & 0x000000FF) / 255.0f;

		properties[i].averageColor = color;
		properties[i].averageLuminance = color.X * 0.333f + color.Y * 0.333f + color.Z * 0.333f;

		properties[i].luminanceMin = properties[i].averageLuminance;
		properties[i].luminanceMax = properties[i].averageLuminance;


		// light
		properties[i].flags |= PE_RENDER_SETTINGS_DIRECTIONALLIGHT;
		properties[i].flags |= PE_RENDER_SETTINGS_SHADOWMAPPING;
		properties[i].flags |= PE_RENDER_SETTINGS_SSAO;
		
		//if ( data[i].W < 0.2f )
		//{
		//	properties[i].flags |= PE_RENDER_SETTINGS_DIRECTIONALLIGHT;
		//	properties[i].flags |= PE_RENDER_SETTINGS_SHADOWMAPPING;
		//	properties[i].flags |= PE_RENDER_SETTINGS_SSAO;
		//
		//	//m_renderArea.Properties[i].isLight = true;
		//}	
		//else if(data[i].W < 0.9f)
		//{
		//	//properties[i].flags |= PE_RENDER_SETTINGS_DIRECTIONALLIGHTBASIC;
		//	properties[i].flags |= PE_RENDER_SETTINGS_DIRECTIONALLIGHT;
		//	//m_renderArea.Properties[i].isLight = false;
		//}

		// parallax
		properties[i].flags |= PE_RENDER_SETTINGS_PARALLAXMAPPING;

		// volumetric
		properties[i].flags |= PE_RENDER_SETTINGS_VOLUMETRICLIGHT;


	}


	// setup RenderTree
	m_renderTree.Clear();
	m_renderTree.Setup(properties, 32, 32);


	//PROFILER_END("Solid Generate MipMaps");
}


//	-----------------------------------------------------------------------------------------------------------------------------
void 
ParabellumEngine::Core::Graphics::DeferredRenderer::DisplayBuffer(_IN_ GraphicsDevice* pDevice, int no)
{
	Vector4D settings(0.9f, 0.6f, 0.0f, 0.0f);
	pDevice->SetDefaultRenderTarget();
	pDevice->SetShader(m_ShaderDisplayQuad);
	pDevice->SetBuffers(m_fullScreenQuad.GetVertexBuffer(), m_fullScreenQuad.GetIndexBuffer());
	
	m_ShaderDisplayQuad->SPARAMETR("g_settings") = settings;

	switch (no)
	{

	case  0: m_ShaderDisplayQuad->GetResource(0)->Set(m_RTGBufferSolid0); break;
	case  1: m_ShaderDisplayQuad->GetResource(0)->Set(m_RTGBufferSolid1); break;
	case  2: m_ShaderDisplayQuad->GetResource(0)->Set(m_RTGBufferSolid2); break;
	case  3: m_ShaderDisplayQuad->GetResource(0)->Set(m_ExDRLight.m_RTLightMapSolid); break;
	case  4: m_ShaderDisplayQuad->GetResource(0)->Set(m_ExDRLight.m_RTPointLightSolid); break;
	case  5: m_ShaderDisplayQuad->GetResource(0)->Set(m_ExDRSSAO.m_RTSSAO); break;
	case  6: m_ShaderDisplayQuad->GetResource(0)->Set(m_ExDRSSAO.m_RTSSAOBlured); break;
	case  7: m_ShaderDisplayQuad->GetResource(0)->Set(m_ExDRHDR.m_RTHDR); break;
	case  8: m_ShaderDisplayQuad->GetResource(0)->Set(m_ExDRHDR.m_RTHDR); break;
	case  9: m_ShaderDisplayQuad->GetResource(0)->Set(m_ExDRVolumetricLight.m_RTVolumetricLight); break;
	case 10: m_ShaderDisplayQuad->GetResource(0)->Set(m_ExDRVolumetricLight.m_RTVolumetricLightPolished); break;
	case 11: m_ShaderDisplayQuad->GetResource(0)->Set(m_ExDRParallaxMapping.m_RTBuffer0); break;
	case 12: m_ShaderDisplayQuad->GetResource(0)->Set(m_ExDRParallaxMapping.m_RTBuffer1); break;
	case 14: m_ShaderDisplayQuad->GetResource(0)->Set(m_RTMergeGBufferLightMap); break;
	case 15: m_ShaderDisplayQuad->GetResource(0)->Set(m_RTMergeGBufferWithEffects); break;
	case 16: m_ShaderDisplayQuad->GetResource(0)->Set(m_RTGBufferProperties); break;
	case 17: m_ShaderDisplayQuad->GetResource(0)->Set(m_RTGBufferPropertiesHisogram); break;
	case 18: m_ShaderDisplayQuad->GetResource(0)->Set(m_RTGBufferMergeBasics); break;

	case 19: m_ShaderDisplayQuad->GetResource(0)->Set(m_ExDRLight.m_ARTShadowDepth[0]); break;
	case 20: m_ShaderDisplayQuad->GetResource(0)->Set(m_ExDRLight.m_ARTShadowDepth[1]); break;
	case 21: m_ShaderDisplayQuad->GetResource(0)->Set(m_ExDRLight.m_ARTShadowDepth[2]); break;
	case 22: m_ShaderDisplayQuad->GetResource(0)->Set(m_ExDRLight.m_RTShadowMap); break;



	default: m_ShaderDisplayQuad->GetResource(0)->Set(m_RTGBufferSolid0); break;
	}

	pDevice->Render(PF_GI_RENDERING_UPDATEVARIABLES | PF_GI_RENDERING_UPDATERESOURCES);
}





void 
ParabellumEngine::Core::Graphics::DeferredRenderer::DisplayBuffer(_IN_ GraphicsDevice* pDevice, _IN_ RenderTarget* pRT)
{
	Vector4D settings(0.9f, 0.6f, 0.0f, 0.0f);
	pDevice->SetDefaultRenderTarget();
	pDevice->SetShader(m_ShaderDisplayQuad);
	pDevice->SetBuffers(m_fullScreenQuad.GetVertexBuffer(), m_fullScreenQuad.GetIndexBuffer());

	m_ShaderDisplayQuad->SPARAMETR("g_settings") = settings;

	m_ShaderDisplayQuad->GetResource(0)->Set(pRT);


	pDevice->Render(PF_GI_RENDERING_UPDATEVARIABLES | PF_GI_RENDERING_UPDATERESOURCES);
}