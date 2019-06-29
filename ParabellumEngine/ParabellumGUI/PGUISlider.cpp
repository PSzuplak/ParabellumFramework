#include "stdafx.h"
#include "PGUISlider.h"

//	--------------------------------------------------------------------
//		Statics
//	--------------------------------------------------------------------

//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumUserInterface::PGUISlider::PGUISlider(
	_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager)
	: PGUIProgressBar(pDevice, pResourceManager)
{

	AddEventMouseClick
	(
		[](void* owner, void* args)
		{
			PUIEventMouse* e = (PUIEventMouse*)args;

			((PGUISlider*)owner)->SetValue(e->MouseX, e->MouseY);
		}
	);
}



ParabellumUserInterface::PGUISlider::~PGUISlider()
{
}

//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------
void 
ParabellumUserInterface::PGUISlider::SetValue(EFLOAT32 mx, EFLOAT32 my)
{
	Vector4D position, size;

	GetPosition(position);
	GetSize(size);

	EFLOAT32 fx = (mx - position.X) / size.X;

	SetPercentageValue(fx);
}


//	--------------------------------------------------------------------
//		Gets And sets
//	--------------------------------------------------------------------
