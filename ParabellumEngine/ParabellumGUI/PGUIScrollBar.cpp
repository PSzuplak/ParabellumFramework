#include "stdafx.h"
#include "PGUIScrollBar.h"

//	--------------------------------------------------------------------
//		Statics
//	--------------------------------------------------------------------

//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumUserInterface::PGUIScrollBar::PGUIScrollBar(
	_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager)
	: PGUIDrawableObject(pDevice, pResourceManager)
{
	m_orientation = PGUIOrientation::Vertical;

	m_pointerPosition = 0.0f;
	m_pointerSize = 1.0f;

	SetPlacementType(PGUIPlacementType::TopRight);
	SetPositionAspect(PGUIPositionAspect::Fixed);
	SetPosition(1.0_uip, 0.0_uip);
	SetSize(0.01_uif, 1.00_uip);

	m_back = new PGUIDrawableObject(pDevice, pResourceManager);
	m_back->SetParent(this);
	m_back->SetPosition(0.5_uip, 0.001_uip);
	m_back->SetSize(0.01_uif, 0.01_uif);
	m_back->SetPlacementType(PGUIPlacementType::CenterX);
	m_back->SetPositionAspect(PGUIPositionAspect::Relative);
	m_back->SetSkingBackgroundCoords(Vector4D(0.65625f, 0.09375f, 0.6796875f, 0.109375f));
	m_back->SetSkinForegroundCoords(Vector4D(0.65625f, 0.09375f, 0.6796875f, 0.109375f));

	m_next = new PGUIDrawableObject(pDevice, pResourceManager);
	m_next->SetParent(this);
	m_next->SetPosition(0.0_uip, 0.98_uip);
	m_next->SetSize(0.9_uip, 0.01_uif);
	m_next->SetPlacementType(PGUIPlacementType::BottomLeft);
	m_next->SetPositionAspect(PGUIPositionAspect::Relative);
	m_next->SetSkingBackgroundCoords(Vector4D(0.65625f, 0.109375f, 0.6796875f, 0.125f));
	m_next->SetSkinForegroundCoords(Vector4D(0.65625f, 0.109375f, 0.6796875f, 0.125f));

	m_pointer = new PGUIDrawableObject(pDevice, pResourceManager);
	m_pointer->SetParent(this);
	m_pointer->SetPosition(0.0_uif, 0.5_uip);
	m_pointer->SetSize(1.00_uip, 1.00_uip);
	m_pointer->SetPlacementType(PGUIPlacementType::TopLeft);
	m_pointer->SetPositionAspect(PGUIPositionAspect::Relative);

	m_bar = new PGUIDrawableObject(pDevice, pResourceManager);
	m_bar->SetParent(this);
	m_bar->SetPosition(0.0_uif, 0.0_uif);
	m_bar->SetSize(1.0_uip, 1.00_uip);
	m_bar->SetPlacementType(PGUIPlacementType::TopLeft);
	m_bar->SetPositionAspect(PGUIPositionAspect::Relative);



	// add events
	m_back->AddEventMouseHold
	(
		[](void* owner, void* args)
		{
			((PGUIScrollBar*)((PGUIButton*)owner)->GetParent())->MovePointer(+0.005f);
		}
	);

	m_next->AddEventMouseHold
	(
			[](void* owner, void* args)
			{
				((PGUIScrollBar*)((PGUIButton*)owner)->GetParent())->MovePointer(-0.005f);
			}
	);

}



ParabellumUserInterface::PGUIScrollBar::~PGUIScrollBar()
{
	delete m_back;
	delete m_next;
	delete m_bar;
	delete m_pointer;
}

//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------
void 
ParabellumUserInterface::PGUIScrollBar::MovePointer(EFLOAT32 dd)
{
	EFLOAT32 nd;

	if (m_parent == NULLPTR)
		return;

	nd = m_pointerPosition + dd;

	m_pointerPosition = nd;


	if(m_orientation == PGUIOrientation::Vertical)
	{
		GetParent()->MoveChildren(0.0f, dd);
	}
	else
	{
		GetParent()->MoveChildren(dd, 0.0f);
	}

	

}


void 
ParabellumUserInterface::PGUIScrollBar::SetPointer(EFLOAT32 dd)
{
	EFLOAT32 nd;

	nd = dd;

	m_pointerPosition = nd;
}


//	--------------------------------------------------------------------
//		Gets And sets
//	--------------------------------------------------------------------
