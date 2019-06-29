#include "Terrain.h"



//	------------------------------------------------------------
//		Statics
//	------------------------------------------------------------



//	------------------------------------------------------------
//		Constructors and destructors
//	------------------------------------------------------------

ParabellumEngine::Components::Terrain::Terrain(
	_IN_ ParabellumFramework::Graphics::GraphicsDevice* pDevice, _IN_ ParabellumFramework::Resources::ResourceManager* pResources,
	const char* fileName,
	EUINT32 slicesCountX, EUINT32 slicesCountY, EFLOAT32 distanceBetweenVertices, EFLOAT32 maxHeight
	)
{
	EUINT32 result = SystemCodes::SUCCESS;


	m_slicesCountX = slicesCountX;
	m_slicesCountY = slicesCountY;
	EUINT32 beginX = 0;
	EUINT32 beginY = 0;
	EUINT32 sliceWidth = 0;
	EUINT32 sliceHeight = 0;

	//
	// read file
	//
	FILE* file = fopen(fileName, "rb");

	if (!file)
	{
		#ifdef DEBUG_LOGLEVEL_1
		PFDLOG("PECO", "TERR", "Terrain::Terrain() : Could not open file : " << fileName);
		#endif

		assert(!"Problems with opening file in Terrain::Terrain()");
	}

	// calculate height and width
	//m_width
	fseek(file, 0L, SEEK_END);
	m_width = m_height = (EUINT32)(ParabellumUtils::Math::FastSqrt(ftell(file)) / 2 );

	PF_DEBUG_EXPERIMENT("Static width and height for heightmap");
	m_width = 2048;
	m_height = 2048;

	fseek(file, 0L, SEEK_SET);

	sliceWidth = m_width / m_slicesCountX;
	sliceHeight = m_height / m_slicesCountY;

	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PECO", "TERR", "Creating Terrain() from : " << fileName << "; width = " << m_width << "; height = " << m_height << "; slices count X = " << slicesCountX << "; slices count Y = " << slicesCountY << "; distance = " << distanceBetweenVertices << "; max height = " << maxHeight << " sliceWidth = " << sliceWidth << " sliceHeight = " << sliceHeight);
	#endif


	EFLOAT32* heightmap = new EFLOAT32[m_width*m_height];

	if (heightmap == NULLPTR)
	{
		#ifdef DEBUG_LOGLEVEL_1
		PFDLOG("PECO", "TERR", "Terrain::Terrain() : Could not allocate " << sliceWidth*sliceHeight*sizeof(EUINT32) << "B for heightmap");
		#endif

		assert(!"Can NOT allocate memory for heightmap in Terrain::Terrain()");
	}

	EFLOAT32 vmin, vmax;
	vmin = +9999999.0f;
	vmax = -9999999.0f;

	for (EUINT32 y = 0; y < m_height; ++y)
	{
		for (EUINT32 x = 0; x < m_width; ++x)
		{
			EFLOAT32 height;

			fread(&height, sizeof(EFLOAT32), 1, file);

			heightmap[x + y*m_width] = height;

			if (vmin > heightmap[x + y*m_width]) vmin = heightmap[x + y*m_width];
			if (vmax < heightmap[x + y*m_width]) vmax = heightmap[x + y*m_width];

		}
	}

	// normalize heights
	for (EUINT32 y = 0; y < m_height; ++y)
	{
		for (EUINT32 x = 0; x < m_width; ++x)
		{
			heightmap[x + y*m_width] += ParabellumUtils::Math::Abs(vmin);
			heightmap[x + y*m_width] /= (vmax + ParabellumUtils::Math::Abs(vmin));
		}
	}


	fclose(file);



	m_terrainCells = new TerrainCell[slicesCountX*slicesCountY];

	if (m_terrainCells == NULLPTR)
	{
		#ifdef DEBUG_LOGLEVEL_1
		PFDLOG("PECO", "TERR", "Terrain::Terrain() : Could not allocate " << slicesCountX*slicesCountY*sizeof(TerrainCell) << "B for m_terrainCells");
		#endif

		assert(!"Can NOT allocate memory for m_terrainCells in Terrain::Terrain()");
	}


	//m_terrainCells[0].Initialize(pDevice, pResources->Load<Texture2D>("Resources/Textures/test1.pft"), 0.2f, 10.0f, 0, 0, 255, 255);
	//m_terrainCells[0].Initialize(pDevice, heightmap, 2048, 2048, 0.2f, 1.0f, 0, 0, 512, 512);
	

	for (EUINT32 y = 0; y < m_slicesCountY; ++y)
	{
		for (EUINT32 x = 0; x < m_slicesCountX; ++x)
		{
			m_terrainCells[x + y*m_slicesCountX].Initialize(
				pDevice, heightmap, m_width, m_height, 
				distanceBetweenVertices, maxHeight, 
				beginX, beginY, sliceWidth, sliceHeight, 
				0.00f);

			if (result != SystemCodes::SUCCESS)
				assert(!"Terrain::Terrain() ; m_terrainCells[].Initialize(..)");

			beginX += sliceWidth;
			
		}
		beginX = 0;
		beginY += sliceHeight;
	}


	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PECO", "TERR", "Terrain::Terrain() : SUCCESS");
	#endif


	delete[] heightmap;
}


ParabellumEngine::Components::Terrain::~Terrain()
{
	if (m_terrainCells != NULLPTR)
		delete[] m_terrainCells;
}

//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------



//	------------------------------------------------------------
//		Gets Sets
//	------------------------------------------------------------
