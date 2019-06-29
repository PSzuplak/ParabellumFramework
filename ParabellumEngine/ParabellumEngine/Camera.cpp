#include "Camera.h"

using namespace ParabellumFramework;

//	------------------------------------------------------------
//		Constructors and destructors
//	------------------------------------------------------------
ParabellumEngine::Components::Camera::Camera()
{
	AddTag("Camera");
}


ParabellumEngine::Components::Camera::Camera(const Camera& other)
{
}


ParabellumEngine::Components::Camera::~Camera()
{
}




//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------

ParabellumFramework::EUINT32
ParabellumEngine::Components::Camera::Initialize(
	_IN_ Vector3D& position, _IN_ Vector3D& target, _IN_ Vector3D& up,
	EUINT32 screenWidth, EUINT32 screenHeight, EFLOAT32 fov, EFLOAT32 nearPlane, EFLOAT32 farPlane)
{
	SetPosition(position);

	Vector3D tmpRot = Vector3D::Zero();
	SetRotation(tmpRot);

	m_direction.X = target.X - position.X;
	m_direction.Y = target.Y - position.Y;
	m_direction.Z = target.Z - position.Z;

	m_direction.Normalize();

	m_up = up;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_aspectRatio = (float)screenWidth / (float)screenHeight;

	m_nearPlane = nearPlane;
	m_farPlane = farPlane;


	m_fov = fov;

	// initialize matrices
	m_projection.MakePerspectiveFieldOfViewLH(m_fov, m_aspectRatio, nearPlane, farPlane);


	m_directionPrevious = m_direction;
	m_positionPrevious = position;

	m_angleChanged = true;
	m_positionChanged = true;

	return 0;
}



void
ParabellumEngine::Components::Camera::Update()
{
	Vector3D up, position, target;
	float yaw, pitch, roll;
	Matrix4x4 rotationMatrix;


	// Up vector
	up.X = 0.0f;
	up.Y = 1.0f;
	up.Z = 0.0f;

	// copy camera position
	GetPosition(position);

	// default camera direction vector
	target.X = 0.0f;
	target.Y = 0.0f;
	target.Z = 1.0f;

	GetRotation(pitch, yaw, roll);
	pitch *= ParabellumUtils::Math::PIO180;
	yaw *= ParabellumUtils::Math::PIO180;
	roll *= ParabellumUtils::Math::PIO180;



	// create rotation matrix
	rotationMatrix.MakeRotationYawPitchRoll(yaw, pitch, 0.0f);

	// create lookat matrix
	Matrix4x4::Multiply(&rotationMatrix, &target);
	target.Normalize();

	// rotate up vector
	Matrix4x4::Multiply(&rotationMatrix, &up);
	up.Normalize();


	// set camera direction
	m_direction.X = target.X;
	m_direction.Y = target.Y;
	m_direction.Z = target.Z;

	target = position + m_direction;
	
	// set up vector
	m_up = up;


	m_view.MakeLookAtLH(&position, &target, &up);


	// set bounding sphere
	Vector3D center;
	center = (*GetPosition()) + (m_direction*m_farPlane) * 0.5f;
	m_boundingSphere.SetCenter(center);
	m_boundingSphere.SetRayLength(m_farPlane*0.5f);


	// update other matrices
	UpdateMatrices();

	// update bounding frustum
	m_frustum.MakeFromMatrix(&m_viewProjection);



	// Update 
	Vector3D corners[8];
	

	Vector4D cameraPositionFar;
	Vector4D ctl, ctr, cbr, cbl, ctl2ctr, ctl2cbl;

	Vector4D c2tl;
	Vector4D c2tr;
	Vector4D c2bl;

	Vector4D tl2tr;
	Vector4D tl2bl;

	GetCorners(corners);
	GetPosition(cameraPositionFar.XYZ);

	ctl = corners[4];
	ctr = corners[5];
	cbr = corners[6];
	cbl = corners[7];

	m_ctl = ctl - cameraPositionFar;
	c2tr = ctr - cameraPositionFar;
	c2bl = cbl - cameraPositionFar;

	m_ctl2ctr = c2tr - m_ctl;
	m_ctl2cbl = c2bl - m_ctl;

	m_ctr = ctr;
	m_cbr = cbr;
	m_cbl = c2bl;

	// check if camera changed its angle
	if (ParabellumUtils::Math::Abs(Vector3D::Dot(&m_directionPrevious, &m_direction)) > 0.99999f)
		m_angleChanged = true;
	else
		m_angleChanged = false;

	// check if camera changed its position

	if (ParabellumUtils::Math::Abs(Vector3D::Distance(&m_positionPrevious, &position)) > 0.0001f)
		m_positionChanged = true;
	else
		m_positionChanged = false;

	// save current values
	m_directionPrevious = m_direction;
	m_positionPrevious = position;
}





//
//
//	Update camera matrices
//
//
void ParabellumEngine::Components::Camera::UpdateMatrices()
{
	m_viewProjection = m_projection * m_view;
	m_inverseView = Matrix4x4::Invert(m_view);
	m_inverseProjection = Matrix4x4::Invert(m_projection);
	m_inverseViewProjection = Matrix4x4::Invert(m_viewProjection);
	m_transponseInverseProjection = Matrix4x4::Transponse(m_inverseViewProjection);
}



//
//
//	Check collision with BoundingSphere
//
//
ParabellumFramework::IntersectCodes
ParabellumEngine::Components::Camera::Intersects(_IN_ BoundingSphere* pVolume)
{
	IntersectCodes result;

	// check frustum
	result = m_frustum.Intersects(pVolume);

	return result;
}

//
//
//	Check collision with BoundingBox
//
//
ParabellumFramework::IntersectCodes
ParabellumEngine::Components::Camera::Intersects(_IN_ BoundingBox* pVolume)
{
	IntersectCodes result;

	// check frustum
	result = m_frustum.Intersects(pVolume);

	return result;

}

//
//
//	Check collision with vector
//
//
ParabellumFramework::IntersectCodes
ParabellumEngine::Components::Camera::Intersects(_IN_ Vector3D* pVolume)
{
	IntersectCodes result;

	// check sphere-point collision
	result = m_boundingSphere.Intersects(pVolume);

	if (result == IntersectCodes::DISJOINT)
		return IntersectCodes::DISJOINT;


	// check frustum-point collision
	result = m_frustum.Intersects(pVolume);

	if (result == IntersectCodes::DISJOINT)
		return IntersectCodes::DISJOINT;

	return IntersectCodes::INTERSECT;
}



//
//
//	Check collision with Componen3D - use all possible ways to do it
//
//
ParabellumFramework::IntersectCodes
ParabellumEngine::Components::Camera::Intersects(_IN_ Component3D* pC3D)
{
	// check collision sphere-sphere
	if (m_boundingSphere.Intersects(pC3D->GetTransformedBoundingSphere()) == IntersectCodes::DISJOINT)
		return IntersectCodes::DISJOINT;

	// check collision frustum-sphere
	if (m_frustum.Intersects(pC3D->GetTransformedBoundingSphere()) == IntersectCodes::DISJOINT)
		return IntersectCodes::DISJOINT;

	// check collisiton frustum-box
	if (m_frustum.Intersects(pC3D->GetTransformedBoundingBox()) == IntersectCodes::DISJOINT)
		return IntersectCodes::DISJOINT;

	return IntersectCodes::INTERSECT;
}






//	------------------------------------------------------------
//		Gets and Sets
//	------------------------------------------------------------

void ParabellumEngine::Components::Camera::GetDirection(Vector3D& vec)
{
	vec = m_direction;
}

ParabellumFramework::Vector3D* 
ParabellumEngine::Components::Camera::GetDirection()
{
	return &m_direction;
}

void 
ParabellumEngine::Components::Camera::GetDirection(EFLOAT32& x, EFLOAT32& y, EFLOAT32& z)
{
	x = m_direction.X;
	y = m_direction.Y;
	z = m_direction.Z;
}


void 
ParabellumEngine::Components::Camera::GetUp(_OUT_ Vector3D& vec)
{
	vec = m_up;
}

ParabellumFramework::Vector3D*
ParabellumEngine::Components::Camera::GetUp()
{
	return &m_up;
}




ParabellumFramework::EFLOAT32 
ParabellumEngine::Components::Camera::GetZNear()
{
	return m_nearPlane;
}


ParabellumFramework::EFLOAT32 
ParabellumEngine::Components::Camera::GetZFar()
{
	return m_farPlane;
}

ParabellumFramework::EFLOAT32 
ParabellumEngine::Components::Camera::GetFOV()
{
	return m_fov;
}

void 
ParabellumEngine::Components::Camera::GetCorners(_OUT_ Vector3D* pVectors)
{
	m_frustum.GetCorners(pVectors);
}







void 
ParabellumEngine::Components::Camera::GetView(_OUT_ Matrix4x4& viewMatrix)
{
	viewMatrix = m_view;
}

ParabellumFramework::Matrix4x4* 
ParabellumEngine::Components::Camera::GetView()
{
	return &m_view;
}


void 
ParabellumEngine::Components::Camera::GetProjection(_OUT_ Matrix4x4& projectionMatrix)
{
	projectionMatrix = m_projection;
}


ParabellumFramework::Matrix4x4* 
ParabellumEngine::Components::Camera::GetProjection()
{
	return &m_projection;
}




void 
ParabellumEngine::Components::Camera::GetViewProjection(_OUT_ Matrix4x4& output)
{
	output = m_viewProjection;
}

ParabellumFramework::Matrix4x4*
ParabellumEngine::Components::Camera::GetViewProjection()
{
	return &m_viewProjection;
}


void 
ParabellumEngine::Components::Camera::GetInverseView(_OUT_ Matrix4x4& output)
{
	output = m_inverseView;
}

ParabellumFramework::Matrix4x4*
ParabellumEngine::Components::Camera::GetInverseView()
{
	return &m_inverseView;
}


void 
ParabellumEngine::Components::Camera::GetInverseProjection(_OUT_ Matrix4x4& output)
{
	output = m_inverseProjection;
}

ParabellumFramework::Matrix4x4*
ParabellumEngine::Components::Camera::GetInverseProjection()
{
	return &m_inverseProjection;
}


void ParabellumEngine::Components::Camera::GetInverseViewProjection(_OUT_ Matrix4x4& output)
{
	output = m_inverseViewProjection;
}

ParabellumFramework::Matrix4x4*
ParabellumEngine::Components::Camera::GetInverseViewProjection()
{
	return &m_inverseViewProjection;
}


void 
ParabellumEngine::Components::Camera::GetTransponseInverseProjection(_OUT_ Matrix4x4& output)
{
	output = m_transponseInverseProjection;
}

ParabellumFramework::Matrix4x4*
ParabellumEngine::Components::Camera::GetTransponseInverseProjection()
{
	return &m_transponseInverseProjection;
}




ParabellumFramework::BoundingFrustum*
ParabellumEngine::Components::Camera::GetFrustum()
{
	return &m_frustum;
}




void ParabellumEngine::Components::Camera::WorldPositionFromLinearDepth(
	_OUT_ Vector3D* pOut, _IN_ Vector3D* pScreenPosition)
{
	Vector4D position;
	position.X = pScreenPosition->X;
	position.Y = pScreenPosition->Y;
	position.Z = pScreenPosition->Z;
	position.W = 1.0f;

	Vector4D viewPosition;
	viewPosition = (*GetInverseProjection()) * position;

	Vector4D viewRay;
	viewRay.X = viewPosition.X * (m_farPlane / viewPosition.Z);
	viewRay.Y = viewPosition.Y * (m_farPlane / viewPosition.Z);
	viewRay.Z = m_farPlane;
	viewRay *= pScreenPosition->Z;
	viewRay.W = 1.0f;

	Vector4D worldPosition;
	worldPosition = (*GetInverseView()) * viewRay;

	pOut->X = worldPosition.X;
	pOut->Y = worldPosition.Y;
	pOut->Z = worldPosition.Z;
}


ParabellumFramework::Vector3D
ParabellumEngine::Components::Camera::GetWorldFromFrustumCorners(Vector2D UV, EFLOAT32 depth)
{
	Vector3D c2f;
	
	Vector3D Nctl2ctr = m_ctl2ctr.XYZ * UV.X;
	Vector3D Nctl2cbl = m_ctl2cbl.XYZ * UV.Y;

	c2f = Nctl2ctr + Nctl2cbl;
	c2f += m_ctl.XYZ;
	c2f *= depth;

	c2f += *GetPosition();

	return c2f;
	
}



void 
ParabellumEngine::Components::Camera::GetProjected(
	_IN_ BoundingBox* pBox, _OUT_ Vector3D* pTopLeft, _OUT_ Vector3D* pBottomRight)
{
	// get box corners
	Vector3D boxCorners[8];
	pBox->GetCorners(boxCorners);

	// transform to screen space
	Vector4D vec;

	Vector3D projectedCoords[8];

	for (int i = 0; i < 8; ++i)
	{
		vec.X = boxCorners[i].X;
		vec.Y = boxCorners[i].Y;
		vec.Z = boxCorners[i].Z;
		vec.W = 1.0f;

		vec = (*GetViewProjection()) * vec;
		vec /= vec.W;

		PF_VECTOR3D_Set(projectedCoords[i], vec.X, vec.Y, vec.Z);
	}

	// clip to screen boundary
	EFLOAT32 minX, minY, maxX, maxY;

	minX = ParabellumUtils::Math::Min(projectedCoords[0].X, projectedCoords[1].X);
	minY = ParabellumUtils::Math::Min(projectedCoords[0].Y, projectedCoords[1].Y);
	maxX = ParabellumUtils::Math::Max(projectedCoords[0].X, projectedCoords[1].X);
	maxY = ParabellumUtils::Math::Max(projectedCoords[0].Y, projectedCoords[1].Y);

	for (int i = 2; i < 8; ++i)
	{
		minX = ParabellumUtils::Math::Min(minX, projectedCoords[i].X);
		minY = ParabellumUtils::Math::Min(minY, projectedCoords[i].Y);
		maxX = ParabellumUtils::Math::Max(maxX, projectedCoords[i].X);
		maxY = ParabellumUtils::Math::Max(maxY, projectedCoords[i].Y);
	}

	// minY and maxY mus be swapped due to screen cooridnates idea
	pTopLeft->Set(minX, maxY, 1.0f);
	pBottomRight->Set(maxX, minY, 1.0f);
}




ParabellumFramework::Vector4D* 
ParabellumEngine::Components::Camera::GetCTL()
{
	return &m_ctl;
}

ParabellumFramework::Vector4D* 
ParabellumEngine::Components::Camera::GetCTL2CTR()
{
	return &m_ctl2ctr;
}

ParabellumFramework::Vector4D* 
ParabellumEngine::Components::Camera::GetCTL2CBL()
{
	return &m_ctl2cbl;
}