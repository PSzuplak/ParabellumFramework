#include "stdafx.h"
#include "PrimitivesFactory.h"




ParabellumEngine::Core::Graphics::PrimitivesFactory& 
ParabellumEngine::Core::Graphics::PrimitivesFactory::Instance(_IN_ GraphicsDevice* pDevice)
{
	static PrimitivesFactory instance(pDevice);
	return instance;
}




ParabellumEngine::Core::Graphics::PrimitivesFactory::PrimitivesFactory(_IN_ GraphicsDevice* pDevice)
{
	// ---------------------------------------------------------------------------------------------------------------------
	// box
	//
	

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

	VertexTypeRenderQuad vertices[24];

	EUINT32 indices[] =
	{
		// front face 0, 1, 2, 3
		0, 1, 2,
		0, 2, 3,

		// back face - 4, 5, 6, 7
		6, 5, 4,
		6, 4, 7,
		
		// top face - 8, 9, 10, 11
		8, 9, 10,
		8, 10, 11,
		
		// bottom face 12, 13, 14, 15
		14, 13, 12,
		14, 12, 15,
		
		// left face 16, 17, 18, 19
		16, 17, 18,
		16, 18, 19,
		
		// right face 20, 21, 22, 23
		22, 21, 20,
		22, 20, 23
	};

	// tl
	vertices[0].position[0] = -1.0f; vertices[0].position[1] = +1.0f; vertices[0].position[2] = -1.0f;
	vertices[0].coords[0] = 0.0f; vertices[0].coords[1] = 0.0f;

	// tr
	vertices[1].position[0] = +1.0f; vertices[1].position[1] = +1.0f; vertices[1].position[2] = -1.0f;
	vertices[1].coords[0] = 1.0f; vertices[1].coords[1] = 0.0f;

	// br
	vertices[2].position[0] = +1.0f; vertices[2].position[1] = -1.0f; vertices[2].position[2] = -1.0f;
	vertices[2].coords[0] = 1.0f; vertices[2].coords[1] = 1.0f;

	// bl
	vertices[3].position[0] = -1.0f; vertices[3].position[1] = -1.0f; vertices[3].position[2] = -1.0f;
	vertices[3].coords[0] = 0.0f; vertices[3].coords[1] = 1.0f;



	//
	// backface
	//

	// tl
	vertices[4].position[0] = -1.0f; vertices[4].position[1] = +1.0f; vertices[4].position[2] = +1.0f;
	vertices[4].coords[0] = 0.0f; vertices[4].coords[1] = 0.0f;

	// tr
	vertices[5].position[0] = +1.0f; vertices[5].position[1] = +1.0f; vertices[5].position[2] = +1.0f;
	vertices[5].coords[0] = 1.0f; vertices[5].coords[1] = 0.0f;

	// br
	vertices[6].position[0] = +1.0f; vertices[6].position[1] = -1.0f; vertices[6].position[2] = +1.0f;
	vertices[6].coords[0] = 1.0f; vertices[6].coords[1] = 1.0f;

	// bl
	vertices[7].position[0] = -1.0f; vertices[7].position[1] = -1.0f; vertices[7].position[2] = +1.0f;
	vertices[7].coords[0] = 0.0f; vertices[7].coords[1] = 1.0f;



	//
	// topface
	//

	// tl
	vertices[8].position[0] = -1.0f; vertices[8].position[1] = +1.0f; vertices[8].position[2] = +1.0f;
	vertices[8].coords[0] = 0.0f; vertices[8].coords[1] = 0.0f;

	// tr
	vertices[9].position[0] = +1.0f; vertices[9].position[1] = +1.0f; vertices[9].position[2] = +1.0f;
	vertices[9].coords[0] = 1.0f; vertices[9].coords[1] = 0.0f;

	// br
	vertices[10].position[0] = +1.0f; vertices[10].position[1] = +1.0f; vertices[10].position[2] = -1.0f;
	vertices[10].coords[0] = 1.0f; vertices[10].coords[1] = 1.0f;

	// bl
	vertices[11].position[0] = -1.0f; vertices[11].position[1] = +1.0f; vertices[11].position[2] = -1.0f;
	vertices[11].coords[0] = 0.0f; vertices[11].coords[1] = 1.0f;


	//
	// bottomface
	//

	// tl
	vertices[12].position[0] = -1.0f; vertices[12].position[1] = -1.0f; vertices[12].position[2] = +1.0f;
	vertices[12].coords[0] = 0.0f; vertices[12].coords[1] = 0.0f;

	// tr
	vertices[13].position[0] = +1.0f; vertices[13].position[1] = -1.0f; vertices[13].position[2] = +1.0f;
	vertices[13].coords[0] = 1.0f; vertices[13].coords[1] = 0.0f;

	// br
	vertices[14].position[0] = +1.0f; vertices[14].position[1] = -1.0f; vertices[14].position[2] = -1.0f;
	vertices[14].coords[0] = 1.0f; vertices[14].coords[1] = 1.0f;

	// bl
	vertices[15].position[0] = -1.0f; vertices[15].position[1] = -1.0f; vertices[15].position[2] = -1.0f;
	vertices[15].coords[0] = 0.0f; vertices[15].coords[1] = 1.0f;



	//
	// leftface
	//

	// tl
	vertices[16].position[0] = -1.0f; vertices[16].position[1] = +1.0f; vertices[16].position[2] = +1.0f;
	vertices[16].coords[0] = 0.0f; vertices[16].coords[1] = 0.0f;

	// tr
	vertices[17].position[0] = -1.0f; vertices[17].position[1] = +1.0f; vertices[17].position[2] = -1.0f;
	vertices[17].coords[0] = 1.0f; vertices[17].coords[1] = 0.0f;

	// br
	vertices[18].position[0] = -1.0f; vertices[18].position[1] = -1.0f; vertices[18].position[2] = -1.0f;
	vertices[18].coords[0] = 1.0f; vertices[18].coords[1] = 1.0f;

	// bl
	vertices[19].position[0] = -1.0f; vertices[19].position[1] = -1.0f; vertices[19].position[2] = +1.0f;
	vertices[19].coords[0] = 0.0f; vertices[19].coords[1] = 1.0f;


	//
	// rightface
	//

	// tl
	vertices[20].position[0] = +1.0f; vertices[20].position[1] = +1.0f; vertices[20].position[2] = +1.0f;
	vertices[20].coords[0] = 0.0f; vertices[20].coords[1] = 0.0f;

	// tr
	vertices[21].position[0] = +1.0f; vertices[21].position[1] = +1.0f; vertices[21].position[2] = -1.0f;
	vertices[21].coords[0] = 1.0f; vertices[21].coords[1] = 0.0f;

	// br
	vertices[22].position[0] = +1.0f; vertices[22].position[1] = -1.0f; vertices[22].position[2] = -1.0f;
	vertices[22].coords[0] = 1.0f; vertices[22].coords[1] = 1.0f;

	// bl
	vertices[23].position[0] = +1.0f; vertices[23].position[1] = -1.0f; vertices[23].position[2] = +1.0f;
	vertices[23].coords[0] = 0.0f; vertices[23].coords[1] = 1.0f;



	m_vbBoxUnit = pDevice->CreateVertexBuffer(vertices, sizeof(VertexTypeRenderQuad), 24);
	m_ibBoxUnit = pDevice->CreateIndexBuffer(indices, 36);





	// ---------------------------------------------------------------------------------------------------------------------
	// Cylinder
	//
	{
		EUINT32 cylinderVerticesCount = 36;

		VertexTypeRenderQuad cylinerVertices[2][36];
		EUINT32 cylinderIndices[36 * 2 * 3];


		for (EUINT32 ax = 0; ax < cylinderVerticesCount; ++ax)
		{
			cylinerVertices[0][ax].position[0] = ParabellumUtils::Math::FastCos(ParabellumUtils::Math::PI2 / (cylinderVerticesCount - 1) * ax);
			cylinerVertices[0][ax].position[1] = 0.0f;
			cylinerVertices[0][ax].position[2] = ParabellumUtils::Math::FastSin(ParabellumUtils::Math::PI2 / (cylinderVerticesCount - 1) * ax);
			cylinerVertices[0][ax].coords[0] = (EFLOAT32)ax / (EFLOAT32)(cylinderVerticesCount - 1);
			cylinerVertices[0][ax].coords[1] = 1.0f;

			cylinerVertices[1][ax].position[0] = ParabellumUtils::Math::FastCos(ParabellumUtils::Math::PI2 / (cylinderVerticesCount - 1) * ax);
			cylinerVertices[1][ax].position[1] = 1.0f;
			cylinerVertices[1][ax].position[2] = ParabellumUtils::Math::FastSin(ParabellumUtils::Math::PI2 / (cylinderVerticesCount - 1) * ax);
			cylinerVertices[1][ax].coords[0] = (EFLOAT32)ax / (EFLOAT32)(cylinderVerticesCount - 1);
			cylinerVertices[1][ax].coords[1] = 0.0f;
		}


		EUINT32 inc = 0;

		for (EUINT32 ax = 0; ax < cylinderVerticesCount; ++ax)
		{
			cylinderIndices[inc + 0] = ax;
			cylinderIndices[inc + 1] = ax + 1;
			cylinderIndices[inc + 2] = ax + 36;

			cylinderIndices[inc + 3] = ax + 1;
			cylinderIndices[inc + 4] = ax + 36 + 1;
			cylinderIndices[inc + 5] = ax + 36;

			inc += 6;
		}

		m_vbCylinder = pDevice->CreateVertexBuffer(cylinerVertices, sizeof(VertexTypeRenderQuad), cylinderVerticesCount * 2);
		m_ibCylinder = pDevice->CreateIndexBuffer(cylinderIndices, cylinderVerticesCount * 2 * 3);
	}
	// ---------------------------------------------------------------------------------------------------------------------
	// Cylinderwith half height
	//
	{
		EUINT32 cylinderVerticesCount = 36;

		VertexTypeRenderQuad cylinerVertices[2][36];
		EUINT32 cylinderIndices[36 * 2 * 3];


		for (EUINT32 ax = 0; ax < cylinderVerticesCount; ++ax)
		{
			cylinerVertices[0][ax].position[0] = ParabellumUtils::Math::FastCos(ParabellumUtils::Math::PI2 / (cylinderVerticesCount - 1) * ax);
			cylinerVertices[0][ax].position[1] = 0.0f;
			cylinerVertices[0][ax].position[2] = ParabellumUtils::Math::FastSin(ParabellumUtils::Math::PI2 / (cylinderVerticesCount - 1) * ax);
			cylinerVertices[0][ax].coords[0] = (EFLOAT32)ax / (EFLOAT32)(cylinderVerticesCount - 1);
			cylinerVertices[0][ax].coords[1] = 1.0f;

			cylinerVertices[1][ax].position[0] = ParabellumUtils::Math::FastCos(ParabellumUtils::Math::PI2 / (cylinderVerticesCount - 1) * ax);
			cylinerVertices[1][ax].position[1] = 0.5f;
			cylinerVertices[1][ax].position[2] = ParabellumUtils::Math::FastSin(ParabellumUtils::Math::PI2 / (cylinderVerticesCount - 1) * ax);
			cylinerVertices[1][ax].coords[0] = (EFLOAT32)ax / (EFLOAT32)(cylinderVerticesCount - 1);
			cylinerVertices[1][ax].coords[1] = 0.0f;
		}


		EUINT32 inc = 0;

		for (EUINT32 ax = 0; ax < cylinderVerticesCount; ++ax)
		{
			cylinderIndices[inc + 0] = ax;
			cylinderIndices[inc + 1] = ax + 1;
			cylinderIndices[inc + 2] = ax + 36;

			cylinderIndices[inc + 3] = ax + 1;
			cylinderIndices[inc + 4] = ax + 36 + 1;
			cylinderIndices[inc + 5] = ax + 36;

			inc += 6;
		}

		m_vbCylinderHO2 = pDevice->CreateVertexBuffer(cylinerVertices, sizeof(VertexTypeRenderQuad), cylinderVerticesCount * 2);
		m_ibCylinderHO2 = pDevice->CreateIndexBuffer(cylinderIndices, cylinderVerticesCount * 2 * 3);
	}
}


ParabellumEngine::Core::Graphics::PrimitivesFactory::~PrimitivesFactory()
{

}





//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------




void 
ParabellumEngine::Core::Graphics::PrimitivesFactory::GenerateSphere(
	_IN_ GraphicsDevice* pDevice, _IN_ VertexBuffer*& pVBOut, _OUT_ IndexBuffer*& pIBOut, EUINT32 countX, EUINT32 countY, EUINT32 countZ)
{
}




//	------------------------------------------------------------
//		Gets and sets
//	------------------------------------------------------------

ParabellumFramework::Graphics::VertexBuffer*
ParabellumEngine::Core::Graphics::PrimitivesFactory::GetBoxVertexBuffer()
{
	return m_vbBoxUnit;
}

ParabellumFramework::Graphics::IndexBuffer*
ParabellumEngine::Core::Graphics::PrimitivesFactory::GetBoxIndexBuffer()
{
	return m_ibBoxUnit;
}



ParabellumFramework::Graphics::VertexBuffer* 
ParabellumEngine::Core::Graphics::PrimitivesFactory::GetCylinderVertexBuffer()
{
	return m_vbCylinder;
}

ParabellumFramework::Graphics::IndexBuffer* 
ParabellumEngine::Core::Graphics::PrimitivesFactory::GetCylinderIndexBuffer()
{
	return m_ibCylinder;
}


ParabellumFramework::Graphics::VertexBuffer*
ParabellumEngine::Core::Graphics::PrimitivesFactory::GetCylinderHO2VertexBuffer()
{
	return m_vbCylinderHO2;
}

ParabellumFramework::Graphics::IndexBuffer*
ParabellumEngine::Core::Graphics::PrimitivesFactory::GetCylinderHO2IndexBuffer()
{
	return m_ibCylinderHO2;
}