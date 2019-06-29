#include "stdafx.h"
#include "RenderQuad.h"


//	---------------------------------------------------
//		Gets and Sets
//	---------------------------------------------------

ParabellumEngine::Core::Graphics::RenderQuad::RenderQuad()
{
	m_vertexBuffer = NULLPTR;
	m_indexBuffer = NULLPTR;
}


ParabellumEngine::Core::Graphics::RenderQuad::~RenderQuad()
{
}


//	---------------------------------------------------
//		Methods
//	---------------------------------------------------

void 
ParabellumEngine::Core::Graphics::RenderQuad::Initialize(
	_IN_ GraphicsDevice* pDevice, EFLOAT32 x1, EFLOAT32 y1, EFLOAT32 x2, EFLOAT32 y2)
{
	// release if already created
	if (m_vertexBuffer != NULLPTR)
		pDevice->ReleaseVertexBuffer(m_vertexBuffer);

	if (m_indexBuffer != NULLPTR)
		pDevice->ReleaseIndexBuffer(m_indexBuffer);


	VertexTypeRenderQuad vertices[4];
	EUINT32 indices[6];


	// set vertices
	vertices[0].position[0] = x1;
	vertices[0].position[1] = y1;
	vertices[0].position[2] = 0.0f;
	vertices[0].coords[0] = 0.0f;
	vertices[0].coords[1] = 0.0f;

	vertices[1].position[0] = x2;
	vertices[1].position[1] = y1;
	vertices[1].position[2] = 0.0f;
	vertices[1].coords[0] = 1.0f;
	vertices[1].coords[1] = 0.0f;

	vertices[2].position[0] = x2;
	vertices[2].position[1] = y2;
	vertices[2].position[2] = 0.0f;
	vertices[2].coords[0] = 1.0f;
	vertices[2].coords[1] = 1.0f;

	vertices[3].position[0] = x1;
	vertices[3].position[1] = y2;
	vertices[3].position[2] = 0.0f;
	vertices[3].coords[0] = 0.0f;
	vertices[3].coords[1] = 1.0f;

	// set indices
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	m_vertexBuffer = pDevice->CreateVertexBuffer(vertices, sizeof(VertexTypeRenderQuad), 4);
	m_indexBuffer = pDevice->CreateIndexBuffer(indices, 6);

}

void 
ParabellumEngine::Core::Graphics::RenderQuad::Release(_IN_ GraphicsDevice* pDevice)
{
	//
	if (m_vertexBuffer != NULLPTR)
	{
		pDevice->ReleaseVertexBuffer(m_vertexBuffer);
		m_vertexBuffer = NULLPTR;
	}

	//
	if (m_indexBuffer != NULLPTR)
	{
		pDevice->ReleaseIndexBuffer(m_indexBuffer);
		m_indexBuffer = NULLPTR;
	}
}





//	---------------------------------------------------
//		Gets and Sets
//	---------------------------------------------------


ParabellumFramework::Graphics::VertexBuffer* 
ParabellumEngine::Core::Graphics::RenderQuad::GetVertexBuffer()
{
	return m_vertexBuffer;
}

ParabellumFramework::Graphics::IndexBuffer* 
ParabellumEngine::Core::Graphics::RenderQuad::GetIndexBuffer()
{
	return m_indexBuffer;
}