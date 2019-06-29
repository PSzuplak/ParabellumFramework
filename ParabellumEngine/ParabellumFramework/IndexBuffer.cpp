#include "stdafx.h"
#include "IndexBuffer.h"

//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------
ParabellumFramework::Graphics::IndexBuffer::IndexBuffer(EUINT64 indicesCount)
{
	m_indicesCount = indicesCount;
}


ParabellumFramework::Graphics::IndexBuffer::~IndexBuffer()
{
}


//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------

void* 
ParabellumFramework::Graphics::IndexBuffer::GetBuffer()
{
	return &m_buffer;
}

ParabellumFramework::EUINT64 
ParabellumFramework::Graphics::IndexBuffer::GetIndicesCount()
{
	return m_indicesCount;
}