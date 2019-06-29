#include "stdafx.h"
#include "PGUIExtDrawableObject.h"

//	--------------------------------------------------------------------
//		Statics
//	--------------------------------------------------------------------

//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumUserInterface::PGUIExtDrawableObject::PGUIExtDrawableObject(
	_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager)
	: PGUIDrawableObject(pDevice, pResourceManager)
{


	//
	// create border
	m_borderTL = new PGUISimple(pDevice, pResourceManager);
	m_borderT = new PGUISimple(pDevice, pResourceManager);
	m_borderTR = new PGUISimple(pDevice, pResourceManager);
	m_borderR = new PGUISimple(pDevice, pResourceManager);
	m_borderBR = new PGUISimple(pDevice, pResourceManager);
	m_borderB = new PGUISimple(pDevice, pResourceManager);
	m_borderBL = new PGUISimple(pDevice, pResourceManager);
	m_borderL = new PGUISimple(pDevice, pResourceManager);

	m_borderTL->SetParent(this);
	m_borderT->SetParent(this);
	m_borderTR->SetParent(this);
	m_borderR->SetParent(this);
	m_borderBR->SetParent(this);
	m_borderB->SetParent(this);
	m_borderBL->SetParent(this);
	m_borderL->SetParent(this);

	m_borderTL->SetPositionAspect(PGUIPositionAspect::Relative);
	m_borderT->SetPositionAspect(PGUIPositionAspect::Relative);
	m_borderTR->SetPositionAspect(PGUIPositionAspect::Relative);
	m_borderR->SetPositionAspect(PGUIPositionAspect::Relative);
	m_borderBR->SetPositionAspect(PGUIPositionAspect::Relative);
	m_borderB->SetPositionAspect(PGUIPositionAspect::Relative);
	m_borderBL->SetPositionAspect(PGUIPositionAspect::Relative);
	m_borderL->SetPositionAspect(PGUIPositionAspect::Relative);

	m_borderTL->SetPlacementType(PGUIPlacementType::TopLeft);
	m_borderT->SetPlacementType(PGUIPlacementType::CenterX);
	m_borderTR->SetPlacementType(PGUIPlacementType::TopLeft);
	m_borderR->SetPlacementType(PGUIPlacementType::CenterY);
	m_borderBR->SetPlacementType(PGUIPlacementType::TopLeft);
	m_borderB->SetPlacementType(PGUIPlacementType::CenterX);
	m_borderBL->SetPlacementType(PGUIPlacementType::TopLeft);
	m_borderL->SetPlacementType(PGUIPlacementType::CenterY);

	//m_borderTL->SetHolder(this);
	//m_borderT->SetHolder(this);
	//m_borderTR->SetHolder(this);
	//m_borderR->SetHolder(this);
	//m_borderBR->SetHolder(this);
	//m_borderB->SetHolder(this);
	//m_borderBL->SetHolder(this);
	//m_borderL->SetHolder(this);

	SetBorderSize(0.003f);


	DisplayBorder(false);

	//
	// events
	m_borderR->AddEventMouseHold
	(
		[](void* owner, void* args)
		{
			PUIEventMouse* e = (PUIEventMouse*)args;
			Vector4D size;
			PGUIUnitData newWidth;

			((PGUIDrawableObject*)owner)->GetSize(size);

			newWidth.Unit = PGUIUnit::Float;
			newWidth.Value = e->MouseX - (size.X * 0.5f);

			((PGUIDrawableObject*)((PGUIDrawableObject*)owner)->GetParent())->SetWidth(newWidth);
		}
	);
	
}



ParabellumUserInterface::PGUIExtDrawableObject::~PGUIExtDrawableObject()
{
	delete m_borderTL;
	delete m_borderT;
	delete m_borderTR;
	delete m_borderR;
	delete m_borderBR;
	delete m_borderB;
	delete m_borderBL;
	delete m_borderL;
}

//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------

void
ParabellumUserInterface::PGUIExtDrawableObject::DisplayBorder(EBOOL val)
{
	if (val == true)
	{
		m_borderTL->SetDisplayable(true);
		m_borderT->SetDisplayable(true);
		m_borderTR->SetDisplayable(true);
		m_borderR->SetDisplayable(true);
		m_borderBR->SetDisplayable(true);
		m_borderB->SetDisplayable(true);
		m_borderBL->SetDisplayable(true);
		m_borderL->SetDisplayable(true);
	}
	else
	{
		m_borderTL->SetDisplayable(false);
		m_borderT->SetDisplayable(false);
		m_borderTR->SetDisplayable(false);
		m_borderR->SetDisplayable(false);
		m_borderBR->SetDisplayable(false);
		m_borderB->SetDisplayable(false);
		m_borderBL->SetDisplayable(false);
		m_borderL->SetDisplayable(false);
	}
}



//	--------------------------------------------------------------------
//		Gets And sets
//	--------------------------------------------------------------------
void
ParabellumUserInterface::PGUIExtDrawableObject::SetBorderSize(EFLOAT32 val)
{
	PGUIUnitData unitData;
	unitData.Value = val;
	unitData.Unit = PGUIUnit::Float;

	m_borderTL->SetPosition(-unitData, -unitData);
	m_borderT->SetPosition(+0.50_uip, -unitData);
	m_borderTR->SetPosition(+1.00_uip, -unitData);
	m_borderR->SetPosition(+1.00_uip, +0.50_uip);
	m_borderBR->SetPosition(+1.00_uip, +1.00_uip);
	m_borderB->SetPosition(+0.50_uip, +1.00_uip);
	m_borderBL->SetPosition(-unitData, +1.00_uip);
	m_borderL->SetPosition(-unitData, +0.50_uip);

	m_borderTL->SetSize(+unitData, +unitData);
	m_borderT->SetSize(+1.00_uip, +unitData);
	m_borderTR->SetSize(+unitData, +unitData);
	m_borderR->SetSize(+unitData, +1.00_uip);
	m_borderBR->SetSize(+unitData, +unitData);
	m_borderB->SetSize(+1.00_uip, +unitData);
	m_borderBL->SetSize(+unitData, +unitData);
	m_borderL->SetSize(+unitData, +1.00_uip);
}