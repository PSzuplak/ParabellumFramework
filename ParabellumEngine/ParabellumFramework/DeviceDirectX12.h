#ifndef _DEVICEDIRECTX12_H_
#define _DEVICEDIRECTX12_H_

//
//	Predefs
//
#include "DLLIE.h"


/*

//
//	Predefs
//
#include "DLLIE.h"

//
//	Libs
//
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

//
//	Includes
//
#include <d3d12.h>
#include <dxgi1_4.h>

#include "GraphicsDevice.h"


namespace ParabellumFramework
{
	namespace Graphics
	{

		//
		//	GraphicsDevice - interface which can handle DirectX, OpenGL or others
		//
	 class XYZ_API DeviceDirectX12 : public GraphicsDevice
		{
		public:
			DeviceDirectX12();
			DeviceDirectX12(const DeviceDirectX12&);
			~DeviceDirectX12();

			//
			// Common functions
			//

			EUINT32 Initialize(int screenHeight, int screenWidth, void* hwnd, EUINT32 settings);
			EUINT32 Release();
			EUINT32 Draw();
			EUINT32 Update();
			

			//
			//	Wrapped functions
			//

			//
			// Methods-Rendering
			//
			EUINT32 RenderBuffers(_IN_ VertexBuffer* pVertexBuffer, _IN_ IndexBuffer* pIndexBuffer);


			//
			// Methods-IndexBuffer
			//


			//
			// Methods-VertexBuffer
			//


			//
			// Methods-Shader
			//
			Shader* CreateShader();
			EUINT32 ReleaseShader(_IO_ Shader* pShader);

			//
			// Methods-RenderTarget
			//

			RenderTarget* CreateRenderTarget(EUINT32 width, EUINT32 height, EUINT32 format, EUINT32 settings = 0);
			EUINT32 ReleaseRenderTarget(_IO_ RenderTarget* pRenderTarget);

			void SetDefaultRenderTarget(bool clearDepth = true);
			void SetRenderTargets(_IN_ RenderTarget* pRenderTarget, void* pDepthStencilView, bool clearDepth = true);
			void SetRenderTargets(_IN_ RenderTarget* pRenderTarget0, _IN_ RenderTarget* pRenderTarget1, void* pDepthStencilView, bool clearDepth = true);
			void SetRenderTargets(_IN_ RenderTarget* pRenderTarget0, _IN_ RenderTarget* pRenderTarget1, _IN_ RenderTarget* pRenderTarget2, _IN_ void* pDepthStencilView, bool clearDepth = true);
			void SetRenderTargets(_IN_ RenderTarget* pRenderTarget0, _IN_ RenderTarget* pRenderTarget1, _IN_ RenderTarget* pRenderTarget2, _IN_ RenderTarget* pRenderTarget3, void* pDepthStencilView, bool clearDepth = true);

			void ClearCurrentRenderTargets(EFLOAT32 r, EFLOAT32 g, EFLOAT32 b, EFLOAT32 a);

			//
			// Methods-Rasterizer
			//
			EUINT32 RasterizerSetMode(EUINT32 flag);
			EUINT32 RasterizerSetModeWireframe();
			EUINT32 RasterizerSetCullNone();
			EUINT32 RasterizerSetCullClockwise();
			EUINT32 RasterizerSetCullCounterClockwise();


		private:
			bool m_vsync_enabled;
			ID3D12Device* m_device;
			ID3D12CommandQueue* m_commandQueue;
			char m_videoCardDescription[128];
			IDXGISwapChain3* m_swapChain;
			ID3D12DescriptorHeap* m_renderTargetViewHeap;
			ID3D12Resource* m_backBufferRenderTarget[2];
			unsigned int m_bufferIndex;
			ID3D12CommandAllocator* m_commandAllocator;
			ID3D12GraphicsCommandList* m_commandList;
			ID3D12PipelineState* m_pipelineState;
			ID3D12Fence* m_fence;
			HANDLE m_fenceEvent;
			unsigned long long m_fenceValue;

		};

	}


}




*/



#endif