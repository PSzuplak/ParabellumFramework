#include "stdafx.h"
#include "ResourceManager.h"

ParabellumFramework::Resources::ResourceManager::ResourceManager()
{
	//m_memory = new char[65534];
	//m_memoryPlace = m_memory;
}

ParabellumFramework::Resources::ResourceManager::~ResourceManager()
{
	Release();
}

ParabellumFramework::EUINT32 
ParabellumFramework::Resources::ResourceManager::Initialize(
	_IN_ ParabellumFramework::Graphics::GraphicsDevice* graphics)
{
	m_graphics = graphics;

	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PFGI", "RSMN", "Initialize (" << (EADDR)m_graphics << ") : succes ");
	#endif

	return SystemCodes::SUCCESS;;
}



//
//
// Release all resources loaded by this resource manager
//
//

ParabellumFramework::EUINT32 
ParabellumFramework::Resources::ResourceManager::Release()
{
	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PFRM", "RMCL", "ResourceManager::Release() ");
	#endif

	EUINT32 result = SystemCodes::SUCCESS;
	Resource* invalidRes = NULLPTR;

	for (auto it : m_resources)
	{
		#ifdef DEBUG_LOGLEVEL_1
		PFDLOG("PFRM", "RMCL", "ResourceManager -> Releasing [ " << (EADDR)it.second << " ] (" << it.second->GetName() << ") ");
		#endif

		result = it.second->Release();

		if (result != SystemCodes::SUCCESS)
		{
			#ifdef DEBUG_LOGLEVEL_1
			PFDLOG("PFGI", "RSMN", "ERROR " << result << " -> Resource[ " << (EADDR)invalidRes << " ] coult not be released ");
			#endif

			invalidRes = it.second;
		}

		delete it.second;
		it.second = NULLPTR;
	}

	m_resources.clear();

	return SystemCodes::SUCCESS;
}



template<>
ParabellumFramework::Graphics::Shader*
ParabellumFramework::Resources::ResourceManager::Load(char* filename)
{
	EUINT64 key = ParabellumUtils::Math::GenerateHash(filename);
	EUINT32 result = SystemCodes::SUCCESS;

	// allocate memory for shader parametr
	if (m_resources[key] == NULLPTR)
	{
		m_resources[key] = new ParabellumFramework::Graphics::Shader();
		m_resources[key]->Load(filename);								// parse file
		m_referencesCount[key] = 0;

		// initializor specific for GraphicsDevice
		// no fucking idea why not working... later
		//result = m_graphics->InitializeShader((ParabellumFramework::Graphics::Shader*)m_resources[key]);
		//result = m_graphics->SetShader((ParabellumFramework::Graphics::Shader*)m_resources[key]);

		#ifdef DEBUG_LOGLEVEL_1
		PFDLOG("PFGI", "RSMN", "Load<Shader> ( " << filename << ") : addr=" << (EADDR)m_resources[key] << " ; result code(" << result << ") =" << GetCodeDesc(result));
		#endif
	}


	// increase number of tries how many times resource was loaded
	// ofcourse resource should be loaded only 1 time. For the rest - return exist pointer
	m_referencesCount[key] += 1;


	return (ParabellumFramework::Graphics::Shader*)m_resources[key];
}


template<>
ParabellumFramework::Graphics::Texture2D*
ParabellumFramework::Resources::ResourceManager::Load(char* filename)
{
	EUINT64 key = ParabellumUtils::Math::GenerateHash(filename);

	// allocate memory for shader parametr
	if (m_resources[key] == NULLPTR)
	{
		m_resources[key] = new ParabellumFramework::Graphics::Texture2D();
		m_resources[key]->Load(filename);								// parse file
		m_referencesCount[key] = 0;

		// graphics devcie stuff
		m_graphics->CreateTexture((ParabellumFramework::Graphics::Texture2D*)m_resources[key]);

		#ifdef DEBUG_LOGLEVEL_1
		PFDLOG("PFGI", "RSMN", "Load<Texture2D> ( " << filename << ") : addr=" << (EADDR)m_resources[key] << " : key=" << key);
		#endif

		// Releasing CPU data
		PF_DEBUG_EXPERIMENT("\nReleasing Texture2D::m_data may cause a problem if someting else require this data");
		((ParabellumFramework::Graphics::Texture2D*)m_resources[key])->ReleaseImageData();
	}

	m_referencesCount[key] += 1;

	return (ParabellumFramework::Graphics::Texture2D*)m_resources[key];
}



template<>
ParabellumFramework::Graphics::Model*
ParabellumFramework::Resources::ResourceManager::Load(char* filename)
{
	EUINT64 key = ParabellumUtils::Math::GenerateHash(filename);

	// allocate memory for shader parametr
	if (m_resources[key] == NULLPTR)
	{
		m_resources[key] = new ParabellumFramework::Graphics::Model();
		m_resources[key]->Load(filename);								// parse file
		m_referencesCount[key] = 0;

		// graphics devcie stuff
		((ParabellumFramework::Graphics::Model*)m_resources[key])->Initialize(m_graphics);

		#ifdef DEBUG_LOGLEVEL_1
		PFDLOG("PFGI", "RSMN", "Load<Model> ( " << filename << ") : addr=" << (EADDR)m_resources[key] << " : key=" << key);
		#endif
	}

	m_referencesCount[key] += 1;

	return (ParabellumFramework::Graphics::Model*)m_resources[key];
}