#include "stdafx.h"
#include "ModelConverter.h"



//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumFramework::ModelConverter::ModelConverter()
{

}


ParabellumFramework::ModelConverter::~ModelConverter()
{

}



//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------

void ParabellumFramework::ModelConverter::ConvertOBJ2PFM(const char* in, const char* out)
{
	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PFGI", "MOCV", "ConvertOBJ2PFM(" << in << "," << out << ") : start " << in);
	#endif


	std::ifstream fin;
	char input, input2;

	int groupNo = -1;

	fin.open(in, std::ios::in);

	if (fin.fail())
	{
		#ifdef DEBUG_LOGLEVEL_1
		PFDLOG("PFGI", "MOCV", "ConvertOBJ2PFM(" << in << "," << out << ") : can not open the file " << in);
		#endif

		return;
	}

	///

	while (!fin.eof())
	{
		fin.get(input);
		
		//
		if (input == 'o')
		{
			fin.get(input);
			if (input == ' ')
			{
				m_objects.push_back(Common3DObject());
			}
		}

		//
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ')
			{
				Vector3D vec;

				fin >> vec.X >> vec.Y >> vec.Z;
				vec.Z *= -1.0f;

				m_objects[m_objects.size() - 1].vertices.push_back(vec);
			}

			if (input == 't')
			{
				Vector3D vec;

				fin >> vec.X >> vec.Y;
				vec.Y = 1.0f - vec.Y;

				m_objects[m_objects.size() - 1].coords.push_back(vec);
			}

			if (input == 'n')
			{
				Vector3D vec;

				fin >> vec.X >> vec.Y >> vec.Z;
				vec.Z *= -1.0f;

				m_objects[m_objects.size() - 1].normals.push_back(vec);
			}
		}

		//
		// read groups
		if (input == 'g')
		{
			fin.get(input);

			if (input == ' ')
			{
				OBJGroup group;
				m_objects[m_objects.size() - 1].groups.push_back(group);
				groupNo++;
			}

		}


		//
		// read materials

		if (input == 'm')
		{
			CommonMaterialType meterial;

			fin.get(input);


			// read textures
			if (input == 't')
			{
				fin.get(input);

				if (input == 'd')
				{
					fin >> meterial.pathTextureDiffuse;
					printf("\n d = %s", meterial.pathTextureDiffuse);
				}

				if (input == 'n')
				{
					fin >> meterial.pathTextureNormal;
					printf("\n n = %s", meterial.pathTextureNormal);
				}

				if (input == 's')
				{
					fin >> meterial.pathTextureLight;
					printf("\n s = %s", meterial.pathTextureLight);
				}

				if (input == 'e')
				{
					fin >> meterial.pathTextureExtenstion;
					printf("\n e = %s", meterial.pathTextureExtenstion);
				}

			}
			

			// read properties
			if (input == 'p')
			{

				for (EUINT32 i = 0; i < 16; ++i)
				{
					fin >> meterial.properties01[i];
				}

				for (EUINT32 i = 0; i < 16; ++i)
				{
					fin >> meterial.properties02[i];
				}

			}


			//
			// save to group

			m_objects[m_objects.size() - 1].groups[groupNo].meterial = meterial;


		}



		//
		// read faces
		if (input == 'f')
		{
			fin.get(input);

			if (input == ' ')
			{
				FaceType face;

				fin >> face.vIndex3 >> input2 >> face.tIndex3 >> input2 >> face.nIndex3
					>> face.vIndex2 >> input2 >> face.tIndex2 >> input2 >> face.nIndex2
					>> face.vIndex1 >> input2 >> face.tIndex1 >> input2 >> face.nIndex1;

				m_objects[m_objects.size() - 1].faces.push_back(face);

				m_objects[m_objects.size() - 1].groups[groupNo].faces.push_back(face);
			}

		}

		// read rest of the line
		while (input != '\n' && !fin.eof())
		{
			fin.get(input);
		}

	} // end while

	fin.close();

	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PFGI", "MOCV", "ConvertOBJ2PFM(im, out) : OBJ loaded");
	#endif

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////


	FILE* fout;
	fout = fopen(out, "wb");


	// set header
	PFMHeader header;

	header.checksum = (EUINT32)PFMChecksum::Id;
	header.fileFormat = (EUINT32)PFMFormatType::BINARY;
	header.versionMajor = 1;
	header.versionMinor = 1;
	header.dateCreated = 1234;

	// set environment
	PFMEnvironmentSettings environment;

	environment.scale = 1.0f;
	environment.units = (EUINT32)PFMUnitType::CENTIMETERE;
	environment.coordinateSystem = (EUINT32)PFMCooridnateSystem::LEFTHANDED;

	// save header
	fwrite(&header, sizeof(PFMHeader), 1, fout);

	// save environment
	fwrite(&environment, sizeof(PFMEnvironmentSettings), 1, fout);



	//save objects
	EUINT32 objectsCount = 1 + m_objects[0].groups.size(); // vertices + index buffers

	fwrite(&objectsCount, sizeof(EUINT32), 1, fout);

	EUINT32* offsets = new EUINT32[objectsCount];

	EUINT32 verticesCount = m_objects[0].vertices.size();
	CommonVertexType* vertices = new CommonVertexType[verticesCount];		// +1 becauze shit happened


	// copy all data to vertices
	// Pass throught all FACES and set CommonVertexTypeS
	for (EUINT32 i = 0; i < m_objects[0].groups.size(); ++i)
	{
		for (EUINT32 j = 0; j < m_objects[0].groups[i].faces.size(); ++j)
		{
			int vIndex = m_objects[0].groups[i].faces[j].vIndex1 - 1;
			int tIndex = m_objects[0].groups[i].faces[j].tIndex1 - 1;
			int nIndex = m_objects[0].groups[i].faces[j].nIndex1 - 1;

			vertices[vIndex].position.X = m_objects[0].vertices[vIndex].X;
			vertices[vIndex].position.Y = m_objects[0].vertices[vIndex].Y;
			vertices[vIndex].position.Z = m_objects[0].vertices[vIndex].Z;
			vertices[vIndex].normal.X = m_objects[0].normals[nIndex].X;
			vertices[vIndex].normal.Y = m_objects[0].normals[nIndex].Y;
			vertices[vIndex].normal.Z = m_objects[0].normals[nIndex].Z;
			vertices[vIndex].tangent.X = 0.0f;
			vertices[vIndex].tangent.Y = 0.0f;
			vertices[vIndex].tangent.Z = 0.0f;
			vertices[vIndex].binormal.X = 0.0f;
			vertices[vIndex].binormal.Y = 0.0f;
			vertices[vIndex].binormal.Z = 0.0f;
			vertices[vIndex].texture.X = m_objects[0].coords[tIndex].X;
			vertices[vIndex].texture.Y = m_objects[0].coords[tIndex].Y;

			vIndex = m_objects[0].groups[i].faces[j].vIndex2 - 1;
			tIndex = m_objects[0].groups[i].faces[j].tIndex2 - 1;
			nIndex = m_objects[0].groups[i].faces[j].nIndex2 - 1;

			vertices[vIndex].position.X = m_objects[0].vertices[vIndex].X;
			vertices[vIndex].position.Y = m_objects[0].vertices[vIndex].Y;
			vertices[vIndex].position.Z = m_objects[0].vertices[vIndex].Z;
			vertices[vIndex].normal.X = m_objects[0].normals[nIndex].X;
			vertices[vIndex].normal.Y = m_objects[0].normals[nIndex].Y;
			vertices[vIndex].normal.Z = m_objects[0].normals[nIndex].Z;
			vertices[vIndex].tangent.X = 0.0f;
			vertices[vIndex].tangent.Y = 0.0f;
			vertices[vIndex].tangent.Z = 0.0f;
			vertices[vIndex].binormal.X = 0.0f;
			vertices[vIndex].binormal.Y = 0.0f;
			vertices[vIndex].binormal.Z = 0.0f;
			vertices[vIndex].texture.X = m_objects[0].coords[tIndex].X;
			vertices[vIndex].texture.Y = m_objects[0].coords[tIndex].Y;

			vIndex = m_objects[0].groups[i].faces[j].vIndex3 - 1;
			tIndex = m_objects[0].groups[i].faces[j].tIndex3 - 1;
			nIndex = m_objects[0].groups[i].faces[j].nIndex3 - 1;

			vertices[vIndex].position.X = m_objects[0].vertices[vIndex].X;
			vertices[vIndex].position.Y = m_objects[0].vertices[vIndex].Y;
			vertices[vIndex].position.Z = m_objects[0].vertices[vIndex].Z;
			vertices[vIndex].normal.X = m_objects[0].normals[nIndex].X;
			vertices[vIndex].normal.Y = m_objects[0].normals[nIndex].Y;
			vertices[vIndex].normal.Z = m_objects[0].normals[nIndex].Z;
			vertices[vIndex].tangent.X = 0.0f;
			vertices[vIndex].tangent.Y = 0.0f;
			vertices[vIndex].tangent.Z = 0.0f;
			vertices[vIndex].binormal.X = 0.0f;
			vertices[vIndex].binormal.Y = 0.0f;
			vertices[vIndex].binormal.Z = 0.0f;
			vertices[vIndex].texture.X = m_objects[0].coords[tIndex].X;
			vertices[vIndex].texture.Y = m_objects[0].coords[tIndex].Y;
		}
	}
	

	//
	// calculate TBN
	//
	for (EUINT32 i = 0; i < m_objects[0].groups.size(); ++i)
	{
		EUINT32 indicesCount = m_objects[0].groups[i].faces.size() * 3;
		EUINT32* indices = new EUINT32[indicesCount];

		int nx = 0;
		int ni = 0;

		for (int j = 0; j < m_objects[0].groups[i].faces.size(); ++j)
		{
			int vIndex1 = m_objects[0].groups[i].faces[j].vIndex1;
			int vIndex2 = m_objects[0].groups[i].faces[j].vIndex2;
			int vIndex3 = m_objects[0].groups[i].faces[j].vIndex3;

			indices[ni + 0] = vIndex3 - 1; // -1 becase OBJ counting faces from 1, so we need to shift all 
			indices[ni + 1] = vIndex2 - 1;
			indices[ni + 2] = vIndex1 - 1;

			nx += 3;
			ni += 3;
		}

		CalculateTBN(vertices, verticesCount, indices, indicesCount);

		delete[] indices;
	}
	
	
	// normalize vectors
	for (EUINT32 i = 0; i < verticesCount; i++)
	{
		vertices[i].normal.Normalize();
		vertices[i].tangent.Normalize();
	}

	// calculate binormal
	for (EUINT32 i = 0; i < verticesCount; ++i)
	{
		Vector3D::Cross(&vertices[i].binormal, &vertices[i].tangent, &vertices[i].normal);	

		//printf("\n\n---------------------------");
		//printf("\ntangent = %s", vertices[i].tangent.ToString().c_str());
		//printf("\nbinormal = %s", vertices[i].binormal.ToString().c_str());
		//printf("\nnormal = %s", vertices[i].normal.ToString().c_str());
	}


	//
	//
	// Set ALL offsets
	//
	//
	offsets[0] = sizeof(PFMHeader) + sizeof(PFMEnvironmentSettings) + sizeof(EUINT32) + sizeof(EUINT32)*objectsCount;
	offsets[1] = offsets[0] + sizeof(EUINT32) * 3 + sizeof(CommonVertexType)*verticesCount;

	for (EUINT32 i = 2; i < objectsCount; ++i)
	{
		offsets[i] = offsets[i-1] + sizeof(EUINT32) * 3 + sizeof(EUINT32)*m_objects[0].groups[i-2].faces.size() * 3 + sizeof(CommonMaterialType);
	}


	fwrite(offsets, sizeof(EUINT32), objectsCount, fout);

	//
	// save meshes

	// save ComplexType mesh
	EUINT32 type = (EUINT32)PFMTypes::VERTICES;
	fwrite(&type, sizeof(EUINT32), 1, fout);

	// vertex format Size
	EUINT32 vertexFormatSize = sizeof(CommonVertexType);
	fwrite(&vertexFormatSize, sizeof(EUINT32), 1, fout);

	

	// vertices
	fwrite(&verticesCount, sizeof(EUINT32), 1, fout);

	for (EUINT32 i = 0; i < verticesCount; ++i)
	{
		fwrite(&vertices[i], sizeof(CommonVertexType), 1, fout);
	}
		

	// groups
	for (EUINT32 i = 0; i < m_objects[0].groups.size(); ++i)
	{
		EUINT32 indicesCount = m_objects[0].groups[i].faces.size() * 3;
		EUINT32* indices = new EUINT32[indicesCount];
		
		int nx = 0;
		int ni = 0;

		for (int j = 0; j < m_objects[0].groups[i].faces.size(); ++j)
		{
			int vIndex1 = m_objects[0].groups[i].faces[j].vIndex1;
			int vIndex2 = m_objects[0].groups[i].faces[j].vIndex2;
			int vIndex3 = m_objects[0].groups[i].faces[j].vIndex3;

			indices[ni + 0] = vIndex3 - 1; // -1 becase OBJ counting faces from 1, so we need to shift all 
			indices[ni + 1] = vIndex2 - 1;
			indices[ni + 2] = vIndex1 - 1;

			//PFDLOG("PFGI", "MOCV", "indexes  : " << vIndex3 << " , " << vIndex2 << " , " << vIndex1 << " , ");

			nx += 3;
			ni += 3;
		}

		//
		// Save to file
		//
		type = (EUINT32)PFMTypes::GROUP;
		fwrite(&type, sizeof(EUINT32), 1, fout);

		EUINT32 verticesNo = 0;
		fwrite(&verticesNo, sizeof(EUINT32), 1, fout);

		fwrite(&indicesCount, sizeof(EUINT32), 1, fout);

		for (EUINT32 i = 0; i < indicesCount; ++i)
			fwrite(&indices[i], sizeof(EUINT32), 1, fout);


		//
		// Save material
		fwrite(&m_objects[0].groups[i].meterial, sizeof(CommonMaterialType), 1, fout);


		delete[] indices;
	}

	//
	// save objects
	//
	PFM_Scene scene;
	scene.objectsCount = m_objects[0].groups.size();
	fwrite(&scene, sizeof(PFM_Scene), 1, fout);


	for (EUINT32 i = 0; i < scene.objectsCount; ++i)
	{
		PFM_Scene_Mesh sceneObj;
		sceneObj.type = (EUINT32)PFMTypes::MESH;
		sceneObj.parent = i; // i - no parent
		sceneObj.vno = 0;
		sceneObj.ino = i;

		fwrite(&sceneObj, sizeof(PFM_Scene_Mesh), 1, fout);
	}

	//
	// cleanup
	//
	delete[] offsets;
	delete[] vertices;
	fclose(fout);


}




//
//
//
//
//
void 
ParabellumFramework::ModelConverter::CalculateTBN(
	CommonVertexType* vertices, EUINT32 verticesCount,
	EUINT32* indices, EUINT32 indicesCount)
{
	for (EUINT32 i = 0; i < indicesCount; i += 3)
	{
		// wyznacz wierzcholki trojkata
		Vector3D v1, v2, v3;

		v1.X = vertices[indices[i + 0]].position.X;
		v1.Y = vertices[indices[i + 0]].position.Y;
		v1.Z = vertices[indices[i + 0]].position.Z;

		v2.X = vertices[indices[i + 1]].position.X;
		v2.Y = vertices[indices[i + 1]].position.Y;
		v2.Z = vertices[indices[i + 1]].position.Z;

		v3.X = vertices[indices[i + 2]].position.X;
		v3.Y = vertices[indices[i + 2]].position.Y;
		v3.Z = vertices[indices[i + 2]].position.Z;


		// UVs
		Vector3D uv1, uv2, uv3;

		uv1.X = vertices[indices[i + 0]].texture.X;
		uv1.Y = vertices[indices[i + 0]].texture.Y;
		uv1.Z = 0.0f;

		uv2.X = vertices[indices[i + 1]].texture.X;
		uv2.Y = vertices[indices[i + 1]].texture.Y;
		uv2.Z = 0.0f;

		uv3.X = vertices[indices[i + 2]].texture.X;
		uv3.Y = vertices[indices[i + 2]].texture.Y;
		uv3.Z = 0.0f;


		// get 2 triangle lines
		Vector3D v2v1, v3v1;

		v2v1.X = v2.X - v1.X;
		v2v1.Y = v2.Y - v1.Y;
		v2v1.Z = v2.Z - v1.Z;

		v3v1.X = v3.X - v1.X;
		v3v1.Y = v3.Y - v1.Y;
		v3v1.Z = v3.Z - v1.Z;

		// distance from UVs
		float c2c1t, c2c1b, c3c1t, c3c1b;

		c2c1t = uv2.X - uv1.X;
		c2c1b = uv2.Y - uv1.Y;

		c3c1t = uv3.X - uv1.X;
		c3c1b = uv3.Y - uv1.Y;

		// calculate tangent
		Vector3D tangent;


		tangent.X = c3c1b * v2v1.X - c2c1b * v3v1.X;
		tangent.Y = c3c1b * v2v1.Y - c2c1b * v3v1.Y;
		tangent.Z = c3c1b * v2v1.Z - c2c1b * v3v1.Z;


		vertices[indices[i + 0]].tangent.X += tangent.X;
		vertices[indices[i + 0]].tangent.Y += tangent.Y;
		vertices[indices[i + 0]].tangent.Z += tangent.Z;

		vertices[indices[i + 1]].tangent.X += tangent.X;
		vertices[indices[i + 1]].tangent.Y += tangent.Y;
		vertices[indices[i + 1]].tangent.Z += tangent.Z;

		vertices[indices[i + 2]].tangent.X += tangent.X;
		vertices[indices[i + 2]].tangent.Y += tangent.Y;
		vertices[indices[i + 2]].tangent.Z += tangent.Z;

	} // end for


}






//	--------------------------------------------------------------------
//		Gets and Sets
//	--------------------------------------------------------------------

