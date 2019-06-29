#include "stdafx.h"
#include "VertexBuffer.h"

//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------


ParabellumFramework::Graphics::VertexBuffer::VertexBuffer(
	EUINT32 vertexSize, EUINT32 verticesCount)
{
	m_vertexSize = vertexSize;
	m_verticesCount = verticesCount;
}


ParabellumFramework::Graphics::VertexBuffer::~VertexBuffer()
{
}


//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------

void*
ParabellumFramework::Graphics::VertexBuffer::GetAddressOfBuffer()
{
	return &m_buffer;
}


ParabellumFramework::EUINT32
ParabellumFramework::Graphics::VertexBuffer::GetVertexSize()
{
	return m_vertexSize;
}

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::VertexBuffer::GetVerticesCount()
{
	return m_verticesCount;
}

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::VertexBuffer::GetBufferSize()
{
	return m_vertexSize * m_verticesCount;
}
