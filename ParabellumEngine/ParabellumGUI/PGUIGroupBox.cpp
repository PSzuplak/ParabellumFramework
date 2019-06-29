#include "stdafx.h"
#include "PGUIGroupBox.h"

//	--------------------------------------------------------------------
//		Statics
//	--------------------------------------------------------------------

//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumUserInterface::PGUIGroupBox::PGUIGroupBox(
	_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager)
	: PGUIExtDrawableObject(pDevice, pResourceManager)
{
	viewport.isEnabled = true;
}



ParabellumUserInterface::PGUIGroupBox::~PGUIGroupBox()
{
}

//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------



//	--------------------------------------------------------------------
//		Gets And sets
//	--------------------------------------------------------------------
