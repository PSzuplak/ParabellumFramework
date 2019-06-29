#include "stdafx.h"
#include "PGUIManager.h"

//	--------------------------------------------------------------------
//		Statics
//	--------------------------------------------------------------------



//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumUserInterface::PGUIManager::PGUIManager(
	_IN_ GraphicsDevice* pDevice, _IN_ InputDevice* pInput, _IN_ ResourceManager* pResourceManager)
{

	m_selectedObject = NULLPTR;

	//
	// mouse 
	m_puiCursor = new PGUICursor(pDevice, pResourceManager);
	m_puiCursor->SetPosition(0.5_uif, 0.5_uif);
	m_puiCursor->SetSize(0.02_uif, 0.02_uif);
	m_puiCursor->SetPlacementType(PGUIPlacementType::TopLeft);
	m_puiCursor->SetPositionAspect(PGUIPositionAspect::Absolute);
	m_puiCursor->SetSkingBackgroundCoords(Vector4D(0.75f, 0.5625f, 0.8115234375f, 0.6240234375f));
	m_puiCursor->SetSkinForegroundCoords(Vector4D(0.75f, 0.5625f, 0.8115234375f, 0.6240234375f));

	cursor.isFixedDisplacement = false;

	Subscribe(pInput->EventMouseLeftReleased, &PGUIManager::HandlerMouseClick);
	Subscribe(pInput->EventMouseLeftDragged,  &PGUIManager::HandlerMouseDrag);
}



ParabellumUserInterface::PGUIManager::~PGUIManager()
{
}

//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------


void
ParabellumUserInterface::PGUIManager::Draw(_IN_ GraphicsDevice* pDevice, _IN_ InputDevice* pInput)
{
	for (auto it : m_objects)
	{
		if (it != m_selectedObject)
			it->Draw(pDevice);
	}

	// draw selected item
	if (m_selectedObject != NULLPTR)
		m_selectedObject->Draw(pDevice);


	// draw cursor at the end
	m_puiCursor->Draw(pDevice);
}


void
ParabellumUserInterface::PGUIManager::Update(_IN_ GraphicsDevice* pDevice, _IN_ InputDevice* pInput)
{
	Vector4D pos, mousePrevious, mouseCurrent;
	PGUIUnitData newPosX, newPosY;

	if (pInput->WasMouseMoved() == true)
	{
		m_puiCursor->GetPosition(mousePrevious);
		
		pInput->GetMousePositionNormalized(mouseCurrent.X, mouseCurrent.Y);

		m_puiCursor->SetPosition(mouseCurrent.X, mouseCurrent.Y);


		// check if mouse moved over on an object
		PGUIDrawableObject* pointedObject;
		bool found = false;

		pointedObject = GetGUIObject(mouseCurrent.X, mouseCurrent.Y);

		if (pointedObject != NULLPTR)
		{
			for (auto it : m_objectsMouseOver)
			{
				if (it == pointedObject)
				{
					found = true;
					break;
				}
			}

			if (found == false)
			{
				m_objectsMouseOver.push_back(pointedObject);
				//printf("\n MouseOver : %p", pointedObject);
			}
		}

		// check if mouse moved out from an object
		for (auto it : m_objectsMouseOver)
		{
			if (it->CheckCollision(mouseCurrent.X, mouseCurrent.Y) == false)
			{
				it->InvokeMouseOut(mouseCurrent.X, mouseCurrent.Y);
				m_objectsMouseOver.remove(it);
			}
		}

	}
	
}

void
ParabellumUserInterface::PGUIManager::Add(_IN_ _KEEP_ PGUIDrawableObject* pObj)
{
	// list can contain only highest parent. 
	// If parents is on the list and You want add child - the child will not be added
	// it is because drawing is recursively
	for (auto it : m_objects)
	{
		if (it == pObj)
			return;

		if (pObj->IsRelatedUp(it) == true)
			return;
	}

	m_objects.push_back(pObj);

}



void
ParabellumUserInterface::PGUIManager::HandlerMouseClick(EFLOAT32 mouseX, EFLOAT32 mouseY)
{
	//bool wasInvoked = false;
	//
	//if (m_selectedObject == NULLPTR)
	//	m_selectedObject = GetGUIObject(mouseX, mouseY);
	//
	//if (m_selectedObject != NULLPTR)
	//{
	//	if (m_selectedObject->CheckCollision(mouseX, mouseY))
	//		m_selectedObject->InvokeMouseClick(mouseX, mouseY);
	//}

	for (auto it : m_objects)
	{
		it->InvokeMouseClick(mouseX, mouseY);
	}




}

void
ParabellumUserInterface::PGUIManager::HandlerMouseDrag(EFLOAT32 beginX, EFLOAT32 beginY, EFLOAT32 currentX, EFLOAT32 currentY, EFLOAT32 deltaX, EFLOAT32 deltaY)
{
	for (auto it : m_objects)
	{
		it->InvokeMouseDrag(beginX, beginY, currentX, currentY, deltaX, deltaY);
	}
}



//	--------------------------------------------------------------------
//		Gets and Sets
//	--------------------------------------------------------------------



ParabellumUserInterface::PGUIDrawableObject* 
ParabellumUserInterface::PGUIManager::GetGUIObject(EFLOAT32 x, EFLOAT32 y)
{
	PGUIDrawableObject* result;
	result = NULLPTR;

	for (auto it : m_objects)
	{
		result = it->GetDepest(x, y);

		if (result != NULLPTR)
			return result;
	}

	return result;
}