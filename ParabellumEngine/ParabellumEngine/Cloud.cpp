#include "Cloud.h"


//	------------------------------------------------------------
//		Constructors and destructors
//	------------------------------------------------------------
ParabellumEngine::Components::Cloud::Cloud(_IN_ GraphicsDevice* pDevice, _IN_ ResourceManager* pResourceManager)
{
	m_RTClouds = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, 1920, 1080);

	m_RTCloudsBluredVertical = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, 1920, 1080);

	m_RTCloudsBluredHorizontal = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, 1920, 1080);

	m_RTFinal = pDevice->CreateRenderTarget(
		GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, GraphicsFormat::FORMAT_R32G32B32A32_FLOAT, 1920, 1080);


	m_SDrawCloud = pResourceManager->Load<Shader>("Resources/Shaders/Sky_DrawCloud.psx");

	m_SDrawCloudSphere = pResourceManager->Load<Shader>("Resources/Shaders/Sky_DrawCloudSphere.psx");

	m_SBlurVertical = pResourceManager->Load<Shader>("Resources/Shaders/Blur_Gaussian_Vertical.psx");

	m_T2DCloud = pResourceManager->Load<Texture2D>("Resources/Textures/cloud_test.pft");
	m_T2DCloudSphere = pResourceManager->Load<Texture2D>("Resources/Textures/cloud_sphere.pft");

	m_MSphereModel = pResourceManager->Load<Model>("Resources/Models/Sphere_16x8.pfm");

	m_renderQuad.Initialize(pDevice, -1.0f, 1.0f, 1.0f, -1.0f);


	//
	// setting

	Vector3D lightDirection;
	lightDirection.Set(0.3f, -1.0f, -0.4f);
	lightDirection.Normalize();

	Vector3D cutVector;
	EFLOAT32 cutAngle;
	bool isOptimized;

	cutVector.Set(0.0f, 1.0f, 0.0f);
	cutAngle = 0.5f;
	isOptimized = false;





	//
	// Set spheres
	Vector3D max;

	max.Set(6.0f, 2.0f, 6.0f);

	m_cloudSpheresCount = 16;

	m_cloudSpheres = new BoundingSphere[m_cloudSpheresCount];
	

	// set spheres
	for (EUINT32 i = 0; i < m_cloudSpheresCount; ++i)
	{
		Vector3D position;
		EFLOAT32 ray;

		position.X = ParabellumUtils::Math::RandF(0.0f, max.X);
		position.Y = ParabellumUtils::Math::RandF(0.0f, max.Y);
		position.Z = ParabellumUtils::Math::RandF(0.0f, max.Z);
		ray = ParabellumUtils::Math::RandF(2.0f, 6.0f) / 3.0f;

		m_cloudSpheres[i].SetCenter(position);
		m_cloudSpheres[i].SetRayLength(ray);
	}


	//
	// initialize mesh 
	EUINT32 bilbCountPerAxis = 8;
	EUINT32 bilbCount = bilbCountPerAxis*bilbCountPerAxis*bilbCountPerAxis;
	EUINT32 verticesCount = bilbCount * 3 * m_cloudSpheresCount;

	EUINT32* indices = new EUINT32[verticesCount];
	CloudVertexFormat* vertices = new CloudVertexFormat[verticesCount];


	//
	// create mesh
	EUINT32 index = 0;
	EUINT32 counter = 0;

	for (EUINT32 i = 0; i < m_cloudSpheresCount; ++i)
	{
		for (EUINT32 ax = 0; ax < bilbCountPerAxis; ++ax)
		{
			for (EUINT32 ay = 0; ay < bilbCountPerAxis; ++ay)
			{
				for (EUINT32 az = 0; az < bilbCountPerAxis; ++az)
				{
					Vector3D position;
					Vector3D targetVec, binormal, tmpvec;
					Vector3D triangle[3];
					EFLOAT32 angleX, angleY, angleZ;
					EFLOAT32 bibloardScale, destScale, density;

					density = 0.0f;

					bibloardScale = m_cloudSpheres[i].GetRayLength() * ParabellumUtils::Math::RandF(0.1f, 0.5f);

					destScale = m_cloudSpheres[i].GetRayLength() * ParabellumUtils::Math::RandF(0.0f, 0.5f) - 1.0f;


					angleX = ((EFLOAT32)ax / (EFLOAT32)bilbCountPerAxis) * ParabellumUtils::Math::PI2 + ParabellumUtils::Math::RandF(0.0f, (EFLOAT32)bilbCountPerAxis / ParabellumUtils::Math::PI2);
					angleY = ((EFLOAT32)ay / (EFLOAT32)bilbCountPerAxis) * ParabellumUtils::Math::PI2 + ParabellumUtils::Math::RandF(0.0f, (EFLOAT32)bilbCountPerAxis / ParabellumUtils::Math::PI2);
					angleZ = ((EFLOAT32)az / (EFLOAT32)bilbCountPerAxis) * ParabellumUtils::Math::PI2 + ParabellumUtils::Math::RandF(0.0f, (EFLOAT32)bilbCountPerAxis / ParabellumUtils::Math::PI2);


					targetVec.Set(0.0f, 1.0f, 0.0f);

					if(ParabellumUtils::Math::RandI() % 2 == 0)
						binormal.Set(1.0f, 0.0f, 0.0f);
					else
						binormal.Set(0.0f, 0.0f, 1.0f);

					targetVec.Normalize();
					binormal.Normalize();

					targetVec.RotateX(angleX);
					targetVec.RotateY(angleY);
					targetVec.RotateZ(angleZ);

					//
					// do NOT draw bilboards which are no seen in camera
					// - they are on the top of the cloud-sphere
					if ( (isOptimized == true) && (Vector3D::Dot(&cutVector, &targetVec) > cutAngle) )
						continue;


					binormal.RotateY(ParabellumUtils::Math::RandF(0.0f, 3.14f) );

					binormal.RotateX(angleX);
					binormal.RotateY(angleY);
					binormal.RotateZ(angleZ);


					triangle[0] = targetVec * bibloardScale;
					triangle[1] = binormal * bibloardScale;
					triangle[2] = -binormal * bibloardScale;

					triangle[0] -= targetVec * bibloardScale * destScale;
					triangle[1] -= targetVec * bibloardScale * destScale;
					triangle[2] -= targetVec * bibloardScale * destScale;



					triangle[0] += targetVec *  m_cloudSpheres[i].GetRayLength();
					triangle[1] += targetVec *  m_cloudSpheres[i].GetRayLength(); 
					triangle[2] += targetVec *  m_cloudSpheres[i].GetRayLength(); 

					triangle[0] += *m_cloudSpheres[i].GetCenter();
					triangle[1] += *m_cloudSpheres[i].GetCenter();
					triangle[2] += *m_cloudSpheres[i].GetCenter();

					triangle[0] -= 1.0f;
					triangle[1] -= 1.0f;
					triangle[2] -= 1.0f;

					// check if triangle collides with any other sphere,
					// if so do NOT draw it
					bool isCollide = false;

					for (EUINT32 n = 0; n < m_cloudSpheresCount; ++n)
					{
						EFLOAT32 dist = Vector3D::Distance(m_cloudSpheres[n].GetCenter(), &triangle[0]);

						if (m_cloudSpheres[n].Intersects(&triangle[0]) != IntersectCodes::DISJOINT)
							isCollide = true;

						if (dist < m_cloudSpheres[n].GetRayLength())
							isCollide = true;
					}


					if (isCollide == true) continue;



					//
					// calculate density
					Vector3D lightStep, currentPosition;
					lightStep = -lightDirection;
					lightStep *= 0.1f;
					currentPosition = triangle[0];

					density = 1.0f;

					for (EUINT32 o = 0; o < 100; ++o)
					{
						currentPosition += lightStep;

						for (EUINT32 n = 0; n < m_cloudSpheresCount; ++n)
						{
							if (m_cloudSpheres[n].Intersects(&currentPosition) != IntersectCodes::DISJOINT)
							{
								density -= 0.01f;
								break;
							}
						}
					}








					//
					// fill the vertices array
					vertices[index + 0].position[0] = triangle[0].X;
					vertices[index + 0].position[1] = triangle[0].Y;
					vertices[index + 0].position[2] = triangle[0].Z;
					vertices[index + 0].coords[0] = 0.5f;
					vertices[index + 0].coords[1] = 0.0f;
					vertices[index + 0].properties[0] = density;
					vertices[index + 0].properties[1] = 0.0f;

					vertices[index + 1].position[0] = triangle[1].X;
					vertices[index + 1].position[1] = triangle[1].Y;
					vertices[index + 1].position[2] = triangle[1].Z;
					vertices[index + 1].coords[0] = 0.0f;
					vertices[index + 1].coords[1] = 1.0f;
					vertices[index + 1].properties[0] = density;
					vertices[index + 1].properties[1] = 0.0f;

					vertices[index + 2].position[0] = triangle[2].X;
					vertices[index + 2].position[1] = triangle[2].Y;
					vertices[index + 2].position[2] = triangle[2].Z;
					vertices[index + 2].coords[0] = 1.0f;
					vertices[index + 2].coords[1] = 1.0f;
					vertices[index + 2].properties[0] = density;
					vertices[index + 2].properties[1] = 0.0f;

					indices[index + 0] = index + 0;
					indices[index + 1] = index + 1;
					indices[index + 2] = index + 2;

					index += 3;
					counter++;
				}
			}
		}
	}

	

	// create buffers
	m_vertexBuffer = pDevice->CreateVertexBuffer(vertices, counter * 3);
	m_indexBuffer = pDevice->CreateIndexBuffer(indices, counter * 3);


	printf("\nCreated cloud vertives = %u", counter * 3);


	// clear
	delete[] indices;
	delete[] vertices;
}


ParabellumEngine::Components::Cloud::~Cloud()
{
	delete[] m_cloudSpheres;
}

//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------

void
ParabellumEngine::Components::Cloud::DrawMerged(_IN_ GraphicsDevice* pDevice, _IN_ Camera* pCamera)
{
	
	// --------------------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------------------
	pDevice->SetRenderTargets(m_RTClouds, m_RTClouds, true);
	pDevice->SetShader(m_SDrawCloudSphere);

	Matrix4x4 world, worldViewProjection;
	Vector3D scale, position;
	EFLOAT32 scaleFactor = 1.0f, ray;
	Vector4D cameraPositionFar, materialProperties;

	Vector4D cloudProperties;
	cloudProperties.Set(0.0f, 0.0f, 0.0f, 0.0f);

	pCamera->GetPosition(cameraPositionFar.X, cameraPositionFar.Y, cameraPositionFar.Z);
	cameraPositionFar.W = pCamera->GetZFar();

	//
	// draw spheres
	pDevice->TurnAlphaBlending(false);
	pDevice->TurnDepthBuffer(true);

	pDevice->SetBuffers(m_MSphereModel->GetVertexBuffer(0), m_MSphereModel->GetIndexBuffer(0));

	for (EUINT32 i = 0; i < m_cloudSpheresCount; ++i)
	{
		position = *m_cloudSpheres[i].GetCenter();
		ray = m_cloudSpheres[i].GetRayLength();
		scale.Set(ray, ray, ray);
	
		world.MakeScaleTranslation(&position, &scale);
		worldViewProjection = (*pCamera->GetViewProjection()) * world;
	
		m_SDrawCloudSphere->GetParametr(0)->Set(&worldViewProjection);
		m_SDrawCloudSphere->GetParametr(1)->Set(&cameraPositionFar);
		m_SDrawCloudSphere->GetResource(0)->Set(m_T2DCloudSphere);
	
		pDevice->Render(PF_GI_RENDERING_UPDATEVARIABLES | PF_GI_RENDERING_UPDATERESOURCES);
	}




	//
	// draw bilboards
	pDevice->TurnAlphaBlending(false);
	pDevice->TurnDepthBuffer(true);

	pDevice->SetShader(m_SDrawCloud);
	pDevice->SetBuffers(m_vertexBuffer, m_indexBuffer);


	position.Set(1.0f, 1.0f, 1.0f);
	scale.Set(1.0f, 1.0f, 1.0f);
	world.MakeScaleTranslation(&position, &scale);

	worldViewProjection = (*pCamera->GetViewProjection()) * world;

	m_SDrawCloud->GetParametr(0)->Set(&worldViewProjection);
	m_SDrawCloud->GetParametr(1)->Set(&cameraPositionFar);
	m_SDrawCloud->GetResource(0)->Set(m_T2DCloud);

	pDevice->Render(PF_GI_RENDERING_UPDATEVARIABLES | PF_GI_RENDERING_UPDATERESOURCES);

	pDevice->TurnAlphaBlending(false);
	pDevice->TurnDepthBuffer(true);



	// --------------------------------------------------------------------------------------
	//
	// --------------------------------------------------------------------------------------
	pDevice->SetRenderTargets(m_RTCloudsBluredVertical, m_RTCloudsBluredVertical, true);
	pDevice->SetShader(m_SBlurVertical);
	pDevice->SetBuffers(m_renderQuad.GetVertexBuffer(), m_renderQuad.GetIndexBuffer());
	Vector4D blurSettings(1920.0f, 8.0f, 1.0f, 0.0f);
	m_SBlurVertical->GetParametr(0)->Set(&cameraPositionFar);
	m_SBlurVertical->GetParametr(1)->Set(&blurSettings);
	m_SBlurVertical->GetResource(0)->Set(m_RTClouds);
	pDevice->Render(PF_GI_RENDERING_UPDATEVARIABLES | PF_GI_RENDERING_UPDATERESOURCES);

	pDevice->SetRenderTargets(m_RTCloudsBluredHorizontal, m_RTCloudsBluredHorizontal, true);
	pDevice->SetShader(m_SBlurVertical);
	pDevice->SetBuffers(m_renderQuad.GetVertexBuffer(), m_renderQuad.GetIndexBuffer());
	blurSettings.Set(1080.0f, 8.0f, 0.0f, 1.0f);
	m_SBlurVertical->GetParametr(0)->Set(&cameraPositionFar);
	m_SBlurVertical->GetParametr(1)->Set(&blurSettings);
	m_SBlurVertical->GetResource(0)->Set(m_RTCloudsBluredVertical);
	pDevice->Render(PF_GI_RENDERING_UPDATEVARIABLES | PF_GI_RENDERING_UPDATERESOURCES);


	for (EUINT32 i = 0; i < 8; ++i)
	{
		pDevice->SetRenderTargets(m_RTCloudsBluredVertical, m_RTCloudsBluredVertical, true);
		pDevice->SetShader(m_SBlurVertical);
		pDevice->SetBuffers(m_renderQuad.GetVertexBuffer(), m_renderQuad.GetIndexBuffer());
		blurSettings.Set(1920.0f, 8.0f, 1.0f, 0.0f);
		m_SBlurVertical->GetParametr(0)->Set(&cameraPositionFar);
		m_SBlurVertical->GetParametr(1)->Set(&blurSettings);
		m_SBlurVertical->GetResource(0)->Set(m_RTCloudsBluredHorizontal);
		pDevice->Render(PF_GI_RENDERING_UPDATEVARIABLES | PF_GI_RENDERING_UPDATERESOURCES);

		pDevice->SetRenderTargets(m_RTCloudsBluredHorizontal, m_RTCloudsBluredHorizontal, true);
		pDevice->SetShader(m_SBlurVertical);
		pDevice->SetBuffers(m_renderQuad.GetVertexBuffer(), m_renderQuad.GetIndexBuffer());
		blurSettings.Set(1080.0f, 8.0f, 0.0f, 1.0f);
		m_SBlurVertical->GetParametr(0)->Set(&cameraPositionFar);
		m_SBlurVertical->GetParametr(1)->Set(&blurSettings);
		m_SBlurVertical->GetResource(0)->Set(m_RTCloudsBluredVertical);
		pDevice->Render(PF_GI_RENDERING_UPDATEVARIABLES | PF_GI_RENDERING_UPDATERESOURCES);
	}
}
