#ifndef _DEMO01_H_
#define _DEMO01_H_



//
// STDs
//
#include <iostream>
#include <thread>
#include <mutex>

std::mutex mu;

//
// Framework
//
#include "../ParabellumFramework/TypesC.h"
#include "../ParabellumFramework/DebugManager.h"
#include "../ParabellumFramework/Profiler.h"
#include "../ParabellumFramework/Shader.h"
#include "../ParabellumFramework/ShaderConverter.h"
#include "../ParabellumFramework/Texture2D.h"
#include "../ParabellumFramework/RenderTarget.h"
#include "../ParabellumFramework/Plane.h"
#include "../ParabellumFramework/BoundingVolumes.h"
#include "../ParabellumFramework/BoundingFrustum.h"
#include "../ParabellumFramework/Model.h"
#include "../ParabellumFramework/ModelFormat.h"
#include "../ParabellumFramework/ParabellumExtensions.h"
#include "../ParabellumFramework/Volume.h"
#include "../ParabellumFramework/Event.h"
#include "../ParabellumFramework/TaskManager.h"
//
// Engine
//
#include "../ParabellumEngine/SystemInstance.h"
#include "../ParabellumEngine/RenderQuad.h"
#include "../ParabellumEngine/Camera.h"
#include "../ParabellumEngine/DeferredRenderer.h"
#include "../ParabellumEngine/EModel.h"
#include "../ParabellumEngine/MapSector.h"
#include "../ParabellumEngine/PrimitivesFactory.h"
#include "../ParabellumEngine/FrustumOutput.h"

// editor
#include "../ParabellumEngine/SceneEditor.h"

// TEST
#include "../ParabellumEngine/SkyLike.h"
#include "../ParabellumEngine/Cloud.h"

//
// GUI
//
#include "../ParabellumGUI/PGUIFont.h"
#include "../ParabellumGUI/PGUIManager.h"


//
// 3rd party
//
#include "../ParabellumFramework/TextureConverter.h"
#include "../ParabellumFramework/ModelConverter.h"

using namespace ParabellumFramework;
using namespace ParabellumFramework::Graphics;
using namespace ParabellumEngine::System;
using namespace ParabellumEngine::Components;
using namespace ParabellumEngine::Core;
using namespace ParabellumEngine::Core::Graphics;
using namespace ParabellumEngine::Core::Scene;
using namespace ParabellumEngine;
using namespace ParabellumEngine::Editor;
using namespace ParabellumUserInterface;



class Demo01 : public ParabellumEngine::System::SystemInstance
{
	Texture2D* t2D;
	Texture2D* t2DHeight;

	Model* models[25];

	EModel* emodel;
	EModel* emodel2;
	EModel* emodel3;

	EModel* emodelq[2500];

	Camera camera;
	bool isCameraFree = true;

	DeferredRenderer renderer;


	std::shared_ptr<MapSector> m_mapSector;

	FrustumOutput* m_frustumManager;


	EUINT32 buff2Disp;

	Shader* SDrawOctree;


	SceneEditor* m_sceneEditor;





	SkyLike* m_skyLike;
	Cloud* m_cloud;


	//2560, 1440
	const int swidth = 2560;
	const int sheight = 1440;
	

	void Initialize()
	{

		buff2Disp = 0;


		Vector3D vmin; vmin.Set(0.0f, 0.0f, 0.0f);
		Vector3D vmax; vmax.Set(1000.0f, 1000.0f, 1000.0f);

		m_mapSector = std::shared_ptr<MapSector>(new MapSector(vmin, vmax));


		//////////////////////////////////////////////////////////
		// model converter
		ModelConverter mcv;
		//mcv.ConvertOBJ2PFM("Resources/Models/bananatree1.obj", "Resources/Models/bananatree1.pfm");
		//mcv.ConvertOBJ2PFM("Resources/Models/box_02.obj", "Resources/Models/box_02.pfm");
		//mcv.ConvertOBJ2PFM("Resources/Models/box_03.obj", "Resources/Models/box_03.pfm");
		//mcv.ConvertOBJ2PFM("Resources/Models/chair_01.obj", "Resources/Models/chair_01.pfm");
		//mcv.ConvertOBJ2PFM("Resources/Models/Skydome/Skydome_01.obj", "Resources/Models/Skydome/Skydome_01.pfm");
		mcv.ConvertOBJ2PFM("Resources/Models/oldhouse.obj", "Resources/Models/oldhouse.pfm");
		//mcv.ConvertOBJ2PFM("Resources/Models/Sphere_128x128.obj", "Resources/Models/Sphere_128x128.pfm");
		//mcv.ConvertOBJ2PFM("Resources/Models/Tropical/obj/Cocos_palm1.obj", "Resources/Models/Tropical/obj/Cocos_palm1.pfm");
		//mcv.ConvertOBJ2PFM("Resources/Models/Car_01.obj", "Resources/Models/Car_01.pfm");
		//mcv.ConvertOBJ2PFM("Resources/Editor/arrowZ.obj", "Resources/Editor/arrowZ.pfm");


		//////////////////////////////////////////////////////////
		// Convert texture
		TextureConverter tcv;
		//tcv.convertBMP2PFT("Resources/Textures/pony.bmp", "Resources/Textures/pony.pft");
		//tcv.convertBMP2PFT("Resources/Textures/noimage.bmp", "Resources/Textures/noimage.pft");
		//tcv.convertBMP2PFT("Resources/Textures/0_default_height32b.bmp", "Resources/Textures/0_default_height32b.pft");
		//tcv.convertBMP2PFT("Resources/Textures/0_default_normal32b.bmp", "Resources/Textures/0_default_normal32b.pft");
		//tcv.convertBMP2PFT("Resources/Textures/test_diffuse_Green.bmp", "Resources/Textures/test_diffuse_Green.pft");
		//tcv.convertBMP2PFT("Resources/Textures/test_light_x.bmp", "Resources/Textures/test_light_x.pft");
		//tcv.convertBMP2PFT("Resources/Textures/test1.bmp", "Resources/Textures/test1.pft");
		//tcv.convertBMP2PFT("Resources/Textures/font_ASCII.bmp", "Resources/Textures/font_ASCII.pft");
		//tcv.convertBMP2PFT("Resources/GUI/GUI_01.bmp", "Resources/GUI/GUI_01.pft");
		//tcv.convertBMP2PFT("Resources/Textures/brick_height.bmp", "Resources/Textures/brick_height.pft");
		//tcv.convertBMP2PFT("Resources/Textures/brick_diffuse.bmp", "Resources/Textures/brick_diffuse.pft");
		//tcv.convertBMP2PFT("Resources/Textures/leaf_x.bmp", "Resources/Textures/leaf_x.pft");
		//tcv.convertBMP2PFT("Resources/Textures/leath05.bmp", "Resources/Textures/leath05.pft");
		//tcv.convertBMP2PFT("Resources/Textures/leath05_normal.bmp", "Resources/Textures/leath05_normal.pft");
		//tcv.convertBMP2PFT("Resources/Textures/wall_dmp.bmp", "Resources/Textures/wall_dmp.pft");
		//tcv.convertBMP2PFT("Resources/Textures/wall_nmp.bmp", "Resources/Textures/wall_nmp.pft");
		//tcv.convertBMP2PFT("Resources/Textures/wall_hmp.bmp", "Resources/Textures/wall_hmp.pft");
		//tcv.convertBMP2PFT("Resources/Textures/wood_tex.bmp", "Resources/Textures/wood_tex.pft");
		//tcv.convertBMP2PFT("Resources/Textures/cloud_sphere.bmp", "Resources/Textures/cloud_sphere.pft");
		//tcv.convertBMP2PFT("Resources/Textures/cloud_test.bmp", "Resources/Textures/cloud_test.pft");
		//tcv.convertBMP2PFT("Resources/Fonts/Calibri14.bmp", "Resources/Fonts/Calibri14.pft");
		//tcv.convertBMP2PFT("Resources/Fonts/Calibri14_smooth.bmp", "Resources/Fonts/Calibri14_smooth.pft");
		//tcv.convertFourBMP2PFT("Resources/Textures/comp0.bmp", "Resources/Textures/comp1.bmp", "Resources/Textures/comp2.bmp", "Resources/Textures/comp3.bmp", "Resources/Textures/comp.pft");
		tcv.convertBMP2PFT("Resources/Textures/comp0.bmp", "Resources/Textures/comp0.pft");

		//tcv.convertBMP2PFT("Resources/Terrain/heightmap_01/map_x0y0.bmp", "Resources/Terrain/heightmap_01/map_x0y0.pft");
		//////////////////////////////////////////////////////////


		Vector3D vZero, vUniX, vUp;
		vZero = Vector3D::Zero();
		vUniX = Vector3D::UnitZ();
		vUp = Vector3D::Up();
		camera.Initialize(vZero, vUniX, vUp, swidth, sheight, ParabellumUtils::Math::PIO4, 1.0f, 1000.0f);
		//camera.Initialize(vZero, vUniX, vUp, 1280, 720, Math::PIO4(), 1.0f, 1000.0f);


		// set camera position
		camera.SetPosition(90.0f, 2.2f, 76.0f);
		camera.Update();
		//m_isCameraRotated = true;

		ShaderConverter sc;
		//sc.Convert("Resources/Shaders/Blur_Gaussian_Vertical.hlsl", "Resources/Shaders/Blur_Gaussian_Vertical.psx");
		//sc.Convert("Resources/Shaders/CreateDepthBuffer.hlsl", "Resources/Shaders/CreateDepthBuffer.psx");
		//sc.Convert("Resources/Shaders/DisplayQuad.hlsl", "Resources/Shaders/DisplayQuad.psx");	
		//sc.Convert("Resources/Shaders/DR_CreateGBufferParticles.hlsl", "Resources/Shaders/DR_CreateGBufferParticles.psx");
		sc.Convert("Resources/Shaders/DR_CreateGBufferSolid_Default.hlsl", "Resources/Shaders/DR_CreateGBufferSolid_Default.psx");	
		//sc.Convert("Resources/Shaders/DR_CreateGBufferSolid_Far.hlsl", "Resources/Shaders/DR_CreateGBufferSolid_Far.psx");
		//sc.Convert("Resources/Shaders/DR_CreateGBufferSolid_GPUCalc.hlsl", "Resources/Shaders/DR_CreateGBufferSolid_GPUCalc.psx");
		//sc.Convert("Resources/Shaders/DR_CreateGBufferSolid_GPUCalcI8.hlsl", "Resources/Shaders/DR_CreateGBufferSolid_GPUCalcI8.psx");
		//sc.Convert("Resources/Shaders/DR_CreateGBufferSolid_Instanced2.hlsl", "Resources/Shaders/DR_CreateGBufferSolid_Instanced2.psx");
		//sc.Convert("Resources/Shaders/DR_CreateGBufferSolid_Instanced8.hlsl", "Resources/Shaders/DR_CreateGBufferSolid_Instanced8.psx");
		//sc.Convert("Resources/Shaders/DR_CreateGBufferGrass.hlsl", "Resources/Shaders/DR_CreateGBufferGrass.psx");
		//sc.Convert("Resources/Shaders/DR_CreateGBufferTerrain.hlsl", "Resources/Shaders/DR_CreateGBufferTerrain.psx");
		sc.Convert("Resources/Shaders/DR_DirectionalLight.hlsl", "Resources/Shaders/DR_DirectionalLight.psx");
		sc.Convert("Resources/Shaders/DR_DirectionalLightBasic.hlsl", "Resources/Shaders/DR_DirectionalLightBasic.psx");
		sc.Convert("Resources/Shaders/DR_GI_ShadowMap_Solid.hlsl", "Resources/Shaders/DR_GI_ShadowMap_Solid.psx");
		sc.Convert("Resources/Shaders/DR_GI_ShadowMap_Transparent.hlsl", "Resources/Shaders/DR_GI_ShadowMap_Transparent.psx");
		//sc.Convert("Resources/Shaders/DR_HDR.hlsl", "Resources/Shaders/DR_HDR.psx");
		//sc.Convert("Resources/Shaders/DR_Light_BlurLightMap.hlsl", "Resources/Shaders/DR_Light_BlurLightMap.psx");
		//sc.Convert("Resources/Shaders/DR_MergeForTree.hlsl", "Resources/Shaders/DR_MergeForTree.psx");
		sc.Convert("Resources/Shaders/DR_MergeGBufferLight.hlsl", "Resources/Shaders/DR_MergeGBufferLight.psx");
		//sc.Convert("Resources/Shaders/DR_MergeGBufferSolidLR.hlsl", "Resources/Shaders/DR_MergeGBufferSolidLR.psx");
		//sc.Convert("Resources/Shaders/DR_MergeSolidNParticles.hlsl", "Resources/Shaders/DR_MergeSolidNParticles.psx");
		//sc.Convert("Resources/Shaders/DR_ParallaxMapping.hlsl", "Resources/Shaders/DR_ParallaxMapping.psx");
		//sc.Convert("Resources/Shaders/DrawOctreeBox.hlsl", "Resources/Shaders/DrawOctreeBox.psx");
		//sc.Convert("Resources/Shaders/DrawSimpleBox.hlsl", "Resources/Shaders/DrawSimpleBox.psx");
		//sc.Convert("Resources/Shaders/DR_ParticleDirectionalLight.hlsl", "Resources/Shaders/DR_ParticleDirectionalLight.psx");
		//sc.Convert("Resources/Shaders/DR_PointLight.hlsl", "Resources/Shaders/DR_PointLight.psx");
		//sc.Convert("Resources/Shaders/DR_SSAO.hlsl", "Resources/Shaders/DR_SSAO.psx");
		//sc.Convert("Resources/Shaders/DR_SSReflections.hlsl", "Resources/Shaders/DR_SSReflections.psx");
		sc.Convert("Resources/Shaders/DR_VolumetricLight.hlsl", "Resources/Shaders/DR_VolumetricLight.psx");
		sc.Convert("Resources/Shaders/DR_VolumetricLightPolish.hlsl", "Resources/Shaders/DR_VolumetricLightPolish.psx");
		//sc.Convert("Resources/Shaders/SetBuffer.hlsl", "Resources/Shaders/SetBuffer.psx");
		//sc.Convert("Resources/Shaders/Sky_DrawCloud.hlsl", "Resources/Shaders/Sky_DrawCloud.psx");
		//sc.Convert("Resources/Shaders/Sky_DrawCloudSphere.hlsl", "Resources/Shaders/Sky_DrawCloudSphere.psx");
		//sc.Convert("Resources/Shaders/SkyLike_DrawSkydome.hlsl", "Resources/Shaders/SkyLike_DrawSkydome.psx");
		//sc.Convert("Resources/Shaders/SkyLike_AddCloud.hlsl", "Resources/Shaders/SkyLike_AddCloud.psx");
		//sc.Convert("Resources/Shaders/SkyLike_GenerateSun.hlsl", "Resources/Shaders/SkyLike_GenerateSun.psx");	
		//sc.Convert("Resources/Shaders/SkyLike_Merge.hlsl", "Resources/Shaders/SkyLike_Merge.psx");
		//sc.Convert("Resources/Shaders/GUIShader.hlsl", "Resources/Shaders/GUIShader.psx");
		//sc.Convert("Resources/Shaders/ShaderCopyGBufferProperties.hlsl", "Resources/Shaders/ShaderCopyGBufferProperties.psx");
		//sc.Convert("Resources/Shaders/ShaderShrinkMaterial.hlsl", "Resources/Shaders/ShaderShrinkMaterial.psx");
		//sc.Convert("Resources/Shaders/ShaderSkydome.hlsl", "Resources/Shaders/ShaderSkydome.psx");
		//sc.Convert("Resources/Shaders/DR_DrawDebugBox.hlsl", "Resources/Shaders/DR_DrawDebugBox.psx");
	
		/////////////////////////////
		t2D = GetResourceManager()->Load<Texture2D>("Resources/Textures/leath05.pft");
		t2DHeight = GetResourceManager()->Load<Texture2D>("Resources/Textures/brick_height.pft");

		int mc = 5;

		
		//models[0] = GetResourceManager()->Load<Model>("Resources/Models/saloon.pem");
		//models[1] = GetResourceManager()->Load<Model>("Resources/Models/woodhouse.pem");
		//models[0] = GetResourceManager()->Load<Model>("Resources/Models/Sphere_128x128.pfm");
		models[0] = GetResourceManager()->Load<Model>("Resources/Models/oldhouse.pfm");
		//models[0] = GetResourceManager()->Load<Model>("Resources/Models/Sphere_16x8.pfm");
		models[1] = GetResourceManager()->Load<Model>("Resources/Models/box_02.pfm");
		//models[2] = GetResourceManager()->Load<Model>("Resources/Models/chair_01.pfm");
		models[3] = GetResourceManager()->Load<Model>("Resources/Models/Tropical/obj/Cocos_palm1.pfm");
		//models[3] = GetResourceManager()->Load<Model>("Resources/Models/bananatree1.pfm");
		//models[3] = GetResourceManager()->Load<Model>("Resources/Models/Car_01.pfm");

		
		renderer.Initialize(GetGraphicsDevice(), GetResourceManager(), swidth, sheight);
		//renderer.Initialize(GetGraphicsDevice(), GetResourceManager(), 1280, 720);
	


		emodel = new EModel(GetResourceManager(), models[0]);
		emodel->SetPosition(40.0f, 1.5f, 40.0f);
		emodel->SetScale(15.00f, 15.00f, 15.00f);


		emodel2 = new EModel(GetResourceManager(), models[3]);
		//emodel2->GetModel(0)->m_t2DA = t2DHeight;
		//emodel2->GetModel(1)->m_t2DA = t2DHeight;
		//emodel2->GetModel(0)->m_t2DC = t2DHeight;
		//emodel2->GetModel(1)->m_t2DC = t2DHeight;
		emodel2->SetPosition(60.0f, 1.0f, 60.0f);
		emodel2->SetRotation(0.0f, 0.0f, 0.0f);
		emodel2->SetScale(0.1f, 0.1f, 0.1f);
		emodel2->GetModel(0)->SetPosition(0.0f, 0.0f, 0.0f);
		emodel2->GetModel(0)->SetScale(1.0f, 1.0f, 1.0f);
		emodel2->GetModel(1)->SetPosition(0.0f, 0.0f, 0.0f);
		emodel2->GetModel(1)->SetScale(1.0f, 1.0f, 1.0f);

		emodel3 = new EModel(GetResourceManager(), models[1]);
		emodel3->SetPosition(85.0f, 1.1f, 85.0f);
		emodel3->SetRotation(0.0f, 0.0f, 0.0f);
		emodel3->SetRotation(0.0f, 0.0f, 0.0f);
		emodel3->SetScale(2.00f, 2.00f, 2.0f);
		emodel3->GetModel(0)->SetPosition(0.0f, 0.0f, 0.0f);
		emodel3->GetModel(0)->SetScale(1.0f, 1.0f, 1.0f);
		emodel3->GetModel(1)->SetPosition(0.0f, 0.0f, 0.0f);
		emodel3->GetModel(1)->SetScale(1.0f, 1.0f, 1.0f);

		int modelen = 0;

		int nz = 0;
		int nx = 0;
		int ny = 0;


		
		for ( nz = 0; nz < 14; ++nz)
		{
			for ( ny = 0; ny < 14; ++ny)
			{
				for ( nx = 0; nx < 14; ++nx)
				{
					//if ((nx == 0 || nx == 6) ||
					//	(ny == 0 || ny == 6) ||
					//	(nz == 0 || nz == 6) 
					//	)
					if (
						//(nx == 0 || nx == 6) ||
						//(ny == 0 || ny == 6) ||
						nz == 0 || ny == 0 || nx == 0
						)
					{

						if ((nz == 7) && (nx == 7))
							continue;
						//
						if ((nz == 7) && (ny == 7))
							continue;

						modelen++;

						emodelq[modelen] = new EModel(GetResourceManager(), models[1]);
						emodelq[modelen]->SetPosition(nx * 2.0f + 50, nz * 2.0f, ny * 2.0f + 50);
						emodelq[modelen]->SetScale(1.000f, 1.000f, 1.000f);

						if ((nz == 4) && (nx == 4))
						{
							emodelq[modelen]->SetScale(1.000f, 1.000f, 0.050f);
						}


						//emodelq[ny*nx]->SetRotation(-0.96f, 0.423f, 0.19f);

						//emodelq[ny*nx]->SetParent(emodel);

						//m_mapSector->AddEModel(emodelq[modelen]);
					}
				}
			}
		}
		

		/*
		//for (int nz = 0; nz < 15; ++nz)
		{
			for (ny = 0; ny < 9; ++ny)
			{
				for (nx = 0; nx < 9; ++nx)
				{
					emodelq[ny*nx] = new EModel(GetResourceManager(), models[1]);
					emodelq[ny*nx]->SetPosition(nx * 3.5f + 50, 1 * 3.5f, ny * 3.5f + 50);
					emodelq[ny*nx]->SetScale(1.000f, 1.000f, 1.000f);
					//emodelq[ny*nx]->SetRotation(-0.96f, 0.423f, 0.19f);

					//emodelq[ny*nx]->SetParent(emodel);

					m_mapSector->AddEModel(emodelq[ny*nx]);
				}
			}
		}
		*/


		
		//emodel2->SetParent(emodel);

		//renderer.m_solidObjects->Add(emodel);
		//renderer.m_solidObjects->Add(emodel2);
		
		m_mapSector->AddEModel(emodel);
		m_mapSector->AddEModel(emodel2);
		//m_mapSector->AddEModel(emodel3);

		//renderer.m_screenoGraph.Add(&camera, emodel);
		//renderer.m_solidObjects->Add(emodel2);
		//.m_screenoGraph.Add(&camera, emodel3);


		//////////////////////////////////////////////////////////////////
		SDrawOctree = GetResourceManager()->Load<Shader>("Resources/Shaders/DrawOctreeBox.psx");



		GetGraphicsDevice()->StencilSetState(PF_GI_STENCIL_LESS);




		m_frustumManager = new FrustumOutput(swidth, sheight);




		// ==================================================================================================
		// Editro
		m_sceneEditor = new SceneEditor(GetGraphicsDevice(), GetInputDevice(), GetResourceManager());
		

		


		// ==================================================================================================
		//TEST skydome
		m_skyLike = new SkyLike(GetGraphicsDevice(), GetResourceManager());

		m_skyLike->AddCloud(GetGraphicsDevice(), &camera, GetResourceManager()->Load<Texture2D>("Resources/Textures/cloud_test.pft"), Vector2D(0.1f, 0.4f), 0.3f);
		m_skyLike->CreateSkydomeTexture(GetGraphicsDevice(), &camera);


		m_cloud = new Cloud(GetGraphicsDevice(), GetResourceManager());

		QWE::YUI ooo = 0;


		e1a.Subscribe( GetInputDevice()->EventMouseLeftPressed, &Reciever1::Test1 );

	}

	


	class Reciever1 : public Event::EventHandler
	{
	public:
			void Test1(EFLOAT32 x, EFLOAT32 y) { printf("\nReceiver1::Test1 - pressed %f", x); }
			void Test2(EFLOAT32 x, EFLOAT32 y) { printf("\nReceiver1::Test2 - pressed %f", x); }
	};
	Reciever1 e1a;

	struct QWE
	{
		typedef int YUI;
	};




	void Release()
	{
	}

        

	void Draw()
	{		
		static float xs = 0.0f;

		static int xc = 0;

		xs += 0.0005f;

		xc++;

		static float b = 0.0001f;

		if (xc > 1000)
		{
			b = -b;
			xc = 0;
		}

		//pui_scrollBar_01->Test();

		//emodel->SetRotation(0.0f, xs, 0.0f);



		Vector3D rpos;
		rpos.Set(0.0f, 0.0f, 0.0f);
		rpos.X = b;

		//puido1->MoveChildren(rpos);

		// ****************************
		//	Frame calculation
		// ****************************

		Vector3D modelRot;

		//
		// Update camera
		//
		if (isCameraFree == true)
		{

			camera.GetRotation(modelRot);


			modelRot.X += GetInputDevice()->GetMouseDeltaY() * 0.3f;
			modelRot.Y += GetInputDevice()->GetMouseDeltaX() * 0.3f;
			modelRot.Z = 0.0f;

			camera.SetRotation(modelRot);


			if (GetInputDevice()->GetCurrentKeyboardState(PFK_W))
			{
				camera.GetPosition(modelRot);
				modelRot.X += camera.GetDirection()->X*0.1f;
				modelRot.Y += camera.GetDirection()->Y*0.1f;
				modelRot.Z += camera.GetDirection()->Z*0.1f;
				camera.SetPosition(modelRot);
			}

			if (GetInputDevice()->GetCurrentKeyboardState(PFK_S))
			{
				camera.GetPosition(modelRot);
				modelRot.X -= camera.GetDirection()->X*0.1f;
				modelRot.Y -= camera.GetDirection()->Y*0.1f;
				modelRot.Z -= camera.GetDirection()->Z*0.1f;
				camera.SetPosition(modelRot);
			}

		}

		//
		// update camera
		camera.Update();

		if (GetInputDevice()->WasKeyReleased(PFK_F))
		{
			isCameraFree = !isCameraFree;
			GetInputDevice()->SetMousePosition(32, 32);

		}

		//EINT32 mox, moy;
		//EFLOAT32 moxf, moyf;
		//GetInputDevice()->GetMousePosition(mox, moy);
		//GetInputDevice()->GetMousePositionNormalized(moxf, moyf);
		//printf("\n --      mouse pos ( %i, %i) f( %f , %f)", mox, moy, moxf, moyf);
		//
		// set render target
		

		// ****************************
		//	Deferred Rendering
		// ****************************
		PROFILER_BEGIN("Deferred Rendering");
	

		// prepare
		renderer.PrepareToDraw(GetGraphicsDevice());


		//GetGraphicsDevice()->SetDefaultSamplers(SamplerType::POINT, SamplerType::POINT, SamplerType::POINT, SamplerType::POINT, SamplerType::POINT, SamplerType::POINT, SamplerType::POINT, SamplerType::POINT, SamplerType::POINT, SamplerType::POINT, SamplerType::POINT, SamplerType::POINT);
		
		//
		// Draw particles
		
		//renderer.DrawParticles(GetGraphicsDevice(), &camera);
		
		
		//printf("\nOctree nodes = %u", m_frustumManager->m_visibleOctreeNodes->GetCount());

		
		// front
		renderer.PrepareAndSetGBufferSolid(GetGraphicsDevice());

		renderer.DrawSolidPhase1(GetGraphicsDevice(), &camera, m_frustumManager );



		//for (EUINT32 i = 0; i < 5; ++i)
		//	renderer.DrawSolidPhase1(GetGraphicsDevice(), &camera, m_frustumManager.GetSceneCart() );

		//for (EUINT32 i = 0; i < 5; ++i)
		//	renderer.DrawSolidPhase1(GetGraphicsDevice(), &camera, renderer.m_occulsionCutter.GetSceneCart() );


		//renderer.DrawTerrainCells(GetGraphicsDevice(), &camera);

		//renderer.DrawSolidBack(GetGraphicsDevice(), &camera);
		//renderer.MergeSolidNParticles(GetGraphicsDevice(), &camera);

		renderer.DrawExtensions(GetGraphicsDevice(), &camera, m_frustumManager);
		
		//
		// merge with lightmap
		renderer.MergeGBufferSolidWithLightMap(GetGraphicsDevice(), &camera);


		//renderer.GBufferSolidGenerateMipMaps(GetGraphicsDevice());


		//m_mapSector->m_octree.DrawRecursive(
		//	GetGraphicsDevice(), SDrawOctree,
		//	PrimitivesFactory::Instance(GetGraphicsDevice()).m_vbBoxUnit,
		//	PrimitivesFactory::Instance(GetGraphicsDevice()).m_ibBoxUnit,
		//	&camera);

		PROFILER_END("Deferred Rendering");

		// ****************************
		//	Draw Octree
		// ****************************


		//...............


		// ****************************
		//	Draw editor Tools
		// ****************************
		PROFILER_BEGIN("Editor");

		m_sceneEditor->Update(GetGraphicsDevice(), GetInputDevice(), &camera, &renderer, &m_mapSector->m_octree);
		m_sceneEditor->Draw(GetGraphicsDevice(), GetInputDevice(), renderer.m_ExDRLight.m_RTLightMapSolid, renderer.m_RTGBufferSolid0, &camera);
		
		PROFILER_END("Editor");



		// ****************************
		//	Show on the screen
		// ****************************


		if (GetInputDevice()->GetCurrentKeyboardState(PFK_N))
		{
			buff2Disp = 3;
		}

		if (GetInputDevice()->GetCurrentKeyboardState(PFK_M))
		{
			buff2Disp = 11;
		}

		if (GetInputDevice()->GetCurrentKeyboardState(PFK_U)) buff2Disp = 0;
		if (GetInputDevice()->GetCurrentKeyboardState(PFK_I)) buff2Disp = 14;
		if (GetInputDevice()->GetCurrentKeyboardState(PFK_O)) buff2Disp = 20;
		if (GetInputDevice()->GetCurrentKeyboardState(PFK_P)) buff2Disp = 21;
		if (GetInputDevice()->GetCurrentKeyboardState(PFK_J)) buff2Disp = 9;

		renderer.DisplayBuffer(GetGraphicsDevice(), buff2Disp);


		// ****************************
		//	Draw editor GUI - always on top
		// ****************************
		GetGraphicsDevice()->SetDefaultRenderTarget();
		m_sceneEditor->DrawGUI(GetGraphicsDevice(), GetInputDevice(), renderer.m_RTGBufferSolid0, renderer.m_RTGBufferSolid0, &camera);


		//buff2Disp = m_sceneEditor->m_guiManager->leftMenu.renderer.settings.cbActiveRT->GetSelectedIndex();

		// -------------------------------------------------------------------------------------
		// directional light
		
		Vector3D dir;
		//Vector3D dart = m_guiManager->m_lmMainCP_renderer_dl_cpDLDirection->m_hsl;

		Vector3D dart = m_sceneEditor->m_guiManager->leftMenu.renderer.directionalLight.cpDLDirection->m_hsl;

		dir.X = ParabellumUtils::Math::FastCos(ParabellumUtils::Math::ToRadians(dart.X));
		dir.Z = ParabellumUtils::Math::FastSin(ParabellumUtils::Math::ToRadians(dart.X));
		dir.Y = -1.0f;	
		dir.X *= 1.0f + dart.Y * 2.0f;
		dir.Z *= 1.0f + dart.Y * 2.0f;
		dir.Normalize();
		
		renderer.m_ExDRLight.m_directionalLight.SetDirection(dir);
		renderer.m_ExDRLight.m_directionalLight.SetColor(m_sceneEditor->m_guiManager->leftMenu.renderer.directionalLight.cpDLColor->m_rgb);
		renderer.m_ExDRLight.m_directionalLight.SetIntensity(m_sceneEditor->m_guiManager->leftMenu.renderer.directionalLight.sliderDLIntensity->GetAbsoluteValue());

	}





	void Update()
	{
		Sleep(10);

		// update visible objects
		m_frustumManager->Update(GetGraphicsDevice(), renderer.GetRenderTarget(0), &(*m_mapSector), &renderer.m_renderTree, &camera);

	}

};






#endif