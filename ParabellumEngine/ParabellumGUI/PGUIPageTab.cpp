#include "stdafx.h"
#include "PGUIPageTab.h"

//	--------------------------------------------------------------------
//		Statics
//	--------------------------------------------------------------------

//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumUserInterface::PGUIPageTab::PGUIPageTab(
	_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager)
	: PGUIDrawableObject(pDevice, pResourceManager)
{
	SetVisibility(false);

	m_selectedPage = -1;
}



ParabellumUserInterface::PGUIPageTab::~PGUIPageTab()
{
	for (EUINT32 i = 0; i < m_pages.size(); ++i)
	{
		delete m_selectors[i];
		delete m_pages[i];
	}
}

//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------
void 
ParabellumUserInterface::PGUIPageTab::AddTab(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager, const char* str)
{
	PGUIPageTab* thisPtr = this;
	PGUIButton* selector;
	PGUIGroupBox* page;
	EUINT32 selectedIndex = -1;

	// memory alloc
	selector = new PGUIButton(pDevice, pResourceManager);

	if (selector == NULLPTR) 
		return;

	page = new PGUIGroupBox(pDevice, pResourceManager);

	if (page == NULLPTR) 
	{
		delete selector;
		return;
	}
		
	//
	// set selector
	if (m_selectors.size() == 0)
	{
		selector->SetParent(this);
		selector->SetPosition(0.0_uif, 0.00_uif);
		selector->SetSize(0.08_uif, 0.02_uif);
		selector->SetPlacementType(PGUIPlacementType::BottomLeft);
		selector->SetPositionAspect(PGUIPositionAspect::Relative);
		selector->SetLabel(str);
	}
	else
	{
		selector->SetParent(m_selectors[m_selectors.size()-1]);
		selector->SetPosition(1.0_uip, 0.0_uif);
		selector->SetSize(0.08_uif, 0.02_uif);
		selector->SetPlacementType(PGUIPlacementType::TopLeft);
		selector->SetPositionAspect(PGUIPositionAspect::Relative);
		selector->SetLabel(str);
	}


	m_selectors.push_back(selector);

	selectedIndex = m_selectors.size() - 1;

	// select page func - 
	selector->AddEventMouseClick
		(
			[thisPtr, selectedIndex](void* owner, void* args)
			{
				((PGUIPageTab*)thisPtr)->SelectPage(selectedIndex);
			}
		);


	//
	// set page
	page = new PGUIGroupBox(pDevice, pResourceManager);
	page->SetParent(this);
	page->SetPosition(0.0_uif, 0.00_uif);
	page->SetSize(1.00_uip, 1.00_uip);
	page->SetPlacementType(PGUIPlacementType::TopLeft);
	page->SetPositionAspect(PGUIPositionAspect::Relative);
	page->SetDisplayable(false);
	

	m_pages.push_back(page);
}


void
ParabellumUserInterface::PGUIPageTab::AddToPage(EUINT32 no, _IN_ PGUIDrawableObject* pGUIObj)
{
	if ((no < m_pages.size()) && (no >= 0))
	{
		pGUIObj->SetParent(m_pages[no]);
	}
}


void
ParabellumUserInterface::PGUIPageTab::SelectPage(EINT32 no)
{
	if ((no < m_pages.size()) && (no >= 0))
	{
		if (m_selectedPage != -1)
			m_pages[m_selectedPage]->SetDisplayable(false);

		m_selectedPage = no;
		m_pages[no]->SetDisplayable(true);
	}
}





//	--------------------------------------------------------------------
//		Gets And sets
//	--------------------------------------------------------------------
