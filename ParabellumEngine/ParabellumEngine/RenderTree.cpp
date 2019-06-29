#include "stdafx.h"
#include "RenderTree.h"


//	---------------------------------------------------
//		Statics
//	---------------------------------------------------

ParabellumFramework::Graphics::VertexBuffer** ParabellumEngine::Core::Graphics::RenderTree::VertexBuffers = NULLPTR;
ParabellumFramework::Graphics::IndexBuffer** ParabellumEngine::Core::Graphics::RenderTree::IndexBuffers = NULLPTR;

//	---------------------------------------------------
//		Gets and Sets
//	---------------------------------------------------

ParabellumEngine::Core::Graphics::RenderTree::RenderTree()
{
	//Properties = NULLPTR;

	m_parent = NULLPTR;

	for (EUINT32 i = 0; i < 4; ++i)
	{
		m_children[i] = NULLPTR;
	}

	Properties.flags = 0;
	Properties.minDepth = +99999.0f;
	Properties.maxDepth = -99999.0f;

	m_treeDepth = 0;
	m_nodeDepth = 0;

	m_isDeckReady = false;

	m_deckFront = 0;
	m_deckBack = 1;
}


ParabellumEngine::Core::Graphics::RenderTree::~RenderTree()
{
	//delete[] Properties;
}


//	---------------------------------------------------
//		Methods
//	---------------------------------------------------


void 
ParabellumEngine::Core::Graphics::RenderTree::Initialize(_IN_ GraphicsDevice* pDevice, EUINT32 depth)
{

	// initialize parent
	m_countX = 2;
	m_countY = 2;

	m_parent = NULLPTR;

	m_treeDepth = depth;
	m_nodeDepth = 0;


	//EUINT32 buffersCount = 0;
	//
	//EUINT32 step = 1;
	//
	//for (EUINT32 i = 0; i < depth; ++i)
	//{
	//	buffersCount += step * step;
	//
	//	step = step * 2;
	//}
	//
	//printf("\n deph(%u) buffers count = %u", depth, buffersCount);
	//
	//
	//VertexBuffers = new VertexBuffer*[m_countX*m_countY];
	//IndexBuffers = new IndexBuffer*[m_countX*m_countY];

	//Properties = new PropertiesRenderTree[m_countX*m_countY];

	VertexTypeRenderTree vertices[4];
	EUINT32 indices[6];

	float x1 = -1.0f;
	float y1 = +1.0f;
	float x2 = +1.0f;
	float y2 = -1.0f;

	float u1 = 0.0f;
	float v1 = 0.0f;
	float u2 = 1.0f;
	float v2 = 1.0f;

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

	m_vertexBuffer = pDevice->CreateVertexBuffer(vertices, sizeof(VertexTypeRenderTree), 4);
	m_indexBuffer = pDevice->CreateIndexBuffer(indices, 6);

	m_screenPosition.Set(x1, y1, x2, y2);


	m_indexX = 0;
	m_indexY = 0;


	
	// create children
	for (EUINT32 i = 0; i < 4; ++i)
		m_children[i] = new RenderTree();

	m_childTL = m_children[0];
	m_childTR = m_children[1];
	m_childBR = m_children[2];
	m_childBL = m_children[3];

	m_depth = 0;

	m_childTL->Initialize(pDevice, this, m_countX, m_countY, Vector4D(0.0f, 0.0f, 0.5f, 0.5f),m_treeDepth, m_depth + 1);
	m_childTR->Initialize(pDevice, this, m_countX, m_countY, Vector4D(0.5f, 0.0f, 1.0f, 0.5f),m_treeDepth, m_depth + 1);
	m_childBR->Initialize(pDevice, this, m_countX, m_countY, Vector4D(0.5f, 0.5f, 1.0f, 1.0f),m_treeDepth, m_depth + 1);
	m_childBL->Initialize(pDevice, this, m_countX, m_countY, Vector4D(0.0f, 0.5f, 0.5f, 1.0f),m_treeDepth, m_depth + 1);
}




void ParabellumEngine::Core::Graphics::RenderTree::Initialize(
	_IN_ GraphicsDevice* pDevice, _IN_ _KEEP_ RenderTree* pParent,
	EUINT32 countX, EUINT32 countY, Vector4D position, EUINT32 treeDepth, EUINT32 depth
	)
{
	m_countX = 2;
	m_countY = 2;

	m_parent = pParent;

	m_treeDepth = treeDepth;

	// div
	EFLOAT32 beginX = position.X;
	EFLOAT32 beginY = position.Y;

	EFLOAT32 width = (position.Z - position.X) * 0.5f;
	EFLOAT32 height = (position.W - position.Y) * 0.5f ;

	Vector4D uvs[4];

	uvs[0].Set(beginX, beginY, beginX + width, beginY + height);
	uvs[1].Set(beginX+width, beginY, beginX+width*2, beginY + height);
	uvs[2].Set(beginX + width, beginY + height, beginX + width * 2, beginY + height*2);
	uvs[3].Set(beginX, beginY + height, beginX + width, beginY + height * 2);

	//printf("\n"); for (EUINT32 i = 0; i < 3 * depth; ++i) printf(" ");
	//printf("pos= %s", position.ToString().c_str());

	EFLOAT32 rowCount = (EFLOAT32)countX;
	EFLOAT32 colCount = (EFLOAT32)countY;

	EUINT32 urowCount = (EUINT32)countX;
	EUINT32 ucolCount = (EUINT32)countY;

	m_indexX = (EUINT32)( rowCount*(beginX ) ) ;
	m_indexY = (EUINT32)( colCount*(beginY ) );


	

	//printf("\n"); for (EUINT32 i = 0; i < 3 * depth; ++i) printf(" "); printf(" [%u,%u] vecTL = %s", m_indexX, m_indexY, uvs[0].ToString().c_str());
	//printf("\n"); for (EUINT32 i = 0; i < 3 * depth; ++i) printf(" "); printf(" [%u,%u] vecTL = %s", m_indexX, m_indexY, uvs[1].ToString().c_str());
	//printf("\n"); for (EUINT32 i = 0; i < 3 * depth; ++i) printf(" "); printf("(%u, %u) [%u,%u] vecBR = %s", urowCount, ucolCount, m_indexX, m_indexY, uvs[2].ToString().c_str());
	//printf("\n"); for (EUINT32 i = 0; i < 3 * depth; ++i) printf(" "); printf(" [%u,%u] vecTL = %s", m_indexX, m_indexY, uvs[3].ToString().c_str());

	//
	// Create buffers
	// 
	VertexTypeRenderTree vertices[4];
	EUINT32 indices[6];

	float x1 = -1.0f + 2.0f*uvs[0].X;
	float y1 = +1.0f - 2.0f*uvs[1].Y;
	float x2 = -1.0f + 2.0f*uvs[2].Z;
	float y2 = +1.0f - 2.0f*uvs[3].W;

	float u1 = uvs[0].X;
	float v1 = uvs[1].Y;
	float u2 = uvs[2].Z;
	float v2 = uvs[3].W;

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

	m_vertexBuffer = pDevice->CreateVertexBuffer(vertices, sizeof(VertexTypeRenderTree), 4);
	m_indexBuffer = pDevice->CreateIndexBuffer(indices, 6);
	
	m_screenPosition.Set(x1, y1, x2, y2);

	if (depth >= treeDepth)
	{
		return;
	}

	// create children
	for (EUINT32 i = 0; i < 4; ++i)
		m_children[i] = new RenderTree();

	m_childTL = m_children[0];
	m_childTR = m_children[1];
	m_childBR = m_children[2];
	m_childBL = m_children[3];

	m_depth = depth + 1;

	m_childTL->Initialize(pDevice, this, countX * 2, countY * 2, uvs[0], treeDepth, depth + 1);
	m_childTR->Initialize(pDevice, this, countX * 2, countY * 2, uvs[1], treeDepth, depth + 1);
	m_childBR->Initialize(pDevice, this, countX * 2, countY * 2, uvs[2], treeDepth, depth + 1);
	m_childBL->Initialize(pDevice, this, countX * 2, countY * 2, uvs[3], treeDepth, depth + 1);

}




void ParabellumEngine::Core::Graphics::RenderTree::Release(_IN_ GraphicsDevice* pDevice)
{
	for (EUINT32 i = 0; i < 4; ++i)
		if (m_children[i] != NULLPTR)
			m_children[i]->Release(pDevice);
}



void
ParabellumEngine::Core::Graphics::RenderTree::Render(_IN_ GraphicsDevice* pDevice, _IN_ Shader* pShader, EUINT32 paramNo, EUINT32 flagsp, EUINT32& no)
{
	if (m_children[0] != NULLPTR)
	{
		for (EUINT32 i = 0; i < 4; ++i)
			m_children[i]->Render(pDevice, pShader, paramNo, flagsp, no);
	}
	else
	{
		pDevice->SetVertexBuffer(m_vertexBuffer);
		pDevice->SetIndexBuffer(m_indexBuffer);

		if (pShader != NULLPTR)
		{
			Vector4D col1, col2;
			col1.Set(1.0f, 0.0f, 0.0f, 0.5f);
			col2.Set(0.0f, 1.0f, 0.0f, 0.5f);

			if(Properties.isBlindSpot == true)
				pShader->GetParametr(8)->Set(&col1);
			else
				pShader->GetParametr(8)->Set(&col2);
		}

		pDevice->SetShaderVariables();
		pDevice->SetShaderResources();
		pDevice->RenderRAW();

		no++;
	}


	/*
	if (Properties.flags & flagsp)
	{
		pDevice->SetVertexBuffer(m_vertexBuffer);
		pDevice->SetIndexBuffer(m_indexBuffer);

		if (pShader != NULLPTR)
		{
			pShader->GetParametr(paramNo)->Set(&Properties.averageColor);
		}

		pDevice->Render();
		//pDevice->Render(PF_GI_RENDERING_USESHADERPARAMETRSBUFFER);

		no++;
	}
	else
	{
		if ((Properties.childrenProperties & flagsp) && (m_children[0] != NULLPTR))
		{
			for (EUINT32 i = 0; i < 4; ++i)
				m_children[i]->Render(pDevice, pShader, paramNo, flagsp, no);
		}

	}
	*/

}


ParabellumFramework::EUINT32
ParabellumEngine::Core::Graphics::RenderTree::Setup(PropertiesRenderTree* properties, EUINT32 width, EUINT32 height)
{
	EUINT32 flag = 0;
	flag = ~flag;

	Properties.averageColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
	Properties.averageLuminance = 0.0f;
	Properties.luminanceMin = 1.0f;
	Properties.luminanceMax = 0.0f;
	Properties.isBlindSpot = false;
	Properties.childrenProperties = 0;

	if (m_children[0] != NULLPTR)
	{
		for (EUINT32 i = 0; i < 4; ++i)
		{
			m_children[i]->Setup(properties, width, height);
		}


		EFLOAT currentMaxDepth = 0.0f;
		EFLOAT32 avgLuminance = 0.0f;

		for (EUINT32 i = 0; i < 4; ++i)
		{
			flag &= m_children[i]->Properties.flags;

			Properties.childrenProperties |= m_children[i]->Properties.childrenProperties;

			Properties.averageColor += m_children[i]->Properties.averageColor;
			Properties.averageLuminance += m_children[i]->Properties.averageLuminance;


			if (m_children[i]->Properties.maxDepth > currentMaxDepth)
				currentMaxDepth = m_children[i]->Properties.maxDepth;

			if (m_children[i]->Properties.luminanceMax > Properties.luminanceMax)
				Properties.luminanceMax = m_children[i]->Properties.luminanceMax;

			if (m_children[i]->Properties.luminanceMin < Properties.luminanceMin)
				Properties.luminanceMin = m_children[i]->Properties.luminanceMin;

			if (m_children[i]->Properties.isBlindSpot == true)
				Properties.isBlindSpot = true;

		}

		Properties.averageColor *= 0.25f;
		Properties.averageLuminance *= 0.25f;
		Properties.flags = flag;
		Properties.maxDepth = currentMaxDepth;
	}
	else
	{
		Properties.flags |= properties[m_indexX + m_indexY*width].flags;
		Properties.childrenProperties = Properties.flags;
		Properties.minDepth = properties[m_indexX + m_indexY*width].minDepth;
		Properties.maxDepth = properties[m_indexX + m_indexY*width].maxDepth;
		Properties.averageColor = properties[m_indexX + m_indexY*width].averageColor;
		Properties.averageLuminance = properties[m_indexX + m_indexY*width].averageLuminance;
		Properties.luminanceMin = properties[m_indexX + m_indexY*width].luminanceMin;
		Properties.luminanceMax = properties[m_indexX + m_indexY*width].luminanceMax;
		Properties.isBlindSpot = properties[m_indexX + m_indexY*width].isBlindSpot;
	}



	return SystemCodes::SUCCESS;
}


void
ParabellumEngine::Core::Graphics::RenderTree::ClearChild(EUINT32 childNo)
{
	Properties.flags = 0;
	Properties.childrenProperties = 0;
	Properties.minDepth = +99999.0f;
	Properties.maxDepth = -99999.0f;
	Properties.isBlindSpot = false;

	if (m_children[0] != NULLPTR)
		m_children[childNo]->Clear();
}


void
ParabellumEngine::Core::Graphics::RenderTree::Clear()
{
	Properties.flags = 0;
	Properties.childrenProperties = 0;
	Properties.minDepth = +99999.0f;
	Properties.maxDepth = -99999.0f;
	Properties.isBlindSpot = false;

	if (m_children[0] != NULLPTR)
		for (EUINT32 i = 0; i < 4; ++i)
			m_children[i]->Clear();
}



ParabellumFramework::IntersectCodes
ParabellumEngine::Core::Graphics::RenderTree::Intersects(_IN_ Camera* pCamera, _IN_ BoundingBox* pBox)
{
	Vector4D area(+2.0f, -2.0f, -2.0f, +2.0f);
	EFLOAT32 areaDepht = 0.0f;


	pCamera->ToScreenPosition(pBox, &area, &areaDepht);

	//printf("\n\n-------------\n area = %s", area.ToString().c_str());

	EBOOL result = Covers(area, areaDepht + 0.02f);

	if (result == true)
		return IntersectCodes::CONTAIN;
	else
		return IntersectCodes::DISJOINT;
}


ParabellumFramework::IntersectCodes
ParabellumEngine::Core::Graphics::RenderTree::Intersects(_IN_ Camera* pCamera, _IN_ BoundingSphere* pSphere)
{
	Vector4D area(+2.0f, -2.0f, -2.0f, +2.0f);
	EFLOAT32 areaDepht = 0.0f;


	pCamera->ToScreenPosition(pSphere, &area, &areaDepht);

	EBOOL result = Covers(area, areaDepht - 0.0304f);

	if (result == true)
		return IntersectCodes::CONTAIN;
	else
		return IntersectCodes::DISJOINT;
}



ParabellumFramework::EBOOL
ParabellumEngine::Core::Graphics::RenderTree::Covers(Vector4D& area, EFLOAT32 areaDepth)
{
	EBOOL flag = true;

	if (m_children[0] == NULLPTR)
	{

		if (
			// (m_screenPosition.X >= area.X && m_screenPosition.Z <= area.Z && m_screenPosition.Y <= area.Y && m_screenPosition.W >= area.W)
			(m_screenPosition.X <= area.Z &&
				m_screenPosition.Z >= area.X &&
				m_screenPosition.Y >= area.W &&
				m_screenPosition.W <= area.Y)
			)
		{
			if (Properties.minDepth < areaDepth)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return true;
		}


	}
	else
	{

		// check children
		for (EUINT32 i = 0; i < 4; ++i)
		{
			if (
				m_children[i]->m_screenPosition.X <= area.Z &&
				m_children[i]->m_screenPosition.Z >= area.X &&
				m_children[i]->m_screenPosition.Y >= area.W &&
				m_children[i]->m_screenPosition.W <= area.Y
				)
				flag &= m_children[i]->Covers(area, areaDepth);
		}

	}

	return flag;
}






//	---------------------------------------------------
//		Gets and Sets
//	---------------------------------------------------

ParabellumFramework::EUINT32
ParabellumEngine::Core::Graphics::RenderTree::GetCount()
{
	return m_countX*m_countY;
}