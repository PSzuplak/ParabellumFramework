#include "stdafx.h"
#include "SelectionTool_Box.h"


//	------------------------------------------------------------
//		Constructors and destructors
//	------------------------------------------------------------
ParabellumEngine::Editor::SelectionTool_Box::SelectionTool_Box(
	_IN_ GraphicsDevice* pDevice, _IN_ ParabellumFramework::Resources::ResourceManager* pResource)
{
	m_shader = pResource->Load<Shader>("Resources/Shaders/DrawSimpleBox.psx");

	m_selectedPointMin.Set(0.0f, 0.0f, 0.0f);
	m_selectedPointMax.Set(2.0f, 2.0f, 2.0f);

	m_visibleOctreeNodes = new Narray<Octree*>(128);
}

ParabellumEngine::Editor::SelectionTool_Box::~SelectionTool_Box()
{
	delete m_visibleOctreeNodes;
}

//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------


void
ParabellumEngine::Editor::SelectionTool_Box::Draw(_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera)
{
	Vector3D boxCenter; 
	Vector4D colorBorder, colorInterior;
	Matrix4x4 translate, scale, world;

	colorBorder.Set(1.0f, 1.0f, 1.0f, 0.7f);
	colorInterior.Set(1.0f, 1.0f, 0.0f, 0.25f);

	pDevice->SetVertexBuffer(PrimitivesFactory::Instance(pDevice).GetBoxVertexBuffer());
	pDevice->SetIndexBuffer(PrimitivesFactory::Instance(pDevice).GetBoxIndexBuffer());
	
	
	m_selectedRegionBox.MakeFromPoints(&m_selectedPointMin, &m_selectedPointMax);
	
	m_selectedRegionBox.GetCenter(&boxCenter);
	
	translate.MakeTranslation(&boxCenter);
	scale.MakeScale(m_selectedRegionBox.GetWidth()*0.5f, m_selectedRegionBox.GetHeight()*0.5f, m_selectedRegionBox.GetDepth()*0.5f);
	world = translate * scale;

	pDevice->SetShader(m_shader);

	m_shader->SetParametrsBuffer(0, &world);
	m_shader->SetParametrsBuffer(1, pCamera->GetView());
	m_shader->SetParametrsBuffer(2, pCamera->GetProjection());
	m_shader->SetParametrsBuffer(3, &colorBorder);
	m_shader->SetParametrsBuffer(4, &colorInterior);

	pDevice->Render(PF_GI_RENDERING_UPDATEVARIABLES);

	//
	// render X rays from the box
	scale.MakeScale(m_selectedRegionBox.GetWidth()*1000.0f, m_selectedRegionBox.GetHeight()*0.505f, m_selectedRegionBox.GetDepth()*0.505f);
	world = translate * scale;

	colorBorder.Set(1.0f, 0.0f, 0.0f, 0.5f);
	colorInterior.Set(0.0f, 0.0f, 0.0f, 0.0f);

	m_shader->SetParametrsBuffer(0, &world);
	m_shader->SetParametrsBuffer(3, &colorBorder);
	m_shader->SetParametrsBuffer(4, &colorInterior);

	pDevice->Render(PF_GI_RENDERING_UPDATEVARIABLES);


	//
	// render Y rays from the box
	scale.MakeScale(m_selectedRegionBox.GetWidth()*0.505f, m_selectedRegionBox.GetHeight()*1000.0f, m_selectedRegionBox.GetDepth()*0.505f);
	world = translate * scale;

	colorBorder.Set(0.0f, 1.0f, 0.0f, 0.5f);
	colorInterior.Set(0.0f, 0.0f, 0.0f, 0.0f);

	m_shader->SetParametrsBuffer(0, &world);
	m_shader->SetParametrsBuffer(3, &colorBorder);
	m_shader->SetParametrsBuffer(4, &colorInterior);

	pDevice->Render(PF_GI_RENDERING_UPDATEVARIABLES);


	//
	// render Z rays from the box
	scale.MakeScale(m_selectedRegionBox.GetWidth()*0.505f, m_selectedRegionBox.GetHeight()*0.505f, m_selectedRegionBox.GetDepth()*1000.0f);
	world = translate * scale;

	colorBorder.Set(0.0f, 0.0f, 1.0f, 0.5f);
	colorInterior.Set(0.0f, 0.0f, 0.0f, 0.0f);

	m_shader->SetParametrsBuffer(0, &world);
	m_shader->SetParametrsBuffer(3, &colorBorder);
	m_shader->SetParametrsBuffer(4, &colorInterior);

	pDevice->Render(PF_GI_RENDERING_UPDATEVARIABLES);

}


void 
ParabellumEngine::Editor::SelectionTool_Box::SelectComponents(_OUT_ std::list<Component3D*>* pComponents, _IN_ Octree* pOctree, SelectionType selectionType)
{
	m_visibleOctreeNodes->Clear();
	pOctree->GetNodes(m_visibleOctreeNodes, &m_selectedRegionBox);

	for (EUINT32 i = 0; i < m_visibleOctreeNodes->GetCount(); ++i)
	{
		for (EUINT32 j = 0; j < m_visibleOctreeNodes->GetItem(i)->m_placedComponentsCount; ++j)
		{
			if (m_selectedRegionBox.Intersects(m_visibleOctreeNodes->GetItem(i)->m_objectsEModel[j]->GetTransformedBoundingBox()) != IntersectCodes::DISJOINT )
			{
				pComponents->push_back(m_visibleOctreeNodes->GetItem(i)->m_objectsEModel[j]);
			}
		}

	}
}






//	------------------------------------------------------------
//		Gets and Sets
//	------------------------------------------------------------


void 
ParabellumEngine::Editor::SelectionTool_Box::SetSelectionRegion(Vector3D v1, Vector3D v2)
{
	m_selectedPointMin = v1;
	m_selectedPointMax = v2;
}