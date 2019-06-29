#ifndef _SCENEEDITORUI_H_
#define _SCENEEDITORUI_H_


//
//	ParabellumFramework dependieces
//
#include "../ParabellumFramework/DLLIE.h"
#include "../ParabellumFramework/TypesC.h"

//
// STDs
//
#include <iostream>


#include "../ParabellumGUI/PGUIManager.h"


using namespace ParabellumUserInterface;
using namespace ParabellumFramework::Graphics;
using namespace ParabellumFramework::Resources;

namespace ParabellumEngine
{
	namespace Editor
	{

		class XYZ_API SceneEditorUI : public ParabellumUserInterface::PGUIManager
		{
		public:

			std::vector<std::string> m_modelsPathes;

			//
			// GUI
			//

			struct LeftMenu
			{
				PGUIGroupBox* gbLeftMenu;

				PGUIComboPage* cbMain;


					struct Editor
					{
						PGUIButtonBox* bbToolBox;

					} editor;



					struct Renderer
					{
						PGUIExpander* exRenderer;

						struct Settings
						{
							// renderer
							PGUIButton* labelActiveRT;
							PGUIComboBox* cbActiveRT;
						} settings;
						

						struct DirectionalLight
						{
							// directional light
							PGUIButton* labelDLDirection;
							PGUIColorPicker* cpDLDirection;

							PGUIButton*labelDLColor;
							PGUIColorPicker* cpDLColor;

							PGUIButton* labelDLIntensity;
							PGUISlider* sliderDLIntensity;
						} directionalLight;


					} renderer;


					struct Profiler
					{

						PGUIGroupBox* gpProfiler;
							PGUIButton* labelFPS;
							PGUIButton* labelFPSValue;



					} profiler;
						


			} leftMenu;


			struct BottomMenu
			{
				PGUIPageTab* ptBottomMenu;

				struct Assets
				{
					PGUIGroupBox* gpAssets;
					PGUIScrollBar* sb_gpAssets;
					PGUIGroupBox* gb_AssetsInside;

					std::vector<PGUIButton*> modelsImages;
					std::vector<PGUIHintable*> modelsHints;
				} assets;

				

			} bottomMenu;


			struct RightMenu
			{
				PGUIGroupBox* gbRightMenu;
			} rightMenu;



			

			

			


			

			// ssao
			PGUIButton* m_lmMainCP_renderer_ssao_labelSSAOradious;
			PGUISlider* m_lmMainCP_renderer_ssao_sliderSSAOradious;


		

			PGUIColorPicker* tpmTP;

		public:

			// ==================================
			SceneEditorUI(_IN_ GraphicsDevice* pDevice, _IN_ InputDevice* pInput, _IN_ ResourceManager* pResourceManager);

			~SceneEditorUI();

		};



	}
}


#endif