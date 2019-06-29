#include "Octree.h"



//	------------------------------------------------------
//	Constructors and destructors
//	------------------------------------------------------

ParabellumEngine::Core::Scene::Octree::Octree()
{
	m_parent = NULLPTR;

	m_children = NULLPTR;

	m_childUFL = NULLPTR;
	m_childUFR = NULLPTR;
	m_childUBL = NULLPTR;
	m_childUBR = NULLPTR;
	m_childDFL = NULLPTR;
	m_childDFR = NULLPTR;
	m_childDBL = NULLPTR;
	m_childDBR = NULLPTR;

	m_placedComponentsCount = 0;
	m_totalComponentsCount = 0;
	m_depth = 0;
}

ParabellumEngine::Core::Scene::Octree::~Octree()
{

}


//	------------------------------------------------------
//	Methods
//	------------------------------------------------------

ParabellumFramework::EUINT32
ParabellumEngine::Core::Scene::Octree::Initialize(_IN_ Vector3D& minv, Vector3D& maxv)
{
	m_parent = NULLPTR;

	m_depth = 0;

	// create box
	m_center = (maxv + minv) * 0.5f;
	m_min = minv;
	m_max = maxv;

	m_box.MakeFromPoints(&minv, &maxv);

	// create sphere
	Vector3D d;
	d = maxv - m_center;
	EFLOAT32 ray = Vector3D::Length(d);
	m_boundingSphere.MakeBoundingSphere(&m_center, ray);

	m_hasContentChanged = false;

	return 0;
}


ParabellumFramework::EUINT32
ParabellumEngine::Core::Scene::Octree::Initialize(_IN_ Octree* pParent, _IN_ Vector3D& minv, Vector3D& maxv)
{
	m_parent = pParent;

	//printf("\n %u->%u ", (EADDR)m_parent, (EADDR)this);

	m_depth = pParent->GetDepth() + 1;

	// create box
	m_box.MakeFromPoints(&minv, &maxv);

	m_box.GetMinMax(m_min, m_max);
	m_center = (m_min + m_max) * 0.5f;

	// create sphere
	Vector3D d;
	d = maxv - m_center;
	EFLOAT32 ray = Vector3D::Length(d);
	m_boundingSphere.MakeBoundingSphere(&m_center, ray);


	return 0;
}



//
//
//
//
//
void ParabellumEngine::Core::Scene::Octree::Release()
{
	if (m_children != NULLPTR)
	{
		for (int i = 0; i < 8; ++i)
		{
			m_children[i].Release();
		}

		delete[] m_children;
		m_children = NULLPTR;

		m_childUFL = NULLPTR;
		m_childUFL = NULLPTR;
		m_childUFR = NULLPTR;
		m_childUBL = NULLPTR;
		m_childUBR = NULLPTR;
		m_childDFL = NULLPTR;
		m_childDFR = NULLPTR;
		m_childDBL = NULLPTR;

		m_parent = NULLPTR;
	}
}


void 
ParabellumEngine::Core::Scene::Octree::DrawRecursive(_IN_ GraphicsDevice* pDevice, _IN_ Shader* pShader,
	_IN_ VertexBuffer* pVB, _IN_ IndexBuffer* pIB, _IN_ Camera* pCamera)
{
	pDevice->SetVertexBuffer(pVB);
	pDevice->SetIndexBuffer(pIB);
	pDevice->SetShader(pShader);

	Vector3D center;
	m_box.GetCenter(&center);

	Matrix4x4 translate, scale, world;
	translate.MakeTranslation(&center);
	scale.MakeScale(m_box.GetWidth()*0.5f, m_box.GetHeight()*0.5f, m_box.GetDepth()*0.5f);
	world = translate * scale;

	Vector4D colorBorder, colorInterior, boxSize;

	colorBorder.X = 1.0f;
	colorBorder.Y = 1.0f;
	colorBorder.Z = 0.0f;
	colorBorder.W = 0.5f;

	colorInterior.X = 0.0f;
	colorInterior.Y = 1.0f;
	colorInterior.Z = 0.0f;
	colorInterior.W = 0.0f;

	boxSize.X = m_box.GetWidth();
	boxSize.Y = m_box.GetHeight();
	boxSize.Z = 0.0f;
	boxSize.W = 0.0f;



	pShader->GetParametr(0)->Set(&world);
	pShader->GetParametr(1)->Set(pCamera->GetView());
	pShader->GetParametr(2)->Set(pCamera->GetProjection());
	pShader->GetParametr(3)->Set(&colorBorder);
	pShader->GetParametr(4)->Set(&colorInterior);
	pShader->GetParametr(5)->Set(&boxSize);

	pDevice->Render();

	if (m_children != NULLPTR)
		for (int i = 0; i < 8; ++i)
		{
			m_children[i].DrawRecursive(pDevice, pShader, pVB, pIB, pCamera);
		}


}


//
//
//
//
//
void 
ParabellumEngine::Core::Scene::Octree::DrawVisible(_IN_ GraphicsDevice* pDevice, _IN_ Shader* pShader,
	_IN_ VertexBuffer* pVB, _IN_ IndexBuffer* pIB, _IN_ Camera* pCamera, _IN_ RenderTree* pRenderTree)
{

}



//
//
//
//
//
ParabellumFramework::EUINT32
ParabellumEngine::Core::Scene::Octree::CreateChildren()
{
	if (m_children == NULLPTR)
	{
		m_children = new Octree[8];
		m_childUFL = &m_children[0];
		m_childUFR = &m_children[1];
		m_childUBL = &m_children[2];
		m_childUBR = &m_children[3];
		m_childDFL = &m_children[4];
		m_childDFR = &m_children[5];
		m_childDBL = &m_children[6];
		m_childDBR = &m_children[7];

		Vector3D boxCorners[8];
		m_box.GetCorners(&boxCorners[0]);

		m_childUFL->Initialize(this, m_center, boxCorners[0]);
		m_childUFR->Initialize(this, m_center, boxCorners[1]);
		m_childUBL->Initialize(this, m_center, boxCorners[4]);
		m_childUBR->Initialize(this, m_center, boxCorners[5]);
		m_childDFL->Initialize(this, boxCorners[3], m_center);
		m_childDFR->Initialize(this, boxCorners[2], m_center);
		m_childDBL->Initialize(this, boxCorners[7], m_center);
		m_childDBR->Initialize(this, boxCorners[6], m_center);
	}
	else
	{
		return SystemCodes::ERROR_EXISTS_ALREADY;
	}

	return SystemCodes::SUCCESS;
}




ParabellumFramework::EUINT32
ParabellumEngine::Core::Scene::Octree::CreateFull(EUINT32 depth)
{
	EUINT32 result = SystemCodes::SUCCESS;

	if (depth == 0)
		return SystemCodes::SUCCESS;

	result = CreateChildren();

	if (result != SystemCodes::SUCCESS)
		return result;

	// recursive call
	for (int i = 0; i < 8; ++i)
	{
		m_children[i].CreateFull(depth - 1);
	}
	

	return result;
}










//	------------------------------------------------------
//	Gets and Sets
//	------------------------------------------------------


//
//
// get bounding box
//
//
ParabellumFramework::BoundingBox* 
ParabellumEngine::Core::Scene::Octree::GetBoundingBox()
{
	return &m_box;
}


//
//
// ger bounding sphere
//
//
ParabellumFramework::BoundingSphere* 
ParabellumEngine::Core::Scene::Octree::GetBoundingSphere()
{
	return &m_boundingSphere;
}

