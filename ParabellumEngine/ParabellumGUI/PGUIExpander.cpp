#include "stdafx.h"
#include "PGUIExpander.h"

//	--------------------------------------------------------------------
//		Statics
//	--------------------------------------------------------------------

//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumUserInterface::PGUIExpander::PGUIExpander(
	_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager)
	: PGUIDrawableObject(pDevice, pResourceManager)
{
	SetVisibility(false);
	SetPosition(0.0_uif, 0.0_uif);
	SetSize(1.0_uif, 1.0_uif);

	m_isMultiselecting = false;
}



ParabellumUserInterface::PGUIExpander::~PGUIExpander()
{
	for (EUINT32 i = 0; i < m_expandBars.size(); ++i)
		delete m_expandBars[i];
}

//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------


void 
ParabellumUserInterface::PGUIExpander::AddExpandBar(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager, const char* label)
{
	PGUIExpandBar* expandBar = new PGUIExpandBar(pDevice, pResourceManager);

	if (expandBar != NULLPTR)
	{
		if (m_expandBars.size() == 0)
		{
			expandBar->SetParent(this);
			expandBar->SetPosition(0.0_uif, 0.0_uif);
			expandBar->SetSize(1.0_uip, 0.3_uif);
			expandBar->SetPlacementType(PGUIPlacementType::TopLeft);
			expandBar->SetPositionAspect(PGUIPositionAspect::Relative);
		}
		else
		{
			expandBar->SetParent(m_expandBars[m_expandBars.size() - 1]);
			expandBar->SetPosition(0.0_uif, 1.0_uip);
			expandBar->SetSize(1.0_uip, 0.3_uif);
			expandBar->SetPlacementType(PGUIPlacementType::TopLeft);
			expandBar->SetPositionAspect(PGUIPositionAspect::Relative);
		}

		expandBar->SetLabel(label);
		expandBar->Expand(false);

		m_expandBars.push_back(expandBar);
	}

}

void 
ParabellumUserInterface::PGUIExpander::AddObject(EUINT32 no, PGUIDrawableObject* pPGUIObject)
{
	if (no < m_expandBars.size())
	{
		m_expandBars[no]->AddObject(pPGUIObject);
	}
}


void
ParabellumUserInterface::PGUIExpander::SetContentVisibility(EUINT32 no, EBOOL val)
{
	if (no < m_expandBars.size())
	{
		//hide all
		if ((m_isMultiselecting == false) && (val == true))
			for (auto it : m_expandBars)
				it->Expand(false);

		// show our
		m_expandBars[no]->Expand(val);
	}
}




//	--------------------------------------------------------------------
//		Gets And sets
//	--------------------------------------------------------------------
