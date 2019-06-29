#include "stdafx.h"
#include "Model.h"

using namespace ParabellumFramework::Resources;


//	--------------------------------------------------------------------
//		Static Data
//	--------------------------------------------------------------------


ParabellumFramework::EUINT32 ParabellumFramework::Graphics::Model::ModelVerticesGroupsCount = 0;
ParabellumFramework::EUINT32 ParabellumFramework::Graphics::Model::ModelIndicesGroupsCount = 0;





ParabellumFramework::EUINT32  ParabellumFramework::Graphics::Model::ModelObjectsType[] = { 0 };

void** ParabellumFramework::Graphics::Model::ModelObjects[] = { NULLPTR };

void* ParabellumFramework::Graphics::Model::GetModelObject(EUINT32 no)
{
	return *Model::ModelObjects[no];
}




ParabellumFramework::EUINT32 ParabellumFramework::Graphics::Model::LoadedObjectsCount = 0;

ParabellumFramework::EUINT32 ParabellumFramework::Graphics::Model::GetLoadedObjectsCount()
{
	return Model::LoadedObjectsCount;
}


//ParabellumFramework::BoundingBox ParabellumFramework::Graphics::Model::ModelsBoundingBox[] = {};
//ParabellumFramework::BoundingSphere ParabellumFramework::Graphics::Model::ModelsBoundingSphere[] = {};



//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumFramework::Graphics::Model::Model() : Resource()
{
	m_vertexBuffersCount = 0;
	m_indexBuffersCount = 0;

	PFDLOG("PFGI", "SHCL", "Model[" << (EADDR)this << "]->Model() : Constructor called");
}


ParabellumFramework::Graphics::Model::~Model()
{
	PFDLOG("PFGI", "SHCL", "Model[" << (EADDR)this << "]->Model() : Destructor called");
}



//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------
ParabellumFramework::EUINT32
ParabellumFramework::Graphics::Model::Initialize(_IN_ GraphicsDevice* pDevice)
{
	#ifdef DEBUG_LOGLEVEL_1
		PFDLOG("PFGI", "SHCL", "Model[" << (EADDR)this << "]->Initialize()");
	#endif
	
	#ifdef DEBUG_LOGLEVEL_1
		PFDLOG("PFGI", "SHCL", "Model[" << (EADDR)this << "]->Members = { ");
		PFDLOG("PFGI", "SHCL", "                                          m_vertexBuffersCount = " << m_vertexBuffersCount);
		PFDLOG("PFGI", "SHCL", "                                          m_indexBuffersCount = " << m_indexBuffersCount);
	#endif

	for (EUINT32 i = 0; i < m_vertexBuffersCount; ++i)
	{
		m_vertexBuffers[i] = pDevice->CreateVertexBuffer(m_dataVertices[i].vertices, m_dataVertices[i].vertexSize, m_dataVertices[i].verticesCount);

		PFDLOG("PFGI", "SHCL", "                                          m_vertexBuffers[" << i << "] : addr = " << m_vertexBuffers[i]);
	}

	//
	// Index buffers
	//
	for (EUINT32 i = 0; i < m_indexBuffersCount; ++i)
	{
		m_indexBuffers[i] = pDevice->CreateIndexBuffer(m_dataIndices[i].indices, m_dataIndices[i].indicesCount);

		PFDLOG("PFGI", "SHCL", "                                          m_indexBuffers[" << i << "] : addr = " << m_indexBuffers[i]);
	}


	#ifdef DEBUG_LOGLEVEL_1
		PFDLOG("PFGI", "SHCL", "                                        } ");
	#endif



	return SystemCodes::SUCCESS;
}

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::Model::Release()
{
	#ifdef DEBUG_LOGLEVEL_1
		PFDLOG("PFGI", "SHCL", "Model[" << (EADDR)this << "]->Release()");
	#endif

	for (EUINT32 i = 0; i < m_vertexBuffersCount; ++i)
		delete[] m_dataVertices[i].vertices;
	delete[] m_dataVertices;

	for (EUINT32 i = 0; i < m_indexBuffersCount; ++i)
		delete[] m_dataIndices[i].indices;
	delete[] m_dataIndices;


	delete[] m_vBIds;
	delete[] m_iBIds;

	return SystemCodes::SUCCESS;
}

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::Model::Parse(const char* filePath)
{
	EUINT32 result = SystemCodes::SUCCESS;

	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PFGI", "MODL", "Model[" << (EADDR)this << "]->Parse (" << filePath << ")");
	#endif

	FILE *fin;
	fin = fopen(filePath, "rb");

	if (!fin)
	{
		#ifdef DEBUG_LOGLEVEL_1
		PFDLOG("PFGI", "MODL", "Model[" << (EADDR)this << "]->Parse (...) : Can not find the file");
		#endif

		return SystemCodes::ERROR_NOFILE;
	}


	long fpos = 0;


	// data
	m_vertexBuffersCount = 0;
	m_indexBuffersCount = 0;

	std::vector<EUINT32> offsetVertices;	// offsets id of Vertices objects in PFMData

	std::vector<EUINT32> offsetGroup;		// offsets id of GROUP objects in PFMData


	// load header
	PFMHeader header;
	fread(&header, sizeof(PFMHeader), 1, fin);

	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PFGI", "MODL", "Model[" << (EADDR)this << "]->Parse (...) : Header=" << "checksum=" << header.checksum << ",format=" << header.fileFormat << ",ver maj=" << header.versionMajor << ",ver min=" << header.versionMinor << ",date=" << header.dateCreated);
	#endif

	// load environment settings
	PFMEnvironmentSettings environment;
	fread(&environment, sizeof(environment), 1, fin);

	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PFGI", "MODL", "Model[" << (EADDR)this << "]->Parse (...) : Settings=" << "\tscale : " << environment.scale << "\tunits : " << environment.units << "\tcoord system : " << environment.coordinateSystem);
	#endif

	// load objects
	EUINT32 objectsCount;
	fread(&objectsCount, sizeof(EUINT32), 1, fin);

	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PFGI", "MODL", "Model[" << (EADDR)this << "]->Parse (...) : Objects count=" << objectsCount);
	#endif

	EUINT32* offsets = new EUINT32[objectsCount];
	fread(offsets, sizeof(EUINT32), objectsCount, fin);

	// check how many objects are meshes
	fpos = ftell(fin);


	// count objects
	for (EUINT32 i = 0; i < objectsCount; ++i)
	{
		fseek(fin, offsets[i], 0);

		EUINT32 type = 0;
		fread(&type, sizeof(EUINT32), 1, fin);

	#ifdef DEBUG_LOGLEVEL_1
		PFDLOG("PFGI", "MODL", "Model[" << (EADDR)this << "]->Parse (...) : Object type=" << type);
	#endif

		// add VERTICES
		if (type == (EUINT32)PFMTypes::VERTICES)
		{
			offsetVertices.push_back(offsets[i]);
			m_vertexBuffersCount++;
		}

		// add GROUP
		if (type == (EUINT32)PFMTypes::GROUP)
		{
			offsetGroup.push_back(offsets[i]);
			m_indexBuffersCount++;
		}

	}

	fseek(fin, fpos, 0);


	//
	// init ModelParts
	//
	m_dataVertices = new DataVertices[m_vertexBuffersCount];
	m_dataIndices = new DataIndices[m_indexBuffersCount];

	m_vertexBuffers = new VertexBuffer*[m_vertexBuffersCount];
	m_indexBuffers = new IndexBuffer*[m_indexBuffersCount];
	m_vBIds = new EUINT32[m_vertexBuffersCount];
	m_iBIds = new EUINT32[m_indexBuffersCount];


	//
	// create vertex buffers
	//
	for (EUINT32 i = 0; i < m_vertexBuffersCount; ++i)
	{

		ModelObjects[LoadedObjectsCount] = (void**)&m_vertexBuffers[i];
		ModelObjectsType[LoadedObjectsCount] = (EUINT32)PFMTypes::VERTICES;
		m_vBIds[i] = LoadedObjectsCount;
		LoadedObjectsCount++;
	}

	//
	// create index buffers
	//
	for (EUINT32 i = 0; i < m_indexBuffersCount; ++i)
	{
		ModelObjects[LoadedObjectsCount] = (void**)&m_indexBuffers[i];
		ModelObjectsType[LoadedObjectsCount] = (EUINT32)PFMTypes::INDICES;
		m_iBIds[i] = LoadedObjectsCount;
		LoadedObjectsCount++;
	}


	//
	// load mesh
	//
	for (EUINT32 i = 0; i < m_vertexBuffersCount; ++i)
	{
		EUINT32 type = 0;
		fread(&type, sizeof(EUINT32), 1, fin);

		EUINT32 vertexFormatSize = 0;
		fread(&vertexFormatSize, sizeof(EUINT32), 1, fin);

		// read vertices
		EUINT32 verticesCount = 0;
		fread(&verticesCount, sizeof(EUINT32), 1, fin);

		#ifdef DEBUG_LOGLEVEL_1
		PFDLOG("PFGI", "MODL", "Model[" << (EADDR)this << "]->Parse (...) : VERTICES no : " << i << " , type :" << type << ", format size : " << vertexFormatSize << ", vertices count : " << verticesCount);
		#endif

		void* vertices = malloc(vertexFormatSize*verticesCount);
		fread(vertices, vertexFormatSize*verticesCount, 1, fin);

		// make model part
		m_dataVertices[i].vertices = vertices;
		m_dataVertices[i].verticesCount = verticesCount;
		m_dataVertices[i].vertexSize = vertexFormatSize;
	}


	// load groups
	for (EUINT32 i = 0; i < m_indexBuffersCount; ++i)
	{
		EUINT32 type = 0;
		fread(&type, sizeof(EUINT32), 1, fin);

		EUINT32 verticesNo = 0;
		fread(&verticesNo, sizeof(EUINT32), 1, fin);

		EUINT32 indicesCount = 0;
		fread(&indicesCount, sizeof(EUINT32), 1, fin);

		void* indices = malloc(sizeof(EUINT32)*indicesCount);
		fread(indices, sizeof(EUINT32)*indicesCount, 1, fin);

		CommonMaterialType material;
		fread(&material, sizeof(CommonMaterialType), 1, fin);

		#ifdef DEBUG_LOGLEVEL_1
		PFDLOG("PFGI", "MODL", "Model[" << (EADDR)this << "]->Parse (...) : GROUP (" << i << ") : VERTICES no : " << verticesNo << ", indices count : " << indicesCount);
		PFDLOG("PFGI", "MODL", "Model[" << (EADDR)this << "]->Parse (...) : GROUP (" << i << ") Textures : diffuse : " << material.pathTextureDiffuse << ", normal : " << material.pathTextureNormal << ", light : " << material.pathTextureLight << ", extension : " << material.pathTextureExtenstion);
		#endif


		// make model part
		m_dataIndices[i].indices = (EUINT32*)indices;
		m_dataIndices[i].indicesCount = indicesCount;
		m_dataIndices[i].material = material;
	}


	//
	// Create Bounding volumes
	Vector3D minv, maxv;

	minv.Set(0.0f, 0.0f, 0.0f);
	maxv.Set(0.0f, 0.0f, 0.0f);

	m_dataIndices[0].bbox.MakeFromPoints(&minv, &maxv);
	m_boundingBox.MakeFromPoints(&minv, &maxv);

	for (EUINT32 i = 0; i < m_indexBuffersCount; ++i)
	{
		for (EUINT32 j = 0; j < m_dataIndices[i].indicesCount; ++j)
		{
			m_dataIndices[i].bbox.Merge(&((CommonVertexType*)m_dataVertices[0].vertices)[m_dataIndices[i].indices[j]].position);
			m_boundingBox.Merge(&m_dataIndices[i].bbox);
		}
	
		m_dataIndices[i].bsphere.MakeFromBoundingBox(&m_dataIndices[i].bbox);
	}

	m_boundingSphere.MakeFromBoundingBox(&m_boundingBox);





	//
	// load scene
	//

	PFM_Scene scene;
	fread(&scene, sizeof(PFM_Scene), 1, fin);

	m_modelParts = new ModelPart[scene.objectsCount];
	m_modelPartsCount = scene.objectsCount;

	for (EUINT32 i = 0; i < scene.objectsCount; ++i)
	{
		PFM_Scene_Mesh sceneObj;
		fread(&sceneObj, sizeof(PFM_Scene_Mesh), 1, fin);

		m_modelParts[i].m_ibIndex = m_iBIds[sceneObj.ino];
		m_modelParts[i].m_vbIndex = m_vBIds[sceneObj.vno];

		m_modelParts[i].m_material = m_dataIndices[i].material;
		m_modelParts[i].m_boundingBox = m_dataIndices[i].bbox;
		m_modelParts[i].m_boundingSphere = m_dataIndices[i].bsphere;

		#ifdef DEBUG_LOGLEVEL_1
				PFDLOG("PFGI", "MODL", "Model[" << (EADDR)this << "]->Parse (...) : SCENE OBJ no : " << i << ", VERTICES no : " << sceneObj.vno << ", indices no : " << sceneObj.ino);
		#endif
	}


	// cleanup
	fclose(fin);


	return result;
}


//	--------------------------------------------------------------------
//		Gets and sets
//	--------------------------------------------------------------------

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::Model::GetModelPartsCount()
{
	return m_modelPartsCount;
}

ParabellumFramework::Graphics::ModelPart*
ParabellumFramework::Graphics::Model::GetModelPart(EUINT32 no)
{
	return &m_modelParts[no];
}


ParabellumFramework::Graphics::VertexBuffer*
ParabellumFramework::Graphics::Model::GetVertexBuffer(EUINT32 no)
{
	return m_vertexBuffers[no];
}

ParabellumFramework::Graphics::IndexBuffer*
ParabellumFramework::Graphics::Model::GetIndexBuffer(EUINT32 no)
{
	return m_indexBuffers[no];
}