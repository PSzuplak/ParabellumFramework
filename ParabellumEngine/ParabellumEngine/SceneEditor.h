#ifndef _SCENEEDITOR_H_
#define _SCENEEDITOR_H_

//
//	ParabellumFramework dependieces
//
#include "../ParabellumFramework/DLLIE.h"
#include "../ParabellumFramework/TypesC.h"



//
// STDs
//
#include <memory>
#include <functional>
#include <list>

//
// ParabellumFramework
//
#include "../ParabellumFramework/GraphicsDevice.h"
#include "../ParabellumFramework/RenderTarget.h"
#include "../ParabellumFramework/InputDevice.h"
#include "../ParabellumFramework/Model.h"
//
// ParabellumEngine
//
#include "EditorTypes.h"
#include "Component3D.h"
#include "MapSector.h"
#include "DeferredRenderer.h"

#include "Pivot.h"
#include "SelectionTool.h"
#include "SelectionTool_Box.h"


// UI
#include "SceneEditorUI.h"



namespace ParabellumEngine
{
	namespace Editor
	{

		using namespace ParabellumFramework;
		using namespace ParabellumFramework::Graphics;
		using namespace ParabellumFramework::IO;
		using namespace ParabellumFramework::Resources;
		using namespace ParabellumEngine::Components;
		//
		//
		//
		class XYZ_API SceneEditor
		{
			
		public:

			SceneEditor(_IN_ GraphicsDevice* pDevice, _IN_ InputDevice* pInput, _IN_ ResourceManager* pResourceManager);
			~SceneEditor();

		//
		// Methods
		//
		public:


			void Draw(_IN_ GraphicsDevice* pDevice, _IN_ InputDevice* pInputDevice, _IN_ RenderTarget* pRenderTarget, _IN_ RenderTarget* pDepthBuffer, _IN_ ParabellumEngine::Components::Camera* pCamera);

			void DrawGUI(_IN_ GraphicsDevice* pDevice, _IN_ InputDevice* pInputDevice, _IN_ RenderTarget* pRenderTarget, _IN_ RenderTarget* pDepthBuffer, _IN_ ParabellumEngine::Components::Camera* pCamera);


			void Update(
				_IN_ GraphicsDevice* pDevice, _IN_ InputDevice* pInputDevice, 
				_IN_ ParabellumEngine::Components::Camera* pCamera,
				_IN_ ParabellumEngine::Core::Graphics::DeferredRenderer* deferredRenderer,
				_IN_ Octree* pOctree
				);



		//
		// Memebers
		//
		public:

			// GUI
			SceneEditorUI* m_guiManager;

			std::list<ParabellumFramework::Graphics::Model*> m_choosedModels;


			GraphicsDevice* m_graphicsDevice;

			
			Pivot* m_pivot;




			struct Selector
			{
				SelectionTool_Box* selectionTool;

				std::list<Component3D*> selectedComponents;

				SelectionType selectionType;

			} m_selector;


			//
			// Manipulation
			//

			struct Manipulator
			{

				PE_ManipulationType m_manipulationType;

				PE_ManipulationHook m_manipulationHook;


				EUINT32 m_manipulationOption;	// which tool used in manipulation

				Vector3D m_manipulationVector;	// vector used in model manipulation functions

				Vector3D m_manipulationFactor;	// value represents size of changes 
												// (bigger factor = bigger scale etc.)


			} m_manipulator;





		};
	}
}






#endif