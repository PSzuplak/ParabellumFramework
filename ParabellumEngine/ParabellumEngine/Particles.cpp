#include "Particles.h"



//	------------------------------------------------------------
//		Statics
//	------------------------------------------------------------



//	------------------------------------------------------------
//		Constructors and destructors
//	------------------------------------------------------------
ParabellumEngine::Components::Particles::Particles()
{
	m_timeElapsed = 0;
}


ParabellumEngine::Components::Particles::~Particles()
{
}

//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------

ParabellumFramework::EUINT32
ParabellumEngine::Components::Particles::Initialize(
	_IN_ PFNS::Graphics::GraphicsDevice* pDevice, _IN_ PFNS::Resources::ResourceManager* pResources,
	EUINT32 particlesCount
	)
{
	m_particlesCount = particlesCount;

	/*
	EUINT32 verticesCount = 8 * particlesCount;
	ParticleVertexFormat* vertices = new ParticleVertexFormat[verticesCount];

	
	EUINT32 indicesCount = 36 * particlesCount;
	EUINT32* indices = new EUINT32[indicesCount];



	EUINT32 indicesLookup[] = //36 indices
	{
		//fl
		0,3,1,

		// fr
		1,3,2,

		// bl
		4,7,6,

		// br
		5,7,6,

		// tl
		4,0,5,

		// tr,
		5,0,1,

		// bl
		3,7,6,

		//br
		2,3,6,

		// rl
		1,2,6,

		// rr
		5,1,6,

		// ll
		0,4,7,

		// lr
		0,7,3

	};


	//
	// create vertices
	for (EUINT32 i = 0; i < verticesCount; i += 8)
	{
		//ftl
		vertices[i + 0].Position[0] = -1.0f;
		vertices[i + 0].Position[1] = +1.0f;
		vertices[i + 0].Position[2] = -1.0f;
		vertices[i + 0].UV[0] = 0.0f;
		vertices[i + 0].UV[1] = 0.0f;
		vertices[i + 0].Normal[0] = 0.0f;
		vertices[i + 0].Normal[1] = 0.0f;
		vertices[i + 0].Normal[2] = -1.0f;


		// ftr
		vertices[i + 1].Position[0] = +1.0f;
		vertices[i + 1].Position[1] = +1.0f;
		vertices[i + 1].Position[2] = -1.0f;
		vertices[i + 1].UV[0] = 1.0f;
		vertices[i + 1].UV[1] = 0.0f;
		vertices[i + 1].Normal[0] = 0.0f;
		vertices[i + 1].Normal[1] = 0.0f;
		vertices[i + 1].Normal[2] = -1.0f;


		// fbr
		vertices[i + 2].Position[0] = +1.0f;
		vertices[i + 2].Position[1] = -1.0f;
		vertices[i + 2].Position[2] = -1.0f;
		vertices[i + 2].UV[0] = 1.0f;
		vertices[i + 2].UV[1] = 1.0f;
		vertices[i + 2].Normal[0] = 0.0f;
		vertices[i + 2].Normal[1] = 0.0f;
		vertices[i + 2].Normal[2] = -1.0f;


		// fbl
		vertices[i + 3].Position[0] = -1.0f;
		vertices[i + 3].Position[1] = -1.0f;
		vertices[i + 3].Position[2] = -1.0f;
		vertices[i + 3].UV[0] = 0.0f;
		vertices[i + 3].UV[1] = 1.0f;
		vertices[i + 3].Normal[0] = 0.0f;
		vertices[i + 3].Normal[1] = 0.0f;
		vertices[i + 3].Normal[2] = -1.0f;



		//////////////////////////////////////////////////////////

		//btl
		vertices[i + 4].Position[0] = -1.0f;
		vertices[i + 4].Position[1] = +1.0f;
		vertices[i + 4].Position[2] = +1.0f;
		vertices[i + 4].UV[0] = 1.0f;
		vertices[i + 4].UV[1] = 1.0f;
		vertices[i + 4].Normal[0] = 0.0f;
		vertices[i + 4].Normal[1] = 0.0f;
		vertices[i + 4].Normal[2] = -1.0f;


		// btr
		vertices[i + 5].Position[0] = +1.0f;
		vertices[i + 5].Position[1] = +1.0f;
		vertices[i + 5].Position[2] = +1.0f;
		vertices[i + 5].UV[0] = 0.0f;
		vertices[i + 5].UV[1] = 1.0f;
		vertices[i + 5].Normal[0] = 0.0f;
		vertices[i + 5].Normal[1] = 0.0f;
		vertices[i + 5].Normal[2] = -1.0f;


		// bbr
		vertices[i + 6].Position[0] = +1.0f;
		vertices[i + 6].Position[1] = -1.0f;
		vertices[i + 6].Position[2] = +1.0f;
		vertices[i + 6].UV[0] = 0.0f;
		vertices[i + 6].UV[1] = 0.0f;
		vertices[i + 6].Normal[0] = 0.0f;
		vertices[i + 6].Normal[1] = 0.0f;
		vertices[i + 6].Normal[2] = -1.0f;

		// bbl
		vertices[i + 7].Position[0] = -1.0f;
		vertices[i + 7].Position[1] = -1.0f;
		vertices[i + 7].Position[2] = +1.0f;
		vertices[i + 7].UV[0] = 1.0f;
		vertices[i + 7].UV[1] = 0.0f;
		vertices[i + 7].Normal[0] = 0.0f;
		vertices[i + 7].Normal[1] = 0.0f;
		vertices[i + 7].Normal[2] = -1.0f;


		// move direction
		Vector3D moveDir, properties;


		moveDir.X = (EFLOAT32)((Math::RandI() % 2000) - 1000) / 1000.0f;
		moveDir.Y = (EFLOAT32)((Math::RandI() % 2000) - 1000) / 1000.0f;
		moveDir.Z = (EFLOAT32)((Math::RandI() % 2000) - 1000) / 1000.0f;

		moveDir.Normalize();
		moveDir.Y = 1.0f;
		moveDir.Normalize();

		moveDir /= 1.0f;

		properties.X = (Math::RandI() % 2000) + 1000;
		properties.Y = (Math::RandI() % 2000) + 1000;
		properties.Z = (Math::RandI() % 2000) + 1000;


		for (EUINT32 n = 0; n < 8; ++n)
		{
			vertices[i + n].DeltaDirection[0] = moveDir.X;
			vertices[i + n].DeltaDirection[1] = moveDir.Y;
			vertices[i + n].DeltaDirection[2] = moveDir.Z;
			vertices[i + n].DeltaDirection[3] = (Math::RandI() % 2000) + 1000;


			vertices[i + n].Properties[0] = properties.X;
			vertices[i + n].Properties[1] = properties.Y;
			vertices[i + n].Properties[2] = properties.Z;


			vertices[i + n].Sprite[0] = 4;
			vertices[i + n].Sprite[1] = 4;
			vertices[i + n].Sprite[2] = 4;

			//vertices[i + n].Position[1] += 3 * i/8;

			//printf(" = (%f, %f, %f)", vertices[i + n].Position[0], vertices[i + n].Position[1], vertices[i + n].Position[2]);
		}
	}


	//
	// create indices
	EUINT32 veNo = 0;

	for (EUINT32 i = 0; i < verticesCount; i += 8)
	{
		for (EUINT32 n = 0; n < 36; ++n)
		{
			indices[veNo * 36 + n] = indicesLookup[n] + veNo*8;

			//printf("\n %i = %i", veNo * 36 + n, indicesLookup[n] + veNo * 8);
		}

		veNo++;
	}
	*/

	

	EFLOAT32 verticesPositions[] =
	{
		0.500000,0.866025,0.000000        ,
		0.866026,0.500000,0.000000,
		1.000000,0.000000,0.000000,
		0.866025,-0.500000,0.000000,
		0.500000,-0.866025,0.000000,
		0.250000,0.866025,0.433013,
		0.433013,0.500000,0.750000,
		0.500000,0.000000,0.866025,
		0.433013,-0.500000,0.750000,
		0.250000,-0.866025,0.433013,
		0.000000,1.000000,-0.000000,
		-0.250000,0.866025,0.433013,
		-0.433013,0.500000,0.750000,
		-0.500000,0.000000,0.866025,
		-0.433012,-0.500000,0.750000,
		-0.250000,-0.866025,0.433013,
		-0.500000,0.866025,-0.000000,
		-0.866025,0.500000,-0.000000,
		-1.000000,0.000000,-0.000000,
		-0.866025,-0.500000,-0.000000,
		-0.500000,-0.866025,-0.000000,
		-0.250000,0.866025,-0.433013,
		-0.433012,0.500000,-0.750000,
		-0.500000,0.000000,-0.866025,
		-0.433012,-0.500000,-0.750000,
		-0.250000,-0.866025,-0.433013,
		0.250000,0.866025,-0.433012,
		0.433013,0.500000,-0.750000,
		0.500000,0.000000,-0.866025,
		0.433013,-0.500000,-0.750000,
		0.250000,-0.866025,-0.433013,
		0.000000,-1.000000,0.000000,
	};

	EFLOAT32 verticesUV[] =
	{
		0.0670, 0.2500,
		0.3792, 0.0815,
		0.2500, 0.0670,
		0.0670, 0.7500,
		0.2662, 0.5000,
		0.0000, 0.5000,
		0.2500, 0.9330,
		0.5000, 1.0000,
		0.3792, 0.9185,
		0.5000, 0.0000,
		0.2958, 0.2642,
		0.2958, 0.7358,
		0.6208, 0.9185,
		0.6208, 0.0815,
		0.7042, 0.2642,
		0.7042, 0.7358,
		0.7338, 0.5000,
		0.7500, 0.0670,
		0.9330, 0.2500,
		0.7500, 0.9330,
		0.9330, 0.7500,
		1.0000, 0.5000,
		0.6295, 0.0514,
		0.7686, 0.5000,
		0.7303, 0.2340,
		0.6295, 0.9486,
		0.7303, 0.7660,
		0.3705, 0.0514,
		0.2314, 0.5000,
		0.2697, 0.2340,
		0.2697, 0.7660,
		0.3705, 0.9486,
	};


	EFLOAT32 verticesNormals[] =
	{
		0.6547,-0.6547,0.3780,
		0.8436,0.2260,0.4871,
		0.2560,0.9553,0.1478,
		0.2560,-0.9553,0.1478,
		0.8436,-0.2260,0.4871,
		0.6547,0.6547,0.3780,
		-0.0000,0.9553,0.2956,
		-0.0000,-0.9553,0.2956,
		0.0000,-0.2260,0.9741,
		-0.0000,0.6547,0.7559,
		-0.0000,-0.6547,0.7559,
		0.0000,0.2260,0.9741,
		-0.2560,-0.9553,0.1478,
		-0.8436,-0.2260,0.4871,
		-0.6547,0.6547,0.3780,
		-0.6547,-0.6547,0.3780,
		-0.8436,0.2260,0.4871,
		-0.2560,0.9553,0.1478,
		-0.2560,-0.9553,-0.1478,
		-0.8436,-0.2260,-0.4871,
		-0.6547,0.6547,-0.3780,
		-0.6547,-0.6547,-0.3780,
		-0.8436,0.2260,-0.4871,
		-0.2560,0.9553,-0.1478,
		0.0000,-0.9553,-0.2956,
		0.0000,-0.2260,-0.9741,
		0.0000,0.6547,-0.7559,
		0.0000,-0.6547,-0.7559,
		0.0000,0.2260,-0.9741,
		0.0000,0.9553,-0.2956,
		0.8436,-0.2260,-0.4871,
		0.6547,0.6547,-0.3780,
		0.6547,-0.6547,-0.3780,
		0.8436,0.2260,-0.4871,
		0.2560,0.9553,-0.1478,
		0.2560 - 0.9553 - 0.1478,
	};



	EUINT32 indicesLP[] =
	{
		4,1,1,10,2,1,5,3,1,
		2,4,2,8,5,2,3,6,2,
		1,7,3,11,8,3,6,9,3,
		32,10,4,5,3,4,10,2,4,
		4,1,5,8,5,5,9,11,5,
		1,7,6,7,12,6,2,4,6,
		6,9,7,11,8,7,12,13,7,
		32,10,8,10,2,8,16,14,8,
		8,5,9,15,15,9,9,11,9,
		6,9,10,13,16,10,7,12,10,
		9,11,11,16,14,11,10,2,11,
		8,5,12,13,16,12,14,17,12,
		32,10,13,16,14,13,21,18,13,
		14,17,14,20,19,14,15,15,14,
		13,16,15,17,20,15,18,21,15,
		15,15,16,21,18,16,16,14,16,
		13,16,17,19,22,17,14,17,17,
		12,13,18,11,8,18,17,20,18,
		32,10,19,21,18,19,26,23,19,
		20,19,20,24,24,20,25,25,20,
		18,21,21,22,26,21,23,27,21,
		20,19,22,26,23,22,21,18,22,
		18,21,23,24,24,23,19,22,23,
		17,20,24,11,8,24,22,26,24,
		32,10,25,26,23,25,31,28,25,
		25,25,26,29,29,26,30,30,26,
		22,26,27,28,31,27,23,27,27,
		26,23,28,30,30,28,31,28,28,
		23,27,29,29,29,29,24,24,29,
		22,26,30,11,8,30,27,32,30,
		29,29,31,4,1,31,30,30,31,
		28,31,32,1,7,32,2,4,32,
		31,28,33,4,1,33,5,3,33,
		28,31,34,3,6,34,29,29,34,
		27,32,35,11,8,35,1,7,35,
		32,10,36,31,28,36,5,3,36,
		4,1,1,9,11,1,10,2,1,
		2,4,2,7,12,2,8,5,2,
		4,1,5,3,6,5,8,5,5,
		1,7,6,6,9,6,7,12,6,
		8,5,9,14,17,9,15,15,9,
		6,9,10,12,13,10,13,16,10,
		9,11,11,15,15,11,16,14,11,
		8,5,12,7,12,12,13,16,12,
		14,17,14,19,22,14,20,19,14,
		13,16,15,12,13,15,17,20,15,
		15,15,16,20,19,16,21,18,16,
		13,16,17,18,21,17,19,22,17,
		20,19,20,19,22,20,24,24,20,
		18,21,21,17,20,21,22,26,21,
		20,19,22,25,25,22,26,23,22,
		18,21,23,23,27,23,24,24,23,
		25,25,26,24,24,26,29,29,26,
		22,26,27,27,32,27,28,31,27,
		26,23,28,25,25,28,30,30,28,
		23,27,29,28,31,29,29,29,29,
		29,29,31,3,6,31,4,1,31,
		28,31,32,27,32,32,1,7,32,
		31,28,33,30,30,33,4,1,33,
		28,31,34,2,4,34,3,6,34,
	};

	EUINT32 trianglesCount = 180;

	

	EUINT32 sphereVerticesCount = 32;


	EUINT32 sphereIndicesCount = 180;
	EUINT32 indicesCount = sphereIndicesCount * particlesCount;
	EUINT32* indices = new EUINT32[indicesCount];

	EUINT32 verticesCountTotal = sphereIndicesCount * particlesCount;
	EUINT32 verticesCountOne = sphereIndicesCount;


	EUINT32* posIndices = new EUINT32[verticesCountOne];
	EUINT32* uvIndices = new EUINT32[verticesCountOne];
	EUINT32* normalIndices = new EUINT32[verticesCountOne];


	ParticleVertexFormat* vertices = new ParticleVertexFormat[verticesCountTotal];


	//
	// copy data
	//
	for (EUINT32 i = 0; i < verticesCountOne; ++i)
	{
		posIndices[i]		= indicesLP[ i*3 + 0 ];
		uvIndices[i]		= indicesLP[ i*3 + 1 ];
		normalIndices[i]	= indicesLP[ i*3 + 2 ];
	}

	//
	// calculate vertices
	//
	for (EUINT32 i = 0; i < particlesCount; ++i)
	{
		// move direction
		Vector3D moveDir, rotProperties, properties;

		moveDir.X = (EFLOAT32)((ParabellumUtils::Math::RandI() % 200) - 100) / 100.0f;
		moveDir.Y = (EFLOAT32)((ParabellumUtils::Math::RandI() % 200) - 100) / 100.0f;
		moveDir.Z = (EFLOAT32)((ParabellumUtils::Math::RandI() % 200) - 100) / 100.0f;
		moveDir.Normalize();
		moveDir.Y = 10.0f;
		moveDir.Normalize();

		moveDir /= 100.0f;

		rotProperties.X = (EFLOAT32)((ParabellumUtils::Math::RandI() % 200) - 100) / 100.0f;
		rotProperties.Y = (EFLOAT32)((ParabellumUtils::Math::RandI() % 200) - 100) / 100.0f;
		rotProperties.Z = (EFLOAT32)((ParabellumUtils::Math::RandI() % 200) - 100) / 100.0f;

		rotProperties /= 400.0f;

		properties.X = (EFLOAT32)(ParabellumUtils::Math::RandI() % 2000) + 1000.0f;
		properties.Y = (EFLOAT32)(ParabellumUtils::Math::RandI() % 2000) + 1000.0f;
		properties.Z = (EFLOAT32)(ParabellumUtils::Math::RandI() % 2000) + 1000.0f;

		EFLOAT32 sizeFactor = (EFLOAT32)(ParabellumUtils::Math::RandI() % 100) / 100.0f + 0.3f;
		//sizeFactor *= 0.25f;

		for (EUINT32 n = 0; n < verticesCountOne; ++n)
		{
			vertices[ (i*verticesCountOne) + n ].Position[0] = verticesPositions[ (posIndices[n] - 1) * 3 + 0 ] * sizeFactor;
			vertices[ (i*verticesCountOne) + n ].Position[1] = verticesPositions[ (posIndices[n] - 1) * 3 + 1 ] * sizeFactor;
			vertices[ (i*verticesCountOne) + n ].Position[2] = verticesPositions[ (posIndices[n] - 1) * 3 + 2 ] * sizeFactor;

			vertices[ (i*verticesCountOne) + n ].UV[0] = verticesUV[ (uvIndices[n] - 1) * 2 + 0];
			vertices[ (i*verticesCountOne) + n ].UV[1] = verticesUV[ (uvIndices[n] - 1) * 2 + 1];

			vertices[ (i*verticesCountOne) + n ].Normal[0] = verticesNormals[ (normalIndices[n] - 1) * 3 + 0];
			vertices[ (i*verticesCountOne) + n ].Normal[1] = verticesNormals[ (normalIndices[n] - 1) * 3 + 1];
			vertices[ (i*verticesCountOne) + n ].Normal[2] = verticesNormals[ (normalIndices[n] - 1) * 3 + 2];

			vertices[(i*verticesCountOne) + n].DeltaDirection[0] = moveDir.X;
			vertices[(i*verticesCountOne) + n].DeltaDirection[1] = moveDir.Y;
			vertices[(i*verticesCountOne) + n].DeltaDirection[2] = moveDir.Z;

			vertices[(i*verticesCountOne) + n].DeltaRotation[0] = rotProperties.X;
			vertices[(i*verticesCountOne) + n].DeltaRotation[1] = rotProperties.Y;
			vertices[(i*verticesCountOne) + n].DeltaRotation[2] = rotProperties.Z;

			vertices[(i*verticesCountOne) + n].Properties[0] = properties.X;
			vertices[(i*verticesCountOne) + n].Properties[1] = properties.Y;
			vertices[(i*verticesCountOne) + n].Properties[2] = properties.Z;

			vertices[(i*verticesCountOne) + n].Sprite[0] = 4;
			vertices[(i*verticesCountOne) + n].Sprite[1] = 4;
			vertices[(i*verticesCountOne) + n].Sprite[2] = 4;

			//printf("%i (%i:%i:%i) = %f / %f / %f \n", (i*verticesCountOne) + n, posIndices[n] * 3 + 0, posIndices[n] * 3 + 1, posIndices[n] * 3 + 2, vertices[(i*verticesCountOne) + n].Position[0], vertices[(i*verticesCountOne) + n].Position[1], vertices[(i*verticesCountOne) + n].Position[2]);
		}
	}

	//
	// calculate indices
	//
	for (EUINT32 i = 0; i < particlesCount; ++i)
	{
		for (EUINT32 n = 0; n < verticesCountOne; ++n)
		{
			indices[i*sphereIndicesCount + n] = i*sphereIndicesCount + n;
		}
	}


	m_vertexBuffer = pDevice->CreateVertexBuffer(vertices, sizeof(ParticleVertexFormat), verticesCountTotal);
	m_indexBuffer = pDevice->CreateIndexBuffer(indices, indicesCount);

	//
	// clear
	delete[] vertices;
	delete[] indices;

	delete[] posIndices;
	delete[] uvIndices;
	delete[] normalIndices;

	return SystemCodes::SUCCESS;
}



//	------------------------------------------------------------
//		Gets Sets
//	------------------------------------------------------------
