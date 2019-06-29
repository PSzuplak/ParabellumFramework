#include "stdafx.h"
#include "PGUIButton.h"

//	--------------------------------------------------------------------
//		Statics
//	--------------------------------------------------------------------

//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumUserInterface::PGUIButton::PGUIButton(
	_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager)
	: PGUIDrawableObject(pDevice, pResourceManager)
{
	m_label = new PGUIText(pDevice, pResourceManager);
	m_label->SetParent(this);

	m_label->SetPosition(0.005_uif, 0.0025_uif);
	m_label->SetPlacementType(PGUIPlacementType::TopLeft);
	m_label->SetPositionAspect(PGUIPositionAspect::Relative);

	m_label->SetText("");	

	//
	//
	SetSkingBackgroundCoords(Vector4D(0.03515625f, 0.720703125f, 0.056640625f, 0.744140625f));
	SetSkinForegroundCoords(Vector4D(0.03515625f, 0.720703125f, 0.056640625f, 0.744140625f));

	//
	// events
	AddEventMouseOver
	(
		[](void* owner, void* args)
		{
			//((PGUIButton*)owner)->SetSkingBackgroundCoords(Vector4D(0.03515625f, 0.66015625f, 0.056640625f, 0.68359375f));
			//((PGUIButton*)owner)->SetSkinForegroundCoords(Vector4D(0.03515625f, 0.66015625f, 0.056640625f, 0.68359375f));
			((PGUIButton*)owner)->SetColor(Vector4D(1.0f, 0.0f, 0.0f, 1.0f));
		}
	);

	AddEventMouseOut
	(
		[](void* owner, void* args)
		{
			//((PGUIButton*)owner)->SetSkingBackgroundCoords(Vector4D(0.03515625f, 0.720703125f, 0.056640625f, 0.744140625f));
			//((PGUIButton*)owner)->SetSkinForegroundCoords(Vector4D(0.03515625f, 0.720703125f, 0.056640625f, 0.744140625f));
			((PGUIButton*)owner)->SetColor(Vector4D(1.0f, 1.0f, 1.0f, 1.0f));
		}
	);

}



ParabellumUserInterface::PGUIButton::~PGUIButton()
{
	delete m_label;
}

//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------



//	--------------------------------------------------------------------
//		Gets And sets
//	--------------------------------------------------------------------
void
ParabellumUserInterface::PGUIButton::SetLabel(const char* str)
{
	m_label->SetText(str);
}

const char* 
ParabellumUserInterface::PGUIButton::GetLabel()
{
	return m_label->GetText();
}