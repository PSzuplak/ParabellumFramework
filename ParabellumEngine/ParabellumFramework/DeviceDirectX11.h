#ifndef _DEVICEDIRECTX11_H_
#define _DEVICEDIRECTX11_H_


//
//	Predefs
//
#include "DLLIE.h"

//
//	Libs
//
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

//
//	Includes
//
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>

#include "GraphicsDevice.h"

#include <iostream>

namespace ParabellumFramework
{
	namespace Graphics
	{

		//
		//	DX device
		//
	 class XYZ_API DeviceDirectX11 : public GraphicsDevice
	{
		public:
			DeviceDirectX11();
			DeviceDirectX11(const DeviceDirectX11&) = delete;
			~DeviceDirectX11();



			void Device_SwapBuffers();

			EUINT32 SetDefaultRenderTarget();

		private:
			EUINT32 Device_Initialize(int screenWidth, int screenHeight, void* hwnd, EUINT32 settings);
			EUINT32 Device_Release();
			EUINT32 Device_Update();

			PF_INLINE void Device_Render(EUINT64 indicesCount);

			EUINT32 Device_CreateVertexBuffer(_IO_ void* ppOutputBuffer, _IN_ void* pVertices, EUINT32 vertexSize, EUINT32 verticesCount);
			EUINT32 Device_ReleaseVertexBuffer(_IO_ void* ppBuffer);
			EUINT32 Device_CreateIndexBuffer(_IO_ void* ppOutputBuffer, _IN_ void* pIndices, EUINT64 indicesCount);
			EUINT32 Device_ReleaseIndexBuffer(_IO_ void*  ppIndexBuffer);
			PF_INLINE EUINT32 Device_SetBuffers(_IN_ void* ppVertexBuffer, EUINT32 vertexSize, _IN_ void* ppIndexBuffer);
			PF_INLINE void Device_SetVertexBuffer(_IN_ void* ppVertexBuffer, EUINT32 vertexSize);
			PF_INLINE void Device_SetIndexBuffer(_IN_ void* ppIndexBuffer);

			// -------------------------------------------------------------------------------
			// Methods-Shader
			// -------------------------------------------------------------------------------
			
			EUINT32 Device_LoadVertexShader(
				_OUT_ void* ppShader, const char* filename, const char* functionName,
				_IN_ ShaderLayout* pInputLayout, EUINT32 layoutFieldsCount, _OUT_ void* ppLayout);
			EUINT32 Device_LoadPixelShader(_OUT_ void* ppShader, const char* filename, const char* functionName);
			EUINT32 Device_ReleaseVertexShader(_OUT_ void* pShader);
			EUINT32 Device_ReleasePixelShader(_OUT_ void* pShader);
			EUINT32 Device_SetVertexShader(_IN_ void* pShader);
			EUINT32 Device_SetPixelShader(_IN_ void* pShader);

			EUINT32 Device_SetInputLayout(_IN_ void* pLayout);
			
			// -------------------------------------------------------------------------------
			// Methods-CBuffer
			// -------------------------------------------------------------------------------

		private:

			EUINT32 Device_CreateCBuffer(_OUT_ void* ppBuffer, EUINT32 bufferSize);
			PF_INLINE EUINT32 Device_SetCBuffer(_IN_ void* ppBuffer, EUINT32 offset, _IN_ void* pIn, EUINT32 inputSize);
			PF_INLINE EUINT32 Device_SetCBuffer(_IN_ void* ppBuffer, _IN_ ShaderParametr** ppParametrs, EUINT32 parametrsCount);
			PF_INLINE EUINT32 Device_SetCBuffer(_IN_ void* ppBuffer, _IN_ void* pParametrsBuffer, EUINT32 bufferSize);
			EUINT32 Device_ReleaseCBuffer(_IN_ void* ppBuffer);

			//
			// Set resources on graphics card shader.
			// ppParametrs - sorted array with resources (basicly Shader->m_sortedPointersToResources)
			//
			EUINT32 Device_SetResources(_IN_ ShaderParametr** ppParametrs, EUINT32 parametrsCount);


			EUINT32 Device_CreateResource(_OUT_ void* ppBuffer, _IN_ const char* filename);
			EUINT32 Device_LoadTexture(_OUT_ void* ppBuffer, _IN_ const char* filename);
			EUINT32 Device_CreateTexture(
				_IN_ void* pTextureData, EUINT32 width, EUINT32 height, GraphicsFormat format, _OUT_ void* ppBuffer, _OUT_ void* ppBufferType);
			EUINT32 Device_ReleaseTexture(_IO_ void* ppBuffer, _IO_ void* ppFormat);

			virtual EUINT32 Device_CreateSampler(
				_OUT_ void* ppBuffer,
				SamplerFilter filter,
				SamplerAddress addressU,
				SamplerAddress addressV,
				SamplerAddress addressW,
				EFLOAT32 mipLODBias,
				EUINT32 maxAnisotropy,
				SamplerComparsion comparisonFunc);

			EUINT32 Device_SetSampler(_IN_ void* ppBuffer, EUINT32 regNo);
			
			EUINT32 Device_ReleaseSampler(_IN_ void* ppBuffer);

			EUINT32 Device_SamplerSetDefault(EUINT32 regNo, SamplerType type);
			
			EUINT32 Device_CreateRenderTarget(
				_OUT_ void* ppBufferTexture, _OUT_ void* ppBufferView, _OUT_ void* ppBufferResource,
				GraphicsFormat textureFormat, EUINT32 width, EUINT32 height);

			EUINT32 Device_ReleaseRenderTarget(_IO_ void* ppBufferTexture, _IO_ void* ppBufferView, _IO_ void* ppBufferResource);

			EUINT32 Device_CreateDepthBuffer(
				_OUT_ void* ppDepthBuffer, void* ppDepthView,
				EUINT32 textureFormat, EUINT32 width, EUINT32 height);

			EUINT32 Device_SetRenderTarget(
				EUINT32 co, _IN_ void* pViewportBuffer,
				_IN_ void* pRTBufferFormat, _IN_ void* pDepthBufferFormat, EFLOAT32 depthValue
				);

			EUINT32 Device_SetRenderTargets(
				EUINT32 count, _IN_ void* pViewportBuffer,
				_OUT_ void** ppRTBufferFormat, _OUT_ void** ppDepthBufferFormat,
				_OUT_ void* pDepthStencilView, EBOOL clearDepth, EFLOAT32 depthValue
				);

			// set depth buffer to some value
			EUINT32 Device_RenderTargetsSetDepth(_OUT_ void* pDepthStencilView, EFLOAT32 depthValue);


			// -------------------------------------------------------------------------------
			//Rasterizer functions
			// -------------------------------------------------------------------------------

			// Turn on/off dept buffer - comparing depths
			EUINT32 Device_TurnDepthBuffer(bool flag);

			// Turn on/ff Alpha Blending
			EUINT32 Device_TurnAlphaBlending(bool flag);

			// Set Rasterizer Mode using defines PF_GI_*
			EUINT32 Device_RasterizerSetMode(EUINT32 flag);

			// Set Stencil State
			EUINT32 Device_StencilSetState(EUINT32 flag);

			// Set Blending State
			EUINT32 Device_BlendSetState(EUINT32 flag);

		public:
			bool m_vsync_enabled;
			int m_videoMemorySize;


			IDXGISwapChain* m_swapChain;
			IDXGISwapChain* m_swapChainCurrent;			// current swap chain

			ID3D11RenderTargetView* m_renderTargetView;
			ID3D11Texture2D* m_depthStencilBuffer;
			ID3D11DepthStencilState* m_stencil_default;
			ID3D11DepthStencilState* m_depthDisabledStencilState;
			ID3D11DepthStencilState* m_stencil_compGreater;

			ID3D11DepthStencilView* m_depthStencilView;


			//
			// rasterizer states
			ID3D11RasterizerState* m_rasterState;
			ID3D11RasterizerState* m_rasterWireframe;
			ID3D11RasterizerState* m_rasterCullNone;
			ID3D11RasterizerState* m_rasterCullClockwise;
			ID3D11RasterizerState* m_rasterCullCounterClockwise;

			ID3D11RasterizerState* m_rasterCullNoneWireframe;
			ID3D11RasterizerState* m_rasterCullNoneSolid;
			ID3D11RasterizerState* m_rasterCullClockwiseWireframe;
			ID3D11RasterizerState* m_rasterCullClockwiseSolid;
			ID3D11RasterizerState* m_rasterCullCounterClockwiseWireframe;
			ID3D11RasterizerState* m_rasterCullCounterClockwiseSolid;

			ID3D11RasterizerState* m_rasterCullNoneWireframeScissors;
			ID3D11RasterizerState* m_rasterCullNoneSolidScissors;
			ID3D11RasterizerState* m_rasterCullClockwiseWireframeScissors;
			ID3D11RasterizerState* m_rasterCullClockwiseSolidScissors;
			ID3D11RasterizerState* m_rasterCullCounterClockwiseWireframeScissors;
			ID3D11RasterizerState* m_rasterCullCounterClockwiseSolidScissors;

			// 
			// blending
			ID3D11BlendState* m_alphaBlendEnabled;
			ID3D11BlendState* m_blendDisable;

			ID3D11BlendState* m_colorBlendEnabled;
			ID3D11BlendState* m_colorBlendDisable;


			//
			// default samplers
			ID3D11SamplerState* m_samplerPoint;
			ID3D11SamplerState* m_samplerLinear;
			ID3D11SamplerState* m_samplerAnisotropic;

			//
			// viewport
			D3D11_VIEWPORT m_viewport;

			//
			//
			//

			ID3D11Texture2D* m_texture1x1;
			ID3D11Texture2D* m_texture16x16;
			ID3D11Texture2D* m_texture32x32;
			ID3D11Texture2D* m_texture64x64;
			ID3D11Texture2D* m_texture128x128;
			ID3D11Texture2D* m_texture256x256;

			ID3D11Texture2D* m_texture64x512;
			ID3D11Texture2D* m_texture512x512;


			ID3D11Texture2D* m_texture1920x1080;
			ID3D11Texture2D* m_texture2560x1440;


		public:
			//
			void GetResourceData1x1(EUINT32 X, EUINT32 Y, _IN_ RenderTarget* pRenderTarget, _OUT_ Vector4D* out)
			{
				D3D11_BOX SrcBox;
				SrcBox.left = X;
				SrcBox.right = X + 1;
				SrcBox.top = Y;
				SrcBox.bottom = Y + 1;
				SrcBox.front = 0;
				SrcBox.back = 1;

				((*(ID3D11DeviceContext**)GetDeviceContext()))->CopySubresourceRegion(m_texture1x1, 0, 0, 0, 0, ((ID3D11Resource*)pRenderTarget->m_rtBufferTexture), 0, &SrcBox);

				D3D11_MAPPED_SUBRESOURCE MappingDesc;
				((*(ID3D11DeviceContext**)GetDeviceContext()))->Map(m_texture1x1, 0, D3D11_MAP_READ, 0, &MappingDesc);

				if (MappingDesc.pData == NULL)
					printf("\n#ERROR DXDevice->GetResourceData");
				else
					memcpy(out, MappingDesc.pData, sizeof(Vector4D));
	
				((*(ID3D11DeviceContext**)GetDeviceContext()))->Unmap(m_texture1x1, 0);
			}

			//
			void GetResourceData512x512(EUINT32 sx, EUINT32 sy, EUINT32 width, EUINT32 height, _IN_ RenderTarget* pRenderTarget, _OUT_ Vector4D* out)
			{
				D3D11_BOX SrcBox;
				SrcBox.left = sx;
				SrcBox.right = sx + width;
				SrcBox.top = sy;
				SrcBox.bottom = sy + height;
				SrcBox.front = 0;
				SrcBox.back = 1;

				//printf("\nRect = %u, %u, %u, %u", SrcBox.left, SrcBox.right, SrcBox.top, SrcBox.bottom);

				((*(ID3D11DeviceContext**)GetDeviceContext()))->CopySubresourceRegion(m_texture64x512, 0, 0, 0, 0, ((ID3D11Resource*)pRenderTarget->m_rtBufferTexture), 0, &SrcBox);

				D3D11_MAPPED_SUBRESOURCE MappingDesc;
				((*(ID3D11DeviceContext**)GetDeviceContext()))->Map(m_texture64x512, 0, D3D11_MAP_READ, 0, &MappingDesc);

				if (MappingDesc.pData == NULL)
				{
					printf("\n#ERROR DXDevice->GetResourceData512x512");
				}
				else
				{
					memcpy(out, MappingDesc.pData, width * height * sizeof(Vector4D));

				}

				((*(ID3D11DeviceContext**)GetDeviceContext()))->Unmap(m_texture64x512, 0);
			}

			//
			void GetResourceData512x512(_IN_ RenderTarget* pRenderTarget, _OUT_ Vector4D* out)
			{
				D3D11_BOX SrcBox;
				SrcBox.left = 0;
				SrcBox.right = 512;
				SrcBox.top = 0;
				SrcBox.bottom = 512;
				SrcBox.front = 0;
				SrcBox.back = 1;

				((*(ID3D11DeviceContext**)GetDeviceContext()))->CopySubresourceRegion(m_texture512x512, 0, 0, 0, 0, ((ID3D11Resource*)pRenderTarget->m_rtBufferTexture), 0, &SrcBox);

				D3D11_MAPPED_SUBRESOURCE MappingDesc;
				((*(ID3D11DeviceContext**)GetDeviceContext()))->Map(m_texture512x512, 0, D3D11_MAP_READ, 0, &MappingDesc);

				if (MappingDesc.pData == NULL)
					printf("\n#ERROR DXDevice->GetResourceData512x512");
				else
				{
					memcpy(out, MappingDesc.pData, 512 * 512 * sizeof(Vector4D));
				}

				((*(ID3D11DeviceContext**)GetDeviceContext()))->Unmap(m_texture512x512, 0);
			}

			//
			void GetResourceData1920x1080(_IN_ RenderTarget* pRenderTarget, _OUT_ Vector4D* out)
			{
				D3D11_BOX SrcBox;
				SrcBox.left = 0;
				SrcBox.right = 1920;
				SrcBox.top = 0;
				SrcBox.bottom = 1080;
				SrcBox.front = 0;
				SrcBox.back = 1;

				((*(ID3D11DeviceContext**)GetDeviceContext()))->CopySubresourceRegion(m_texture1920x1080, 0, 0, 0, 0, ((ID3D11Resource*)pRenderTarget->m_rtBufferTexture), 0, &SrcBox);

				D3D11_MAPPED_SUBRESOURCE MappingDesc;
				((*(ID3D11DeviceContext**)GetDeviceContext()))->Map(m_texture1920x1080, 0, D3D11_MAP_READ, 0, &MappingDesc);

				if (MappingDesc.pData == NULL)
					printf("\n#ERROR DXDevice->GetResourceData");
				else
					memcpy(out, MappingDesc.pData, 1920 * 1080 * sizeof(Vector4D));

				((*(ID3D11DeviceContext**)GetDeviceContext()))->Unmap(m_texture1920x1080, 0);
			}

			//
			void GetResourceData2560x1440(_IN_ RenderTarget* pRenderTarget, _OUT_ Vector4D* out)
			{
				D3D11_BOX SrcBox;
				SrcBox.left = 0;
				SrcBox.right = 2560;
				SrcBox.top = 0;
				SrcBox.bottom = 1440;
				SrcBox.front = 0;
				SrcBox.back = 1;

				((*(ID3D11DeviceContext**)GetDeviceContext()))->CopySubresourceRegion(m_texture2560x1440, 0, 0, 0, 0, ((ID3D11Resource*)pRenderTarget->m_rtBufferTexture), 0, &SrcBox);

				D3D11_MAPPED_SUBRESOURCE MappingDesc;
				((*(ID3D11DeviceContext**)GetDeviceContext()))->Map(m_texture2560x1440, 0, D3D11_MAP_READ, 0, &MappingDesc);

				if (MappingDesc.pData == NULL)
					printf("\n#ERROR DXDevice->GetResourceData");
				else
					memcpy(out, MappingDesc.pData, 2560 * 1440 * sizeof(Vector4D));

				((*(ID3D11DeviceContext**)GetDeviceContext()))->Unmap(m_texture2560x1440, 0);
			}

			//
			void GetResourceData(EUINT32 X, EUINT32 Y, EUINT32 width, EUINT32 height, _IN_ RenderTarget* pRenderTarget, _OUT_ Vector4D* out)
			{
				D3D11_BOX SrcBox;
				SrcBox.left = X;
				SrcBox.right = X + width;
				SrcBox.top = Y;
				SrcBox.bottom = Y + height;
				SrcBox.front = 0;
				SrcBox.back = 1;

				((*(ID3D11DeviceContext**)GetDeviceContext()))->CopySubresourceRegion(m_texture32x32, 0, 0, 0, 0, ((ID3D11Resource*)pRenderTarget->m_rtBufferTexture), 0, &SrcBox);
	
				D3D11_MAPPED_SUBRESOURCE MappingDesc;
				((*(ID3D11DeviceContext**)GetDeviceContext()))->Map(m_texture32x32, 0, D3D11_MAP_READ, 0, &MappingDesc);

				if (MappingDesc.pData == NULL)
					printf("\n#ERROR DXDevice->GetResourceData");
				else
					memcpy(out, MappingDesc.pData, width * height * sizeof(Vector4D));

				((*(ID3D11DeviceContext**)GetDeviceContext()))->Unmap(m_texture32x32, 0);
			}

		};
	}
};


#endif