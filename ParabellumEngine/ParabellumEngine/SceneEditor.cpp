#include "stdafx.h"
#include "SceneEditor.h"




//	------------------------------------------------------------
//		Constructors and destructors
//	------------------------------------------------------------
ParabellumEngine::Editor::SceneEditor::SceneEditor(_IN_ GraphicsDevice* pDevice, _IN_ InputDevice* pInput, _IN_ ResourceManager* pResourceManager)
{
	m_graphicsDevice = pDevice;
	m_guiManager = new SceneEditorUI(pDevice, pInput, pResourceManager);
	m_pivot = new Pivot(pDevice, pResourceManager);
	m_pivot->SetPosition(Vector4D(80.0f, 10.0f, 80.0f, 0.0f));
	m_selector.selectionTool = new SelectionTool_Box(pDevice, pResourceManager);
}

ParabellumEngine::Editor::SceneEditor::~SceneEditor()
{
	delete m_guiManager;
}

//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------

void 
ParabellumEngine::Editor::SceneEditor::Draw(
	_IN_ GraphicsDevice* pDevice, _IN_ InputDevice* pInputDevice, 
	_IN_ RenderTarget* pRenderTarget, _IN_ RenderTarget* pDepthBuffer,
	_IN_ ParabellumEngine::Components::Camera* pCamera)
{

	//
	// Update GUI
	Vector2D mouseUV;

	pInputDevice->GetMousePositionNormalized(mouseUV.X, mouseUV.Y);

	//
	// draw tools
	//pDevice->SetDefaultRenderTarget();
	pDevice->SetRenderTargets(pRenderTarget, pDepthBuffer, false);

	pDevice->TurnAlphaBlending(true);
	pDevice->TurnDepthBuffer(true);

	m_selector.selectionTool->Draw(pDevice, pCamera);

	pDevice->TurnDepthBuffer(true);
	pDevice->TurnAlphaBlending(false);

	// draw pivot
	m_pivot->Draw(pDevice, pInputDevice, pCamera, mouseUV);

}


void
ParabellumEngine::Editor::SceneEditor::DrawGUI(
	_IN_ GraphicsDevice* pDevice, _IN_ InputDevice* pInputDevice,
	_IN_ RenderTarget* pRenderTarget, _IN_ RenderTarget* pDepthBuffer,
	_IN_ ParabellumEngine::Components::Camera* pCamera)
{
	pDevice->TurnAlphaBlending(true);
	pDevice->TurnDepthBuffer(false);

	m_guiManager->Update(pDevice, pInputDevice);
	m_guiManager->Draw(pDevice, pInputDevice);

	pDevice->TurnDepthBuffer(true);
	pDevice->TurnAlphaBlending(false);

}


void 
ParabellumEngine::Editor::SceneEditor::Update(
	_IN_ GraphicsDevice* pDevice, _IN_ InputDevice* pInputDevice,
	_IN_ ParabellumEngine::Components::Camera* pCamera, _IN_ ParabellumEngine::Core::Graphics::DeferredRenderer* deferredRenderer,
	_IN_ Octree* pOctree
	)
{
	Vector2D mousePressed, mouseCurrent, mouseDelta; 
	Vector4D gbufferDataPressed, gbufferDataCurrent;
	Vector4D worldPositionPressed, worldPositionCurrent;

	pInputDevice->GetMousePositionNormalized(mouseCurrent.X, mouseCurrent.Y);

	// update pivot
	m_pivot->Update(pDevice, pInputDevice, pCamera, mouseCurrent);

	// ------------------------------------------------------------------------------------------------
	// if choosed pivot or other tools do manipulation stuff


	if (pInputDevice->IsLeftButtonDown())
	{

		// check collision with pivot
		if (
			(m_pivot->markedAxis == PE_AxisType::None) &&
			(pInputDevice->WasLeftButtonDown() == false)
		)
		{
			m_pivot->MarkAxis(mouseCurrent);
		}


		if (m_pivot->markedAxis != PE_AxisType::None)
		{

			// fit mouse movement to the pivot axis
			Vector2D displacementVector = m_pivot->MarkedAxis().m_UVPositionEnd - m_pivot->MarkedAxis().m_UVPositionStart;
			pInputDevice->SetDisplacementVector(&displacementVector, true);

			// move pivot by the delta factor
			if (pInputDevice->GetMouseDragged(mousePressed.X, mousePressed.Y, mouseCurrent.X, mouseCurrent.Y, mouseDelta.X, mouseDelta.Y))
			{
				Vector4D delta;

				delta.XYZ +=
					(m_pivot->MarkedAxis2().GetEnd().XYZ - m_pivot->MarkedAxis2().GetBegin().XYZ) *
					Ratio(m_pivot->MarkedAxis().m_UVPositionStart, mouseCurrent - mouseDelta, mouseCurrent, m_pivot->MarkedAxis().m_UVPositionEnd);

				m_pivot->Move(delta);

			}


		}

	}
	// left button released - drop pivot
	else
	{
		pInputDevice->SetDisplacementVector(nullptr, false);
		m_pivot->MarkAxis(PE_AxisType::None);
	}



	// ------------------------------------------------------------------------------------------------
	// Selection tool
	
	if (pInputDevice->GetMouseDragged(mousePressed.X, mousePressed.Y, mouseCurrent.X, mouseCurrent.Y, mouseDelta.X, mouseDelta.Y))
	{
		m_selector.selectedComponents.clear();

		pDevice->GetResourceData1x1(mousePressed.X*deferredRenderer->GetRenderTarget(0)->GetWidth(), mousePressed.Y*deferredRenderer->GetRenderTarget(0)->GetHeight(), deferredRenderer->GetRenderTarget(0), &gbufferDataPressed);
		pDevice->GetResourceData1x1(mouseCurrent.X*deferredRenderer->GetRenderTarget(0)->GetWidth(), mouseCurrent.Y*deferredRenderer->GetRenderTarget(0)->GetHeight(), deferredRenderer->GetRenderTarget(0), &gbufferDataCurrent);

		worldPositionPressed = pCamera->GetWorldFromFrustumCorners(mousePressed, gbufferDataPressed.W);
		worldPositionCurrent = pCamera->GetWorldFromFrustumCorners(mouseCurrent, gbufferDataCurrent.W);

		m_selector.selectionTool->SetSelectionRegion(worldPositionPressed.XYZ, worldPositionCurrent.XYZ);
		m_selector.selectionTool->SelectComponents(&m_selector.selectedComponents, pOctree, m_selector.selectionType);

		//BoundingBox sssss;
		//Vector4D centers;
		//
		//for (auto it : m_selector.selectedComponents)
		//{
		//	sssss.Merge(it->GetTransformedBoundingBox());
		//}
		//sssss.MakeFromPoints(&worldPositionPressed.XYZ, &worldPositionCurrent.XYZ);
		//sssss.GetCenter(&centers.XYZ);
		//
		//m_pivot->SetPosition(centers);
	}
}
