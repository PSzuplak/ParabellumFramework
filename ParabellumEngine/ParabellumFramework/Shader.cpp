#include "stdafx.h"
#include "Shader.h"

//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumFramework::Graphics::Shader::Shader()
{
	PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "]->Shader() : Constructor called");

	PFDLOG("PFGI", "PGRT", "AddressOf(m_vertexShaderBuffer) : " << &m_vertexShaderBuffer);
	PFDLOG("PFGI", "PGRT", "AddressOf(m_pixelShaderBuffer) : " << &m_pixelShaderBuffer);
	PFDLOG("PFGI", "PGRT", "AddressOf(m_vertexShaderLayout) : " << &m_vertexShaderLayout);
	PFDLOG("PFGI", "PGRT", "AddressOf(m_parametrsBuffer) : " << &m_parametrsBuffer);
	PFDLOG("PFGI", "PGRT", "AddressOf(m_bufferShaderParametrs) : " << &m_bufferShaderParametrs);

	m_vertexShaderBuffer = NULLPTR;
	m_pixelShaderBuffer = NULLPTR;
	m_vertexShaderLayout = NULLPTR;
	m_parametrsBuffer = NULLPTR;
	m_bufferShaderParametrs = NULLPTR;

	m_isExternalBufferUsed = false;
	m_externalBuffers = NULLPTR;
}

ParabellumFramework::Graphics::Shader::~Shader()
{
	PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "]->~Shader() : Destructor called");
}


//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------



// initialize CBuffers, Resources and Parametrs. Should be executed after Parse(...)
ParabellumFramework::EUINT32
ParabellumFramework::Graphics::Shader::InitializeData()
{
	PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "][" << m_shaderFilename << "]"  << "  ->InitializeData() :");


	class MyData
	{
	public:
		EUINT64 key;
		EUINT32 size;
		std::string name;

		bool operator<(const MyData &rhs) const { return key < rhs.key; }
	};

	PFDLOG("PFGI", "SHCL", "  Members = {");
	PFDLOG("PFGI", "SHCL", "               m_vertexShaderFunctionName         = " << m_vertexShaderFunctionName);
	PFDLOG("PFGI", "SHCL", "               m_pixelShaderFunctionName          = " << m_pixelShaderFunctionName);
	PFDLOG("PFGI", "SHCL", "               m_geometryShaderFunctionName       = " << m_geometryShaderFunctionName);
	PFDLOG("PFGI", "SHCL", "               tesselShaderFunctionNamealization  = " << m_tesselShaderFunctionName);
	PFDLOG("PFGI", "SHCL", "               m_vsLayoutCount                    = " << m_vsLayoutCount);
	PFDLOG("PFGI", "SHCL", "               m_samplersCount                    = " << m_samplersCount);
	PFDLOG("PFGI", "SHCL", "            }");


	//
	// initialize m_parametrsBuffer
	//
	m_parametrsOffsets = new EUINT32*[m_cbuffersCount];
	m_parametrsBuffersOffsets = new EUINT32[m_cbuffersCount];
	m_parametrsBuffersSizes = new EUINT32[m_cbuffersCount];
	m_parametrsBufferSize = 0;
	m_parametrsCount = 0;

	std::vector<MyData> keys;

	for (EUINT32 i = 0; i < m_cbuffersCount; ++i)
	{
		m_parametrsOffsets[i] = new EUINT32[m_cbuffers[i]->GetVariablesCount()];
		m_parametrsOffsets[i][0] = m_parametrsBufferSize;
		m_parametrsBuffersSizes[i] = 0;

		for (EUINT32 j = 0; j < m_cbuffers[i]->GetVariablesCount(); ++j)
		{
			EUINT64 key = ParabellumUtils::Math::GenerateHash(m_cbuffers[i]->GetVariable(j)->GetName().c_str());
			
			MyData tmp;
			tmp.key = key;
			tmp.size = m_cbuffers[i]->GetVariable(j)->GetSize();
			tmp.name = m_cbuffers[i]->GetVariable(j)->GetName().c_str();
			keys.push_back(tmp);


			m_parametrsOffsets[i][j] = m_parametrsBufferSize;
			m_parametrsBufferSize += m_cbuffers[i]->GetVariable(j)->GetSize();
			m_parametrsBuffersSizes[i] += m_cbuffers[i]->GetVariable(j)->GetSize();

			m_parametrsCount++;
		}
	}

	//
	m_parametrsBuffer = new char[m_parametrsBufferSize];
	PFDLOG("PFGI", "SHCL", "   m_parametrsBuffer allocated at [address] : " << (EADDR)m_parametrsBuffer);

	m_bufferShaderParametrs = malloc(m_parametrsCount * sizeof(ShaderParametr) + sizeof(ShaderParametr));
	PFDLOG("PFGI", "SHCL", "   m_bufferShaderParametrs allocated at [address] : " << (EADDR)m_bufferShaderParametrs << "; of size : " << m_parametrsCount * sizeof(ShaderParametr) + sizeof(ShaderParametr) );

	//
	m_sortedPointersToParametrs = new ShaderParametr*[m_parametrsCount];

	m_bufferKeys = new EUINT64[m_parametrsCount];

	// sort keys
	std::sort(keys.begin(), keys.end());


	//
	// create parametrs for cbuffers variables
	//
	PFDLOG("PFGI", "SHCL", "   m_parametrs[] = {        ShaderParametr ADDR   |     m_parametrsBuffer ADDR      |       Key      |      Name");

	// set memory for each ShaderParametr in m_parametrs
	EUINT32 currentParam = 0;

	for (EUINT32 i = 0; i < keys.size(); ++i)
	{
		if (m_parametrs[keys[i].key] == NULLPTR)
		{
			m_parametrs[keys[i].key] = (ShaderParametr*)(m_bufferShaderParametrs)+currentParam;
			m_bufferKeys[currentParam] = keys[i].key;
			currentParam++;
		}
	}

	// set rest of the parametrs data data
	currentParam = 0;

	for (EUINT32 i = 0;  i < m_cbuffersCount; ++i)
		for (EUINT32 j = 0; j < m_cbuffers[i]->GetVariablesCount(); ++j)
		{
			EUINT64 key = ParabellumUtils::Math::GenerateHash(m_cbuffers[i]->GetVariable(j)->GetName().c_str());
			m_parametrs[key]->m_shaderBufferLocation = (void*)(m_parametrsBuffer + m_parametrsOffsets[i][j]);
			m_parametrs[key]->SetSize(m_cbuffers[i]->GetVariable(j)->GetSize());
			m_umParams[m_cbuffers[i]->GetVariable(j)->GetName()] = m_parametrs[key];
			m_sortedPointersToParametrs[currentParam] = m_parametrs[key];
			currentParam++;
			PFDLOG("PFGI", "SHCL", "                    " << "          " << (EADDR)m_parametrs[key] << "              " << (EADDR)m_parametrs[key]->m_shaderBufferLocation << "          " << key << "            " << m_cbuffers[i]->GetVariable(j)->GetName().c_str() );
		
		}

	PFDLOG("PFGI", "SHCL", "   } ");



	PFDLOG("PFGI", "SHCL", "    m_parametrs -> Key - Memory relation (sorted)");
	PFDLOG("PFGI", "SHCL", "    {        ADDR      |      Key       ");
	for (auto it : m_parametrs)
	{
	PFDLOG("PFGI", "SHCL", "       " << it.second << "   " << it.first);
	}
	PFDLOG("PFGI", "SHCL", "    } ");


	PFDLOG("PFGI", "SHCL", "    m_bufferKeys = {          Key");
	for (EUINT32 i = 0; i < m_parametrsCount; ++i)
	{
		PFDLOG("PFGI", "SHCL", "                       " << m_bufferKeys[i]);
	}
	PFDLOG("PFGI", "SHCL", "                   } ");








	//	-------------------------------------------------------------------------
	// create parametrs for shader resources
	//
	PFDLOG("PFGI", "SHCL", "   m_resourcesNames[] = {            ADDR        |         Key        |         Name        |     Size");
	
	m_shaderResourcesPointersCount = m_resourcesNames.size();
	m_sortedPointersToResources = new ShaderParametr*[m_shaderResourcesPointersCount];

	EUINT32 currentResouce = 0;

	for (EUINT32 i = 0; i < m_resourcesNames.size(); ++i)
	{
		EUINT64 key = ParabellumUtils::Math::GenerateHash(m_resourcesNames[i].c_str());

		// create parametr only if it doesnt exist
		if (m_parametrs[key] == NULLPTR)
		{
			m_parametrs[key] = new ShaderParametr();
			m_parametrs[key]->m_shaderBufferLocation = NULLPTR;

			m_umParams[m_resourcesNames[i]] = m_parametrs[key];

			m_sortedPointersToResources[i] = m_parametrs[key];
			m_cbuffers[0]->m_sortedPointersToResources[i] = m_sortedPointersToResources[i];

			m_resourcesKeys.push_back(key);

			PFDLOG("PFGI", "SHCL", "                             " << m_parametrs[key]->m_shaderBufferLocation << "      " << key << "      " << m_resourcesNames[i].c_str() << "           " << m_parametrs[key]->m_parametrSize);
		}
		else
		{
			PFDLOG("PFGI", "SHCL", "ERROR : m_parametrs[" << key << "] Already Exists!");
			return SystemCodes::ERROR_INIT;
		}
	}

	PFDLOG("PFGI", "SHCL", "   } // END m_resourcesNames ");



	// allocate memory for eternal buffers if any
	m_externalBuffers = new void*[m_cbuffersCount];







	return SystemCodes::SUCCESS;
}






ParabellumFramework::EUINT32
ParabellumFramework::Graphics::Shader::Release()
{
	PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "]->Release()");


	if (m_externalBuffers != NULLPTR)
	{
		delete[] m_externalBuffers;
	}


	// release layout
	if (m_vertexShaderLayout != NULLPTR)
	{
		for (EUINT32 i = 0; i < m_vsLayoutCount; ++i)
		{
			m_vertexShaderLayout[i].Release();
			delete[] m_vertexShaderLayout;
			m_vertexShaderLayout = NULLPTR;
		}
	}


	delete[] m_samplerStates;
	m_samplerStates = NULLPTR;

	// releate for ShaderParametrs
	if (m_bufferShaderParametrs != NULLPTR)
	{
		free(m_bufferShaderParametrs);
		m_bufferShaderParametrs = NULLPTR;
	}
	

	if (m_sortedPointersToParametrs != NULLPTR)
	{
		delete[] m_sortedPointersToParametrs;
		m_sortedPointersToParametrs = NULLPTR;
	}

	for (EUINT32 i = 0; i < m_shaderResourcesPointersCount; ++i)
	{
		if (m_sortedPointersToResources[i] != NULLPTR)
		{
			m_sortedPointersToResources[i]->Release();
			delete m_sortedPointersToResources[i];
			m_sortedPointersToResources[i] = NULLPTR;
		}
	}

	if (m_sortedPointersToResources != NULLPTR)
	{
		delete[] m_sortedPointersToResources;
		m_sortedPointersToResources = NULLPTR;
	}

	if (m_parametrsBuffer != NULLPTR)
	{
		delete[] m_parametrsBuffer;
		m_parametrsBuffer = NULLPTR;
	}


	delete[] m_cbuffers;
	m_cbuffers = NULLPTR;

	delete[] m_samplerStates;
	m_samplerStates = NULLPTR;


	return SystemCodes::SUCCESS;
}









ParabellumFramework::EUINT32
ParabellumFramework::Graphics::Shader::Parse(const char* filePath)
{
	
	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "]->Parse(" << filePath << ")");
	#endif

	FILE* fs = fopen(filePath, "rb");

	if (fs == NULLPTR)
	{
		#ifdef DEBUG_LOGLEVEL_1
		PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "]->Parse(...) : Can not find the file");
		#endif

		return SystemCodes::ERROR_NOFILE;
	}

	//
	// read header
	//
	EUINT32 shaderType = 0;
	EUINT32 shaderVersion = 0;
	EUINT32 converterId = 0;
	EUINT32 converterFlag = 0;
	EUINT32 shaderOptions = 0;

	fread(&shaderType, sizeof(EUINT32), 1, fs);
	fread(&shaderVersion, sizeof(EUINT32), 1, fs);
	fread(&converterId, sizeof(EUINT32), 1, fs);
	fread(&converterFlag, sizeof(EUINT32), 1, fs);
	fread(&shaderOptions, sizeof(EUINT32), 1, fs);
	ReadString(m_shaderFilename, fs);



	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "]->Parse(...) : Got shader header");
	PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "]->Parse(...) : Shader type : " << shaderType);
	PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "]->Parse(...) : Shader version : " << shaderVersion);
	PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "]->Parse(...) : Converter id : " << converterId);
	PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "]->Parse(...) : Converter flags : " << converterFlag);
	PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "]->Parse(...) : Shader options : " << shaderOptions);
	PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "]->Parse(...) : Shader filename : " << m_shaderFilename);
	#endif


	//
	// read passes and functions names
	//
	EUINT32 passesCount = 1; //passes.size();
	fread(&passesCount, sizeof(EUINT32), 1, fs);

	for (EUINT32 i = 0; i < passesCount; ++i)
	{
		ReadString(m_vertexShaderFunctionName, fs);
		ReadString(m_pixelShaderFunctionName, fs);
		ReadString(m_geometryShaderFunctionName, fs);
		ReadString(m_tesselShaderFunctionName, fs);

		#ifdef DEBUG_LOGLEVEL_1
		PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "]->Parse(...) : Got shader PASS " << i);
		PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "]->Parse(...) : VS name : " << m_vertexShaderFunctionName);
		PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "]->Parse(...) : PS name : " << m_pixelShaderFunctionName);
		PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "]->Parse(...) : GS name : " << m_geometryShaderFunctionName);
		PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "]->Parse(...) : TS name : " << m_tesselShaderFunctionName);
		#endif
	}

	//
	// read layout
	//
	fread(&m_vsLayoutCount, sizeof(EUINT32), 1, fs);
	m_vertexShaderLayout = new ShaderLayout[m_vsLayoutCount];

	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "]->Parse(...) : vertex layouts count : " << m_vsLayoutCount);
	#endif

	for (EUINT32 i = 0; i < m_vsLayoutCount; ++i)
	{
		std::string type;
		std::string name;
		EUINT32 semantic;
		EUINT32 semanticIndex;
		EUINT32 format;


		ReadString(type, fs);
		ReadString(name, fs);
		fread(&semantic, sizeof(EUINT32), 1, fs);
		fread(&semanticIndex, sizeof(EUINT32), 1, fs);
		fread(&format, sizeof(EUINT32), 1, fs);

		#ifdef DEBUG_LOGLEVEL_1
		PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "]->Parse(...) : vertex layout : " << i << " \ttype : " << name << " \tvariable : " << semantic << " \tsemantic type : " << semanticIndex << " \tsemantic format : " << format);
		#endif

		EUINT32 allignedDataOffset = 0;

		if (i > 0) allignedDataOffset = 0xffffffff;//D3D11_APPEND_ALIGNED_ELEMENT;

		m_vertexShaderLayout[i].Initialize(
			name,
			semantic,
			semanticIndex,
			format,
			0,
			allignedDataOffset,
			0, //D3D11_INPUT_PER_VERTEX_DATA,
			0
			);

	}


	//
	// Read Cbuffers
	//
	fread(&m_cbuffersCount, sizeof(EUINT32), 1, fs);

	if (m_cbuffersCount > 0)
		m_cbuffers = new ShaderCBuffer*[m_cbuffersCount];
	else
		m_cbuffers = NULLPTR;

	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "]->Parse(...) : CBuffers count=" << m_cbuffersCount);
	#endif

	if (m_cbuffersCount > 0)
		for (EUINT32 i = 0; i < m_cbuffersCount; ++i)
		{
			m_cbuffers[i] = new ShaderCBuffer();

			std::string name;
			EUINT32 variablesCount = 0;

			ReadString(name, fs);
			fread(&variablesCount, sizeof(EUINT32), 1, fs);

			ShaderVariable* variables = new ShaderVariable[variablesCount];

			#ifdef DEBUG_LOGLEVEL_1
			PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "]->Parse(...) : CBuffer no " << i << " has " << variablesCount << " variables");
			#endif

			for (EUINT32 j = 0; j < variablesCount; ++j)
			{
				std::string varName;
				EUINT32 varType = 0;
				EUINT32 size = 0;

				ReadString(varName, fs);
				fread(&varType, sizeof(EUINT32), 1, fs);

				switch (varType)
				{
				case GraphicsType::FLOAT1: size = 4; break;
				case GraphicsType::FLOAT2: size = 8; break;
				case GraphicsType::FLOAT3: size = 12; break;
				case GraphicsType::FLOAT4: size = 16; break;
				case GraphicsType::FLOAT4X4: size = 64; break;
				default: size = 0; break;
				}

				variables[j].Initialize(varName, varType, size);

				#ifdef DEBUG_LOGLEVEL_1
				PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "]->Parse(...) : name=" << varName << ",type=" << varType << ",size=" << size);
				#endif
			}

			m_cbuffers[i]->Initialize(variables, variablesCount, i);


			delete[] variables;

		}


	//
	// read resources
	//
	EUINT32 resourcesCount = 0;
	fread(&resourcesCount, sizeof(EUINT32), 1, fs);

	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "]->Parse(...) : resources count=" << resourcesCount);
	#endif

	for (EUINT32 i = 0; i < resourcesCount; ++i)
	{
		std::string name;
		EUINT32 type = 0;

		ReadString(name, fs);
		fread(&type, sizeof(EUINT32), 1, fs);

		m_resourcesNames.push_back(name);
		m_resourcesTypes.push_back(type);

		#ifdef DEBUG_LOGLEVEL_1
		PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "]->Parse(...) : name=" << name << ", type=" << type);
		#endif
	}


	// save samplers
	fread(&m_samplersCount, sizeof(EUINT32), 1, fs);
	m_samplerStates = new ShaderSamplerState*[m_samplersCount];

	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "]->Parse(...) : samplers count=" << m_samplersCount);
	#endif

	for (EUINT32 i = 0; i < m_samplersCount; ++i)
	{
		m_samplerStates[i] = new ShaderSamplerState();
		//ReadString(samplers[i].name, fs);
		//fwrite(&samplers[i].type, sizeof(EUINT32), 1, fconv);

		m_samplerStates[i]->Initialize(
			SamplerFilter::MIN_MAG_MIP_POINT,
			SamplerAddress::WRAP,
			SamplerAddress::WRAP,
			SamplerAddress::WRAP,
			0.0f, 1,
			SamplerComparsion::ALWAYS
			);

		#ifdef DEBUG_LOGLEVEL_1
		PFDLOG("PFGI", "SHCL", "Shader[" << (EADDR)this << "]->Parse(...) : filter=" << (EUINT32)SamplerFilter::MIN_MAG_MIP_POINT << ",cmp" << (EUINT32)SamplerComparsion::ALWAYS);
		#endif
	}


	fclose(fs);

	return SystemCodes::SUCCESS;
}


std::string&
ParabellumFramework::Graphics::Shader::GetShaderFilename()
{
	return m_shaderFilename;
}

void*
ParabellumFramework::Graphics::Shader::GetVertexBuffer()
{
	return &m_vertexShaderBuffer;
}

void*
ParabellumFramework::Graphics::Shader::GetPixelBuffer()
{
	return &m_pixelShaderBuffer;
}

std::string&
ParabellumFramework::Graphics::Shader::GetVSFunctionName()
{
	return m_vertexShaderFunctionName;
}

std::string&
ParabellumFramework::Graphics::Shader::GetPSFunctionName()
{
	return m_pixelShaderFunctionName;
}


ParabellumFramework::EUINT32
ParabellumFramework::Graphics::Shader::GetCBuffersCount()
{
	return m_cbuffersCount;
}


ParabellumFramework::Graphics::Shader::ShaderCBuffer*
ParabellumFramework::Graphics::Shader::GetCBuffer(EUINT32 no)
{
	return m_cbuffers[no];
}


ParabellumFramework::EUINT32
ParabellumFramework::Graphics::Shader::GetLayoutDataCount()
{
	return m_vsLayoutCount;
}

ParabellumFramework::Graphics::ShaderLayout*
ParabellumFramework::Graphics::Shader::GetLayout()
{
	return m_vertexShaderLayout;
}


void*
ParabellumFramework::Graphics::Shader::GetInputLayout()
{
	return &m_inputLayout;
}





//	--------------------------------------------------------------------
//		Operators
//	--------------------------------------------------------------------


ParabellumFramework::Graphics::ShaderParametr&
ParabellumFramework::Graphics::Shader::operator[](EUINT64 key)
{
	return Parametrs(key);
}

ParabellumFramework::Graphics::ShaderParametr*
ParabellumFramework::Graphics::Shader::operator()(EUINT32 index)
{
	//return *m_sortedPointersToParametrs[index];
	return m_parametrs[index];
}

ParabellumFramework::Graphics::ShaderParametr*
ParabellumFramework::Graphics::Shader::GetParametr(EUINT32 index)
{
	return m_sortedPointersToParametrs[index];

	// error here means that there some error inside Shader (not correct word? letter?)
}

ParabellumFramework::Graphics::ShaderParametr*
ParabellumFramework::Graphics::Shader::GetResource(EUINT32 index)
{
	return m_sortedPointersToResources[index];
}

ParabellumFramework::Graphics::ShaderParametr**
ParabellumFramework::Graphics::Shader::GetResources()
{
	return m_sortedPointersToResources;
}

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::Shader::GetResourcesCount()
{
	return m_shaderResourcesPointersCount;
}