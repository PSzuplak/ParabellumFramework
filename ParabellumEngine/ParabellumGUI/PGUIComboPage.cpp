#include "stdafx.h"
#include "PGUIComboPage.h"

//	--------------------------------------------------------------------
//		Statics
//	--------------------------------------------------------------------

//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumUserInterface::PGUIComboPage::PGUIComboPage(
	_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager)
	: PGUIDrawableObject(pDevice, pResourceManager)
{
	SetVisibility(false);
	
	m_selectedPage = -1;

	m_selector = new PGUIComboBox(pDevice, pResourceManager);
	m_selector->SetParent(this);
	m_selector->SetPosition(0.0_uif, 0.0_uif);
	m_selector->SetSize(1.0_uip, 0.02_uif);
	m_selector->SetPlacementType(PGUIPlacementType::TopLeft);
	m_selector->SetPositionAspect(PGUIPositionAspect::Relative);

	//
	// Add event for selector

	m_selector->AddEventChange
	(
		[](void* owner, void* args)
		{
			EINT32 index = ((PGUIComboBox*)owner)->GetSelectedIndex();
			((PGUIComboPage*)((PGUIComboBox*)owner)->GetParent())->SelectPage(index);
		}
	);



}


ParabellumUserInterface::PGUIComboPage::~PGUIComboPage()
{
	for (EUINT32 i = 0; i < m_pages.size(); ++i)
	{
		delete m_pages[i];
		delete m_verticalScrolls[i];
	}

	delete m_selector;
}

//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------
void 
ParabellumUserInterface::PGUIComboPage::AddPage(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager, const char* str)
{
	PGUIButton* elem = new PGUIButton(pDevice, pResourceManager);
	elem->SetLabel(str);
	m_selector->Add(elem);

	PGUIGroupBox* page = new PGUIGroupBox(pDevice, pResourceManager);
	page->SetParent(this);
	page->SetPosition(0.0_uif, 0.02_uif);
	page->SetSize(1.0_uip, 1.00_uip);
	page->SetPlacementType(PGUIPlacementType::TopLeft);
	page->SetPositionAspect(PGUIPositionAspect::Relative);
	page->SetDisplayable(false);

	m_pages.push_back(page);

	PGUIScrollBar* scroll = new PGUIScrollBar(pDevice, pResourceManager);
	scroll->SetParent(page);
	
	m_verticalScrolls.push_back(scroll);
}



void 
ParabellumUserInterface::PGUIComboPage::AddToPage(EUINT32 no, PGUIDrawableObject* pPUIObj)
{
	if ( (no < m_pages.size()) && (no >= 0) )
	{
		pPUIObj->SetParent(m_pages[no]);
	}
}


void ParabellumUserInterface::PGUIComboPage::SelectPage(EINT32 no)
{
	if ( (no < m_pages.size()) && (no >= 0) )
	{
		if(m_selectedPage != -1)
			m_pages[m_selectedPage]->SetDisplayable(false);
		
		m_selectedPage = no;
		m_pages[no]->SetDisplayable(true);
	}
}


//	--------------------------------------------------------------------
//		Gets And sets
//	--------------------------------------------------------------------
