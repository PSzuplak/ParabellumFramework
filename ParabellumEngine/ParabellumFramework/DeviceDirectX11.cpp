#include "stdafx.h"
#include "DeviceDirectX11.h"

//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumFramework::Graphics::DeviceDirectX11::DeviceDirectX11()
{
	m_swapChain = 0;
	m_renderTargetView = 0;
	m_depthStencilBuffer = 0;
	m_stencil_default = 0;
	m_depthStencilView = 0;
	m_rasterState = 0;
}


ParabellumFramework::Graphics::DeviceDirectX11::~DeviceDirectX11()
{
	Device_Release();
}


//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------

void
ParabellumFramework::Graphics::DeviceDirectX11::Device_SwapBuffers()
{
	if (m_vsync_enabled)
		m_swapChain->Present(1, 0);
	else
		m_swapChain->Present(0, 0);
}







ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_Initialize(int screenWidth, int screenHeight, void* hwnd, EUINT32 settings)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator;
	DXGI_MODE_DESC* displayModeList;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_TEXTURE2D_DESC textureDecription;
	D3D11_BLEND_DESC blendStateDescription;
	


	// set vsync
	m_vsync_enabled = settings & PF_GO_VSYNC;

	// create dx factory
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
		return false;


	// create the adapter and setup a display which matches to FORMAT_R8G8B8A8
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		SystemCodes::ERROR_INIT;
	}

	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		SystemCodes::ERROR_INIT;
	}
	
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		SystemCodes::ERROR_INIT;
	}

	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		SystemCodes::ERROR_INIT;
	}


	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		SystemCodes::ERROR_INIT;
	}


	// get refresh rate of the monitor
	for (i = 0; i<numModes; i++)
	{
		if ((displayModeList[i].Width == (unsigned int)screenWidth) && (displayModeList[i].Height == (unsigned int)screenHeight))
		{
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
		}
	}

	// free memory
	delete[] displayModeList;
	adapterOutput->Release();
	adapter->Release();
	factory->Release();

	//
	// setup a swap chain
	//
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;

	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (m_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = (HWND)hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	if (settings & PF_GO_FULLSCREEN)
		swapChainDesc.Windowed = false;
	else
		swapChainDesc.Windowed = true;

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	// set featureLevel to dx11.1
	featureLevel = D3D_FEATURE_LEVEL_11_1;


	// create a directx interfaces and swap chain
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, (ID3D11Device**)GetDevice(), NULL, (ID3D11DeviceContext**)GetDeviceContext());
	if (FAILED(result))
	{
		return false;
	}


	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	// create DEFAULT render target
	result = ((*(ID3D11Device**)GetDevice()))->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	// free memory for back buffer
	backBufferPtr->Release();
	backBufferPtr = 0;


	// depth buffer
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = ((*(ID3D11Device**)GetDevice()))->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//
	// depth stencil state
	//
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = ((*(ID3D11Device**)GetDevice()))->CreateDepthStencilState(&depthStencilDesc, &m_stencil_default);
	if (FAILED(result))
	{
		return false;
	}

	(*(ID3D11DeviceContext**)GetDeviceContext())->OMSetDepthStencilState(m_stencil_default, 1);


	//
	// Comparsion GREATER
	//
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER;


	result = ((*(ID3D11Device**)GetDevice()))->CreateDepthStencilState(&depthStencilDesc, &m_stencil_compGreater);
	if (FAILED(result))
	{
		return false;
	}
	





	//
	// setup depth stencil view
	//
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = ((*(ID3D11Device**)GetDevice()))->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	// set render target as default
	(*(ID3D11DeviceContext**)GetDeviceContext())->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);


	//	-----------------------------------------------------------------------------------
	//	Common rasterizer states
	//	-----------------------------------------------------------------------------------


	// rasterizer
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	result = ((*(ID3D11Device**)GetDevice()))->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
	{
		return false;
	}

	// set this rasterizer state as default
	(*(ID3D11DeviceContext**)GetDeviceContext())->RSSetState(m_rasterState);





	// rasterizer wireframe
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.ScissorEnable = false;
	result = ((*(ID3D11Device**)GetDevice()))->CreateRasterizerState(&rasterDesc, &m_rasterWireframe);

	// rasterizer cull none
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.ScissorEnable = false;
	result = ((*(ID3D11Device**)GetDevice()))->CreateRasterizerState(&rasterDesc, &m_rasterCullNone);

	// rasterizer clockwise
	rasterDesc.CullMode = D3D11_CULL_FRONT;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.ScissorEnable = false;
	result = ((*(ID3D11Device**)GetDevice()))->CreateRasterizerState(&rasterDesc, &m_rasterCullClockwise);

	// rasterizer counter clockwise
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.ScissorEnable = false;
	result = ((*(ID3D11Device**)GetDevice()))->CreateRasterizerState(&rasterDesc, &m_rasterCullCounterClockwise);


	// rasterizer Cull None - Wireframe
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.ScissorEnable = false;
	result = ((*(ID3D11Device**)GetDevice()))->CreateRasterizerState(&rasterDesc, &m_rasterCullNoneWireframe);

	// rasterizer Cull None - Solid
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.ScissorEnable = false;
	result = ((*(ID3D11Device**)GetDevice()))->CreateRasterizerState(&rasterDesc, &m_rasterCullNoneSolid);

	// rasterizer Cull Clockwise - Wireframe
	rasterDesc.CullMode = D3D11_CULL_FRONT;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.ScissorEnable = false;
	result = ((*(ID3D11Device**)GetDevice()))->CreateRasterizerState(&rasterDesc, &m_rasterCullClockwiseWireframe);

	// rasterizer Cull Clockwise - Solid
	rasterDesc.CullMode = D3D11_CULL_FRONT;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.ScissorEnable = false;
	result = ((*(ID3D11Device**)GetDevice()))->CreateRasterizerState(&rasterDesc, &m_rasterCullClockwiseSolid);

	// rasterizer Cull CounterClockwise - Wireframe
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.ScissorEnable = false;
	result = ((*(ID3D11Device**)GetDevice()))->CreateRasterizerState(&rasterDesc, &m_rasterCullCounterClockwiseWireframe);


	// rasterizer Cull CounterClockwise - Solid
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.ScissorEnable = false;
	result = ((*(ID3D11Device**)GetDevice()))->CreateRasterizerState(&rasterDesc, &m_rasterCullCounterClockwiseSolid);


	////////////
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.ScissorEnable = true;
	result = ((*(ID3D11Device**)GetDevice()))->CreateRasterizerState(&rasterDesc, &m_rasterCullNoneWireframeScissors);

	////////////
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.ScissorEnable = true;
	result = ((*(ID3D11Device**)GetDevice()))->CreateRasterizerState(&rasterDesc, &m_rasterCullNoneSolidScissors);

	////////////
	rasterDesc.CullMode = D3D11_CULL_FRONT;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.ScissorEnable = true;
	result = ((*(ID3D11Device**)GetDevice()))->CreateRasterizerState(&rasterDesc, &m_rasterCullClockwiseWireframeScissors);

	////////////
	rasterDesc.CullMode = D3D11_CULL_FRONT;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.ScissorEnable = true;
	result = ((*(ID3D11Device**)GetDevice()))->CreateRasterizerState(&rasterDesc, &m_rasterCullClockwiseSolidScissors);

	////////////
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.ScissorEnable = true;
	result = ((*(ID3D11Device**)GetDevice()))->CreateRasterizerState(&rasterDesc, &m_rasterCullCounterClockwiseWireframeScissors);

	////////////
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.ScissorEnable = true;
	result = ((*(ID3D11Device**)GetDevice()))->CreateRasterizerState(&rasterDesc, &m_rasterCullCounterClockwiseSolidScissors);



	//	-----------------------------------------------------------------------------------
	//	Blend states
	//	-----------------------------------------------------------------------------------


	// aplha blending enabled
	for (EUINT32 i = 0; i < PF_GI_RENDERTARGETSCOUNT; ++i)
	{
		blendStateDescription.RenderTarget[i].BlendEnable = TRUE;
		blendStateDescription.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendStateDescription.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDescription.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendStateDescription.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendStateDescription.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		blendStateDescription.AlphaToCoverageEnable = false;
	}

	result = ((*(ID3D11Device**)GetDevice()))->CreateBlendState(&blendStateDescription, &m_alphaBlendEnabled);

	if (FAILED(result))
	{
		return false;
	}

	// alpha blending disabled
	for (EUINT32 i = 0; i < PF_GI_RENDERTARGETSCOUNT; ++i)
	{
		blendStateDescription.RenderTarget[i].BlendEnable = FALSE;
		blendStateDescription.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendStateDescription.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDescription.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendStateDescription.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendStateDescription.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[i].RenderTargetWriteMask = 0x0f;
	}

	result = ((*(ID3D11Device**)GetDevice()))->CreateBlendState(&blendStateDescription, &m_blendDisable);

	if (FAILED(result))
	{
		return false;
	}




	// color blending enabled
	for (EUINT32 i = 0; i < PF_GI_RENDERTARGETSCOUNT; ++i)
	{
		blendStateDescription.RenderTarget[i].BlendEnable = TRUE;
		blendStateDescription.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_COLOR;
		blendStateDescription.RenderTarget[i].DestBlend = D3D11_BLEND_ONE;
		blendStateDescription.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ZERO;
		blendStateDescription.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendStateDescription.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		blendStateDescription.AlphaToCoverageEnable = false;
	}

	result = ((*(ID3D11Device**)GetDevice()))->CreateBlendState(&blendStateDescription, &m_colorBlendEnabled);

	if (FAILED(result))
	{
		return false;
	}










	//	-----------------------------------------------------------------------------------
	//	Samplers
	//	-----------------------------------------------------------------------------------

	D3D11_SAMPLER_DESC samplerDescription;

	samplerDescription.Filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
	samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.MipLODBias = 0.0f;
	samplerDescription.MaxAnisotropy = 0;
	samplerDescription.ComparisonFunc = D3D11_COMPARISON_GREATER;
	samplerDescription.BorderColor[0] = 0;
	samplerDescription.BorderColor[1] = 0;
	samplerDescription.BorderColor[2] = 0;
	samplerDescription.BorderColor[3] = 0;
	samplerDescription.MinLOD = 0;
	samplerDescription.MaxLOD = 0;

	samplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	((*(ID3D11Device**)GetDevice()))->CreateSamplerState(&samplerDescription, &m_samplerPoint);

	samplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	((*(ID3D11Device**)GetDevice()))->CreateSamplerState(&samplerDescription, &m_samplerLinear);

	samplerDescription.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDescription.ComparisonFunc = D3D11_COMPARISON_GREATER;
	samplerDescription.MaxAnisotropy = 16;
	samplerDescription.MinLOD = 0;
	samplerDescription.MaxLOD = 8;
	((*(ID3D11Device**)GetDevice()))->CreateSamplerState(&samplerDescription, &m_samplerAnisotropic);

	for (EUINT32 i = 0; i < 12; ++i)
	{
		(*(ID3D11DeviceContext**)GetDeviceContext())->VSSetSamplers(0, 1, &m_samplerPoint);
		(*(ID3D11DeviceContext**)GetDeviceContext())->PSSetSamplers(0, 1, &m_samplerPoint);
	}

	//
	// Setup the viewport for rendering
	//
	m_viewport.Width = (float)screenWidth;
	m_viewport.Height = (float)screenHeight;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;

	// Create the viewport.
	(*(ID3D11DeviceContext**)GetDeviceContext())->RSSetViewports(1, &m_viewport);



	//
	// Create depth stencil state with disabled Depth comparing
	//
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = ((*(ID3D11Device**)GetDevice()))->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);

	if (FAILED(result))
		return SystemCodes::ERROR_INIT;



	//
	//
	//
	ZeroMemory(&textureDecription, sizeof(textureDecription));

	textureDecription.MipLevels = 1;
	textureDecription.ArraySize = 1;
	textureDecription.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDecription.SampleDesc.Count = 1;
	textureDecription.SampleDesc.Quality = 0;
	textureDecription.Usage = D3D11_USAGE_STAGING;
	textureDecription.BindFlags = 0;
	textureDecription.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	textureDecription.MiscFlags = 0;

	textureDecription.Width = 1;
	textureDecription.Height = 1;
	result = ((*(ID3D11Device**)GetDevice()))->CreateTexture2D(&textureDecription, NULL, &m_texture1x1);

	textureDecription.Width = 16;
	textureDecription.Height = 16;
	result = ((*(ID3D11Device**)GetDevice()))->CreateTexture2D(&textureDecription, NULL, &m_texture16x16);

	textureDecription.Width = 32;
	textureDecription.Height = 32;
	result = ((*(ID3D11Device**)GetDevice()))->CreateTexture2D(&textureDecription, NULL, &m_texture32x32);

	textureDecription.Width = 64;
	textureDecription.Height = 64;
	result = ((*(ID3D11Device**)GetDevice()))->CreateTexture2D(&textureDecription, NULL, &m_texture64x64);

	textureDecription.Width = 128;
	textureDecription.Height = 128;
	result = ((*(ID3D11Device**)GetDevice()))->CreateTexture2D(&textureDecription, NULL, &m_texture128x128);

	textureDecription.Width = 256;
	textureDecription.Height = 256;
	result = ((*(ID3D11Device**)GetDevice()))->CreateTexture2D(&textureDecription, NULL, &m_texture256x256);

	textureDecription.Width = 64;
	textureDecription.Height = 512;
	result = ((*(ID3D11Device**)GetDevice()))->CreateTexture2D(&textureDecription, NULL, &m_texture64x512);

	textureDecription.Width = 512;
	textureDecription.Height = 512;
	result = ((*(ID3D11Device**)GetDevice()))->CreateTexture2D(&textureDecription, NULL, &m_texture512x512);

	textureDecription.Width = 1920;
	textureDecription.Height = 1080;
	result = ((*(ID3D11Device**)GetDevice()))->CreateTexture2D(&textureDecription, NULL, &m_texture1920x1080);

	textureDecription.Width = 2560;
	textureDecription.Height = 1440;
	result = ((*(ID3D11Device**)GetDevice()))->CreateTexture2D(&textureDecription, NULL, &m_texture2560x1440);

	return SystemCodes::SUCCESS;
}


ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_Release()
{
	EUINT32 result = SystemCodes::SUCCESS;

	if (m_swapChain != NULLPTR)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	if (m_rasterState != NULLPTR)
	{
		m_rasterState->Release();
		m_rasterState = NULLPTR;
	}

	if (m_depthStencilView != NULLPTR)
	{
		m_depthStencilView->Release();
		m_depthStencilView = NULLPTR;
	}

	if (m_stencil_default != NULLPTR)
	{
		m_stencil_default->Release();
		m_stencil_default = NULLPTR;
	}

	if (m_depthStencilBuffer != NULLPTR)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = NULLPTR;
	}

	if (m_renderTargetView != NULLPTR)
	{
		m_renderTargetView->Release();
		m_renderTargetView = NULLPTR;
	}

	if (m_swapChain != NULLPTR)
	{
		m_swapChain->Release();
		m_swapChain = NULLPTR;
	}

	if ((*(ID3D11DeviceContext**)GetDeviceContext()) != NULLPTR)
	{
		(*(ID3D11DeviceContext**)GetDeviceContext())->Release();
		(*(ID3D11DeviceContext**)GetDeviceContext()) = NULLPTR;
	}

	if (((*(ID3D11Device**)GetDevice())) != NULLPTR)
	{
		((*(ID3D11Device**)GetDevice()))->Release();
		((*(ID3D11Device**)GetDevice())) = NULLPTR;
	}



	return result;
}

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_Update()
{
	EUINT32 result;
	result = SystemCodes::SUCCESS;


	float color[4];


	// Setup the color to clear the buffer to.
	color[0] = 1.0f;
	color[1] = 1.0f;
	color[2] = 0.0f;
	color[3] = 0.5f;

	// Clear the back buffer.
	(*(ID3D11DeviceContext**)GetDeviceContext())->ClearRenderTargetView(m_renderTargetView, color);

	// Clear the depth buffer.
	(*(ID3D11DeviceContext**)GetDeviceContext())->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);


	return result;
}




void
ParabellumFramework::Graphics::DeviceDirectX11::Device_Render(EUINT64 indicesCount)
{
	(*(ID3D11DeviceContext**)GetDeviceContext())->DrawIndexed(indicesCount, 0, 0);
}













ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_CreateVertexBuffer(_IO_ void* ppOutputBuffer, _IN_ void* pVertices, EUINT32 vertexSize, EUINT32 verticesCount)
{
	EUINT32 result;
	result = SystemCodes::SUCCESS;

	HRESULT dxresult;
	ID3D10Blob* errorMessage = 0;

	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = vertexSize * verticesCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = pVertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	dxresult = ((*(ID3D11Device**)GetDevice()))->CreateBuffer(&vertexBufferDesc, &vertexData, (ID3D11Buffer**)ppOutputBuffer);

	if (FAILED(dxresult))
	{
		char* compileErrors;
		std::ofstream fout;

		compileErrors = (char*)(errorMessage->GetBufferPointer());

		PFDLOG("PFGI", "GDDX", "FAIL creating vertex shader :" << compileErrors);

		errorMessage->Release();
		errorMessage = 0;

		return SystemCodes::ERROR_INIT;
	}
		




	return result;
}


ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_ReleaseVertexBuffer(_IO_ void* ppBuffer)
{
	EUINT32 result;
	result = SystemCodes::SUCCESS;
	(*(ID3D11Buffer**)ppBuffer)->Release();
	return result;
}



ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_CreateIndexBuffer(_IO_ void* ppOutputBuffer, _IN_ void* pIndices, EUINT64 indicesCount)
{
	EUINT32 result;
	result = SystemCodes::SUCCESS;

	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indicesCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	indexData.pSysMem = pIndices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	((*(ID3D11Device**)GetDevice()))->CreateBuffer(&indexBufferDesc, &indexData, (ID3D11Buffer**)ppOutputBuffer);

	return result;
}


ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_ReleaseIndexBuffer(_IO_ void*  ppIndexBuffer)
{
	EUINT32 result;
	result = SystemCodes::SUCCESS;
	(*(ID3D11Buffer**)ppIndexBuffer)->Release();
	return result;
}


ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_SetBuffers(
	_IN_ void* ppVertexBuffer, EUINT32 vertexSize, _IN_ void* ppIndexBuffer)
{
	EUINT32 result;
	result = SystemCodes::SUCCESS;

	EUINT32 stride;
	EUINT32 offset;

	stride = vertexSize;
	offset = 0;

	(*(ID3D11DeviceContext**)GetDeviceContext())->IASetVertexBuffers(0, 1, (ID3D11Buffer**)ppVertexBuffer, &stride, &offset);
	(*(ID3D11DeviceContext**)GetDeviceContext())->IASetIndexBuffer( (*(ID3D11Buffer**)ppIndexBuffer), DXGI_FORMAT_R32_UINT, 0);
	(*(ID3D11DeviceContext**)GetDeviceContext())->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	return result;
}


void 
ParabellumFramework::Graphics::DeviceDirectX11::Device_SetVertexBuffer(_IN_ void* ppVertexBuffer, EUINT32 vertexSize)
{
	EUINT32 stride;
	EUINT32 offset;

	stride = vertexSize;
	offset = 0;

	(*(ID3D11DeviceContext**)GetDeviceContext())->IASetVertexBuffers(0, 1, (ID3D11Buffer**)ppVertexBuffer, &stride, &offset);
}

void 
ParabellumFramework::Graphics::DeviceDirectX11::Device_SetIndexBuffer(_IN_ void* ppIndexBuffer)
{
	// set the index buffer
	(*(ID3D11DeviceContext**)GetDeviceContext())->IASetIndexBuffer((*(ID3D11Buffer**)ppIndexBuffer), DXGI_FORMAT_R32_UINT, 0);

	// type of primitives - always triangles - to change one day
	(*(ID3D11DeviceContext**)GetDeviceContext())->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}




// -------------------------------------------------------------------------------
// Methods-Shader
// -------------------------------------------------------------------------------

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_LoadVertexShader(
	_OUT_ void* ppShader, const char* filename, const char* functionName,
	_IN_ ShaderLayout* pInputLayout, EUINT32 layoutFieldsCount, _OUT_ void* ppLayout)
{
	EUINT32 result;
	result = SystemCodes::SUCCESS;

	HRESULT dxresult;
	ID3D10Blob* errorMessage;
	ID3D10Blob* shaderBuffer;
	D3D11_INPUT_ELEMENT_DESC* polygonLayout = new D3D11_INPUT_ELEMENT_DESC[layoutFieldsCount];

	errorMessage = NULLPTR;
	shaderBuffer = NULLPTR;


	// ceck if file exists
	std::fstream fShader;
	fShader.open(filename);

	if (fShader.good() == false)
	{
		PFDLOG("PFGI", "GDDX", "Device_LoadVertexShader("<< filename <<") : Can NOT open file");
		return SystemCodes::ERROR_NOFILE;
	}

	fShader.close();


	std::string s(filename);
	std::wstring ws;
	ws.assign(s.begin(), s.end());

	// compile shader
	dxresult = D3DCompileFromFile(ws.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		functionName, "vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&shaderBuffer, &errorMessage);


	if (FAILED(dxresult))
	{
		char* compileErrors = (char*)(errorMessage->GetBufferPointer());

		PFDLOG("PFGI", "GDDX", "Device_LoadVertexShader() : Failed compiling vertex shader : " << compileErrors);

		errorMessage->Release();
		errorMessage = 0;

		return SystemCodes::ERROR_BADFILETYPE;
	}



	// create vertex shader
	dxresult = (*(ID3D11Device**)GetDevice())->CreateVertexShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, (ID3D11VertexShader**)ppShader);
	
	if (FAILED(dxresult))
	{
		char* compileErrors = (char*)(errorMessage->GetBufferPointer());

		PFDLOG("PFGI", "GDDX", "DirectX FAILED : ID3D11Device::CreateVertexShader " << compileErrors);

		errorMessage->Release();
		errorMessage = 0;

		return SystemCodes::ERROR_INIT;
	}


	// setup vertex input layout
	// set the semantic data

	for (EUINT32 i = 0; i<layoutFieldsCount; ++i)
	{
		polygonLayout[i].SemanticName = pInputLayout[i].GetSemanticName().c_str();
		polygonLayout[i].SemanticIndex = pInputLayout[i].GetSemanticIndex();


		EUINT32 fmt = 0;
		switch (pInputLayout[i].GetFormat())
		{
		case GraphicsFormat::FORMAT_R32G32_FLOAT: fmt = DXGI_FORMAT_R32G32_FLOAT; break;
		case GraphicsFormat::FORMAT_R32G32B32_FLOAT: fmt = DXGI_FORMAT_R32G32B32_FLOAT; break;
		case GraphicsFormat::FORMAT_R32G32B32A32_FLOAT: fmt = DXGI_FORMAT_R32G32B32A32_FLOAT; break;

		default: polygonLayout[i].Format = DXGI_FORMAT_R32G32B32_FLOAT; break;
		}
		polygonLayout[i].Format = (DXGI_FORMAT)fmt;

		polygonLayout[i].InputSlot = pInputLayout[i].GetInputSlot();
		polygonLayout[i].AlignedByteOffset = pInputLayout[i].GetAlignedByteOffset();
		polygonLayout[i].InputSlotClass = (D3D11_INPUT_CLASSIFICATION)pInputLayout[i].GetInputSlotClass();
		polygonLayout[i].InstanceDataStepRate = pInputLayout[i].GetInstanceDataStepRate();


		/*
		printf("\n---------------------------------");
		printf("\npolygonLayout[i].SemanticName = %s", pInputLayout[i].GetSemanticName().c_str());
		printf("\npolygonLayout[i].SemanticIndex = %u", pInputLayout[i].GetSemanticIndex());


		fmt = 0;
		switch (pInputLayout[i].GetFormat())
		{
		case GraphicsFormat::FORMAT_R32G32_FLOAT: fmt = DXGI_FORMAT_R32G32_FLOAT; break;
		case GraphicsFormat::FORMAT_R32G32B32_FLOAT: fmt = DXGI_FORMAT_R32G32B32_FLOAT; break;
		case GraphicsFormat::FORMAT_R32G32B32A32_FLOAT: fmt = DXGI_FORMAT_R32G32B32A32_FLOAT; break;

		default: polygonLayout[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
		}
		printf("\npolygonLayout[i].Format = %u",(DXGI_FORMAT)fmt);

		printf("\npolygonLayout[i].InputSlot =  %u", pInputLayout[i].GetInputSlot());
		printf("\npolygonLayout[i].AlignedByteOffset =  %u", pInputLayout[i].GetAlignedByteOffset());
		printf("\npolygonLayout[i].InputSlotClass = %u", (D3D11_INPUT_CLASSIFICATION)pInputLayout[i].GetInputSlotClass());
		printf("\npolygonLayout[i].InstanceDataStepRate =  %u", pInputLayout[i].GetInstanceDataStepRate());
		*/
	}


	// create vertex input layout
	dxresult = (*(ID3D11Device**)GetDevice())->CreateInputLayout
		(polygonLayout,
			layoutFieldsCount,
			shaderBuffer->GetBufferPointer(),
			shaderBuffer->GetBufferSize(),
			(ID3D11InputLayout**)ppLayout);

	if (FAILED(dxresult))
	{
		char* compileErrors = (char*)(errorMessage->GetBufferPointer());

		PFDLOG("PFGI", "GDDX", "DirectX FAILED : ID3D11Device::CreateInputLayout " << compileErrors);

		errorMessage->Release();
		errorMessage = 0;

		return SystemCodes::ERROR_INIT;
	}


	// free memory
	delete[] polygonLayout;

	shaderBuffer->Release();
	shaderBuffer = 0;

	return result;
}





ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_LoadPixelShader(_OUT_ void* pShader, const char* filename, const char* functionName)
{
	EUINT32 result;
	result = SystemCodes::SUCCESS;

	HRESULT dxresult;
	ID3D10Blob* errorMessage;
	ID3D10Blob* shaderBuffer;

	errorMessage = 0;
	shaderBuffer = 0;


	std::string s(filename);
	std::wstring ws;
	ws.assign(s.begin(), s.end());

	// Compile the pixel shader code.
	dxresult = D3DCompileFromFile(ws.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, 
		functionName, "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&shaderBuffer, &errorMessage);

	if (FAILED(dxresult))
	{
		char* compileErrors = (char*)(errorMessage->GetBufferPointer());

		PFDLOG("PFGI", "GDDX", "Device_LoadPixelShader() : ERROR compiling pixel shader : " << compileErrors);

		errorMessage->Release();
		errorMessage = 0;

		return SystemCodes::ERROR_INIT;
	}


	// create pixel shader
	dxresult = ((*(ID3D11Device**)GetDevice()))->CreatePixelShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, (ID3D11PixelShader**)pShader);

	if (FAILED(dxresult))
	{
		char* compileErrors = (char*)(errorMessage->GetBufferPointer());
		
		PFDLOG("PFGI", "GDDX", "Device_LoadPixelShader() : FAIL creating input layout :" << compileErrors);

		errorMessage->Release();
		errorMessage = 0;

		return SystemCodes::ERROR_INIT;
	}

	// free memory
	shaderBuffer->Release();
	shaderBuffer = 0;

	return result;
}

//
// release Vertex Shader
//
ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_ReleaseVertexShader(_OUT_ void* pShader)
{
	( (*(ID3D11VertexShader**)pShader))->Release();

	return SystemCodes::SUCCESS;
}

//
// release Vertex Shader
//
ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_ReleasePixelShader(_OUT_ void* pShader)
{
	( (*(ID3D11PixelShader**)pShader))->Release();

	return SystemCodes::SUCCESS;
}


ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_SetVertexShader(_IN_ void* pShader)
{
	EUINT32 result;
	result = SystemCodes::SUCCESS;

	((*(ID3D11DeviceContext**)GetDeviceContext()))->VSSetShader((*(ID3D11VertexShader**)pShader), NULL, 0);

	return result;
}

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_SetPixelShader(_IN_ void* pShader)
{
	EUINT32 result;
	result = SystemCodes::SUCCESS;

	((*(ID3D11DeviceContext**)GetDeviceContext()))->PSSetShader( *((ID3D11PixelShader**)pShader), NULL, 0);

	return result;
}


ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_SetInputLayout(_IN_ void* pLayout)
{
	EUINT32 result;
	result = SystemCodes::SUCCESS;

	(*(ID3D11DeviceContext**)GetDeviceContext())->IASetInputLayout((*(ID3D11InputLayout**)pLayout));

	return result;

	// if program crashed here - there is some name mistake in shader code...
}


ParabellumFramework::EUINT32 
ParabellumFramework::Graphics::DeviceDirectX11::Device_CreateCBuffer(
	_OUT_ void* ppBuffer, EUINT32 bufferSize)
{
	PFDLOG("PFGI", "GDDX", "Device_CreateCBuffer(" << (EADDR)ppBuffer << ", " << bufferSize <<")");

	EUINT32 result;
	result = SystemCodes::SUCCESS;

	HRESULT dxresult;
	ID3D10Blob* errorMessage;
	errorMessage = 0;

	D3D11_BUFFER_DESC matrixBufferDesc1;
	matrixBufferDesc1.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc1.ByteWidth = bufferSize;
	matrixBufferDesc1.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc1.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc1.MiscFlags = 0;
	matrixBufferDesc1.StructureByteStride = 0;

	dxresult = (*(ID3D11Device**)GetDevice())->CreateBuffer(&matrixBufferDesc1, NULL, (ID3D11Buffer**)ppBuffer);

	if (FAILED(dxresult))
	{
		char* compileErrors = (char*)(errorMessage->GetBufferPointer());

		PFDLOG("PFGI", "GDDX", "Device_CreateCBuffer() : FAIL creating CBuffer : " << compileErrors);

		errorMessage->Release();
		errorMessage = 0;

		return SystemCodes::ERROR_INIT;
	}


	return result;
}





ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_SetCBuffer(_IN_ void* ppBuffer, EUINT32 offset, _IN_ void* pIn, EUINT32 inputSize)
{
	EUINT32 result;
	result = SystemCodes::SUCCESS;
	

	HRESULT dxresult;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	void* dataPtr1;
	EUINT32 bufferNumber;

	char* bufferPlace = (char*)ppBuffer+offset;

	//printf("\n\n---------------------------------------");
	//printf("\n ppBuffer = %u", (EUINT32)ppBuffer);
	//printf("\n bufferPlace = %u", (EUINT32)bufferPlace);

	//return result;

	dxresult = (*(ID3D11DeviceContext**)GetDeviceContext())->Map((*(ID3D11Buffer**)bufferPlace), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	dataPtr1 = (void*)mappedResource.pData;

	EUINT32 memoryPosition = 0;
	EUINT32 size = 0;

	//memcpy(dataPtr1, pIn, inputSize);

	(*(ID3D11DeviceContext**)GetDeviceContext())->Unmap((*(ID3D11Buffer**)bufferPlace), 0);
	bufferNumber = 0;

	// set buffer for vertex and pixel shader
	(*(ID3D11DeviceContext**)GetDeviceContext())->VSSetConstantBuffers(bufferNumber, 1, (ID3D11Buffer**)ppBuffer);
	(*(ID3D11DeviceContext**)GetDeviceContext())->PSSetConstantBuffers(bufferNumber, 1, (ID3D11Buffer**)ppBuffer);


	return result;
}


ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_SetCBuffer(
	_IN_ void* ppBuffer, _IN_ ShaderParametr** ppParametrs, EUINT32 parametrsCount)
{
	EUINT32 result;
	result = SystemCodes::SUCCESS;

	HRESULT dxresult;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	void* dataPtr1;
	EUINT32 bufferNumber;

	if (parametrsCount > 0)
	{
		dxresult = (*(ID3D11DeviceContext**)GetDeviceContext())->Map( (*(ID3D11Buffer**)ppBuffer), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		/*
		if (FAILED(dxresult))
		{
			return SystemCodes::ERROR_INIT;
		}
		*/

		dataPtr1 = (void*)mappedResource.pData;


		EUINT32 memoryPosition = 0;
		EUINT32 size = 0;

		
		for (EUINT32 i = 0; i<parametrsCount; ++i)
		{
			// align to 16 bytes if variable is less than
			size = ppParametrs[i]->GetSize();

			if (size < 16) size = 16;

			memcpy((void*)((char*)dataPtr1 + memoryPosition), ppParametrs[i]->Get<void>(), size);
			memoryPosition += size;


			// ERROR INFO
			// error here means that no all parametrs have been initialized !!! 
			// Check operators () and [] for parametrs in Shader
		}







		(*(ID3D11DeviceContext**)GetDeviceContext())->Unmap((*(ID3D11Buffer**)ppBuffer), 0);
		bufferNumber = 0;

		// set buffer for vertex and pixel shader
		(*(ID3D11DeviceContext**)GetDeviceContext())->VSSetConstantBuffers(bufferNumber, 1, (ID3D11Buffer**)ppBuffer);
		(*(ID3D11DeviceContext**)GetDeviceContext())->PSSetConstantBuffers(bufferNumber, 1, (ID3D11Buffer**)ppBuffer);
	}



	return result;
}



ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_SetCBuffer(_IN_ void* ppBuffer, _IN_ void* pParametrsBuffer, EUINT32 bufferSize)
{
	EUINT32 result = SystemCodes::SUCCESS;

	HRESULT dxresult;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	void* dataPtr1;
	EUINT32 bufferNumber;

	dxresult = (*(ID3D11DeviceContext**)GetDeviceContext())->Map((*(ID3D11Buffer**)ppBuffer), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	
	dataPtr1 = (void*)mappedResource.pData;

	EUINT32 memoryPosition = 0;
	EUINT32 size = 0;

	memcpy(dataPtr1, pParametrsBuffer, bufferSize);

	(*(ID3D11DeviceContext**)GetDeviceContext())->Unmap((*(ID3D11Buffer**)ppBuffer), 0);
	bufferNumber = 0;

	// set buffer for vertex and pixel shader
	(*(ID3D11DeviceContext**)GetDeviceContext())->VSSetConstantBuffers(bufferNumber, 1, (ID3D11Buffer**)ppBuffer);
	(*(ID3D11DeviceContext**)GetDeviceContext())->PSSetConstantBuffers(bufferNumber, 1, (ID3D11Buffer**)ppBuffer);


	return result;
}






ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_ReleaseCBuffer(_IN_ void* ppBuffer)
{
	(*(ID3D11Buffer**)ppBuffer)->Release();

	return SystemCodes::SUCCESS;
}







ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_SetResources(_IN_ ShaderParametr** ppParametrs, EUINT32 parametrsCount)
{
	EUINT32 result;
	result = SystemCodes::SUCCESS;

	ID3D11ShaderResourceView* resx;

	if (parametrsCount > 0)
	{
		for (EUINT32 i = 0; i <parametrsCount; ++i)
		{
			resx = ppParametrs[i]->Get<ID3D11ShaderResourceView>();

			(*(ID3D11DeviceContext**)GetDeviceContext())->PSSetShaderResources(i, 1, &resx);
			(*(ID3D11DeviceContext**)GetDeviceContext())->VSSetShaderResources(i, 1, &resx);
		}
	}


	return result;
}



ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_CreateResource(_OUT_ void* ppBuffer, _IN_ const char* filename)
{
	EUINT32 result;
	result = SystemCodes::SUCCESS;



	ppBuffer = 0;


	return result;
}


ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_LoadTexture(_OUT_ void* ppBuffer, _IN_ const char* filename)
{
	EUINT32 result;
	result = SystemCodes::SUCCESS;



	ppBuffer = 0;


	return result;
}


ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_CreateTexture(
	_IN_ void* pTextureData, EUINT32 width, EUINT32 height, GraphicsFormat format, _OUT_ void* ppBuffer, _OUT_ void* ppBufferType)
{
	PFDLOG("PFGI", "GDDX", "DeviceDirectX11::Device_CreateTexture( (pTextureData)"<< pTextureData <<", (width)" << width << ", (height)" << height << ", (format)" << format << ", (ppBuffer)" << ppBuffer << ", (ppBufferType)" << ppBufferType << ") ");

	EUINT32 result;
	result = SystemCodes::SUCCESS;

	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT dxresult;
	EUINT32 pitchSize;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	// fill texture data
	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;

	switch (format)
	{
	case GraphicsFormat::FORMAT_R8G8B8A8_INT:
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		pitchSize = (width * 4) * sizeof(unsigned char);
		break;

	case GraphicsFormat::FORMAT_R32G32B32A32_INT:
		textureDesc.MipLevels = 8;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
		pitchSize = (width * 4) * sizeof(EUINT32);
		break;
	}

	

	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// Create the empty texture.
	dxresult = (*(ID3D11Device**)GetDevice())->CreateTexture2D(&textureDesc, NULL, (ID3D11Texture2D**)ppBuffer);
	if (FAILED(dxresult))
	{
		PFDLOG("PFGI", "GDDX", "DirectX FAILED : ID3D11Device::CreateTexture2D ");

		return SystemCodes::ERROR_INIT;
	}

	// Set the row pitch of the targa image data
	

	// Copy the targa image data into the texture
	(*(ID3D11DeviceContext**)GetDeviceContext())->UpdateSubresource( *(ID3D11Resource**)ppBuffer, 0, NULL, pTextureData, pitchSize, 0);

	// Setup the shader resource view description.
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	// Create the shader resource view for the texture.
	dxresult = (*(ID3D11Device**)GetDevice())->CreateShaderResourceView(*(ID3D11Resource**)ppBuffer, &srvDesc, (ID3D11ShaderResourceView**)ppBufferType);
	if (FAILED(dxresult))
	{
		PFDLOG("PFGI", "GDDX", "DirectX FAILED : ID3D11Device::CreateShaderResourceView");

		return SystemCodes::ERROR_INIT;
	}

	// Generate mipmaps for this texture.
	(*(ID3D11DeviceContext**)GetDeviceContext())->GenerateMips(*(ID3D11ShaderResourceView**)ppBufferType);

	if (FAILED(dxresult))
	{
		PFDLOG("PFGI", "GDDX", "DirectX FAILED : ID3D11Device::GenerateMips");

		return SystemCodes::ERROR_INIT;
	}

	return result;
}



ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_ReleaseTexture(_IO_ void* ppBuffer, _IO_ void* ppFormat)
{
	PFDLOG("PFGI", "GDDX", "DeviceDirectX11::Device_ReleaseTexture( (ppBuffer)" << ppBuffer << ", (ppFormat)" << ppFormat << ") ");

	EUINT32 result;
	result = SystemCodes::SUCCESS;

	(*(ID3D11ShaderResourceView**)ppFormat)->Release();
	(*(ID3D11Texture2D**)ppBuffer)->Release();

	return result;
}




ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_CreateSampler(
	_OUT_ void* ppBuffer,
	SamplerFilter filter,
	SamplerAddress addressU,
	SamplerAddress addressV,
	SamplerAddress addressW,
	EFLOAT32 mipLODBias,
	EUINT32 maxAnisotropy,
	SamplerComparsion comparisonFunc)
{
	PFDLOG("PFGI", "GDDX", "DeviceDirectX11::Device_CreateSampler(" << ppBuffer << ", ..."  << ")");


	EUINT32 result;
	result = SystemCodes::SUCCESS;

	D3D11_SAMPLER_DESC samplerDesc;

	// Create a texture sampler state description.

	switch (filter)
	{
		case SamplerFilter::MIN_MAG_MIP_POINT: samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; break;
		case SamplerFilter::MIN_MAG_MIP_LINEAR: samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; break;
		case SamplerFilter::MIN_MAG_MIP_ANISOTROPIC: samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC; break;
	}
	

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT dxresult;

	dxresult = (*(ID3D11Device**)GetDevice())->CreateSamplerState(&samplerDesc, (ID3D11SamplerState**)ppBuffer);

	if (FAILED(dxresult))
	{
		PFDLOG("PFGI", "GDDX", "Device_CreateSampler() : Can NOT create a sampler");

		result = SystemCodes::ERROR_INIT;
		return result;
	}

	return result;
}




ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_SetSampler(_IN_ void* ppBuffer, EUINT32 regNo)
{
	(*(ID3D11DeviceContext**)GetDeviceContext())->VSSetSamplers(regNo, 1, (ID3D11SamplerState**)ppBuffer);
	(*(ID3D11DeviceContext**)GetDeviceContext())->PSSetSamplers(regNo, 1, (ID3D11SamplerState**)ppBuffer);

	return SystemCodes::SUCCESS;
}


ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_ReleaseSampler(_IN_ void* ppBuffer)
{
	PFDLOG("PFGI", "GDDX", "DeviceDirectX11::Device_ReleaseSampler( (ppBuffer)" << ppBuffer << ")" );

	(*(ID3D11SamplerState**)ppBuffer)->Release();
	
	return SystemCodes::SUCCESS;
}


ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_SamplerSetDefault(EUINT32 regNo, SamplerType type)
{
	switch (type)
	{
	case SamplerType::POINT:		Device_SetSampler(&m_samplerPoint, regNo); break;
	case SamplerType::LINEAR:		Device_SetSampler(&m_samplerLinear, regNo); break;
	case SamplerType::ANISOTROPIC:	Device_SetSampler(&m_samplerAnisotropic, regNo); break;
	}

	return SystemCodes::SUCCESS;
}


ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_CreateRenderTarget(
	_OUT_ void* ppBufferTexture, _OUT_ void* ppBufferView, _OUT_ void* ppBufferResource,
	GraphicsFormat textureFormat, EUINT32 width, EUINT32 height)
{
	PFDLOG("PFGI", "GDDX", "DeviceDirectX11::Device_CreateRenderTarget( (ppBufferTexture)" << ppBufferTexture << ", (ppBufferView)" << ppBufferView << ", (ppBufferResource)" << ppBufferResource << ", (textureFormat)" << textureFormat << ", (width)" << width << ", (height)" << height << ") ");

	EUINT32 result = SystemCodes::SUCCESS;

	HRESULT dxresult;

	D3D11_TEXTURE2D_DESC textureDescription;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetDescription;
	D3D11_SHADER_RESOURCE_VIEW_DESC resourceDescription;

	// initialize texture description
	ZeroMemory(&textureDescription, sizeof(textureDescription));

	textureDescription.Width = width;
	textureDescription.Height = height;
	textureDescription.MipLevels = 1;
	textureDescription.ArraySize = 1;
	//textureDescription.Format = (DXGI_FORMAT)textureFormat;
	textureDescription.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;


	textureDescription.SampleDesc.Count = 1;
	textureDescription.Usage = D3D11_USAGE_DEFAULT;
	textureDescription.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDescription.CPUAccessFlags = 0;
	textureDescription.MiscFlags = 0;

	// create texture
	dxresult = (*(ID3D11Device**)GetDevice())->CreateTexture2D(&textureDescription, NULL, (ID3D11Texture2D**)ppBufferTexture);

	if (FAILED(dxresult))
	{
		PFDLOG("PFGI", "GDDX", "ID3D11Device::CreateTexture2D() ");

		return SystemCodes::ERROR_INIT;
	}

	// initialize render target view
	renderTargetDescription.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;


	//renderTargetDescription.Format = textureDescription.Format;

	renderTargetDescription.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetDescription.Texture2D.MipSlice = 0;

	dxresult = (*(ID3D11Device**)GetDevice())->CreateRenderTargetView(*(ID3D11Resource**)ppBufferTexture,
		&renderTargetDescription,
		(ID3D11RenderTargetView**)ppBufferView);
	if (FAILED(dxresult))
	{
		return SystemCodes::ERROR_INIT;
	}

	// initialize shader resource
	resourceDescription.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;


	//resourceDescription.Format = textureDescription.Format;
	resourceDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceDescription.Texture2D.MostDetailedMip = 0;
	resourceDescription.Texture2D.MipLevels = 1;

	dxresult = (*(ID3D11Device**)GetDevice())->CreateShaderResourceView(*((ID3D11Resource**)ppBufferTexture),
		&resourceDescription,
		(ID3D11ShaderResourceView**)ppBufferResource);
	if (FAILED(dxresult))
	{
		return SystemCodes::ERROR_INIT;
	}




	return result;
}


ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_ReleaseRenderTarget(
	_IO_ void* ppBufferTexture, _IO_ void* ppBufferView, _IO_ void* ppBufferResource)
{
	(*((ID3D11ShaderResourceView**)ppBufferResource))->Release();
	(*(ID3D11RenderTargetView**)ppBufferView)->Release();
	(*(ID3D11Resource**)ppBufferTexture)->Release();

	return SystemCodes::SUCCESS;
}


ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_CreateDepthBuffer(
	_OUT_ void* ppDepthBuffer, void* ppDepthView,
	EUINT32 textureFormat, EUINT32 width, EUINT32 height)
{
	PFDLOG("PFGI", "GDDX", "DeviceDirectX11::Device_CreateRenderTarget( (ppDepthBuffer)" << ppDepthBuffer << ", (ppDepthView)" << ppDepthView << ", (textureFormat)" << textureFormat << ", (width)" << width << ", (height)" << height << ") ");

	EUINT32 result = SystemCodes::SUCCESS;
	HRESULT dxresult;
	D3D11_TEXTURE2D_DESC textureDescription;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	ZeroMemory(&textureDescription, sizeof(textureDescription));

	textureDescription.Width = width;
	textureDescription.Height = height;
	textureDescription.MipLevels = 1;
	textureDescription.ArraySize = 1;
	textureDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDescription.SampleDesc.Count = 1;
	textureDescription.SampleDesc.Quality = 0;
	textureDescription.Usage = D3D11_USAGE_DEFAULT;
	textureDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDescription.CPUAccessFlags = 0;
	textureDescription.MiscFlags = 0;

	dxresult = (*(ID3D11Device**)GetDevice())->CreateTexture2D(&textureDescription, NULL, (ID3D11Texture2D**)ppDepthBuffer);
	if (FAILED(dxresult))
	{
		PFDLOG("PFGI", "GDDX", "ID3D11Device::CreateTexture2D()");

		return SystemCodes::ERROR_INIT;
	}

	//
	// Create the depth stencil state.
	//
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// front face
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// back face
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	dxresult = (*(ID3D11Device**)GetDevice())->CreateDepthStencilState(&depthStencilDesc, &m_stencil_default);

	if (FAILED(dxresult))
	{
		PFDLOG("PFGI", "GDDX", "ID3D11Device::CreateDepthStencilState()");

		return SystemCodes::ERROR_INIT;
	}

	(*(ID3D11DeviceContext**)GetDeviceContext())->OMSetDepthStencilState(m_stencil_default, 1);

	// create the depth stencil view description
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	dxresult = (*(ID3D11Device**)GetDevice())->CreateDepthStencilView(*(ID3D11Resource**)ppDepthBuffer,
		&depthStencilViewDesc,
		(ID3D11DepthStencilView**)ppDepthView);

	if (FAILED(dxresult))
	{
		PFDLOG("PFGI", "GDDX", "ID3D11Device::CreateDepthStencilView()");

		return SystemCodes::ERROR_INIT;
	}

	return result;
}



ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::SetDefaultRenderTarget()
{
	(*(ID3D11DeviceContext**)GetDeviceContext())->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	float color[4];
	color[0] = 1.0f; color[1] = 1.0f; color[2] = 1.0f; color[3] = 1.0f;

	(*(ID3D11DeviceContext**)GetDeviceContext())->ClearDepthStencilView(
		m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	(*(ID3D11DeviceContext**)GetDeviceContext())->RSSetViewports(1, &m_viewport);

	return SystemCodes::SUCCESS;
}

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_SetRenderTarget(
	EUINT32 no, _IN_ void* pViewportBuffer,
	_IN_ void* ppRTBufferFormat, _IN_ void* pDepthBufferFormat, EFLOAT32 depthValue
	)
{
	EUINT32 result = SystemCodes::SUCCESS;

	ID3D11RenderTargetView* mc[5];
	//mc[0] = (ID3D11RenderTargetView*)pRTBufferFormat;

	float color[4];
	color[0] = 0.0f; color[1] = 0.0f; color[2] = 0.0f; color[3] = 0.0f;

	(*(ID3D11DeviceContext**)GetDeviceContext())->ClearRenderTargetView(
		*(ID3D11RenderTargetView**)ppRTBufferFormat, color);


	(*(ID3D11DeviceContext**)GetDeviceContext())->OMSetRenderTargets(
		1, (ID3D11RenderTargetView**)ppRTBufferFormat, (ID3D11DepthStencilView*)pDepthBufferFormat);

	//if (clearDepth == true)
	(*(ID3D11DeviceContext**)GetDeviceContext())->ClearDepthStencilView((ID3D11DepthStencilView*)pDepthBufferFormat, D3D11_CLEAR_DEPTH, depthValue, 0);
	
		


	(*(ID3D11DeviceContext**)GetDeviceContext())->RSSetViewports(1, (D3D11_VIEWPORT*)pViewportBuffer);

	return result;
}



ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_SetRenderTargets(
	EUINT32 count, _IN_ void* pViewportBuffer,
	_OUT_ void** ppRTBufferFormat, _OUT_ void** ppDepthBufferFormat,
	_OUT_ void* pDepthStencilView,
	EBOOL clearDepth, EFLOAT32 depthValue
	)
{
	EUINT32 result = SystemCodes::SUCCESS;

	float color[4];
	color[0] = 0.0f; color[1] = 0.0f; color[2] = 0.0f; color[3] = 1.0f;
	
	if (clearDepth == true)
		for (EUINT32 i = 0; i < count; ++i)
			(*(ID3D11DeviceContext**)GetDeviceContext())->ClearRenderTargetView((ID3D11RenderTargetView*)ppRTBufferFormat[i], color);

	if (pDepthStencilView != NULLPTR)
	{
		(*(ID3D11DeviceContext**)GetDeviceContext())->OMSetRenderTargets(
			count, (ID3D11RenderTargetView**)ppRTBufferFormat, (ID3D11DepthStencilView*)pDepthStencilView);

		if (clearDepth == true)
			(*(ID3D11DeviceContext**)GetDeviceContext())->ClearDepthStencilView((ID3D11DepthStencilView*)pDepthStencilView, D3D11_CLEAR_DEPTH, depthValue, 0);


		(*(ID3D11DeviceContext**)GetDeviceContext())->RSSetViewports(1, (D3D11_VIEWPORT*)pViewportBuffer);
	}
	else
	{
		(*(ID3D11DeviceContext**)GetDeviceContext())->OMSetRenderTargets(
			count, (ID3D11RenderTargetView**)ppRTBufferFormat, m_depthStencilView);

		if (clearDepth == true)
			(*(ID3D11DeviceContext**)GetDeviceContext())->ClearDepthStencilView((ID3D11DepthStencilView*)pDepthStencilView, D3D11_CLEAR_DEPTH, depthValue, 0);

		(*(ID3D11DeviceContext**)GetDeviceContext())->RSSetViewports(1, &m_viewport);
	}

	

	return result;
}


ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_RenderTargetsSetDepth(_OUT_ void* pDepthStencilView, EFLOAT32 depthValue)
{
	EUINT32 result = SystemCodes::SUCCESS;
	(*(ID3D11DeviceContext**)GetDeviceContext())->ClearDepthStencilView((ID3D11DepthStencilView*)pDepthStencilView, D3D11_CLEAR_DEPTH, depthValue, 0);
	return result;
}




// -------------------------------------------------------------------------------
//Rasterizer functions
// -------------------------------------------------------------------------------

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_TurnDepthBuffer(bool flag)
{
	if (flag == true)
		(*(ID3D11DeviceContext**)GetDeviceContext())->OMSetDepthStencilState(m_stencil_default, 1);
	else
		(*(ID3D11DeviceContext**)GetDeviceContext())->OMSetDepthStencilState(m_depthDisabledStencilState, 1);

	return SystemCodes::SUCCESS;
}

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_TurnAlphaBlending(bool flag)
{
	float factor[4];
	factor[0] = 0.5f;
	factor[1] = 0.5f;
	factor[2] = 0.5f;
	factor[3] = 0.5f;

	if (flag == true)
		(*(ID3D11DeviceContext**)GetDeviceContext())->OMSetBlendState(m_alphaBlendEnabled, factor, 0xffffffff);
	else
		(*(ID3D11DeviceContext**)GetDeviceContext())->OMSetBlendState(0, 0, 0xffffffff);


	return SystemCodes::SUCCESS;
}


ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_RasterizerSetMode(EUINT32 flag)
{
	// cull none
	if ((flag & PF_GI_RS_CULL_NONE) != 0)
	{
		if ((flag & PF_GI_RS_MODE_SCISSORS) != 0)
		{
			if ((flag & PF_GI_RS_MODE_WIREFRAME) != 0)
				(*(ID3D11DeviceContext**)GetDeviceContext())->RSSetState(m_rasterCullNoneWireframeScissors);
			else if ((flag & PF_GI_RS_MODE_SOLID) != 0)
				(*(ID3D11DeviceContext**)GetDeviceContext())->RSSetState(m_rasterCullNoneSolidScissors);
			else
				(*(ID3D11DeviceContext**)GetDeviceContext())->RSSetState(m_rasterCullNoneSolidScissors);
		}
		else
		{
			if ((flag & PF_GI_RS_MODE_WIREFRAME) != 0)
				(*(ID3D11DeviceContext**)GetDeviceContext())->RSSetState(m_rasterCullNoneWireframe);
			else if ((flag & PF_GI_RS_MODE_SOLID) != 0)
				(*(ID3D11DeviceContext**)GetDeviceContext())->RSSetState(m_rasterCullNoneSolid);
			else
				(*(ID3D11DeviceContext**)GetDeviceContext())->RSSetState(m_rasterCullNoneSolid);
		}

	}
	else
		if ((flag & PF_GI_RS_CULL_CLOCKWISE) != 0)
		{
			if ((flag & PF_GI_RS_MODE_SCISSORS) != 0)
			{
				if ((flag & PF_GI_RS_MODE_WIREFRAME) != 0)
					(*(ID3D11DeviceContext**)GetDeviceContext())->RSSetState(m_rasterCullClockwiseWireframeScissors);
				else if ((flag & PF_GI_RS_MODE_SOLID) != 0)
					(*(ID3D11DeviceContext**)GetDeviceContext())->RSSetState(m_rasterCullClockwiseSolidScissors);
				else
					(*(ID3D11DeviceContext**)GetDeviceContext())->RSSetState(m_rasterCullClockwiseSolidScissors);
			}
			else
			{
				if ((flag & PF_GI_RS_MODE_WIREFRAME) != 0)
					(*(ID3D11DeviceContext**)GetDeviceContext())->RSSetState(m_rasterCullClockwiseWireframe);
				else if ((flag & PF_GI_RS_MODE_SOLID) != 0)
					(*(ID3D11DeviceContext**)GetDeviceContext())->RSSetState(m_rasterCullClockwiseSolid);
				else
					(*(ID3D11DeviceContext**)GetDeviceContext())->RSSetState(m_rasterCullClockwiseSolid);
			}

		}
		else
			if ((flag & PF_GI_RS_CULL_COUNTERCLOCKWISE) != 0)
			{
				if ((flag & PF_GI_RS_MODE_SCISSORS) != 0)
				{
					if ((flag & PF_GI_RS_MODE_WIREFRAME) != 0)
						(*(ID3D11DeviceContext**)GetDeviceContext())->RSSetState(m_rasterCullCounterClockwiseWireframeScissors);
					else if ((flag & PF_GI_RS_MODE_SOLID) != 0)
						(*(ID3D11DeviceContext**)GetDeviceContext())->RSSetState(m_rasterCullCounterClockwiseSolidScissors);
					else
						(*(ID3D11DeviceContext**)GetDeviceContext())->RSSetState(m_rasterCullCounterClockwiseSolidScissors);
				}
				else
				{
					if ((flag & PF_GI_RS_MODE_WIREFRAME) != 0)
						(*(ID3D11DeviceContext**)GetDeviceContext())->RSSetState(m_rasterCullCounterClockwiseWireframe);
					else if ((flag & PF_GI_RS_MODE_SOLID) != 0)
						(*(ID3D11DeviceContext**)GetDeviceContext())->RSSetState(m_rasterCullCounterClockwiseSolid);
					else
						(*(ID3D11DeviceContext**)GetDeviceContext())->RSSetState(m_rasterCullCounterClockwiseSolid);
				}

			}

	return SystemCodes::SUCCESS;
}




ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_StencilSetState(EUINT32 flag)
{
	if ((flag & PF_GI_STENCIL_GREATER) != 0)
	{
		(*(ID3D11DeviceContext**)GetDeviceContext())->OMSetDepthStencilState(m_stencil_compGreater, 1);
	}
	else
	{
		(*(ID3D11DeviceContext**)GetDeviceContext())->OMSetDepthStencilState(m_stencil_default, 1);
	}

	//
	// reset the current render targets


	return SystemCodes::SUCCESS;
}



ParabellumFramework::EUINT32
ParabellumFramework::Graphics::DeviceDirectX11::Device_BlendSetState(EUINT32 flag)
{
	float factor[4];
	factor[0] = 0.5f;
	factor[1] = 0.5f;
	factor[2] = 0.5f;
	factor[3] = 0.5f;


	if (flag == PF_GI_BLEND_ALPHA_INV)
	{
		(*(ID3D11DeviceContext**)GetDeviceContext())->OMSetBlendState(m_alphaBlendEnabled, factor, 0xffffffff);
	}
	else if (flag == PF_GI_BLEND_COLOR_ADD)
	{
		(*(ID3D11DeviceContext**)GetDeviceContext())->OMSetBlendState(m_colorBlendEnabled, 0, 0xffffffff);
	}
	else if (flag == PF_GI_BLEND_COLOR_SUB)
	{
		(*(ID3D11DeviceContext**)GetDeviceContext())->OMSetBlendState(m_colorBlendEnabled, 0, 0xffffffff);
	}
	else if(flag == PF_GI_BLEND_ALPHA_NONE)
	{
		(*(ID3D11DeviceContext**)GetDeviceContext())->OMSetBlendState(0, 0, 0xffffffff);
	}
	else
	{
		(*(ID3D11DeviceContext**)GetDeviceContext())->OMSetBlendState(0, 0, 0xffffffff);
	}

	return SystemCodes::SUCCESS;
}