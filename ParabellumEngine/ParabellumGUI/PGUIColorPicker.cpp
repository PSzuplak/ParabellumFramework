#include "stdafx.h"
#include "PGUIColorPicker.h"

//	--------------------------------------------------------------------
//		Statics
//	--------------------------------------------------------------------

//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumUserInterface::PGUIColorPicker::PGUIColorPicker(
	_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager)
	: PGUIDrawableObject(pDevice, pResourceManager)
{

	SetSkingBackgroundCoords(Vector4D(0.5f, 0.3125f, 0.75f, 0.5625f));
	SetSkinForegroundCoords(Vector4D(0.5f, 0.3125f, 0.75f, 0.5625f));

	m_hsl.Set(0.0f, 0.0f, 0.0f);
	m_rgb.Set(0.0f, 0.0f, 0.0f);

	AddEventMouseClick
	(
		[](void* owner, void* args)
		{
			PUIEventMouse* e = (PUIEventMouse*)args;

			((PGUIColorPicker*)owner)->CalculateValue(e->MouseX, e->MouseY);
		}
	);
}



ParabellumUserInterface::PGUIColorPicker::~PGUIColorPicker()
{
}

//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------
void 
ParabellumUserInterface::PGUIColorPicker::CalculateValue(EFLOAT32 mx, EFLOAT32 my)
{
	Vector4D position, size;
	Vector2D vM, vW, right;
	Vector2D polar;

	GetPosition(position);
	GetSize(size);

	right.X = 0.5f;
	right.Y = 0.0f;

	// calculate mouse positon from center
	vM.X = ( ( (mx - position.X) / size.X ) - 0.5f ) * 2.0f;
	vM.Y = (1.0f - ( (my - position.Y) / size.Y ) - 0.5f) * 2.0f;


	//vM.Normalize();
	UnitHelper::CartesianToPolar2D(vM, polar);

	polar.Y = ToDegrees(polar.Y);

	m_hsl.X = polar.Y;
	m_hsl.Y = polar.X;
	m_hsl.Z = 0.5f;


	Math::UnitHelper::HSLtoRGB(m_hsl, m_rgb);
}


//	--------------------------------------------------------------------
//		Gets And sets
//	--------------------------------------------------------------------
