#include "stdafx.h"
#include "PGUIExpandBar.h"

//	--------------------------------------------------------------------
//		Statics
//	--------------------------------------------------------------------

//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumUserInterface::PGUIExpandBar::PGUIExpandBar(
	_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager)
	: PGUIButton(pDevice, pResourceManager)
{
	//
	// selector
	m_selector = new PGUIDrawableObject(pDevice, pResourceManager);
	m_selector->SetParent(this);
	m_selector->SetPosition(0.0_uif, 0.0_uif);
	m_selector->SetSize(1.0_uip, 0.02_uif);
	m_selector->SetPositionAspect(PGUIPositionAspect::Relative);
	m_selector->SetPlacementType(PGUIPlacementType::TopLeft);
	m_selector->viewport.Set(false);

	//
	// content - first due draw order
	m_content = new PGUIGroupBox(pDevice, pResourceManager);
	m_content->SetParent(this);
	m_content->SetPosition(0.0_uif, 0.00_uif);
	m_content->SetSize(1.0_uip, 1.0_uip);
	m_content->SetPositionAspect(PGUIPositionAspect::Relative);
	m_content->SetPlacementType(PGUIPlacementType::TopLeft);


	//
	//
	SetVisibility(false);
	Expand(false);

	//
	// event to expand the bar
	
	//m_selector->AddEventMouseClick
	//(
	//	[](void* owner, void* args) 
	//	{ 
	//		((PGUIExpandBar*)((PGUIExpandBar*)owner)->GetParent())->Expand();
	//	}
	//);

	m_selector->AddEventMouseClick
	(
		[](void* owner, void* args)
		{
			((PGUIExpandBar*)((PGUIExpandBar*)owner)->GetParent())->Expand();
			auto task = ParabellumFramework::Event::CreateTask(((PGUIExpandBar*)((PGUIExpandBar*)owner)->GetParent()), &PGUIExpandBar::Expand2, 5);
			PGUIDrawableObject::taskManager.Push(task, 2, 60);
		}
	);


}



ParabellumUserInterface::PGUIExpandBar::~PGUIExpandBar()
{
	delete m_selector;
	delete m_content;
}

//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------


void
ParabellumUserInterface::PGUIExpandBar::Expand2(int h)
{
	if (m_isExpanded == true)
	{
		Vector4D currentSize;
		PGUIUnitData width, height;

		m_content->SetDisplayable(true);

		GetSize(currentSize);

		currentSize.Y += 0.025f;
		
		if (currentSize.Y > 0.6f)
		{
			currentSize.Y = 0.6f;
		}

		SetSize(1.0_uip, currentSize.Y);
	}
	else
	{
		Vector4D currentSize, parentSize;
		PGUIUnitData width, height;

		GetSize(currentSize);

		currentSize.Y -= 0.025f;
		

		if (currentSize.Y < 0.02f)
		{
			currentSize.Y = 0.02f;
			m_content->SetDisplayable(false);
		}

		SetSize(1.0_uip, currentSize.Y);
	}
}


void
ParabellumUserInterface::PGUIExpandBar::Expand()
{
	m_isExpanded = !m_isExpanded;
}


void 
ParabellumUserInterface::PGUIExpandBar::Expand(EBOOL val)
{
	// show
	if(val == false)
	{
		m_content->SetDisplayable(false);
		GetSize(m_savedSize);
		SetSize(1.0_uip, 0.02_uif);
		m_isExpanded = val;
	}
	// hide
	else if(val == true)
	{
		m_content->SetDisplayable(true);
		SetSize(1.0_uip, m_savedSize.Y);
		m_isExpanded = val;
	}

}

void 
ParabellumUserInterface::PGUIExpandBar::ChangeContentVisibility()
{
	if (m_isExpanded == true)
		SetVisibility(false);
	else
		SetVisibility(true);
}


void
ParabellumUserInterface::PGUIExpandBar::AddObject(PGUIDrawableObject* pPGUIObject)
{
	pPGUIObject->SetParent(m_content);
}





//	--------------------------------------------------------------------
//		Gets And sets
//	--------------------------------------------------------------------


ParabellumFramework::EBOOL 
ParabellumUserInterface::PGUIExpandBar::IsExpanded()
{
	return m_isExpanded;
}