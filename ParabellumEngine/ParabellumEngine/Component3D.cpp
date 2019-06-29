#include "Component3D.h"
#include "stdafx.h"



//	-----------------------------------------------------------------
//		Statics
//	-----------------------------------------------------------------
//	-----------------------------------------------------------------
//		Constructors and descrutors
//	-----------------------------------------------------------------

ParabellumEngine::Components::Component3D::Component3D()
{
	m_parent = NULLPTR;

	m_position.MakeZero();
	m_rotation.MakeZero();
	m_scale.MakeOne();

	m_scaleMatrix.MakeScale(1.0f, 1.0f, 1.0f);
	m_rotationMatrix.MakeRotationYawPitchRoll(0.0f, 0.0f, 0.0f);
	m_translationMatrix.MakeTranslation(0.0f, 0.0f, 0.0f);
	m_world = m_translationMatrix * m_rotationMatrix * m_scaleMatrix;

	m_boundingConvex = NULLPTR;
}


ParabellumEngine::Components::Component3D::Component3D(Component3D&)
{
}


ParabellumEngine::Components::Component3D::~Component3D()
{
	if (m_boundingConvex != NULLPTR)
		delete m_boundingConvex;
}



//	-----------------------------------------------------------------
//		Methods
//	-----------------------------------------------------------------

//
//
//	Initialize
//
//
void ParabellumEngine::Components::Component3D::Initialize()
{
}


//
//
//	Release
//
//
void ParabellumEngine::Components::Component3D::Release()
{

}


//
//
//	Draw
//
//
void ParabellumEngine::Components::Component3D::Draw()
{
}


//
//
//	Update
//
//
void ParabellumEngine::Components::Component3D::Update()
{
}


//
//
//	Update m_world matrix depending on actual value of position, rotation and scale
//
//
void ParabellumEngine::Components::Component3D::UpdateWorldMatrix()
{
	// exit if world matrix does not need udate
	if (m_parent == NULLPTR)
	{
		if (m_isWorldNeedsUpdate == true)
			m_isWorldNeedsUpdate = false;
		else
			return;
	}
	else if (IsUpdateNeeded() == false)
	{
		return;
	}
	else
		m_isWorldNeedsUpdate = false;



	// calculate matrices
	//m_scaleMatrix.SetScaleFields(m_scale.X, m_scale.Y, m_scale.Z);
	//m_rotationMatrix.MakeRotationYawPitchRoll(m_rotation.Y, m_rotation.X, m_rotation.Z);
	//m_translationMatrix.SetPositionFields(m_position.X, m_position.Y, m_position.Z);

	//PF_MATRIX4X4_SetScaleFields(m_scaleMatrix, m_scale.X, m_scale.Y, m_scale.Z);
	//PF_MATRIX4X4_MakeRotationYawPitchRoll(m_rotationMatrix, m_rotation.Y, m_rotation.X, m_rotation.Z);
	//PF_MATRIX4X4_SetPositionFields(m_translationMatrix, m_position.X, m_position.Y, m_position.Z);
	
	PF_MATRIX4X4_SetScaleFields(m_scaleMatrix, m_scale.X, m_scale.Y, m_scale.Z);
	PF_MATRIX4X4_MakeRotationYawPitchRoll(m_rotationMatrix, m_rotation.Y, m_rotation.X, m_rotation.Z);
	PF_MATRIX4X4_SetPositionFields(m_translationMatrix, m_position.X, m_position.Y, m_position.Z);


	if (m_parent != NULLPTR)
	{
		m_world = (m_translationMatrix * m_rotationMatrix * m_scaleMatrix) * (*m_parent->GetWorldMatrix());
	}
	else
	{
		m_world = m_translationMatrix;
		PF_MATRIX4X4_MULTIPLY(m_rotationMatrix, m_world, m_world);
		PF_MATRIX4X4_MULTIPLY(m_scaleMatrix, m_world, m_world);
	}


	// update bounding volumes
	UpdateBoundingVolumes();


	// update children matrices
	ForceUpdateChildren();


	// set no need to update
	m_isWorldNeedsUpdate = false;


	// evecute events
	Events.EventOnMove(this);
}


//
//
//	Update m_boundingBoxTransformed and m_boundingSphereTransformed
//
//
void ParabellumEngine::Components::Component3D::UpdateBoundingVolumes()
{
	// update AABB
	Vector3D points[8];
	Vector3D position, scale, rotation, vmin, vmax;
	
	m_boundingBox.GetCorners(points);

	GetRelativePosition(position);
	GetRelativeScale(scale);
	GetRelativeRotation(rotation);
	scale *= 0.5f;

	m_boundingBoxExtendedTransformed.Make(&m_boundingBox, &scale, &rotation, &position);
	m_boundingBoxTransformed.MakeFromPoints(m_boundingBoxExtendedTransformed.GetCorners(), 8);

	// Update sphere
	m_boundingBoxTransformed.GetMinMax(vmin, vmax);

	EFLOAT32 rayLen = (vmax - vmin).Length() * 0.5f;

	vmin = *m_boundingSphere.GetCenter();
	vmin += position;

	m_boundingSphereTransformed.MakeBoundingSphere(&vmin, rayLen);

	// update Volume
	//if (m_collisionVolume != NULLPTR)
	//{
	//	m_collisionVolume->Transform(&m_world);
	//}

	m_isBoundingConvexNeedUpdate = true;
}



//	-----------------------------------------------------------------
//		Gets and Sets
//	-----------------------------------------------------------------


/*
Octree* ParabellumEngine::Components::Component3D::GetOctreeNode()
{
return m_octrreNode;
}


void ParabellumEngine::Components::Component3D::SetOctreeNode(_IN_ Octree* pNode)
{
m_octrreNode = pNode;
}
*/



ParabellumFramework::Vector3D*
ParabellumEngine::Components::Component3D::GetPosition()
{
	return &m_position.XYZ;
}


ParabellumFramework::Vector3D*
ParabellumEngine::Components::Component3D::GetRotation()
{
	return &m_rotation.XYZ;;
}


ParabellumFramework::Vector3D*
ParabellumEngine::Components::Component3D::GetScale()
{
	return &m_scale.XYZ;;
}


void
ParabellumEngine::Components::Component3D::GetPosition(_OUT_ Vector3D& out)
{
	out = m_position;
}


void
ParabellumEngine::Components::Component3D::GetRotation(_OUT_ Vector3D& out)
{
	out = m_rotation;
}


void
ParabellumEngine::Components::Component3D::GetScale(_OUT_ Vector3D& out)
{
	out = m_scale;
}


void
ParabellumEngine::Components::Component3D::GetPosition(EFLOAT32& x, EFLOAT32& y, EFLOAT32& z)
{
	x = m_position.X;
	y = m_position.Y;
	z = m_position.Z;
}

void
ParabellumEngine::Components::Component3D::GetRotation(EFLOAT32& x, EFLOAT32& y, EFLOAT32& z)
{
	x = m_rotation.X;
	y = m_rotation.Y;
	z = m_rotation.Z;
}


void
ParabellumEngine::Components::Component3D::GetScale(EFLOAT32& x, EFLOAT32& y, EFLOAT32& z)
{
	x = m_scale.X;
	y = m_scale.Y;
	z = m_scale.Z;
}



//
//
//
//
//
void
ParabellumEngine::Components::Component3D::GetRelativePosition(_OUT_ Vector3D& out)
{
	UpdateWorldMatrix();
	m_world.GetTranslation(&out);
}


//
//
//
//
//
void
ParabellumEngine::Components::Component3D::GetRelativeRotation(_OUT_ Vector3D& out)
{
	UpdateWorldMatrix();
	m_world.GetRotation(&out);
}

//
//
//
//
//
void
ParabellumEngine::Components::Component3D::GetRelativeScale(_OUT_ Vector3D& out)
{
	//UpdateWorldMatrix();
	//
	//Vector3D tpm(0.0f, 0.0f, 0.0f);
	//
	//out += m_scale;
	//
	//if (m_parent != NULLPTR)
	//{
	//	m_parent->GetRelativeScale(out);
	//}

	UpdateWorldMatrix();
	m_world.GetScale(&out);
}


void
ParabellumEngine::Components::Component3D::SetPosition(_IN_ Vector3D& out)
{
	m_position = out;
	m_isWorldNeedsUpdate = true;
}


void
ParabellumEngine::Components::Component3D::SetRotation(_IN_ Vector3D& out)
{
	m_rotation = out;
	m_isWorldNeedsUpdate = true;
}


void
ParabellumEngine::Components::Component3D::SetScale(_IN_ Vector3D& out)
{
	m_scale = out;
	m_isWorldNeedsUpdate = true;
}


void
ParabellumEngine::Components::Component3D::SetPosition(EFLOAT32 x, EFLOAT32 y, EFLOAT32 z)
{
	m_position.X = x;
	m_position.Y = y;
	m_position.Z = z;

	m_isWorldNeedsUpdate = true;
}


void
ParabellumEngine::Components::Component3D::SetRotation(EFLOAT32 x, EFLOAT32 y, EFLOAT32 z)
{
	m_rotation.X = x;
	m_rotation.Y = y;
	m_rotation.Z = z;

	m_isWorldNeedsUpdate = true;
}


void
ParabellumEngine::Components::Component3D::SetScale(EFLOAT32 x, EFLOAT32 y, EFLOAT32 z)
{
	m_scale.X = x;
	m_scale.Y = y;
	m_scale.Z = z;

	m_isWorldNeedsUpdate = true;
}






ParabellumFramework::Matrix4x4*
ParabellumEngine::Components::Component3D::GetTranslationMatrix()
{
	UpdateWorldMatrix();
	return &m_translationMatrix;
}


ParabellumFramework::Matrix4x4*
ParabellumEngine::Components::Component3D::GetRotationMatrix()
{
	UpdateWorldMatrix();
	return &m_rotationMatrix;
}


ParabellumFramework::Matrix4x4*
ParabellumEngine::Components::Component3D::GetScaleMatrix()
{
	UpdateWorldMatrix();
	return &m_scaleMatrix;
}


ParabellumFramework::Matrix4x4*
ParabellumEngine::Components::Component3D::GetWorldMatrix()
{
	UpdateWorldMatrix();
	return &m_world;
}




//
//
// Return m_boundingBoxTransformed
//
//
ParabellumFramework::BoundingBox*
ParabellumEngine::Components::Component3D::GetTransformedBoundingBox()
{
	UpdateWorldMatrix();
	return &m_boundingBoxTransformed;
}


//
//
// Return m_boundingSphereTransformed
//
//
ParabellumFramework::BoundingSphere*
ParabellumEngine::Components::Component3D::GetTransformedBoundingSphere()
{
	UpdateWorldMatrix();
	return &m_boundingSphereTransformed;
}

//
//
// Return m_boundingSphereTransformed
//
//
ParabellumFramework::OrientedBoundingBox*
ParabellumEngine::Components::Component3D::GetTransformedOrientedBoundingBox()
{
	UpdateWorldMatrix();
	return &m_boundingBoxExtendedTransformed;
}

//
//
//
//
//
ParabellumFramework::Volume* 
ParabellumEngine::Components::Component3D::GetBoundingConvex()
{
	if (m_isBoundingConvexNeedUpdate == true)
	{
		m_boundingConvex->TransformOriginal(&m_world);
		m_isBoundingConvexNeedUpdate = false;
	}

	return m_boundingConvex;
}

//
//
// Get Bounding Box in components Local Coordinates
//
//
ParabellumFramework::BoundingBox*
ParabellumEngine::Components::Component3D::GetLocalBoundingBox()
{
	return &m_boundingBox;
}



//
//
// Get Bounding Sphere in components Local Coordinates
//
//
ParabellumFramework::BoundingSphere*
ParabellumEngine::Components::Component3D::GetLocalBoundingSphere()
{
	UpdateWorldMatrix();
	return &m_boundingSphere;
}



//
//
//
//
//
void
ParabellumEngine::Components::Component3D::GetBoundingBox(_OUT_ BoundingBox* box)
{
	Vector3D vmin, vmax;
	m_boundingBox.GetMinMax(vmin, vmax);
	box->MakeFromPoints(&vmin, &vmax);
}

//
//
//
//
//
void
ParabellumEngine::Components::Component3D::GetBoundingSphere(_OUT_ BoundingSphere* sphere)
{
	Vector3D center;
	center = *m_boundingSphere.GetCenter();
	center += (*GetPosition());
	sphere->SetCenter(center);

	EFLOAT32 ray;
	ray = m_boundingSphere.GetRayLength() * GetScale()->GetMax();
	sphere->SetRayLength(ray);
}





//
//
//
//
//
void
ParabellumEngine::Components::Component3D::SetParent(_IN_ Component3D* pParent)
{
	// quit if tries to set the same parent
	if (m_parent == pParent)
		return;


	// remove parent by setting NULL
	if ((pParent == NULLPTR) && (m_parent != NULLPTR))
	{
		m_parent->RemoveChild(this);
		m_parent = NULLPTR;
	}


	if (pParent != NULLPTR)
	{
		if (m_parent != NULLPTR)
		{
			m_parent->RemoveChild(this);
			m_parent = NULLPTR;
		}

		m_parent = pParent;
		m_parent->AddChild(this);
	}
}




//
//
//
//
//
ParabellumEngine::Components::Component3D*
ParabellumEngine::Components::Component3D::GetParent()
{
	return m_parent;
}




//
//
//
//
//
void
ParabellumEngine::Components::Component3D::AddChild(_IN_ Component3D* pChild)
{
	for (auto it : m_children)
		if (it == pChild)
			return;

	m_children.push_back(pChild);
	pChild->SetParent(this);
}



//
//
//
//
//
void
ParabellumEngine::Components::Component3D::RemoveChild(_IN_ Component3D* pChild)
{
	for (auto it : m_children)
		if (it == pChild)
		{
			m_children.remove(pChild);
			pChild->m_parent = NULLPTR;
		}	
}




bool 
ParabellumEngine::Components::Component3D::IsUpdateNeeded()
{
	if (m_parent != NULLPTR)
	{
		return m_isWorldNeedsUpdate | m_parent->IsUpdateNeeded();
	}
	else
		return m_isWorldNeedsUpdate;
}




void 
ParabellumEngine::Components::Component3D::ForceUpdateChildren()
{
	if (m_children.size() > 0)
		for (auto it : m_children)
		{
			it->m_isWorldNeedsUpdate = true;
			it->ForceUpdateChildren();
		}
}