#include "stdafx.h"
#include "SceneEditorUI.h"




//	------------------------------------------------------------
//		Constructors and destructors
//	------------------------------------------------------------
ParabellumEngine::Editor::SceneEditorUI::SceneEditorUI(_IN_ GraphicsDevice* pDevice, _IN_ InputDevice* pInput, _IN_ ResourceManager* pResourceManager)
	: PGUIManager(pDevice, pInput, pResourceManager)
{
	//////////////////////////////////////////////////////

	//tpmTP = new PGUIColorPicker(pDevice, pResourceManager);
	//tpmTP->SetPosition(0.5_uif, 0.5_uif);
	//tpmTP->SetSize(0.2_uif, 0.2_uif);
	//tpmTP->SetPlacementType(PGUIPlacementType::TopLeft);
	//tpmTP->SetPositionAspect(PGUIPositionAspect::Absolute);
	//
	//Add(tpmTP);


	//
	// Initialize pathes
	m_modelsPathes.push_back("A");
	m_modelsPathes.push_back("B");
	m_modelsPathes.push_back("C");
	m_modelsPathes.push_back("D");
	m_modelsPathes.push_back("E");
	m_modelsPathes.push_back("F");
	m_modelsPathes.push_back("G");
	m_modelsPathes.push_back("H");
	m_modelsPathes.push_back("A");
	m_modelsPathes.push_back("B");
	m_modelsPathes.push_back("C");
	m_modelsPathes.push_back("D");
	m_modelsPathes.push_back("E");
	m_modelsPathes.push_back("F");
	m_modelsPathes.push_back("G");
	m_modelsPathes.push_back("H");
	m_modelsPathes.push_back("A");
	m_modelsPathes.push_back("B");
	m_modelsPathes.push_back("C");
	m_modelsPathes.push_back("D");
	m_modelsPathes.push_back("E");
	m_modelsPathes.push_back("F");
	m_modelsPathes.push_back("G");
	m_modelsPathes.push_back("H");
	m_modelsPathes.push_back("A");
	m_modelsPathes.push_back("B");
	m_modelsPathes.push_back("C");
	m_modelsPathes.push_back("D");
	m_modelsPathes.push_back("E");
	m_modelsPathes.push_back("F");
	m_modelsPathes.push_back("G");
	m_modelsPathes.push_back("H");
	m_modelsPathes.push_back("A");
	m_modelsPathes.push_back("B");
	m_modelsPathes.push_back("C");


	// ------------------------------------------------------------
	// - left menu
	//
	

	leftMenu.gbLeftMenu = new PGUIGroupBox(pDevice, pResourceManager);
	leftMenu.gbLeftMenu->SetPosition(0.0_uif, 0.0_uif);
	leftMenu.gbLeftMenu->SetSize(0.15_uif, 0.80_uif);
	leftMenu.gbLeftMenu->SetPlacementType(PGUIPlacementType::TopLeft);
	leftMenu.gbLeftMenu->SetPositionAspect(PGUIPositionAspect::Relative);
	leftMenu.gbLeftMenu->DisplayBorder(true);

	Add(leftMenu.gbLeftMenu);
	
		// -- add ComboPage
		leftMenu.cbMain = new PGUIComboPage(pDevice, pResourceManager);
		leftMenu.cbMain->SetParent(leftMenu.gbLeftMenu);
		leftMenu.cbMain->SetPosition(0.5_uip, 0.00_uif);
		leftMenu.cbMain->SetSize(1.0_uip, 1.00_uip);
		leftMenu.cbMain->SetPlacementType(PGUIPlacementType::CenterX);
		leftMenu.cbMain->SetPositionAspect(PGUIPositionAspect::Fixed);

		leftMenu.cbMain->AddPage(pDevice, pResourceManager, "Settings");
		leftMenu.cbMain->AddPage(pDevice, pResourceManager, "Editor");
		leftMenu.cbMain->AddPage(pDevice, pResourceManager, "Renderer");
		leftMenu.cbMain->AddPage(pDevice, pResourceManager, "Profiler");
		
		
			// --- Editor
			leftMenu.editor.bbToolBox = new PGUIButtonBox(pDevice, pResourceManager);;
			leftMenu.editor.bbToolBox->SetPosition(0.04_uif, 0.04_uif);
			leftMenu.editor.bbToolBox->SetSize(1.0_uip, 1.0_uip);
			leftMenu.editor.bbToolBox->SetPlacementType(PGUIPlacementType::CenterX);
			leftMenu.editor.bbToolBox->SetPositionAspect(PGUIPositionAspect::Relative);

			auto calcUV = [](int x, int y) -> Vector4D 
			{   
				Vector4D result;
				result.X = ((EFLOAT32)x * 32.0f / 1024.0f);
				result.Y = ((EFLOAT32)y * 32.0f / 1024.0f);
				result.Z = ((EFLOAT32)(x+1) * 32.0f / 1024.0f);
				result.W = ((EFLOAT32)(y+1) * 32.0f / 1024.0f);
				return result;
			};

			leftMenu.editor.bbToolBox->AddButton(pDevice, pResourceManager, calcUV(27, 2));  // rect selection
			leftMenu.editor.bbToolBox->AddButton(pDevice, pResourceManager, calcUV(28, 2)); // circle selectin
			leftMenu.editor.bbToolBox->AddButton(pDevice, pResourceManager, calcUV(29, 2)); // freeselection
			leftMenu.editor.bbToolBox->AddButton(pDevice, pResourceManager, calcUV(28, 4)); // move
			leftMenu.editor.bbToolBox->AddButton(pDevice, pResourceManager, calcUV(31, 4)); // rotate
			leftMenu.editor.bbToolBox->AddButton(pDevice, pResourceManager, calcUV(27, 5)); // scale
			leftMenu.editor.bbToolBox->AddButton(pDevice, pResourceManager, calcUV(31, 6)); // select vertices
			leftMenu.editor.bbToolBox->AddButton(pDevice, pResourceManager, calcUV(31, 7)); // brush
			leftMenu.editor.bbToolBox->AddButton(pDevice, pResourceManager, calcUV(27, 8)); // rubber

			leftMenu.cbMain->AddToPage(1, leftMenu.editor.bbToolBox);
			


				// --- rect selectin group box










			// --- Renderer
			leftMenu.renderer.exRenderer = new PGUIExpander(pDevice, pResourceManager);
			leftMenu.renderer.exRenderer->SetPosition(0.5_uip, 0.04_uif);
			leftMenu.renderer.exRenderer->SetSize(0.9_uip, 1.0_uip);
			leftMenu.renderer.exRenderer->SetPlacementType(PGUIPlacementType::CenterX);
			leftMenu.renderer.exRenderer->SetPositionAspect(PGUIPositionAspect::Relative);

			leftMenu.renderer.exRenderer->AddExpandBar(pDevice, pResourceManager, "Renderer");
			leftMenu.renderer.exRenderer->AddExpandBar(pDevice, pResourceManager, "Directional Light");
			leftMenu.renderer.exRenderer->AddExpandBar(pDevice, pResourceManager, "Parallax Mapping");

			leftMenu.cbMain->AddToPage(2, leftMenu.renderer.exRenderer);

			// ---- renderer
			


			leftMenu.renderer.settings.labelActiveRT = new PGUIButton(pDevice, pResourceManager);
			leftMenu.renderer.settings.labelActiveRT->SetPosition(0.0_uif, 0.04_uif);
			leftMenu.renderer.settings.labelActiveRT->SetSize(0.9_uip, 0.02_uif);
			leftMenu.renderer.settings.labelActiveRT->SetPlacementType(PGUIPlacementType::TopLeft);
			leftMenu.renderer.settings.labelActiveRT->SetPositionAspect(PGUIPositionAspect::Relative);
			leftMenu.renderer.settings.labelActiveRT->SetVisibility(false);
			leftMenu.renderer.settings.labelActiveRT->SetLabel("Render Target :");

			leftMenu.renderer.exRenderer->AddObject(0, leftMenu.renderer.settings.labelActiveRT);

			leftMenu.renderer.settings.cbActiveRT = new PGUIComboBox(pDevice, pResourceManager);
			leftMenu.renderer.settings.cbActiveRT->SetPosition(0.005_uif, 0.06_uif);
			leftMenu.renderer.settings.cbActiveRT->SetSize(0.9_uip, 0.02_uif);
			leftMenu.renderer.settings.cbActiveRT->SetPlacementType(PGUIPlacementType::TopLeft);
			leftMenu.renderer.settings.cbActiveRT->SetPositionAspect(PGUIPositionAspect::Relative);

			leftMenu.renderer.settings.cbActiveRT->Add(pDevice, pResourceManager, "GBufferSolid0");
			leftMenu.renderer.settings.cbActiveRT->Add(pDevice, pResourceManager, "GBufferSolid1");
			leftMenu.renderer.settings.cbActiveRT->Add(pDevice, pResourceManager, "GBufferSolid2");
			leftMenu.renderer.settings.cbActiveRT->Add(pDevice, pResourceManager, "LightMapSolid");
			leftMenu.renderer.settings.cbActiveRT->Add(pDevice, pResourceManager, "PointLightSolid");
			leftMenu.renderer.settings.cbActiveRT->Add(pDevice, pResourceManager, "SSAO");
			leftMenu.renderer.settings.cbActiveRT->Add(pDevice, pResourceManager, "SSAOBlured");
			leftMenu.renderer.settings.cbActiveRT->Add(pDevice, pResourceManager, "HDR");
			leftMenu.renderer.settings.cbActiveRT->Add(pDevice, pResourceManager, "HDR");
			leftMenu.renderer.settings.cbActiveRT->Add(pDevice, pResourceManager, "VolumetricLight");
			leftMenu.renderer.settings.cbActiveRT->Add(pDevice, pResourceManager, "VolumetricLightPolished");
			leftMenu.renderer.settings.cbActiveRT->Add(pDevice, pResourceManager, "Parallax0");
			leftMenu.renderer.settings.cbActiveRT->Add(pDevice, pResourceManager, "Parallax1");
			leftMenu.renderer.settings.cbActiveRT->Add(pDevice, pResourceManager, "MergeGBufferLightMap");
			leftMenu.renderer.settings.cbActiveRT->Add(pDevice, pResourceManager, "MergeGBufferWithEffects");
			leftMenu.renderer.settings.cbActiveRT->Add(pDevice, pResourceManager, "GBufferProperties");
			leftMenu.renderer.settings.cbActiveRT->Add(pDevice, pResourceManager, "GBufferPropertiesHisogram");
			leftMenu.renderer.settings.cbActiveRT->Add(pDevice, pResourceManager, "GBufferMergeBasics");


			leftMenu.renderer.exRenderer->AddObject(0, leftMenu.renderer.settings.cbActiveRT);


				// ---- directional light

				//
			
				//
				leftMenu.renderer.directionalLight.labelDLDirection = new PGUIButton(pDevice, pResourceManager);
				leftMenu.renderer.directionalLight.labelDLDirection->SetPosition(0.02_uif, 0.03_uif);
				leftMenu.renderer.directionalLight.labelDLDirection->SetSize(0.9_uip, 0.02_uif);
				leftMenu.renderer.directionalLight.labelDLDirection->SetVisibility(false);
				leftMenu.renderer.directionalLight.labelDLDirection->SetLabel("Direction :");

				leftMenu.renderer.exRenderer->AddObject(1, leftMenu.renderer.directionalLight.labelDLDirection);

				//
				leftMenu.renderer.directionalLight.cpDLDirection = new PGUIColorPicker(pDevice, pResourceManager);
				leftMenu.renderer.directionalLight.cpDLDirection->SetPosition(0.5_uip, 0.05_uif);
				leftMenu.renderer.directionalLight.cpDLDirection->SetSize(0.12_uif, 0.15_uif);
				leftMenu.renderer.directionalLight.cpDLDirection->SetPlacementType(PGUIPlacementType::CenterX);
				leftMenu.renderer.directionalLight.cpDLDirection->SetPositionAspect(PGUIPositionAspect::Relative);

				leftMenu.renderer.exRenderer->AddObject(1, leftMenu.renderer.directionalLight.cpDLDirection);

				//
				leftMenu.renderer.directionalLight.labelDLColor = new PGUIButton(pDevice, pResourceManager);
				leftMenu.renderer.directionalLight.labelDLColor->SetPosition(0.02_uif, 0.21_uif);
				leftMenu.renderer.directionalLight.labelDLColor->SetSize(0.9_uip, 0.02_uif);
				leftMenu.renderer.directionalLight.labelDLColor->SetVisibility(false);
				leftMenu.renderer.directionalLight.labelDLColor->SetLabel("Color :");

				leftMenu.renderer.exRenderer->AddObject(1, leftMenu.renderer.directionalLight.labelDLColor);

				//

				leftMenu.renderer.directionalLight.cpDLColor = new PGUIColorPicker(pDevice, pResourceManager);
				leftMenu.renderer.directionalLight.cpDLColor->SetPosition(0.5_uip, 0.24_uif);
				leftMenu.renderer.directionalLight.cpDLColor->SetSize(0.12_uif, 0.15_uif);
				leftMenu.renderer.directionalLight.cpDLColor->SetPlacementType(PGUIPlacementType::CenterX);
				leftMenu.renderer.directionalLight.cpDLColor->SetPositionAspect(PGUIPositionAspect::Relative);

				leftMenu.renderer.exRenderer->AddObject(1, leftMenu.renderer.directionalLight.cpDLColor);

				//
				leftMenu.renderer.directionalLight.labelDLIntensity = new PGUIButton(pDevice, pResourceManager);
				leftMenu.renderer.directionalLight.labelDLIntensity->SetPosition(0.02_uif, 0.40_uif);
				leftMenu.renderer.directionalLight.labelDLIntensity->SetSize(0.9_uip, 0.02_uif);
				leftMenu.renderer.directionalLight.labelDLIntensity->SetVisibility(false);
				leftMenu.renderer.directionalLight.labelDLIntensity->SetLabel("Intensity :");

				leftMenu.renderer.exRenderer->AddObject(1, leftMenu.renderer.directionalLight.labelDLIntensity);

				//
				leftMenu.renderer.directionalLight.sliderDLIntensity = new PGUISlider(pDevice, pResourceManager);
				leftMenu.renderer.directionalLight.sliderDLIntensity->SetPosition(0.5_uip, 0.43_uif);
				leftMenu.renderer.directionalLight.sliderDLIntensity->SetSize(0.8_uip, 0.02_uif);
				leftMenu.renderer.directionalLight.sliderDLIntensity->SetPlacementType(PGUIPlacementType::CenterX);
				leftMenu.renderer.directionalLight.sliderDLIntensity->SetPositionAspect(PGUIPositionAspect::Relative);
				leftMenu.renderer.directionalLight.sliderDLIntensity->SetMin(0.0f);
				leftMenu.renderer.directionalLight.sliderDLIntensity->SetMax(2.0f);

				leftMenu.renderer.exRenderer->AddObject(1, leftMenu.renderer.directionalLight.sliderDLIntensity);

				// SSAO options

				//m_lmMainCP_renderer_ssao_labelSSAOradious = new PGUIButton(pDevice, pResourceManager);
				//m_lmMainCP_renderer_ssao_labelSSAOradious->SetPosition(0.0_uif, 0.03_uif);
				//m_lmMainCP_renderer_ssao_labelSSAOradious->SetSize(0.9_uip, 0.02_uif);
				//m_lmMainCP_renderer_ssao_labelSSAOradious->SetPlacementType(PGUIPlacementType::TopLeft);
				//m_lmMainCP_renderer_ssao_labelSSAOradious->SetPositionAspect(PGUIPositionAspect::Relative);
				//m_lmMainCP_renderer_ssao_labelSSAOradious->SetVisibility(false);
				//m_lmMainCP_renderer_ssao_labelSSAOradious->SetLabel("Radious :");
				//
				//m_lmMainCP_renderer->AddObject(1, m_lmMainCP_renderer_ssao_labelSSAOradious);
				//
				//
				//// SSAO slider radious
				//m_lmMainCP_renderer_ssao_sliderSSAOradious = new PGUISlider(pDevice, pResourceManager);
				//m_lmMainCP_renderer_ssao_sliderSSAOradious->SetPosition(0.05_uif, 0.05_uif);
				//m_lmMainCP_renderer_ssao_sliderSSAOradious->SetSize(0.8_uip, 0.02_uif);
				//m_lmMainCP_renderer_ssao_sliderSSAOradious->SetPlacementType(PGUIPlacementType::TopLeft);
				//m_lmMainCP_renderer_ssao_sliderSSAOradious->SetPositionAspect(PGUIPositionAspect::Relative);
				//
				//m_lmMainCP_renderer_ssao_sliderSSAOradious->SetMin(0.001f);
				//m_lmMainCP_renderer_ssao_sliderSSAOradious->SetMax(0.2000f);
				//
				//m_lmMainCP_renderer_ssao_sliderSSAOradious->SetPercentageValue(0.5f);
				//
				//m_lmMainCP_renderer->AddObject(1, m_lmMainCP_renderer_ssao_sliderSSAOradious);



			// --- Profiler
			leftMenu.profiler.gpProfiler = new PGUIGroupBox(pDevice, pResourceManager);
			leftMenu.profiler.gpProfiler->SetPosition(0.0_uif, 0.0_uif);
			leftMenu.profiler.gpProfiler->SetSize(1.0_uip, 1.00_uip);
			leftMenu.profiler.gpProfiler->SetPlacementType(PGUIPlacementType::TopLeft);
			leftMenu.profiler.gpProfiler->SetPositionAspect(PGUIPositionAspect::Relative);
			leftMenu.profiler.gpProfiler->DisplayBorder(false);

			leftMenu.cbMain->AddToPage(3, leftMenu.profiler.gpProfiler);


				leftMenu.profiler.labelFPS = new PGUIButton(pDevice, pResourceManager);
				leftMenu.profiler.labelFPS->SetPosition(0.01_uif, 0.02_uif);
				leftMenu.profiler.labelFPS->SetSize(0.9_uip, 0.02_uif);
				leftMenu.profiler.labelFPS->SetVisibility(false);
				leftMenu.profiler.labelFPS->SetLabel("FPS :");

				leftMenu.profiler.labelFPS->SetParent(leftMenu.profiler.gpProfiler);




	// logger

	// debugger


	// ------------------------------------------------------------
	// bottom menu
	//
			
	bottomMenu.ptBottomMenu = new PGUIPageTab(pDevice, pResourceManager);
	bottomMenu.ptBottomMenu->SetPosition(0.15_uif, 0.8_uif);
	bottomMenu.ptBottomMenu->SetSize(0.75_uif, 0.2_uif);
	bottomMenu.ptBottomMenu->SetPlacementType(PGUIPlacementType::TopLeft);
	bottomMenu.ptBottomMenu->SetPositionAspect(PGUIPositionAspect::Relative);

	Add(bottomMenu.ptBottomMenu);


	bottomMenu.ptBottomMenu->AddTab(pDevice, pResourceManager, "Assets");
	bottomMenu.ptBottomMenu->AddTab(pDevice, pResourceManager, "Console");

	bottomMenu.ptBottomMenu->SelectPage(0);

	
		// - Assets

		bottomMenu.assets.gpAssets = new PGUIGroupBox(pDevice, pResourceManager);
		bottomMenu.assets.gpAssets->SetPosition(0.5_uip, 0.5_uip);
		bottomMenu.assets.gpAssets->SetSize(0.9_uip, 0.9_uip);
		bottomMenu.assets.gpAssets->SetPlacementType(PGUIPlacementType::CenterXY);
		bottomMenu.assets.gpAssets->SetPositionAspect(PGUIPositionAspect::Relative);

		bottomMenu.ptBottomMenu->AddToPage(0, bottomMenu.assets.gpAssets);

		bottomMenu.assets.sb_gpAssets = new PGUIScrollBar(pDevice, pResourceManager);
		bottomMenu.assets.sb_gpAssets->SetParent(bottomMenu.assets.gpAssets);

		bottomMenu.assets.gb_AssetsInside = new PGUIGroupBox(pDevice, pResourceManager);
		bottomMenu.assets.gb_AssetsInside->SetParent(bottomMenu.assets.gpAssets);
		bottomMenu.assets.gb_AssetsInside->SetPosition(0.00_uif, 0.00_uif);
		bottomMenu.assets.gb_AssetsInside->SetSize(1.00_uip, 1.00_uip);
		bottomMenu.assets.gb_AssetsInside->SetPlacementType(PGUIPlacementType::TopLeft);
		bottomMenu.assets.gb_AssetsInside->SetPositionAspect(PGUIPositionAspect::Relative);
		bottomMenu.assets.gb_AssetsInside->SetVisibility(false);


		//std::vector<PGUIButton*> m_bm_assets_modelsImages;
		//std::vector<PGUIHintable*> m_bm_assets_modelsHints;
		for (auto it : m_modelsPathes)
		{
			PGUIButton* btn = new PGUIButton(pDevice, pResourceManager);
			btn->SetPosition(0.05_uif, 0.05_uif);
			btn->SetSize(0.05_uif, 0.05_uif);
			btn->SetPlacementType(PGUIPlacementType::TopLeft);
			btn->SetPositionAspect(PGUIPositionAspect::Inline);
			btn->SetMargin(Vector4D(0.005f, 0.01f, 0.01f, 0.005f));

			btn->SetParent(bottomMenu.assets.gb_AssetsInside);


			PGUIHintable* hint = new PGUIHintable(pDevice, pResourceManager);
			hint->SetParent(btn);
			hint->SetHint(it.c_str());


			bottomMenu.assets.modelsImages.push_back(btn);
			bottomMenu.assets.modelsHints.push_back(hint);
		}



	// - Animation


	// - Console






	// ------------------------------------------------------------
	// right menu
	//

	rightMenu.gbRightMenu = new PGUIGroupBox(pDevice, pResourceManager);

	rightMenu.gbRightMenu->SetPosition(0.90_uif, 0.0_uif);
	rightMenu.gbRightMenu->SetSize(0.10_uif, 0.8_uif);
	rightMenu.gbRightMenu->SetPlacementType(PGUIPlacementType::TopLeft);
	rightMenu.gbRightMenu->SetPositionAspect(PGUIPositionAspect::Relative);

	Add(rightMenu.gbRightMenu);


}

ParabellumEngine::Editor::SceneEditorUI::~SceneEditorUI()
{
}

//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------

