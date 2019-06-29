#include "stdafx.h"
#include "PGUIHintable.h"

//	--------------------------------------------------------------------
//		Statics
//	--------------------------------------------------------------------

//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumUserInterface::PGUIHintable::PGUIHintable(
	_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager)
	: PGUIDrawableObject(pDevice, pResourceManager)
{
	SetPosition(0.0_uif, 0.0_uif);
	SetSize(1.0_uip, 1.00_uip);
	SetPlacementType(PGUIPlacementType::TopLeft);
	SetPositionAspect(PGUIPositionAspect::Relative);
	SetVisibility(false);

	m_hintArea = new PGUIButton(pDevice, pResourceManager);
	m_hintArea->SetParent(this);
	m_hintArea->SetPosition(0.0_uif, 0.0_uif);
	m_hintArea->SetSize(1.0_uip, 1.00_uip);
	m_hintArea->SetPlacementType(PGUIPlacementType::TopLeft);
	m_hintArea->SetPositionAspect(PGUIPositionAspect::Absolute);

	DisplayHint(false);

	//
	// event
	AddEventMouseMove
	(
		[](void* owner, void* args)
		{
			PUIEventMouse* e = (PUIEventMouse*)args;

			((PGUIHintable*)owner)->DisplayHint(true);
			((PGUIHintable*)owner)->SetHintPosition(e->MouseX + 0.015f, e->MouseY + 0.015f);
		}
	);

	AddEventMouseOut
	(
		[](void* owner, void* args)
		{
			( (PGUIHintable*)owner)->DisplayHint(false);
		}
	);
}



ParabellumUserInterface::PGUIHintable::~PGUIHintable()
{
	delete m_hintArea;
}

//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------
void
ParabellumUserInterface::PGUIHintable::DisplayHint(EBOOL val)
{
	m_hintArea->SetDisplayable(val);
}


//	--------------------------------------------------------------------
//		Gets And sets
//	--------------------------------------------------------------------
void 
ParabellumUserInterface::PGUIHintable::SetHint(const char* str)
{
	m_hintArea->SetLabel(str);
}


void
ParabellumUserInterface::PGUIHintable::SetHintPosition(EFLOAT32 x, EFLOAT32 y)
{
	PGUIUnitData nx, ny;

	nx.Unit = PGUIUnit::Float;
	nx.Value = x;

	ny.Unit = PGUIUnit::Float;
	ny.Value = y;

	m_hintArea->SetPosition(nx, ny);
}