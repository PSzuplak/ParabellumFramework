#include "stdafx.h"
#include "Shader.h"



//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumFramework::Graphics::Shader::ShaderCBuffer::ShaderCBuffer()
{
	m_buffer = NULLPTR;
	m_size = 0;
	m_variablesCount = 0;
	m_variables = NULLPTR;
	m_bufferNo = 0;
	m_sortedPointersToResources = NULLPTR;
}

ParabellumFramework::Graphics::Shader::ShaderCBuffer::ShaderCBuffer(const ShaderCBuffer& ref)
{

}

ParabellumFramework::Graphics::Shader::ShaderCBuffer::~ShaderCBuffer()
{
}
