#include "stdafx.h"
#include "Pivot.h"

//	------------------------------------------------------------
//		Constructors and destructors
//	------------------------------------------------------------
ParabellumEngine::Editor::Pivot::Pivot(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager)
{
	pivotData.scale = 1.0f / 16.0f;

	axisX.shader = pResourceManager->Load<Shader>("Resources/Shaders/DrawSimpleBox.psx");
	axisY.shader = pResourceManager->Load<Shader>("Resources/Shaders/DrawSimpleBox.psx");
	axisZ.shader = pResourceManager->Load<Shader>("Resources/Shaders/DrawSimpleBox.psx");

	axisX.model = pResourceManager->Load<Model>("Resources/Editor/arrowX.pfm");
	axisY.model = pResourceManager->Load<Model>("Resources/Editor/arrowY.pfm");
	axisZ.model = pResourceManager->Load<Model>("Resources/Editor/arrowZ.pfm");

	axisX.SetColor(Vector4D(1.0f, 0.0f, 0.0f, 1.0f));
	axisY.SetColor(Vector4D(0.0f, 1.0f, 0.0f, 1.0f));
	axisZ.SetColor(Vector4D(0.0f, 0.0f, 1.0f, 1.0f));

	axisX.SetBegin(Vector4D(0.0f, 0.0f, 0.0f, 0.0f));
	axisY.SetBegin(Vector4D(0.0f, 0.0f, 0.0f, 0.0f));
	axisZ.SetBegin(Vector4D(0.0f, 0.0f, 0.0f, 0.0f));

	axisX.SetEnd(Vector4D(1.0f, 0.0f, 0.0f, 0.0f));
	axisY.SetEnd(Vector4D(0.0f, 1.0f, 0.0f, 0.0f));
	axisZ.SetEnd(Vector4D(0.0f, 0.0f, 1.0f, 0.0f));
}

ParabellumEngine::Editor::Pivot::~Pivot()
{
}

//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------

void
ParabellumEngine::Editor::Pivot::Update(
	_IN_ GraphicsDevice* pDevice, _IN_ InputDevice* pInputDevice, _IN_ ParabellumEngine::Components::Camera* pCamera, Vector2D& mouseUV)
{
	axisX.Update(pDevice, pInputDevice, pCamera, mouseUV);
	axisY.Update(pDevice, pInputDevice, pCamera, mouseUV);
	axisZ.Update(pDevice, pInputDevice, pCamera, mouseUV);
}


void 
ParabellumEngine::Editor::Pivot::Draw(_IN_ GraphicsDevice* pDevice, _IN_ InputDevice* pInputDevice, _IN_ ParabellumEngine::Components::Camera* pCamera, Vector2D& mouseUV)
{
	axisX.Draw(pDevice, pInputDevice, pCamera, mouseUV);
	axisY.Draw(pDevice, pInputDevice, pCamera, mouseUV);
	axisZ.Draw(pDevice, pInputDevice, pCamera, mouseUV);
}





//	------------------------------------------------------------
//		Gets and Sets
//	------------------------------------------------------------

void
ParabellumEngine::Editor::Pivot::SetPosition(_IN_ Vector4D& input)
{
	m_positionBegin = input;

	axisX.SetBegin(input);
	axisY.SetBegin(input);
	axisZ.SetBegin(input);
}

void
ParabellumEngine::Editor::Pivot::Move(_IN_ Vector4D& input)
{
	m_positionBegin += input;

	axisX.Move(input);
	axisY.Move(input);
	axisZ.Move(input);
}


ParabellumFramework::EBOOL
ParabellumEngine::Editor::Pivot::IsSelected()
{
	if (markedAxis == PE_AxisType::None)
		return false;
	return true;
}
