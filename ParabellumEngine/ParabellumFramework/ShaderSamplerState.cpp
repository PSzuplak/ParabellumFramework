#include "stdafx.h"
#include "ShaderSamplerState.h"

ParabellumFramework::Graphics::ShaderSamplerState::ShaderSamplerState()
{

}

ParabellumFramework::Graphics::ShaderSamplerState::ShaderSamplerState(const ShaderSamplerState&)
{
}

ParabellumFramework::Graphics::ShaderSamplerState::~ShaderSamplerState()
{
	/*
	if(m_parametr != NULL)
	{
	delete m_parametr;
	m_parametr = NULL;
	}
	*/
}

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::ShaderSamplerState::Initialize(
	SamplerFilter filter,
	SamplerAddress addressU,
	SamplerAddress addressV,
	SamplerAddress addressW,
	EFLOAT32 mipLODBias,
	EUINT32 maxAnisotropy,
	SamplerComparsion comparisonFunc)
{
	m_buffer = NULLPTR;

	m_filter = filter;
	m_addressU = addressU;
	m_addressV = addressV;
	m_addressW = addressW;
	m_mipLODBias = mipLODBias;
	m_maxAnisotropy = maxAnisotropy;
	m_comparisonFunc = comparisonFunc;
	m_borderColor[0] = 0;
	m_borderColor[1] = 0;
	m_borderColor[2] = 0;
	m_borderColor[3] = 0;
	m_minLOD = 0.0f;
	m_maxLOD = 3.402823466e+38f;


	return SystemCodes::SUCCESS;
}

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::ShaderSamplerState::Initialize(
	SamplerFilter filter,
	SamplerAddress addressU,
	SamplerAddress addressV,
	SamplerAddress addressW,
	EFLOAT32 mipLODBias,
	EUINT32 maxAnisotropy,
	SamplerComparsion comparisonFunc,
	EUINT32 borderColor0, EUINT32 borderColor1, EUINT32 borderColor2, EUINT32 borderColor3,
	EUINT32 minLOD,
	EUINT32 maxLOD)
{
	m_buffer = NULLPTR;

	m_filter = filter;
	m_addressU = addressU;
	m_addressV = addressV;
	m_addressW = addressW;
	m_mipLODBias = mipLODBias;
	m_maxAnisotropy = maxAnisotropy;
	m_comparisonFunc = comparisonFunc;
	m_borderColor[0] = borderColor0;
	m_borderColor[1] = borderColor1;
	m_borderColor[2] = borderColor2;
	m_borderColor[3] = borderColor3;
	m_minLOD = minLOD;
	m_maxLOD = maxLOD;


	return SystemCodes::SUCCESS;
}



void* 
ParabellumFramework::Graphics::ShaderSamplerState::GetAddressOfBuffer()
{
	return &m_buffer;
}