#include "stdafx.h"
#include "ShaderLayout.h"



ParabellumFramework::Graphics::ShaderLayout::ShaderLayout()
{
	m_semanticType = 0;
	m_semanticIndex = 0;
	m_format = 0;
	m_inputSlot = 0;
	m_alignedByteOffset = 0;
	m_inputSlotClass = 0;
	m_instanceDataStepRate = 0;
}

ParabellumFramework::Graphics::ShaderLayout::ShaderLayout(const ShaderLayout&)
{

}

ParabellumFramework::Graphics::ShaderLayout::~ShaderLayout()
{

}




ParabellumFramework::EUINT32 
ParabellumFramework::Graphics::ShaderLayout::Initialize(
	std::string& semanticName, EUINT32 semanticType, EUINT32 index, EUINT32 format,
	EUINT32 inputSlot, EUINT32 allignetByteOffset,
	EUINT32 inputSlottClass, EUINT32 instanceDataStepRate)
{
	m_semanticName = semanticName;
	m_semanticType = semanticType;
	m_semanticIndex = index;
	m_format = format;
	m_inputSlot = inputSlot;
	m_alignedByteOffset = allignetByteOffset;
	m_inputSlotClass = inputSlottClass;
	m_instanceDataStepRate = instanceDataStepRate;

	return SystemCodes::SUCCESS;
}


void 
ParabellumFramework::Graphics::ShaderLayout::Release()
{

}



void* ParabellumFramework::Graphics::ShaderLayout::GetBuffer()
{
	return &m_buffer;
}


std::string& 
ParabellumFramework::Graphics::ShaderLayout::GetSemanticName()
{
	return m_semanticName;
}

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::ShaderLayout::GetSemanticType()
{
	return m_semanticType;
}

ParabellumFramework::EUINT32 
ParabellumFramework::Graphics::ShaderLayout::GetSemanticIndex()
{
	return m_semanticIndex;
}


ParabellumFramework::EUINT32 
ParabellumFramework::Graphics::ShaderLayout::GetFormat()
{
	return m_format;
}


ParabellumFramework::EUINT32 
ParabellumFramework::Graphics::ShaderLayout::GetInputSlot()
{
	return m_inputSlot;
}


ParabellumFramework::EUINT32 
ParabellumFramework::Graphics::ShaderLayout::GetAlignedByteOffset()
{
	return m_alignedByteOffset;
}


ParabellumFramework::EUINT32 
ParabellumFramework::Graphics::ShaderLayout::GetInputSlotClass()
{
	return m_inputSlotClass;
}


ParabellumFramework::EUINT32 
ParabellumFramework::Graphics::ShaderLayout::GetInstanceDataStepRate()
{
	return m_instanceDataStepRate;
}

