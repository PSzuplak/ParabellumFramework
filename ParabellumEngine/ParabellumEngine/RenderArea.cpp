#include "stdafx.h"
#include "RenderArea.h"


//	---------------------------------------------------
//		Gets and Sets
//	---------------------------------------------------

ParabellumEngine::Core::Graphics::RenderArea::RenderArea()
{
	m_vertexBuffers = NULLPTR;
	m_indexBuffers = NULLPTR;
	Properties = NULLPTR;
	Effects = NULLPTR;
}


ParabellumEngine::Core::Graphics::RenderArea::~RenderArea()
{
	delete[] m_vertexBuffers;
	delete[] m_indexBuffers;

	delete[] Properties;

	delete Effects;

}


//	---------------------------------------------------
//		Methods
//	---------------------------------------------------


//
//
//	Initialize Buffers for RenderQuad
//
//
void ParabellumEngine::Core::Graphics::RenderArea::Initialize(
	_IN_ GraphicsDevice* pDevice, EUINT32 countX, EUINT32 countY)
{
	m_countX = countX;
	m_countY = countY;

	m_vertexBuffers = new VertexBuffer*[m_countX*m_countY];
	m_indexBuffers = new IndexBuffer*[m_countX*m_countY];

	Properties = new RenderProperties[m_countX*m_countY];
	
	Effects = new EffectsApplied(m_countX*m_countY);

	for (EUINT32 y = 0; y < countY; ++y)
	{
		for (EUINT32 x = 0; x < countX; ++x)
		{
			VertexTypeRenderArea vertices[4];
			EUINT32 indices[6];

			EFLOAT32 x1 = -1.0f + 2.0f*(EFLOAT32)x / (EFLOAT32)m_countX;
			EFLOAT32 y1 = 1.0f - 2.0f*(EFLOAT32)y / (EFLOAT32)countY;
			EFLOAT32 x2 = -1.0f + 2.0f*(EFLOAT32)(x + 1) / (EFLOAT32)m_countX;
			EFLOAT32 y2 = 1.0f - 2.0f*(EFLOAT32)(y + 1) / (EFLOAT32)countY;

			EFLOAT32 u1 = (EFLOAT32)x / (EFLOAT32)m_countX;
			EFLOAT32 v1 = (EFLOAT32)y / (EFLOAT32)countY;
			EFLOAT32 u2 = (EFLOAT32)(x + 1) / (EFLOAT32)m_countX;
			EFLOAT32 v2 = (EFLOAT32)(y + 1) / (EFLOAT32)countY;

			vertices[0].position[0] = x1;
			vertices[0].position[1] = y1;
			vertices[0].position[2] = 0.0f;
			vertices[0].uv[0] = u1;
			vertices[0].uv[1] = v1;

			vertices[1].position[0] = x2;
			vertices[1].position[1] = y1;
			vertices[1].position[2] = 0.0f;
			vertices[1].uv[0] = u2;
			vertices[1].uv[1] = v1;

			vertices[2].position[0] = x2;
			vertices[2].position[1] = y2;
			vertices[2].position[2] = 0.0f;
			vertices[2].uv[0] = u2;
			vertices[2].uv[1] = v2;

			vertices[3].position[0] = x1;
			vertices[3].position[1] = y2;
			vertices[3].position[2] = 0.0f;
			vertices[3].uv[0] = u1;
			vertices[3].uv[1] = v2;

			// set indices
			indices[0] = 0;
			indices[1] = 1;
			indices[2] = 2;
			indices[3] = 0;
			indices[4] = 2;
			indices[5] = 3;

			m_vertexBuffers[y * m_countX + x] = pDevice->CreateVertexBuffer(vertices, sizeof(VertexTypeRenderArea), 4);
			m_indexBuffers[y * m_countX + x] = pDevice->CreateIndexBuffer(indices, 6);
		}
	}

}




//
//
//	Initialize Buffers for RenderQuad
//
//
void ParabellumEngine::Core::Graphics::RenderArea::Release(_IN_ GraphicsDevice* pDevice)
{
}