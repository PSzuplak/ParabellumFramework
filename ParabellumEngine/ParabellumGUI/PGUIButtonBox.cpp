#include "stdafx.h"
#include "PGUIButtonBox.h"

//	--------------------------------------------------------------------
//		Statics
//	--------------------------------------------------------------------

//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumUserInterface::PGUIButtonBox::PGUIButtonBox(
	_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager)
	: PGUIDrawableObject(pDevice, pResourceManager)
{
	m_buttonsContainer = new PGUIGroupBox(pDevice, pResourceManager);
	m_buttonsContainer->SetParent(this);
	m_buttonsContainer->SetPosition(0.0_uif, 0.2_uif);
	m_buttonsContainer->SetSize(1.00_uip, 0.2_uif);
	m_buttonsContainer->SetPlacementType(PGUIPlacementType::TopLeft);
	m_buttonsContainer->SetPositionAspect(PGUIPositionAspect::Relative);
}



ParabellumUserInterface::PGUIButtonBox::~PGUIButtonBox()
{
	for (EUINT32 i = 0; m_content.size(); ++i)
		delete m_content[i];


	for (EUINT32 i = 0; m_buttons.size(); ++i)
		delete m_buttons[i];

	delete m_buttonsContainer;
}

//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------

void 
ParabellumUserInterface::PGUIButtonBox::AddButton(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager, Vector4D skinUV)
{
	PGUIButton* btn;

	EUINT32 currentIndex = m_buttons.size();

	btn = new PGUIButton(pDevice, pResourceManager);
	btn->SetParent(m_buttonsContainer);
	btn->SetPosition(0.0_uif, 0.0_uif);
	btn->SetSize(0.03_uif, 0.04_uif);
	btn->SetPlacementType(PGUIPlacementType::TopLeft);
	btn->SetPositionAspect(PGUIPositionAspect::Inline);
	btn->SetSkingBackgroundCoords(skinUV);
	btn->SetSkinForegroundCoords(skinUV);
	
	btn->AddEventMouseClick
	(
		[currentIndex](void* owner, void* args)
		{
			((PGUIButtonBox*)(((PGUIGroupBox*)(((PGUIButton*)owner)->GetParent()))->GetParent()))->SelectIndex(currentIndex);
		}
	);

	m_buttons.push_back(btn);


	//
	// containers
	PGUIGroupBox* gb;

	gb = new PGUIGroupBox(pDevice, pResourceManager);
	gb->SetParent(this);
	gb->SetPosition(0.0_uif, 0.1_uif);
	gb->SetSize(1.00_uif, 0.3_uif);
	gb->SetPlacementType(PGUIPlacementType::TopLeft);
	gb->SetPositionAspect(PGUIPositionAspect::Relative);
	gb->SetDisplayable(false);

	m_content.push_back(gb);

}



void 
ParabellumUserInterface::PGUIButtonBox::Add(EUINT32 containerNo, _IN_ PGUIDrawableObject* pGUIObj)
{
	if ( (m_content.size() > 0) && (m_content.size() < containerNo) )
	{
		pGUIObj->SetParent(m_content[containerNo]);
	}
}




void
ParabellumUserInterface::PGUIButtonBox::SelectIndex(EINT32 index)
{
	for (auto it : m_content)
		it->SetDisplayable(false);
	
	m_content[index]->SetDisplayable(true);

	m_selectedIndex = index;
}

//	--------------------------------------------------------------------
//		Gets And sets
//	--------------------------------------------------------------------
