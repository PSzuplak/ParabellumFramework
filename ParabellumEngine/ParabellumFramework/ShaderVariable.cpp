#include "stdafx.h"
#include "Shader.h"



ParabellumFramework::Graphics::Shader::ShaderVariable::ShaderVariable(ShaderVariable&)
{
}


ParabellumFramework::Graphics::Shader::ShaderVariable::ShaderVariable()
{
}

void ParabellumFramework::Graphics::Shader::ShaderVariable::Release()
{
	m_size = 0;
	m_key = 0;
}

void ParabellumFramework::Graphics::Shader::ShaderVariable::Initialize(std::string& name, EUINT32 type, EUINT32 size)
{
	m_key = ParabellumUtils::Math::GenerateHash(name.c_str());
	m_name = name;
	m_type = type;
	m_size = size;
}


ParabellumFramework::Graphics::Shader::ShaderVariable::~ShaderVariable()
{
}


ParabellumFramework::EUINT32 ParabellumFramework::Graphics::Shader::ShaderVariable::GetType()
{
	return m_type;
}

ParabellumFramework::EUINT32 ParabellumFramework::Graphics::Shader::ShaderVariable::GetSize()
{
	return m_size;
}

ParabellumFramework::EUINT64 ParabellumFramework::Graphics::Shader::ShaderVariable::GetKey()
{
	return m_key;
}

std::string& ParabellumFramework::Graphics::Shader::ShaderVariable::GetName()
{
	return m_name;
}