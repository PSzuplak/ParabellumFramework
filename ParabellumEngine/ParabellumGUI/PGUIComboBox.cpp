#include "stdafx.h"
#include "PGUIComboBox.h"

//	--------------------------------------------------------------------
//		Statics
//	--------------------------------------------------------------------

//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumUserInterface::PGUIComboBox::PGUIComboBox(
	_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager)
	: PGUIDrawableObject(pDevice, pResourceManager)
{
	SetVisibility(false);
	

	m_boxBtn = new PGUIButton(pDevice, pResourceManager);
	m_boxBtn->SetParent(this);
	m_boxBtn->SetPosition(0.0_uif, 0.0_uif);
	m_boxBtn->SetSize(1.0_uip, 1.00_uip);
	m_boxBtn->SetPlacementType(PGUIPlacementType::TopLeft);
	m_boxBtn->SetPositionAspect(PGUIPositionAspect::Relative);

	Show(false);

	//
	// event to expand the bar

	m_boxBtn->AddEventMouseClick
	(
		[](void* owner, void* args)
		{
			((PGUIComboBox*)((PGUIButton*)owner)->GetParent())->Turn();
		}
	);
}



ParabellumUserInterface::PGUIComboBox::~PGUIComboBox()
{
	for (EUINT32 i = 0; i < m_allocatedObjects.size(); ++i)
		delete m_allocatedObjects[i];

	delete m_boxBtn;
}

//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------
void 
ParabellumUserInterface::PGUIComboBox::Add(_IN_ PGUIDrawableObject* pPUIObj)
{

	PGUIComboBox* thisPtr = this;
	EUINT32 currentIndex = m_objects.size();

	if (m_objects.size() == 0)
	{
		pPUIObj->SetParent(this);
		pPUIObj->SetPosition(0.0_uif, 1.0_uip);
		pPUIObj->SetSize(1.0_uip, 0.02_uif);
		pPUIObj->SetPlacementType(PGUIPlacementType::TopLeft);
		pPUIObj->SetPositionAspect(PGUIPositionAspect::Relative);
	}
	else
	{
		pPUIObj->SetParent(m_objects[m_objects.size() - 1]);
		pPUIObj->SetPosition(0.0_uif, 1.0_uip);
		pPUIObj->SetSize(1.0_uip, 0.02_uif);
		pPUIObj->SetPlacementType(PGUIPlacementType::TopLeft);
		pPUIObj->SetPositionAspect(PGUIPositionAspect::Relative);
	}

	m_objects.push_back(pPUIObj);

	Show(false);

	pPUIObj->AddEventMouseClick
	(
		[thisPtr, currentIndex](void* owner, void* args)
		{
			((PGUIComboBox*)thisPtr)->SelectIndex(currentIndex);
			((PGUIComboBox*)thisPtr)->Show(false);
		}
	);
}


void
ParabellumUserInterface::PGUIComboBox::Add(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager, const char* str)
{
	PGUIButton* btn = new PGUIButton(pDevice, pResourceManager);

	if (btn != NULLPTR)
	{
		btn->SetLabel(str);

		m_allocatedObjects.push_back(btn);
		Add(btn);
	}
}

void
ParabellumUserInterface::PGUIComboBox::SelectIndex(EUINT32 index)
{
	if (m_selectedIndex == index)
		return;

	m_selectedIndex = index;

	// invoke on change
	InvokeOnChange();
}


void
ParabellumUserInterface::PGUIComboBox::Show(EBOOL val)
{
	if (m_objects.size() > 0)
	{
		m_objects[0]->SetDisplayable(val);
	}

	m_isExpanded = val;
}


// show/hide content
void
ParabellumUserInterface::PGUIComboBox::Turn()
{
	m_isExpanded = !m_isExpanded;
	Show(m_isExpanded);
}



//	--------------------------------------------------------------------
//		Gets And sets
//	--------------------------------------------------------------------

ParabellumFramework::EINT32 
ParabellumUserInterface::PGUIComboBox::GetSelectedIndex()
{
	return m_selectedIndex;
}