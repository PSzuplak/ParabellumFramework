#include "DirectionalLight.h"



//	------------------------------------------------------------
//		Constructors and destructors
//	------------------------------------------------------------
ParabellumEngine::Components::DirectionalLight::DirectionalLight()
{
	m_lightDirection = Vector3D(0.0f, -0.9f, 0.0f);
	m_lightColor = Vector3D(1.0f, 1.0f, 1.0f);
	m_intensity = 1.0f;

	m_CSMCount = 3;

	m_projectionsSizes[0] = 0.1f;
	m_projectionsSizes[1] = 0.1f;
	m_projectionsSizes[2] = 0.1f;
	m_projectionsSizes[3] = 0.02f;
	m_projectionsSizes[4] = 0.05f;
	m_projectionsSizes[5] = 0.01f;
	m_projectionsSizes[6] = 0.1f;
	m_projectionsSizes[7] = 0.1f;
	m_projectionsSizes[8] = 0.1f;

	m_sliceSize[0] = 0.0f;
	m_sliceSize[1] = 0.002f;
	m_sliceSize[2] = 0.05f;
	m_sliceSize[3] = 0.1f;
	m_sliceSize[4] = 0.2f;
	m_sliceSize[5] = 0.6f;
	m_sliceSize[6] = 0.7f;
	m_sliceSize[7] = 0.8f;
	m_sliceSize[8] = 1.0f;


	for (int i = 0; i < 9; ++i)
	{
		m_zNear[i] = 1.0f;
		m_zFar[i] = 1000.0f;

		m_projection[i].MakeOrthoLH(m_projectionsSizes[i], m_projectionsSizes[i], m_zNear[i], m_zFar[i]);
	}
}


ParabellumEngine::Components::DirectionalLight::~DirectionalLight()
{
}

//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------


void
ParabellumEngine::Components::DirectionalLight::Initialize(_IN_ Vector3D& lightDirection, _IN_ Vector3D& lightColor, EFLOAT32 lightIntensity)
{
	m_lightDirection = lightDirection;
	m_lightColor = lightColor;
	m_intensity = lightIntensity;

	m_CSMCount = 3;

	m_projectionsSizes[0] = 0.6f;
	m_projectionsSizes[1] = 0.3f;
	m_projectionsSizes[2] = 0.1f;
	m_projectionsSizes[3] = 0.02f;
	m_projectionsSizes[4] = 0.05f;
	m_projectionsSizes[5] = 0.01f;
	m_projectionsSizes[6] = 0.1f;
	m_projectionsSizes[7] = 0.1f;
	m_projectionsSizes[8] = 0.1f;

	m_sliceSize[0] = 0.0f;
	m_sliceSize[1] = 0.01f;
	m_sliceSize[2] = 0.025f;
	m_sliceSize[3] = 0.070f;
	m_sliceSize[4] = 0.15f;
	m_sliceSize[5] = 0.6f;
	m_sliceSize[6] = 0.7f;
	m_sliceSize[7] = 0.8f;
	m_sliceSize[8] = 1.0f;


	for (int i = 0; i < 9; ++i)
	{
		m_zNear[i] = 1.0f;
		m_zFar[i] = 1000.0f;

		m_projection[i].MakeOrthoLH(m_projectionsSizes[i], m_projectionsSizes[i], m_zNear[i], m_zFar[i]);
	}
}


void ParabellumEngine::Components::DirectionalLight::Update()
{

}

void
ParabellumEngine::Components::DirectionalLight::CalculateLight3DData(_IN_ Camera* pCamera)
{
	m_up = Vector3D::Up();

	// calculate directional light position
	Vector3D cameraPos;

	Vector3D corners[8];

	// caculate slicesw
	Vector3D sliceCorners[8], position;


	Vector3D vecZero, vecUp, target;

	/*
	for (EUINT32 i = 0; i < 4; ++i)
	{
		pCamera->GetFrustum()->GetSpliceCorners(sliceCorners, m_sliceSize[i], m_sliceSize[i + 1]);

		BoundingSphere spherex;
		spherex.MakeFromPoints(sliceCorners, 8);

		m_CSMBoxes[i].MakeFromPoints(sliceCorners, 8);

		EFLOAT32 sliceRad = spherex.GetRayLength();

		position = *spherex.GetCenter();
		position.Y = 0.0f;

		position -= (m_lightDirection * 500.0f);

		target = m_lightDirection;
		target += position;

		m_viewTemp[i].MakeLookAtLH(&position, &target, &m_up);
		m_projection[i].MakeOrthoLH(sliceRad, sliceRad, m_zNear[i], m_zFar[i]);
	}
	*/


	//00000000000000000000000000000000000000
	// light frustom alvays above the camera
	for (EUINT32 i = 0; i < 4; ++i)
	{
		pCamera->GetFrustum()->GetSpliceCorners(sliceCorners, m_sliceSize[i], m_sliceSize[i + 1]);

		BoundingSphere spherex;
		spherex.MakeFromPoints(sliceCorners, 8);

		m_CSMBoxes[i].MakeFromPoints(sliceCorners, 8);

		EFLOAT32 sliceRad = spherex.GetRayLength();

		position = *pCamera->GetPosition();
		position.Y = 0.0f;

		position -= (m_lightDirection * 500.0f);

		target = m_lightDirection;
		target += position;

		m_viewTemp[i].MakeLookAtLH(&position, &target, &m_up);
		m_projection[i].MakeOrthoLH(sliceRad, sliceRad, m_zNear[i], m_zFar[i]);
	}

}



void
ParabellumEngine::Components::DirectionalLight::CreateLightViewProjectionMatrix(_IN_ EUINT32 no)
{
	m_view[no] = m_viewTemp[no];
	m_viewProjection[no] = m_projection[no] * m_view[no];

	m_frustums[no].MakeFromMatrix(&m_viewProjection[no]);
}



//	------------------------------------------------------------
//		Gets and Sets
//	------------------------------------------------------------

ParabellumFramework::Vector3D*
ParabellumEngine::Components::DirectionalLight::GetDirection()
{
	return &m_lightDirection;
}

ParabellumFramework::Vector3D*
ParabellumEngine::Components::DirectionalLight::GetColor()
{
	return &m_lightColor;
}

ParabellumFramework::EFLOAT32
ParabellumEngine::Components::DirectionalLight::GetIntensity()
{
	return m_intensity;
}


void
ParabellumEngine::Components::DirectionalLight::SetDirection(Vector3D& val)
{
	m_lightDirection = val;
}

void
ParabellumEngine::Components::DirectionalLight::SetColor(Vector3D& val)
{
	m_lightColor = val;
}

void
ParabellumEngine::Components::DirectionalLight::SetIntensity(EFLOAT32 val)
{
	m_intensity = val;
}
