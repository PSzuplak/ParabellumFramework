#include "TerrainCell.h"



//	------------------------------------------------------------
//		Statics
//	------------------------------------------------------------



//	------------------------------------------------------------
//		Constructors and destructors
//	------------------------------------------------------------
ParabellumEngine::Components::TerrainCell::TerrainCell()
{
	m_heights = NULLPTR;
}


ParabellumEngine::Components::TerrainCell::~TerrainCell()
{
	if (m_heights != NULLPTR)
	{
		delete[] m_heights;
		m_heights = NULLPTR;
	}
}

//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------

ParabellumFramework::EUINT32
ParabellumEngine::Components::TerrainCell::Initialize(
	_IN_ ParabellumFramework::Graphics::GraphicsDevice* pDevice, _IN_ ParabellumFramework::Graphics::Texture2D* pTexture, 
	EFLOAT32 distanceBetweenVertices, EFLOAT32 maxHeight,
	EUINT32 beginX, EUINT32 beginY, EUINT32 width, EUINT32 height
	)
{
	EUINT32 result = SystemCodes::SUCCESS;
	m_startHeight = 0.0f;
	m_distanceBetweenVertices = distanceBetweenVertices;
	m_maxHeight = maxHeight;
	m_startHeight = 0.0f;
	m_T2Dtex = pTexture;

	TerrainVertexFormat* vertices;
	EUINT32* indices;
	EUINT32 verticesCount;
	EUINT32 indicesCount;

	if(width == 0)
		m_vertexCountX = pTexture->GetWidth();
	else
		m_vertexCountX = beginX + width;

	if (height == 0)
		m_vertexCountZ = pTexture->GetHeight();
	else
		m_vertexCountZ = beginY + height;
		
	verticesCount = m_vertexCountX * m_vertexCountZ;
	indicesCount = (m_vertexCountX - 1) * (m_vertexCountZ - 1) * 6;

	vertices = new TerrainVertexFormat[verticesCount];

	m_heights = new EFLOAT32[m_vertexCountZ*m_vertexCountX];


	for (EUINT32 z = 0; z < m_vertexCountZ; ++z)
	{
		for (EUINT32 x = 0; x < m_vertexCountX; ++x)
		{
			m_heights[z * m_vertexCountX + x] = pTexture->GetChannelVal<EFLOAT32>(beginX + x, beginY + z, ChannelNo::R) / 255.0f * maxHeight;

			// calculate vertex position
			vertices[z * m_vertexCountX + x].Position[0] = x * m_distanceBetweenVertices;
			vertices[z * m_vertexCountX + x].Position[1] = m_heights[z * m_vertexCountX + x];
			vertices[z * m_vertexCountX + x].Position[2] = z * m_distanceBetweenVertices;

			vertices[z * m_vertexCountX + x].AbsoluteUV[0] = (float)x / (float)m_vertexCountX;
			vertices[z * m_vertexCountX + x].AbsoluteUV[1] = (float)z / (float)m_vertexCountZ;

			vertices[z * m_vertexCountX + x].RelativeUV[0] = 0.5f;
			vertices[z * m_vertexCountX + x].RelativeUV[1] = 0.5f;

			// set normal to Up direction - initialy
			vertices[z * m_vertexCountX + x].Normal[0] = 0.0f;
			vertices[z * m_vertexCountX + x].Normal[1] = 1.0f;
			vertices[z * m_vertexCountX + x].Normal[2] = 0.0f;

			vertices[z * m_vertexCountX + x].Tangent[0] = 1.0f;
			vertices[z * m_vertexCountX + x].Tangent[1] = 0.0f;
			vertices[z * m_vertexCountX + x].Tangent[2] = 0.0f;

			vertices[z * m_vertexCountX + x].Binormal[0] = 0.0f;
			vertices[z * m_vertexCountX + x].Binormal[1] = 0.0f;
			vertices[z * m_vertexCountX + x].Binormal[2] = 1.0f;
		}
	}


	indices = new EUINT32[indicesCount];

	EUINT32 i = 0;

	for (EUINT32 x = 0; x < m_vertexCountX - 1; ++x)
		for (EUINT32 z = 0; z < m_vertexCountZ - 1; ++z)
		{
			// indices of quad (which make 2 triangles)
			EUINT32 ul = z * m_vertexCountX + x;
			EUINT32 ur = ul + 1;
			EUINT32 ll = ul + m_vertexCountX;
			EUINT32 lr = ll + 1;

			// top triangle
			indices[i++] = ur;
			indices[i++] = ul;
			indices[i++] = ll;

			// down triangle
			indices[i++] = lr;
			indices[i++] = ur;
			indices[i++] = ll;
		}

	// calculate vertices data
	///CalculateAUV();
	///CalculateRUV(2);
	///CalculateTBN();


	m_vertexBuffer = pDevice->CreateVertexBuffer(vertices, sizeof(TerrainVertexFormat), verticesCount);
	m_indexBuffer = pDevice->CreateIndexBuffer(indices, indicesCount);

	// fin min and max height
	m_minHeight = +17000.0f;
	m_maxHeight = -17000.0f;

	for (EUINT32 i = 0; i < m_vertexCountX * m_vertexCountZ; ++i)
	{
		if (vertices[i].Position[1] > m_maxHeight)
			m_maxHeight = vertices[i].Position[1];

		if (vertices[i].Position[1] < m_minHeight)
			m_minHeight = vertices[i].Position[1];
	}


	// clean up
	delete[] vertices;
	delete[] indices;

	return SystemCodes::SUCCESS;
}





ParabellumFramework::EUINT32 
ParabellumEngine::Components::TerrainCell::Initialize(
	_IN_ ParabellumFramework::Graphics::GraphicsDevice* pDevice,
	_IN_ EFLOAT32* heightmap, EUINT32 heightmapWidth, EUINT32 heightmapHeight,
	EFLOAT32 distanceBetweenVertices, EFLOAT32 maxHeight,
	EUINT32 sliceBeginX, EUINT32 sliceBeginY, EUINT32 sliceWidth, EUINT32 sliceHeight,
	EFLOAT32 optimizedSlope
	)
{
	EUINT32 result = SystemCodes::SUCCESS;
	m_startHeight = 0.0f;
	m_distanceBetweenVertices = distanceBetweenVertices;
	m_maxHeight = maxHeight;
	m_startHeight = 0.0f;
	m_T2Dtex = NULLPTR;

	TerrainVertexFormat* vertices;
	EUINT32* indices;
	EUINT32 verticesCount;
	EUINT32 indicesCount;

	if (sliceWidth == 0)
		m_vertexCountX = heightmapWidth;
	else
		m_vertexCountX = sliceBeginX + sliceWidth;

	if (sliceHeight == 0)
		m_vertexCountZ = heightmapHeight;
	else
		m_vertexCountZ = sliceBeginY + sliceHeight;

	verticesCount = m_vertexCountX * m_vertexCountZ;
	indicesCount = (m_vertexCountX - 1) * (m_vertexCountZ - 1) * 6;

	vertices = new TerrainVertexFormat[verticesCount];

	// which vertices were marked as optimized - 0 not optimized ,
	// 1 - optmized vertice in the middle, 2 - neighbours of 1, 3 - beighbours of 2 etc.
	EUINT32* verticesMarked = new EUINT32[verticesCount];


	m_heights = new EFLOAT32[m_vertexCountZ*m_vertexCountX];


	for (EUINT32 z = 0; z < m_vertexCountZ; ++z)
	{
		for (EUINT32 x = 0; x < m_vertexCountX; ++x)
		{
			// mark as unoptimized
			verticesMarked[z * m_vertexCountX + x] = 0;

			m_heights[z * m_vertexCountX + x] = heightmap[x + z*heightmapWidth] * m_maxHeight;

			// calculate vertex position
			vertices[z * m_vertexCountX + x].Position[0] = x * m_distanceBetweenVertices;
			vertices[z * m_vertexCountX + x].Position[1] = m_heights[z * m_vertexCountX + x];
			vertices[z * m_vertexCountX + x].Position[2] = z * m_distanceBetweenVertices;

			vertices[z * m_vertexCountX + x].AbsoluteUV[0] = (float)x / (float)m_vertexCountX;
			vertices[z * m_vertexCountX + x].AbsoluteUV[1] = (float)z / (float)m_vertexCountZ;

			vertices[z * m_vertexCountX + x].RelativeUV[0] = 0.5f;
			vertices[z * m_vertexCountX + x].RelativeUV[1] = 0.5f;

			// set normal to Up direction - initialy
			vertices[z * m_vertexCountX + x].Normal[0] = 0.0f;
			vertices[z * m_vertexCountX + x].Normal[1] = 1.0f;
			vertices[z * m_vertexCountX + x].Normal[2] = 0.0f;

			vertices[z * m_vertexCountX + x].Tangent[0] = 1.0f;
			vertices[z * m_vertexCountX + x].Tangent[1] = 0.0f;
			vertices[z * m_vertexCountX + x].Tangent[2] = 0.0f;

			vertices[z * m_vertexCountX + x].Binormal[0] = 0.0f;
			vertices[z * m_vertexCountX + x].Binormal[1] = 0.0f;
			vertices[z * m_vertexCountX + x].Binormal[2] = 1.0f;
		}
	}



	//
	// Mesh optimization - step 1 - mark vertices
	//

	EUINT32 i = 0;

	indices = new EUINT32[indicesCount];

	for (EUINT32 x = 0; x < m_vertexCountX - 2; x += 2)
		for (EUINT32 z = 0; z < m_vertexCountZ - 2; z += 2)
		{
			//if(verticesMarked[z * m_vertexCountX + x] != 0)
			//	continue;

			// get indices of vertices and its neigbours
			EUINT32 ul = z * m_vertexCountX + x;
			EUINT32 um = ul + 1;
			EUINT32 ur = ul + 2;

			EUINT32 ml = ul + m_vertexCountX;
			EUINT32 mm = ul + m_vertexCountX + 1;
			EUINT32 mr = ul + m_vertexCountX + 2;

			EUINT32 ll = ul + m_vertexCountX * 2;
			EUINT32 lm = ll + 1;
			EUINT32 lr = ll + 2;

			// check difference of the highs
			if (
				//(verticesMarked[ul] == 0) && (verticesMarked[um] == 0) && (verticesMarked[ur] == 0) &&
				//(verticesMarked[ml] == 0) && (verticesMarked[mm] == 0) && (verticesMarked[mr] == 0) &&
				//(verticesMarked[ll] == 0) && (verticesMarked[lm] == 0) && (verticesMarked[lr] == 0) &&

				// corner ul and lr
				((vertices[ul].Position[1] + vertices[lr].Position[1]) *0.5f) > (vertices[mm].Position[1] - optimizedSlope) &&
				((vertices[ul].Position[1] + vertices[lr].Position[1]) *0.5f) < (vertices[mm].Position[1] + optimizedSlope) &&

				// corner ur and ll
				((vertices[ur].Position[1] + vertices[ll].Position[1]) *0.5f) > (vertices[mm].Position[1] - optimizedSlope) &&
				((vertices[ur].Position[1] + vertices[ll].Position[1]) *0.5f) < (vertices[mm].Position[1] + optimizedSlope) &&


				((vertices[ml].Position[1] + vertices[mr].Position[1]) *0.5f) > (vertices[mm].Position[1] - optimizedSlope) &&
				((vertices[ml].Position[1] + vertices[mr].Position[1]) *0.5f) < (vertices[mm].Position[1] + optimizedSlope) &&
				 
				((vertices[um].Position[1] + vertices[lm].Position[1]) *0.5f) > (vertices[mm].Position[1] - optimizedSlope) &&
				((vertices[um].Position[1] + vertices[lm].Position[1]) *0.5f) < (vertices[mm].Position[1] + optimizedSlope)

				//&& 2<1
				)
			{
				
				//top triangle
				indices[i++] = ur;
				indices[i++] = ul;
				indices[i++] = ll;
				
				//down triangle
				indices[i++] = lr;
				indices[i++] = ur;
				indices[i++] = ll;
				
				// mark vertices
				verticesMarked[ur] += 2;
				verticesMarked[ul] += 2;
				verticesMarked[ll] += 2;
				verticesMarked[ml] += 2;
				verticesMarked[mm] += 1;
				verticesMarked[mr] += 2;
				verticesMarked[lr] += 2;
				verticesMarked[ur] += 2;
				verticesMarked[ll] += 2;

			}
			else
			{
				// top left 1
				indices[i++] = um;
				indices[i++] = ul;
				indices[i++] = ml;
				
				// top left 2
				indices[i++] = mm;
				indices[i++] = um;
				indices[i++] = ml;
				
				// top right 1
				indices[i++] = ur;
				indices[i++] = um;
				indices[i++] = mm;
				
				// top right 2
				indices[i++] = mr;
				indices[i++] = ur;
				indices[i++] = mm;
				
				// bottom left 1
				indices[i++] = mm;
				indices[i++] = ml;
				indices[i++] = ll;
				
				// bottom left 2
				indices[i++] = lm;
				indices[i++] = mm;
				indices[i++] = ll;
				
				// bottom right 1
				indices[i++] = mr;
				indices[i++] = mm;
				indices[i++] = lm;
				
				// bottom right 2
				indices[i++] = lr;
				indices[i++] = mr;
				indices[i++] = lm;
		
			}
				

		}


	//
	// Mesh optimization - step 1 - remove vertices
	//
	/*
	i = 0;

	printf("\n indices count before = %u", m_indicesCount);

	m_indices = new EUINT32[m_indicesCount];

	for (EUINT32 x = 0; x < m_vertexCountX - 0; x += 1)
		for (EUINT32 z = 0; z < m_vertexCountZ - 0; z += 1)
		{
			//if(verticesMarked[z * m_vertexCountX + x] != 0)
			//	continue;

			// get indices of vertices and its neigbours
			EUINT32 ul = z * m_vertexCountX + x;
			EUINT32 um = ul + 1;
			EUINT32 ur = ul + 2;

			EUINT32 ml = ul + m_vertexCountX;
			EUINT32 mm = ul + m_vertexCountX + 1;
			EUINT32 mr = ul + m_vertexCountX + 2;

			EUINT32 ll = ul + m_vertexCountX * 2;
			EUINT32 lm = ll + 1;
			EUINT32 lr = ll + 2;


			if(verticesMarked[mm] == 1)
			{
				// t1
				m_indices[i++] = mm;
				m_indices[i++] = ur;
				m_indices[i++] = ul;

				// t2
				m_indices[i++] = mm;
				m_indices[i++] = lr;
				m_indices[i++] = ur;

				// t3
				m_indices[i++] = mm;
				m_indices[i++] = ll;
				m_indices[i++] = lr;

				// t4
				m_indices[i++] = mm;
				m_indices[i++] = ul;
				m_indices[i++] = ll;

				z += 1;
			}

			else if(verticesMarked[mm] == 0)
			{
				// top
			//	if ((verticesMarked[ul] == 2) && (verticesMarked[um] == 2) && (verticesMarked[ur] == 2))
				//if (verticesMarked[um] == 2)
				//{
				//	m_indices[i++] = ul; m_indices[i++] = ml; m_indices[i++] = mm;
				//	m_indices[i++] = ul; m_indices[i++] = mm; m_indices[i++] = ur;
				//	m_indices[i++] = ur; m_indices[i++] = mm; m_indices[i++] = mr;
				//}
				//// right
				//if ((verticesMarked[ur] == 2) && (verticesMarked[mr] == 2) && (verticesMarked[lr] == 2))
				//else if (verticesMarked[ur] == 2)
				//{
				//	m_indices[i++] = ur; m_indices[i++] = um; m_indices[i++] = mm;
				//	m_indices[i++] = ur; m_indices[i++] = mm; m_indices[i++] = lr;
				//	m_indices[i++] = mm; m_indices[i++] = lm; m_indices[i++] = lr;
				//}
				// bottom
				//else if ((verticesMarked[ll] == 2) && (verticesMarked[lm] == 2) && (verticesMarked[lr] == 2))
				//else if (verticesMarked[lr] == 2)
				//{
				//	m_indices[i++] = mm; m_indices[i++] = ml; m_indices[i++] = ll;
				//	m_indices[i++] = mm; m_indices[i++] = ll; m_indices[i++] = lr;
				//	m_indices[i++] = mr; m_indices[i++] = mm; m_indices[i++] = lr;
				//}
				//// left
				//else if ((verticesMarked[ul] == 2) && (verticesMarked[ml] == 2) && (verticesMarked[ll] == 2))
				//else if (verticesMarked[ll] == 2)
				//{
				//	m_indices[i++] = ul; m_indices[i++] = mm; m_indices[i++] = um;
				//	m_indices[i++] = ul; m_indices[i++] = ll; m_indices[i++] = mm;
				//	m_indices[i++] = ll; m_indices[i++] = lm; m_indices[i++] = mm;
				//
				//}
				//else
				{
					
					// top left 1
					m_indices[i++] = um;
					m_indices[i++] = ul;
					m_indices[i++] = ml;

					// top left 2
					m_indices[i++] = mm;
					m_indices[i++] = um;
					m_indices[i++] = ml;

					// top right 1
					m_indices[i++] = ur;
					m_indices[i++] = um;
					m_indices[i++] = mm;

					// top right 2
					m_indices[i++] = mr;
					m_indices[i++] = ur;
					m_indices[i++] = mm;

					// bottom left 1
					m_indices[i++] = mm;
					m_indices[i++] = ml;
					m_indices[i++] = ll;

					// bottom left 2
					m_indices[i++] = lm;
					m_indices[i++] = mm;
					m_indices[i++] = ll;

					// bottom right 1
					m_indices[i++] = mr;
					m_indices[i++] = mm;
					m_indices[i++] = lm;

					// bottom right 2
					m_indices[i++] = lr;
					m_indices[i++] = mr;
					m_indices[i++] = lm;
				}
			}


		}


	m_indicesCount = i;

	printf("\n indices count after  = %u", m_indicesCount);
	*/
	//
	// Calculate indices
	//
	/*
	m_indices = new EUINT32[m_indicesCount];

	i = 0;

	for (EUINT32 x = 0; x < m_vertexCountX - 1; x+=1)
		for (EUINT32 z = 0; z < m_vertexCountZ - 1; z+=1)
		{
			// indices of quad (which make 2 triangles)
			EUINT32 ul = z * m_vertexCountX + x;
			EUINT32 ur = ul + 1;
			EUINT32 ll = ul + m_vertexCountX;
			EUINT32 lr = ll + 1;

			// top triangle
			m_indices[i++] = ur;
			m_indices[i++] = ul;
			m_indices[i++] = ll;

			// down triangle
			m_indices[i++] = lr;
			m_indices[i++] = ur;
			m_indices[i++] = ll;
		}
	*/
	/*
	for (EUINT32 x = 0; x < m_vertexCountX - 2; x += 2)
		for (EUINT32 z = 0; z < m_vertexCountZ - 2; z += 2)
		{
			// indices of quad (which make 2 triangles)
			EUINT32 ul = z * m_vertexCountX + x;
			EUINT32 ur = ul + 1 + 1;
			EUINT32 ll = ul + m_vertexCountX * 2;
			EUINT32 lr = ll + 1 + 1;

			// top triangle
			m_indices[i++] = ur;
			m_indices[i++] = ul;
			m_indices[i++] = ll;

			// down triangle
			m_indices[i++] = lr;
			m_indices[i++] = ur;
			m_indices[i++] = ll;
		}
	*/

	// calculate vertices data
	///CalculateAUV();
	///CalculateRUV(2);
	///CalculateTBN();


	m_vertexBuffer = pDevice->CreateVertexBuffer(vertices, sizeof(TerrainVertexFormat), verticesCount);
	m_indexBuffer = pDevice->CreateIndexBuffer(indices, indicesCount);


	// fin min and max height
	m_minHeight = +17000.0f;
	m_maxHeight = -17000.0f;

	for (EUINT32 i = 0; i < m_vertexCountX * m_vertexCountZ; ++i)
	{
		if (vertices[i].Position[1] > m_maxHeight)
			m_maxHeight = vertices[i].Position[1];

		if (vertices[i].Position[1] < m_minHeight)
			m_minHeight = vertices[i].Position[1];
	}


	// clean up
	delete[] vertices;
	delete[] indices;
	delete[] verticesMarked;


	return SystemCodes::SUCCESS;
}














//	------------------------------------------------------------
//		Gets Sets
//	------------------------------------------------------------

ParabellumFramework::Graphics::VertexBuffer*
ParabellumEngine::Components::TerrainCell::GetVertexBuffer()
{
	return m_vertexBuffer;
}


ParabellumFramework::Graphics::IndexBuffer*
ParabellumEngine::Components::TerrainCell::GetIndexBuffer()
{
	return m_indexBuffer;
}