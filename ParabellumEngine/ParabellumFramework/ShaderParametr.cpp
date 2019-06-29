#include "stdafx.h"
#include "ShaderParametr.h"

ParabellumFramework::Graphics::ShaderParametr::ShaderParametr()
{
	m_parametr = NULLPTR;
	m_parametrSize = 0;
}

ParabellumFramework::Graphics::ShaderParametr::ShaderParametr(ShaderParametr&)
{
}

ParabellumFramework::Graphics::ShaderParametr::~ShaderParametr()
{
}

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::ShaderParametr::Initialize()
{

	return SystemCodes::SUCCESS;
}

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::ShaderParametr::Release()
{

	return SystemCodes::SUCCESS;
}


ParabellumFramework::EUINT32
ParabellumFramework::Graphics::ShaderParametr::GetSize()
{
	return m_parametrSize;
}


void 
ParabellumFramework::Graphics::ShaderParametr::SetParametr(void* ptr)
{
	m_parametr = ptr;
}

void 
ParabellumFramework::Graphics::ShaderParametr::SetSize(EUINT32 value)
{
	m_parametrSize = value;
}





template<>
void ParabellumFramework::Graphics::ShaderParametr::Set(Texture2D* ptr)
{
	m_parametr = *((char**)ptr->GetAddressOBufferType());
}

template<>
void ParabellumFramework::Graphics::ShaderParametr::Set(RenderTarget* ptr)
{
	m_parametr = ptr->m_rtBufferResource;
}
