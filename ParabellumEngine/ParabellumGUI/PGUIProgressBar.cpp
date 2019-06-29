#include "stdafx.h"
#include "PGUIProgressBar.h"

//	--------------------------------------------------------------------
//		Statics
//	--------------------------------------------------------------------

//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumUserInterface::PGUIProgressBar::PGUIProgressBar(
	_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager)
	: PGUIExtDrawableObject(pDevice, pResourceManager)
{
	m_minValue = 0.0f;
	m_maxValue = 5.0f;
	m_currentValue = 0.0f;

	SetVisibility(true);

	m_label = new PGUIText(pDevice, pResourceManager);
	m_label->SetParent(this);
	m_label->SetPosition(0.5_uip, 0.5_uip);
	m_label->SetPlacementType(PGUIPlacementType::TopLeft);
	m_label->SetPositionAspect(PGUIPositionAspect::Relative);
	m_label->SetText("0");

	m_progress = new PGUIDrawableObject(pDevice, pResourceManager);
	m_progress->SetParent(this);
	m_progress->SetPosition(0.0_uif, 0.0_uif);
	m_progress->SetSize(1.0_uip, 1.00_uip);
	m_progress->SetPlacementType(PGUIPlacementType::TopLeft);
	m_progress->SetPositionAspect(PGUIPositionAspect::Relative);
}



ParabellumUserInterface::PGUIProgressBar::~PGUIProgressBar()
{
}

//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------

ParabellumFramework::EFLOAT32
ParabellumUserInterface::PGUIProgressBar::GetPercentageValue()
{
	return m_currentValue;
}

ParabellumFramework::EFLOAT32
ParabellumUserInterface::PGUIProgressBar::GetAbsoluteValue()
{
	return m_currentValue;
}


ParabellumFramework::EFLOAT32
ParabellumUserInterface::PGUIProgressBar::GetMin()
{
	return m_minValue;
}


ParabellumFramework::EFLOAT32
ParabellumUserInterface::PGUIProgressBar::GetMax()
{
	return m_maxValue;
}








void 
ParabellumUserInterface::PGUIProgressBar::SetPercentageValue(EFLOAT32 val)
{
	EFLOAT32 cutedVal = val;

	if (cutedVal < 0.0f) cutedVal = 0.0f;
	if (cutedVal > 1.0f) cutedVal = 1.0f;

	m_currentValue = (m_maxValue - m_minValue) * cutedVal + m_minValue;

	PGUIUnitData width;
	width.Unit = PGUIUnit::Percentage;
	width.Value = (m_currentValue - m_minValue) / (m_maxValue - m_minValue);

	m_progress->SetWidth(width);

	m_label->SetText(std::to_string(m_currentValue).c_str());
}


void
ParabellumUserInterface::PGUIProgressBar::SetAbsoluteValue(EFLOAT32 val)
{
	m_currentValue = val;

	if (m_currentValue < m_minValue) m_currentValue = m_minValue;
	if (m_currentValue > m_maxValue) m_currentValue = m_maxValue;

	PGUIUnitData width;
	width.Unit = PGUIUnit::Percentage;

	width.Value = (m_currentValue - m_minValue) / (m_maxValue - m_minValue);

	m_progress->SetWidth(width);

	m_label->SetText(std::to_string(m_currentValue).c_str());
}



void
ParabellumUserInterface::PGUIProgressBar::SetMin(EFLOAT32 val)
{
	m_minValue = val;
}

void
ParabellumUserInterface::PGUIProgressBar::SetMax(EFLOAT32 val)
{
	m_maxValue = val;
}



//	--------------------------------------------------------------------
//		Gets And sets
//	--------------------------------------------------------------------
