#include "BaseLight.h"



//	------------------------------------------------------------
//		Constructors and destructors
//	------------------------------------------------------------
ParabellumEngine::Components::BaseLight::BaseLight()
{
	AddTag("Light");
}


ParabellumEngine::Components::BaseLight::~BaseLight()
{
}

//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------


//	------------------------------------------------------------
//		Gets and Sets
//	------------------------------------------------------------

ParabellumFramework::Vector4D*
ParabellumEngine::Components::BaseLight::GetColor()
{
	return &m_lightColor;
}


ParabellumFramework::EFLOAT32
ParabellumEngine::Components::BaseLight::GetIntensity()
{
	return m_intensity;
}



ParabellumFramework::EBOOL
ParabellumEngine::Components::BaseLight::IsThrowingShadows()
{
	return m_isThrowShadows;
}