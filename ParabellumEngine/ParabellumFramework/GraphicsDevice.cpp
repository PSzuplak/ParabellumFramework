#include "stdafx.h"
#include "GraphicsDevice.h"


//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumFramework::Graphics::GraphicsDevice::GraphicsDevice()
{
	m_currentVertexBuffer = NULLPTR;
	m_currentIndexBuffer = NULLPTR;
	m_currentShader = NULLPTR;

	m_deviceBuffer = new EBYTE[512];
	m_deviceBufferPosition = 0;

	
	// init debug data
	profilerData.UsedGPUMemory = 0;
	profilerData.DrawCallsCount = 0;
	profilerData.RenderedVerticesCount = 0;
	profilerData.RenderedTrianglesCount = 0;
	profilerData.SetShaderCount = 0;
	profilerData.SentDataSize = 0;
	profilerData.FPS = 0;
	profilerData.FrameCounter = 0;
	profilerData.FrameTimeBegin = std::clock();
}

ParabellumFramework::Graphics::GraphicsDevice::GraphicsDevice(const GraphicsDevice& ref)
{

}

ParabellumFramework::Graphics::GraphicsDevice::~GraphicsDevice()
{
	delete[] m_deviceBuffer;
}







//	--------------------------------------------------------------------
//		Interface with a graphics devices
//	--------------------------------------------------------------------



ParabellumFramework::EUINT32
ParabellumFramework::Graphics::GraphicsDevice::Initialize(EUINT32 screenWidth, EUINT32 screenHeight, void* hwnd, EUINT32 settings)
{
	EUINT32 result;

	result = Device_Initialize(screenWidth, screenHeight, hwnd, settings);

	#ifdef DEBUG_GRAPHICSDEVICE_L1
	PFDLOG("PFGI", "GHDV", "Initialize::Device_Initialize ( " << screenWidth << ", " << screenHeight << ", " << (EADDR)hwnd << ", " << settings << " ) : " << GetCodeDesc(result) );
	#endif

	return result;
}

//
// Release - 
//
ParabellumFramework::EUINT32
ParabellumFramework::Graphics::GraphicsDevice::Release()
{
	PFDLOG("PFGI", "GHDV", "Release()");

	EUINT32 result = SystemCodes::SUCCESS;

	// release vertex buffers
	for (auto it : m_vertexBuffers)
	{
		result = Device_ReleaseVertexBuffer(it->GetAddressOfBuffer());

		#ifdef DEBUG_GRAPHICSDEVICE_L1
		PFDLOG("PFGI", "GHDV", "->Device_ReleaseVertexBuffer ( addr = " << it << ") : " << GetCodeDesc(result));
		#endif	
	}

	m_vertexBuffers.clear();

	// release index buffers
	for (auto it : m_indexBuffers)
	{
		result = Device_ReleaseIndexBuffer(it->GetBuffer());

		#ifdef DEBUG_GRAPHICSDEVICE_L1
		PFDLOG("PFGI", "GHDV", "->Device_ReleaseIndexBuffer ( addr = " << it << ") : " << GetCodeDesc(result));
		#endif
	}
	m_indexBuffers.clear();

	// release shader buffers
	for (auto it : m_shaders)
	{
		result = ReleaseShader(it);

		#ifdef DEBUG_GRAPHICSDEVICE_L1
		PFDLOG("PFGI", "GHDV", "->ReleaseShader ( addr = " << it << ") : " << GetCodeDesc(result));
		#endif
	}
	m_shaders.clear();

	// release textures 
	for (auto it : m_textures)
	{
		result = ReleaseTexture(it);

		#ifdef DEBUG_GRAPHICSDEVICE_L1
		PFDLOG("PFGI", "GHDV", "->ReleaseTexture ( addr = " << it << ") : " << GetCodeDesc(result));
		#endif
	}
	m_textures.clear();

	// release render targets
	for (auto it : m_renderTargets)
	{
		result = ReleaseRenderTarget(it);

		#ifdef DEBUG_GRAPHICSDEVICE_L1
		PFDLOG("PFGI", "GHDV", "ReleaseRenderTarget ( " << (EADDR)it << ") : " << GetCodeDesc(result));
		#endif
	}
	m_renderTargets.clear();


	////////////////////////////
	// RELEASE DEFAULT SAMPLERS
	

	result = Device_Release();

	#ifdef DEBUG_GRAPHICSDEVICE_L1
	PFDLOG("PFGI", "GHDV", "->Device_Release() : " << GetCodeDesc(result) );
	#endif

	return result;
}

//
// Update -
//
ParabellumFramework::EUINT32
ParabellumFramework::Graphics::GraphicsDevice::Update()
{
	return Device_Update();
}


void
ParabellumFramework::Graphics::GraphicsDevice::SwapBuffers()
{
	Device_SwapBuffers();

	// get debug/ info
	#ifdef DEBUG_GRAPHICSDEVICE_DEBUGDATA_ENABLED
	profilerData.FrameCounter++;

	profilerData.FrameTimePrevious = profilerData.FrameTimeCurrent;
	profilerData.FrameTimeCurrent = std::clock();

	// After 1 second calculate a FPS and clear info
	if (profilerData.FrameTimeCurrent - profilerData.FrameTimeBegin > 1000)
	{
		profilerData.FPS = profilerData.FrameCounter;
		profilerData.FrameCounter = 0;

		profilerData.FrameTimeBegin = std::clock();
	}
	#endif

	//
	// clear debug data
	#ifdef DEBUG_GRAPHICSDEVICE_DEBUGDATA_ENABLED
	profilerData.DrawCallsCount = 0;
	profilerData.RenderedVerticesCount = 0;
	profilerData.RenderedTrianglesCount = 0;
	profilerData.SetShaderCount = 0;
	profilerData.SentDataSize = 0;
	#endif
}


void
ParabellumFramework::Graphics::GraphicsDevice::Render(EUINT32 settings)
{
	//if (settings & PF_GI_RENDERING_UPDATEVARIABLES)
	SetShaderVariables();

	//if (settings & PF_GI_RENDERING_UPDATERESOURCES)
	SetShaderResources();

	RenderRAW();
}

void
ParabellumFramework::Graphics::GraphicsDevice::RenderRAW()
{
	Device_Render(m_currentIndexBuffer->m_indicesCount);

	// debug data
	#ifdef DEBUG_GRAPHICSDEVICE_DEBUGDATA_ENABLED
	profilerData.DrawCallsCount++;
	profilerData.RenderedVerticesCount += m_currentVertexBuffer->GetVerticesCount();
	profilerData.RenderedTrianglesCount += m_currentIndexBuffer->GetIndicesCount() / 3;
	#endif
}



void
ParabellumFramework::Graphics::GraphicsDevice::SetShaderVariables(EUINT32 cbufferNo)
{
	Device_SetCBuffer(m_currentShader->GetAddressOfCBuffer(cbufferNo), m_currentShader->GetCBufferBuffer(cbufferNo), m_currentShader->GetCBufferSize(cbufferNo) );

	// debug data
	#ifdef DEBUG_GRAPHICSDEVICE_DEBUGDATA_ENABLED
	profilerData.SentDataSize += m_currentShader->m_parametrsBufferSize;
	#endif
}


void
ParabellumFramework::Graphics::GraphicsDevice::SetShaderResources()
{
	Device_SetResources(m_currentShader->GetResources(), m_currentShader->GetResourcesCount());
}






ParabellumFramework::Graphics::VertexBuffer* 
ParabellumFramework::Graphics::GraphicsDevice::CreateVertexBuffer(_IN_ void* pBuffer, EUINT32 vertexSize, EUINT32 verticesCount)
{
	PFDLOG("PFGI", "GHDV", "CreateIndexBuffer( (pBuffer)," << pBuffer << "(vertexSize), " << vertexSize << "(verticesCount), " << verticesCount << ")");

	EUINT32 result = SystemCodes::SUCCESS;

	// memory allocation
	VertexBuffer* vb = new VertexBuffer(vertexSize, verticesCount);

	if (vb == NULLPTR)
	{
		#ifdef DEBUG_GRAPHICSDEVICE_L1
		PFDLOG("PFGI", "GHDV", "CreateVertexBuffer (bad alloc)");
		#endif

		return vb;
	}
	
	#ifdef DEBUG_GRAPHICSDEVICE_L1
	PFDLOG("PFGI", "GHDV", "CreateVertexBuffer ( addr = " << (EADDR)vb << ") : " << GetCodeDesc(result));
	#endif

	result = Device_CreateVertexBuffer(&vb->m_buffer, pBuffer, vertexSize, verticesCount);


	#ifdef DEBUG_GRAPHICSDEVICE_L1
	PFDLOG("PFGI", "GHDV", "CreateVertexBuffer::Device_CreateVertexBuffer ( " << &vb->m_buffer << ", " << pBuffer << ", " << vertexSize << ", " << verticesCount << " ) : " << GetCodeDesc(result));
	#endif

	if (result == SystemCodes::SUCCESS)
	{
		m_vertexBuffers.push_back(vb);
		profilerData.UsedGPUMemory += vertexSize * verticesCount;
		return vb;
	}
	else
	{
		delete vb;
		return NULLPTR;
	}


}















ParabellumFramework::EUINT32 
ParabellumFramework::Graphics::GraphicsDevice::ReleaseVertexBuffer(_IO_ VertexBuffer* pVertexBuffer)
{
	PFDLOG("PFGI", "GHDV", "ReleaseVertexBuffer( (pVertexBuffer)," << pVertexBuffer << ")");

	EUINT32 result = SystemCodes::SUCCESS;

	// remove vertex buffer from list
	m_vertexBuffers.remove(pVertexBuffer);

	// remove from graphics card
	result = Device_ReleaseVertexBuffer(&pVertexBuffer->m_buffer);

	#ifdef DEBUG_GRAPHICSDEVICE_L1
		PFDLOG("PFGI", "GHDV", "ReleaseVertexBuffer ( " << GetCodeDesc(result) << ") : addr = " << (EADDR)pVertexBuffer);
	#endif


	return result;
}



ParabellumFramework::Graphics::IndexBuffer* 
ParabellumFramework::Graphics::GraphicsDevice::CreateIndexBuffer(_IN_ void* pBuffer, EUINT64 indicesCount)
{
	PFDLOG("PFGI", "GHDV", "CreateIndexBuffer( (pBuffer),"<< pBuffer << "(indicesCount), "<< indicesCount <<")");

	EUINT32 result = SystemCodes::SUCCESS;

	// memory allocation
	IndexBuffer* ib = new IndexBuffer(indicesCount);

	if (ib == NULLPTR)
	{
		#ifdef DEBUG_GRAPHICSDEVICE_L1
		PFDLOG("PFGI", "GHDV", "CreateIndexBuffer (bad alloc)");
		#endif

		return ib;
	}

	result = Device_CreateIndexBuffer(&ib->m_buffer, pBuffer, indicesCount);

	#ifdef DEBUG_GRAPHICSDEVICE_L1
	PFDLOG("PFGI", "GHDV", "Device_CreateIndexBuffer ( " << GetCodeDesc(result) << ") : addr = " << (EADDR)ib);
	#endif

	if (result == SystemCodes::SUCCESS)
	{
		m_indexBuffers.push_back(ib);
		profilerData.UsedGPUMemory += indicesCount * sizeof(EUINT32);
		return ib;	
	}
	else
	{
		delete ib;
		return nullptr;
	}

}


//
// ReleaseIndexBuffer -
//
ParabellumFramework::EUINT32  
ParabellumFramework::Graphics::GraphicsDevice::ReleaseIndexBuffer(_IO_ IndexBuffer* pIndexBuffer)
{
	PFDLOG("PFGI", "GHDV", "ReleaseIndexBuffer( (pIndexBuffer)," << pIndexBuffer << ")");

	EUINT32 result;

	// remove vertex buffer from list
	m_indexBuffers.remove(pIndexBuffer);

	// remove from graphics card
	result = Device_ReleaseIndexBuffer(&pIndexBuffer->m_buffer);

	#ifdef DEBUG_GRAPHICSDEVICE_L1
	PFDLOG("PFGI", "GHDV", "Device_ReleaseIndexBuffer ( " << GetCodeDesc(result) << ") : addr = " << (EADDR)pIndexBuffer);
	#endif

	return result;
}




//
// SetBuffers -
//
ParabellumFramework::EUINT32 
ParabellumFramework::Graphics::GraphicsDevice::SetBuffers(_IN_ VertexBuffer* pVB, _IN_ IndexBuffer* pIndexBuffer)
{
	//EUINT32 result;
	//result = SystemCodes::SUCCESS;

	// dont set new buffers on graphisc card if already seted
	if((m_currentVertexBuffer != pVB) || (m_currentIndexBuffer != pIndexBuffer))
	{
		m_currentVertexBuffer = pVB;
		m_currentIndexBuffer = pIndexBuffer;

		Device_SetBuffers(pVB->GetAddressOfBuffer(), m_currentVertexBuffer->GetVertexSize(), pIndexBuffer->GetBuffer());
	}


	return SystemCodes::SUCCESS;
}


void 
ParabellumFramework::Graphics::GraphicsDevice::SetVertexBuffer(_IN_ VertexBuffer* pVB)
{
	if (m_currentVertexBuffer != pVB)
	{
		m_currentVertexBuffer = pVB;
		Device_SetVertexBuffer(pVB->GetAddressOfBuffer(), m_currentVertexBuffer->GetVertexSize());
	}
	
}



void 
ParabellumFramework::Graphics::GraphicsDevice::SetIndexBuffer(_IN_ IndexBuffer* pIB)
{
	if (m_currentIndexBuffer != pIB)
	{
		m_currentIndexBuffer = pIB;
		Device_SetIndexBuffer(pIB->GetBuffer());
	}
}

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::GraphicsDevice::InitializeShader(_IO_ Shader* pShader)
{
	PFDLOG("PFGI", "GHDV", "GraphicsDevice::InitializeShader ( " << (EADDR)pShader << ")" );
	PFDLOG("PFGI", "GHDV", "GetName() : " << pShader->GetName() << ")");

	EUINT32 result = SystemCodes::SUCCESS;

	if (pShader->IsCreated() == false)
	{
		result = Device_LoadVertexShader(pShader->GetVertexBuffer(), pShader->GetShaderFilename().c_str(), pShader->GetVSFunctionName().c_str(),
			pShader->GetLayout(), pShader->GetLayoutDataCount(), pShader->GetInputLayout());

		if (result != SystemCodes::SUCCESS)
		{
			#ifdef DEBUG_GRAPHICSDEVICE_L1
			PFDLOG("PFGI", "GHDV", "SetShader ( " << (EADDR)pShader << ") : " << GetCodeDesc(result) << " - in Device_LoadVertexShader");
			#endif

			return result;
		}

		result = Device_LoadPixelShader(pShader->GetPixelBuffer(), pShader->GetShaderFilename().c_str(), pShader->GetPSFunctionName().c_str());

		if (result != SystemCodes::SUCCESS)
		{
			#ifdef DEBUG_GRAPHICSDEVICE_L1
			PFDLOG("PFGI", "GHDV", "SetShader ( " << (EADDR)pShader << ") : " << GetCodeDesc(result) << " - in Device_LoadPixelShader");
			#endif

			return result;
		}

		// Create CBuffers
		for (EUINT32 i = 0; i < pShader->GetCBuffersCount(); ++i)
		{
			PFDLOG("PFGI", "GHDV", "Device_CreateCBuffer("<< pShader->GetCBuffer(i)->GetAddressOfBuffer() << ", " << pShader->GetCBuffer(i)->GetSize() << ")");

			result = Device_CreateCBuffer(pShader->GetCBuffer(i)->GetAddressOfBuffer(), pShader->GetCBuffer(i)->GetSize());

			if (result != SystemCodes::SUCCESS)
			{
				#ifdef DEBUG_GRAPHICSDEVICE_L1
				PFDLOG("PFGI", "GHDV", "SetShader ( " << (EADDR)pShader << ") : " << GetCodeDesc(result) << " - in Device_CreateCBuffer no=" << i);
				#endif

				return result;
			}
		}

		// initialize CBuffers, Resources and parametrs adn samplers
		pShader->InitializeData();


		// create samplers
		for (EUINT32 i = 0; i < pShader->m_samplersCount; ++i)
		{
			result = CreateSampler(pShader->m_samplerStates[i]);

			if (result != SystemCodes::SUCCESS)
			{
				#ifdef DEBUG_GRAPHICSDEVICE_L1
				PFDLOG("PFGI", "GHDV", "SetShader ( " << (EADDR)pShader << ") : " << GetCodeDesc(result) << " - in CreateSampler no=" << i);
				#endif

				return result;
			}
		}


		// mark as created
		pShader->Create();

		// add to list
		m_shaders.push_back(pShader);

	}
	// end loading shader



	return SystemCodes::SUCCESS;
}



ParabellumFramework::EUINT32
ParabellumFramework::Graphics::GraphicsDevice::ReleaseShader(_IO_ Shader* pShader)
{
	PFDLOG("PFGI", "GHDV", "GraphicsDevice::ReleaseShader ( " << (EADDR)pShader << ")");

	EUINT32 result = SystemCodes::SUCCESS;

	if (pShader->IsCreated() == false)
		return SystemCodes::ERROR_NOT_EXIST;

	// remove shader from graphics memory
	result = Device_ReleaseVertexShader(pShader->GetVertexBuffer());
	PFDLOG("PFGI", "GHDV", "[" << (EADDR)pShader << "] -> ReleaseShader::Device_ReleaseVertexShader () : " << GetCodeDesc(result));

	result = Device_ReleasePixelShader(pShader->GetVertexBuffer());
	PFDLOG("PFGI", "GHDV", "[" << (EADDR)pShader << "] -> ReleaseShader::Device_ReleasePixelShader () : " << GetCodeDesc(result));

	// relase cbuffers
	for (EUINT32 i = 0; i < pShader->GetCBuffersCount(); ++i)
	{
		result = Device_ReleaseCBuffer(pShader->GetCBuffer(i)->GetAddressOfBuffer());
		PFDLOG("PFGI", "GHDV", "[" << (EADDR)pShader << "] -> ReleaseShader::Device_ReleaseCBuffer () : " << GetCodeDesc(result));
	}

	// release samplers
	for (EUINT32 i = 0; i < pShader->m_samplersCount; ++i)
	{
		result = Device_ReleaseSampler(pShader->m_samplerStates[i]->GetAddressOfBuffer());
		PFDLOG("PFGI", "GHDV", "[" << (EADDR)pShader << "] -> ReleaseShader::Device_ReleaseSampler () : " << GetCodeDesc(result));
	}

	#ifdef DEBUG_GRAPHICSDEVICE_L1
	PFDLOG("PFGI", "GHDV", "[" << (EADDR)pShader << "] -> Release() : " << GetCodeDesc(result));
	#endif

	return result;
}








//
//
//
ParabellumFramework::EUINT32
ParabellumFramework::Graphics::GraphicsDevice::SetShader(_IO_ Shader* pShader, ShaderFlagSet renderSettings)
{
	EUINT32 result = SystemCodes::SUCCESS;

	// some bug... shader must be initialized here
	if ( (pShader->IsCreated() == false) || (pShader->IsInitialized() == false) )
	{
		PFDLOG("PFGI", "GHDV", "[WARNING] SetShader ( " << (EADDR)pShader << ") : Initializing shader at runtime");
		result = InitializeShader(pShader);
	}


	//
	// set shader on graphics device if it wasnt before
	//
	if (m_currentShader != pShader)
	{
		#ifdef DEBUG_GRAPHICSDEVICE_DEBUGDATA_ENABLED
		profilerData.SetShaderCount++;
		#endif

		m_currentShader = pShader;

		result = Device_SetInputLayout(pShader->GetInputLayout());
			
		#ifdef DEBUG_GRAPHICSDEVICE_L1
		if (result != SystemCodes::SUCCESS)
		{
			#ifdef DEBUG_GRAPHICSDEVICE_L1
			PFDLOG("PFGI", "GHDV", "SetShader ( " << (EADDR)pShader << ") : " << GetCodeDesc(result) << " - in Device_SetInputLayout");
			#endif

			return result;
		}
		#endif

		// set vertex shader
		result = Device_SetVertexShader(pShader->GetVertexBuffer());

		#ifdef DEBUG_GRAPHICSDEVICE_L1
		if (result != SystemCodes::SUCCESS)
		{
			#ifdef DEBUG_GRAPHICSDEVICE_L1
			PFDLOG("PFGI", "GHDV", "SetShader ( " << (EADDR)pShader << ") : " << GetCodeDesc(result) << " - in Device_SetVertexShader");
			#endif
			
			return result;
		}
		#endif

		// set pixel shader
		result = Device_SetPixelShader(pShader->GetPixelBuffer());

		#ifdef DEBUG_GRAPHICSDEVICE_L1
		if (result != SystemCodes::SUCCESS)
		{
			#ifdef DEBUG_GRAPHICSDEVICE_L1
			PFDLOG("PFGI", "GHDV", "SetShader ( " << (EADDR)pShader << ") : " << GetCodeDesc(result) << " - in Device_SetPixelShader");
			#endif

			return result;
		}
		#endif

		//
		// set samplers
		if (renderSettings != ShaderFlagSet::NO_SAMPLERS)
		{
			for (EUINT32 i = 0; i < pShader->m_samplersCount; ++i)
			{
				result = Device_SetSampler(pShader->m_samplerStates[i], i);

				if (result != SystemCodes::SUCCESS)
				{
					#ifdef DEBUG_GRAPHICSDEVICE_L1
					PFDLOG("PFGI", "GHDV", "SetShader ( " << (EADDR)pShader << ") : " << GetCodeDesc(result) << " - in Device_SetSampler no=" << i);
					#endif

					return result;
				}
			}
		}
		

	}


	return SystemCodes::SUCCESS;
}





ParabellumFramework::EUINT32
ParabellumFramework::Graphics::GraphicsDevice::CreateTexture(_IO_ Texture2D* pTexture)
{
	PFDLOG("PFGI", "GHDV", "CreateTexture ( " << (EADDR)pTexture << ")");

	EUINT32 result = SystemCodes::SUCCESS;

	if (pTexture->GetRAWData() == NULLPTR)
	{
		EUINT32 result = SystemCodes::ERROR_DANGLINGPTR;

		#ifdef DEBUG_GRAPHICSDEVICE_L1
		PFDLOG("PFGI", "GHDV", "CreateTexture ( " << (EADDR)pTexture << ") -> Device_CreateTexture ( [RAWData]" << pTexture->GetWidth() << ", " << pTexture->GetHeight() << ", " << pTexture->GetFormat() << ", " << pTexture->GetAddressOfBuffer() << ", [bufferType]) :" << GetCodeDesc(result));
		#endif

		#ifdef DEBUG_GRAPHICSDEVICE_L1
		PFDLOG("PFGI", "GHDV", "GetName() :" << pTexture->GetName());
		PFDLOG("PFGI", "GHDV", "GetBuffer() :" << pTexture->GetAddressOfBuffer());
		#endif

		return result;
	}

	result = Device_CreateTexture(pTexture->GetRAWData(), pTexture->GetWidth(), pTexture->GetHeight(), pTexture->GetFormat(), pTexture->GetAddressOfBuffer(), pTexture->GetAddressOBufferType());

	#ifdef DEBUG_GRAPHICSDEVICE_DEBUGDATA_ENABLED
	profilerData.UsedGPUMemory += pTexture->GetWidth() * pTexture->GetHeight() * 4;
	#endif
	
	if (result == SystemCodes::SUCCESS)
	{
		m_textures.push_back(pTexture);
	}

	return result;
}



//
// Release Texture2D
//
ParabellumFramework::EUINT32
ParabellumFramework::Graphics::GraphicsDevice::ReleaseTexture(_IO_ Texture2D* pTexture)
{
	EUINT32 result = SystemCodes::SUCCESS;

	result = Device_ReleaseTexture(pTexture->GetAddressOfBuffer(), pTexture->GetAddressOBufferType());

	#ifdef DEBUG_GRAPHICSDEVICE_L1
	PFDLOG("PFGI", "GHDV", "ReleaseTexture ( " << (EADDR)pTexture << ") -> Device_ReleaseTexture( " << pTexture->GetAddressOfBuffer() << ", " << pTexture->GetAddressOBufferType() << " ) : " << GetCodeDesc(result) );
	#endif

	return result;
}



ParabellumFramework::EUINT32
ParabellumFramework::Graphics::GraphicsDevice::CreateSampler(_IN_ ShaderSamplerState* pSampler)
{
	#ifdef DEBUG_GRAPHICSDEVICE_L1
	PFDLOG("PFGI", "GHDV", "GraphicsDevice::CreateSampler( " << (EADDR)pSampler << ") ");
	#endif

	EUINT32 result = SystemCodes::SUCCESS;

	result = Device_CreateSampler(
		pSampler->GetAddressOfBuffer(),
		pSampler->m_filter,
		pSampler->m_addressU,
		pSampler->m_addressV,
		pSampler->m_addressW,
		pSampler->m_mipLODBias,
		pSampler->m_maxAnisotropy,
		pSampler->m_comparisonFunc);

	#ifdef DEBUG_GRAPHICSDEVICE_L1
	PFDLOG("PFGI", "GHDV", "GraphicsDevice::CreateSampler( " << (EADDR)pSampler << ") : " << GetCodeDesc(result) );
	#endif

	return result;
}


ParabellumFramework::EUINT32
ParabellumFramework::Graphics::GraphicsDevice::SetSampler(EUINT32 regNo, SamplerType type)
{
	Device_SamplerSetDefault(regNo, type);
	return SystemCodes::SUCCESS;
}

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::GraphicsDevice::SetSampler(EUINT32 regNo)
{
	return SystemCodes::ERROR_NOT_IMPLEMENTED;
}


ParabellumFramework::Graphics::RenderTarget* 
ParabellumFramework::Graphics::GraphicsDevice::CreateRenderTarget(
	GraphicsFormat RTFormat, GraphicsFormat DepthFormat, EUINT32 width, EUINT32 height)
{
	PFDLOG("PFGI", "GHDV", "GraphicsDevice::CreateRenderTarget(" << RTFormat<< ", " << DepthFormat << ", " << width << ", " << height << ")" );

	EUINT32 result;

	RenderTarget* rt = new RenderTarget();

	if (rt == NULLPTR)
	{
		#ifdef DEBUG_GRAPHICSDEVICE_L1
		PFDLOG("PFGI", "GHDV", "CreateRenderTarget () : bad alloc");
		#endif

		return NULLPTR;
	}
	
	rt->m_width = width;
	rt->m_height = height;
	rt->m_rtFormat = RTFormat;
	rt->m_depthFormat = DepthFormat;
	
	rt->m_viewportBuffer[0] = 0.0f;
	rt->m_viewportBuffer[1] = 0.0f;
	rt->m_viewportBuffer[2] = (float)width;
	rt->m_viewportBuffer[3] = (float)height;
	rt->m_viewportBuffer[4] = 0.0f;
	rt->m_viewportBuffer[5] = 1.0f;

	result = Device_CreateRenderTarget(
		&rt->m_rtBufferTexture, &rt->m_rtBufferFormat, &rt->m_rtBufferResource,
		RTFormat, width, height);

	#ifdef DEBUG_GRAPHICSDEVICE_L1
	PFDLOG("PFGI", "GHDV", "CreateRenderTarget( " << (EADDR)rt << " ) -> Device_CreateRenderTarget( " << RTFormat << "," << DepthFormat << "," << width << "," << height << "," << ") : " << GetCodeDesc(result) );
	#endif

	#ifdef DEBUG_GRAPHICSDEVICE_DEBUGDATA_ENABLED
	profilerData.UsedGPUMemory += width * height * 4;
	#endif

	if (result != SystemCodes::SUCCESS)
	{
		PFDLOG("PFGI", "GHDV", "Device_CreateRenderTarget FAILED");
		delete rt;
		return NULLPTR;
	}


	result = Device_CreateDepthBuffer(
		&rt->m_depthBuffer, &rt->m_depthBufferFormat,
		DepthFormat, width, height);

	#ifdef DEBUG_GRAPHICSDEVICE_L1
	PFDLOG("PFGI", "GHDV", "CreateRenderTarget( " << (EADDR)rt << " ) -> Device_CreateDepthBuffer( " << RTFormat << "," << DepthFormat << "," << width << "," << height << "," << ") : " << (EADDR)rt << " : " << GetCodeDesc(result) << " - in Device_CreateDepthBuffer");
	#endif

	#ifdef DEBUG_GRAPHICSDEVICE_DEBUGDATA_ENABLED
	profilerData.UsedGPUMemory += width * height * 4;
	#endif

	if (result != SystemCodes::SUCCESS)
	{
		PFDLOG("PFGI", "GHDV", "Device_CreateDepthBuffer FAILED");
		delete rt;
		return NULLPTR;
	}

	// put TenderTarget in list
	m_renderTargets.push_back(rt);


	return rt;
}

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::GraphicsDevice::ReleaseRenderTarget(_IN_ RenderTarget* pRT)
{
	PFDLOG("PFGI", "GHDV", "GraphicsDevice::ReleaseRenderTarget ( " << (EADDR)pRT << ")");

	EUINT32 result = SystemCodes::SUCCESS;

	result = Device_ReleaseRenderTarget(&pRT->m_rtBufferTexture, &pRT->m_rtBufferFormat, &pRT->m_rtBufferResource);


	return result;
}


// Set Render Target. Also set DepthBuffer assocated in RenderTarget object
ParabellumFramework::EUINT32 
ParabellumFramework::Graphics::GraphicsDevice::SetRenderTarget(EUINT32 No, _IN_ RenderTarget* RT)
{
	EUINT32 result = SystemCodes::SUCCESS;

	EFLOAT32 depthValue = 1.0f;

	if ((m_currentStencilState & PF_GI_STENCIL_GREATER) != 0)
		depthValue = 0.0f;

	result = Device_SetRenderTarget(No, RT->m_viewportBuffer, &RT->m_rtBufferFormat, RT->m_depthBufferFormat, depthValue);

	if (result == SystemCodes::SUCCESS)
	{
		m_currentRenderTargets[No] = RT;
	}
	#ifdef DEBUG_GRAPHICSDEVICE_L1
	else
	{	
		PFDLOG("PFGI", "GDAI", "SetRenderTarget(" << (EADDR)RT << ") = " << GetCodeDesc(result) << " in Device_SetRenderTarget");
	}
	#endif

	return result;
}

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::GraphicsDevice::SetRenderTargets(
	_OUT_ RenderTarget* RT0,
	_OUT_ RenderTarget* RTDepth, EBOOL clearDepth)
{
	EUINT32 result = SystemCodes::SUCCESS;

	m_currentRenderTargetsCount = 1;
	m_currentRenderTargets[0] = RT0;


	void* renderTargetsFormat[1];
	void* depthBuffersFormat[1];

	renderTargetsFormat[0] = RT0->m_rtBufferFormat;

	depthBuffersFormat[0] = RT0->m_depthBufferFormat;

	EFLOAT32 depthValue = 1.0f;

	if ((m_currentStencilState & PF_GI_STENCIL_GREATER) != 0)
		depthValue = 0.0f;

	if (RTDepth != NULLPTR)
	{
		result = Device_SetRenderTargets(
			m_currentRenderTargetsCount, RTDepth->m_viewportBuffer,
			renderTargetsFormat, depthBuffersFormat, RTDepth->m_depthBufferFormat, clearDepth, depthValue);
	}
	else
	{
		result = Device_SetRenderTargets(
			m_currentRenderTargetsCount, RTDepth->m_viewportBuffer,
			renderTargetsFormat, depthBuffersFormat, NULLPTR, clearDepth, depthValue);
	}

	#ifdef DEBUG_GRAPHICSDEVICE_L1
	if(result != SystemCodes::SUCCESS)
		PFDLOG("PFGI", "GDAI", "SetRenderTargets(" << (EADDR)RT0 << "," << (EADDR)RTDepth << ") = " << GetCodeDesc(result) << " in Device_SetRenderTargets");
	#endif

	#ifdef DEBUG_GRAPHICSDEVICE_L3
	if (result != SystemCodes::SUCCESS)
		PFDLOG("PFGI", "GDAI", "SetRenderTargets(" << (EADDR)RT0 << "," << (EADDR)RTDepth << ") = " << GetCodeDesc(result) << " in Device_SetRenderTargets");
	#endif

	return result;

}

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::GraphicsDevice::SetRenderTargets(
	_OUT_ RenderTarget* RT0, _OUT_ RenderTarget* RT1,
	_OUT_ RenderTarget* RTDepth, EBOOL clearDepth)
{
	EUINT32 result = SystemCodes::SUCCESS;

	m_currentRenderTargetsCount = 2;
	m_currentRenderTargets[0] = RT0;
	m_currentRenderTargets[1] = RT1;

	void* renderTargetsFormat[2];
	void* depthBuffersFormat[2];

	renderTargetsFormat[0] = RT0->m_rtBufferFormat;
	renderTargetsFormat[1] = RT1->m_rtBufferFormat;

	depthBuffersFormat[0] = RT0->m_depthBufferFormat;
	depthBuffersFormat[1] = RT1->m_depthBufferFormat;

	EFLOAT32 depthValue = 1.0f;

	if ((m_currentStencilState & PF_GI_STENCIL_GREATER) != 0)
		depthValue = 0.0f;

	if (RTDepth != NULLPTR)
	{
		result = Device_SetRenderTargets(
			m_currentRenderTargetsCount, RTDepth->m_viewportBuffer,
			renderTargetsFormat, depthBuffersFormat, RTDepth->m_depthBufferFormat, clearDepth, depthValue);
	}
	else
	{
		result = Device_SetRenderTargets(
			m_currentRenderTargetsCount, RTDepth->m_viewportBuffer,
			renderTargetsFormat, depthBuffersFormat, NULLPTR, clearDepth, depthValue);
	}


	#ifdef DEBUG_GRAPHICSDEVICE_L1
	if(result != SystemCodes::SUCCESS)
		PFDLOG("PFGI", "GDAI", "SetRenderTargets(" << (EADDR)RT0 << "," << (EADDR)RT1 << "," << (EADDR)RTDepth << ") = " << GetCodeDesc(result) << " in Device_SetRenderTargets");
	#endif

	#ifdef DEBUG_GRAPHICSDEVICE_L3
	if (result != SystemCodes::SUCCESS)
		PFDLOG("PFGI", "GDAI", "SetRenderTargets(" << (EADDR)RT0 << "," << (EADDR)RT1 << "," << (EADDR)RTDepth << ") = " << GetCodeDesc(result) << " in Device_SetRenderTargets");
	#endif

	return result;

}

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::GraphicsDevice::SetRenderTargets(
	_OUT_ RenderTarget* RT0, _OUT_ RenderTarget* RT1, _OUT_ RenderTarget* RT2,
	_OUT_ RenderTarget* RTDepth, EBOOL clearDepth)
{
	EUINT32 result = SystemCodes::SUCCESS;

	m_currentRenderTargetsCount = 3;
	m_currentRenderTargets[0] = RT0;
	m_currentRenderTargets[1] = RT1;
	m_currentRenderTargets[2] = RT2;

	void* renderTargetsFormat[3];
	void* depthBuffersFormat[3];

	renderTargetsFormat[0] = RT0->m_rtBufferFormat;
	renderTargetsFormat[1] = RT1->m_rtBufferFormat;
	renderTargetsFormat[2] = RT2->m_rtBufferFormat;

	depthBuffersFormat[0] = RT0->m_depthBufferFormat;
	depthBuffersFormat[1] = RT1->m_depthBufferFormat;
	depthBuffersFormat[2] = RT2->m_depthBufferFormat;

	EFLOAT32 depthValue = 1.0f;

	if ((m_currentStencilState & PF_GI_STENCIL_GREATER) != 0)
		depthValue = 0.0f;

	if (RTDepth != NULLPTR)
	{
		result = Device_SetRenderTargets(
			m_currentRenderTargetsCount, RTDepth->m_viewportBuffer,
			renderTargetsFormat, depthBuffersFormat, RTDepth->m_depthBufferFormat, clearDepth, depthValue);
	}
	else
	{
		result = Device_SetRenderTargets(
			m_currentRenderTargetsCount, RTDepth->m_viewportBuffer,
			renderTargetsFormat, depthBuffersFormat, NULLPTR, clearDepth, depthValue);
	}

	#ifdef DEBUG_GRAPHICSDEVICE_L1
	if (result != SystemCodes::SUCCESS)
		PFDLOG("PFGI", "GDAI", "SetRenderTargets(" << (EADDR)RT0 << "," << (EADDR)RT1 << "," << (EADDR)RT2 << "," << (EADDR)RTDepth << ") = " << GetCodeDesc(result) << " in Device_SetRenderTargets");
	#endif

	#ifdef DEBUG_GRAPHICSDEVICE_L3
	PFDLOG("PFGI", "GDAI", "SetRenderTargets(" << (EADDR)RT0 << "," << (EADDR)RT1 << "," << (EADDR)RT2 << "," << (EADDR)RTDepth << ") = " << GetCodeDesc(result) << " in Device_SetRenderTargets");
	#endif

	return result;

}

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::GraphicsDevice::SetRenderTargets(
	_OUT_ RenderTarget* RT0, _OUT_ RenderTarget* RT1, _OUT_ RenderTarget* RT2, _OUT_ RenderTarget* RT3,
	_OUT_ RenderTarget* RTDepth, EBOOL clearDepth)
{
	EUINT32 result = SystemCodes::SUCCESS;

	m_currentRenderTargetsCount = 4;
	m_currentRenderTargets[0] = RT0;
	m_currentRenderTargets[1] = RT1;
	m_currentRenderTargets[2] = RT2;
	m_currentRenderTargets[3] = RT3;

	void* renderTargetsFormat[4];
	void* depthBuffersFormat[4];

	renderTargetsFormat[0] = RT0->m_rtBufferFormat;
	renderTargetsFormat[1] = RT1->m_rtBufferFormat;
	renderTargetsFormat[2] = RT2->m_rtBufferFormat;
	renderTargetsFormat[3] = RT3->m_rtBufferFormat;

	depthBuffersFormat[0] = RT0->m_depthBufferFormat;
	depthBuffersFormat[1] = RT1->m_depthBufferFormat;
	depthBuffersFormat[2] = RT2->m_depthBufferFormat;
	depthBuffersFormat[3] = RT3->m_depthBufferFormat;

	EFLOAT32 depthValue = 1.0f;

	if ((m_currentStencilState & PF_GI_STENCIL_GREATER) != 0)
		depthValue = 0.0f;

	if (RTDepth != NULLPTR)
	{
		result = Device_SetRenderTargets(
			m_currentRenderTargetsCount, RTDepth->m_viewportBuffer,
			renderTargetsFormat, depthBuffersFormat, RTDepth->m_depthBufferFormat, clearDepth, depthValue);
	}
	else
	{
		result = Device_SetRenderTargets(
			m_currentRenderTargetsCount, RTDepth->m_viewportBuffer,
			renderTargetsFormat, depthBuffersFormat, NULLPTR, clearDepth, depthValue);
	}

	#ifdef DEBUG_GRAPHICSDEVICE_L1
	if(result != SystemCodes::SUCCESS)
		PFDLOG("PFGI", "GDAI", "SetRenderTargets(" << (EADDR)RT0 << "," << (EADDR)RT1 << "," << (EADDR)RT2 << "," << (EADDR)RT3 << "," << (EADDR)RTDepth << ") = " << GetCodeDesc(result) << " in Device_SetRenderTargets");
	#endif

	#ifdef DEBUG_GRAPHICSDEVICE_L3
	PFDLOG("PFGI", "GDAI", "SetRenderTargets(" << (EADDR)RT0 << "," << (EADDR)RT1 << "," << (EADDR)RT2 << "," << (EADDR)RT3 << "," << (EADDR)RTDepth << ") = " << GetCodeDesc(result) << " in Device_SetRenderTargets");
	#endif

	return result;

}



// ---------------------------------------------------------------------
//Rasterizer functions
// ---------------------------------------------------------------------

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::GraphicsDevice::TurnDepthBuffer(bool flag)
{
	Device_TurnDepthBuffer(flag);
	return SystemCodes::SUCCESS;
}

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::GraphicsDevice::TurnAlphaBlending(bool flag)
{
	Device_TurnAlphaBlending(flag);
	return SystemCodes::SUCCESS;
}


ParabellumFramework::EUINT32
ParabellumFramework::Graphics::GraphicsDevice::RasterizerSetMode(EUINT32 flag)
{
	m_currentRasterizerState = flag;

	Device_RasterizerSetMode(flag);
	return SystemCodes::SUCCESS;
}


// Set Stencil State
ParabellumFramework::EUINT32
ParabellumFramework::Graphics::GraphicsDevice::StencilSetState(EUINT32 flag)
{
	m_currentStencilState = flag;

	Device_StencilSetState(flag);

	return SystemCodes::SUCCESS;
}


// set blending mode
ParabellumFramework::EUINT32 
ParabellumFramework::Graphics::GraphicsDevice::BlendSetState(EUINT32 flag)
{
	m_currentBlendState = flag;

	Device_BlendSetState(flag);

	return SystemCodes::SUCCESS;
}


void 
ParabellumFramework::Graphics::GraphicsDevice::GetResourceData(
	_IN_ RenderTarget* pResource, _IN_ Vector2D& uv, _OUT_ Vector4D* pOut)
{
	GetResourceData1x1(
		(EUINT32)(uv.X * (EFLOAT32)pResource->GetWidth() ) ,
		(EUINT32)(uv.Y * (EFLOAT32)pResource->GetHeight() ),
		pResource,
		pOut	
		);
}


// ======================================================================================================





//	--------------------------------------------------------------------
//		Gets and Sets
//	--------------------------------------------------------------------



void* 
ParabellumFramework::Graphics::GraphicsDevice::GetDevice()
{
	return &m_device;
}


void* 
ParabellumFramework::Graphics::GraphicsDevice::GetDeviceContext()
{
	return &m_deviceContext;
}


void* 
ParabellumFramework::Graphics::GraphicsDevice::GetDeviceInstance()
{
	return &m_deviceInstance;
}

// 
void* 
ParabellumFramework::Graphics::GraphicsDevice::GetDeviceInterface()
{
	return m_deviceInterface;
}


void* 
ParabellumFramework::Graphics::GraphicsDevice::GetDeviceState()
{
	return m_deviceState;
}



ParabellumFramework::EUINT32
ParabellumFramework::Graphics::GraphicsDevice::GetFrameCounter()
{
	return profilerData.FrameCounter;
}


//	--------------------------------------------------------------------
//		ProfilerUnit
//	--------------------------------------------------------------------

//ParabellumFramework::ProfileGraphicsDevice*
//ParabellumFramework::Graphics::GraphicsDevice::GetProfileTrack()
//{
//	return &Data;
//}