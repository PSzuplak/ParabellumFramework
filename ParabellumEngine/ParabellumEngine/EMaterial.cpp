#include "EMaterial.h"

using namespace ParabellumFramework;



//	------------------------------------------------------------
//		Constructors and destructors
//	------------------------------------------------------------
ParabellumEngine::Components::EMaterial::EMaterial()
{
	m_T2D00 = NULLPTR;
	m_T2D01 = NULLPTR;
	m_T2D02 = NULLPTR;
}



ParabellumEngine::Components::EMaterial::~EMaterial()
{
}


//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------


ParabellumFramework::EBOOL
ParabellumEngine::Components::EMaterial::IsBumped()
{
	if ((m_isParallax == true) && (m_T2D02 != NULLPTR))
		return true;

	return false;
}


ParabellumFramework::EBOOL 
ParabellumEngine::Components::EMaterial::IsParallaxed()
{
	if ((m_isParallax == true) && (m_T2D02 != NULLPTR))
		return true;

	return false;
}

ParabellumFramework::EBOOL
ParabellumEngine::Components::EMaterial::IsReflective()
{
	if ((m_isParallax == true) && (m_T2D02 != NULLPTR))
		return true;

	return false;
}

ParabellumFramework::EBOOL
ParabellumEngine::Components::EMaterial::IsRefractive()
{
	if ((m_isParallax == true) && (m_T2D02 != NULLPTR))
		return true;

	return false;
}