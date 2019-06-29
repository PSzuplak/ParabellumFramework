#ifndef _GRAPHICSDEVICE_H_
#define _GRAPHICSDEVICE_H_


//
//	Predefs
//
#include "DLLIE.h"

//
//	includes STD
//
#include <list>

//
//	includes ParabellumFramework
//
#include "MemoryManager.h"
#include "Convenctions.h"
#include "TypesC.h"
#include "SystemCodes.h"
#include "GraphicsTypes.h"
#include "Settings.h"
#include "DebugManager.h"
#include "Profiler.h"

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "ShaderParametr.h"
#include "Texture2D.h"
#include "RenderTarget.h"

#include "GraphicsCommands.h"
#include "GraphicsQueue.h"

//
// Statuses
//

#define GRAPHICS_STATUS_LOCKED EBYTENO_3				// if set then the Device functions are not avialable



namespace ParabellumFramework
{

	namespace Graphics
	{
		//
		// Interface/Manager which handles DirectX, OpenGL. 
		// A factory and manager which creates and manages graphics related data
		// Child classes have to implement Device_* functions
		//
		class XYZ_API GraphicsDevice //: public ProfilerUnit<ProfileGraphicsDevice>
		{
			// friendships
			friend class Resources::ResourceManager;


			// the status of the all Graphics Devices - uses GRAPHICS_STATUS_* values
			static EUINT32 m_deviceStatus;


		protected:
			GraphicsDevice();
			GraphicsDevice(const GraphicsDevice& ref);

		public:
			virtual ~GraphicsDevice();

		public:
			//
			// Common functions
			//

			//
			// Initialize screen
			// screenWidth - width of the screen
			// screenHeight - height of the screen
			// void* hwnd - may be used as HWND hwnd if widow was created in WinApi
			// settings - 32bit flag with settings
			//
			virtual EUINT32 Initialize(EUINT32 screenWidth, EUINT32 screenHeight, void* hwnd, EUINT32 settings) final;

			//
			// Release ALL objects (vertex/index buffers, shaders etc.) created by GraphicsDevice (if the weren't released explictly)
			//
			virtual EUINT32 Release() final;

			//
			// Update -
			//
			virtual EUINT32 Update() final;

		public:


			// ======================================================================================================
			//	
			// ======================================================================================================

			PF_INLINE virtual void SwapBuffers() final;

			PF_INLINE virtual void Device_SwapBuffers() = 0;

			// params PF_GI_RENDERING_UPDATE_*
			PF_INLINE virtual void Render(EUINT32 settings = (PF_GI_RENDERING_UPDATEVARIABLES | PF_GI_RENDERING_UPDATERESOURCES) ) final;

			PF_INLINE virtual void RenderRAW() final;

			// Copy memory for CBuffer onto GraphicsCard - must be called before 'Render'
			// cbufferNo - number of CBuffer
			PF_INLINE virtual void SetShaderVariables(EUINT32 cbufferNo = 0) final;

			// Set shader resources on device. Shaders Resources must be set by Shader::GetResource(0)->Set(...) first
			// Shader MUST be set firs by GraphicsDevice::SetShader(...)
			PF_INLINE virtual void SetShaderResources() final;


			// ======================================================================================================
			//	Interface fnctions - wrapping grahics device functions
			// ======================================================================================================

			//
			// Create VertexBuffer
			// pBuffer - pointer to vertices
			// vertexSize - size of ONE element in pBuffer
			// verticesCount - number of elements in pBuffer
			//
			virtual VertexBuffer* CreateVertexBuffer(_IN_ void* pBuffer, EUINT32 vertexSize, EUINT32 verticesCount) final;

			//
			// Create VertexBuffer - element T must be a single data struct (must not have ponters to external data)
			// pBuffer - pointer to vertices
			// verticesCount - number of elements in pBuffer
			//
			template<typename T>
			VertexBuffer* CreateVertexBuffer(_IN_ T* pBuffer, EUINT32 verticesCount)
			{
				EUINT32 result = SystemCodes::SUCCESS;

				// memory allocation
				VertexBuffer* vb = new VertexBuffer(sizeof(T), verticesCount);

				if (vb == NULLPTR)
					return vb;

				result = Device_CreateVertexBuffer(&vb->m_buffer, pBuffer, sizeof(T), verticesCount);

				if (result == SystemCodes::SUCCESS)
				{
					m_vertexBuffers.push_back(vb);
					return vb;
				}
				else
				{
					delete vb;
					return NULLPTR;
				}

			}


			//
			// Release VertexBuffer
			//
			virtual EUINT32 ReleaseVertexBuffer(_IO_ VertexBuffer* pVertexBuffer) final;



			//
			// Create IndexBuffer
			// ppBuffer - pointer to indices (EUINT32 elements)
			// indicesCount - number of elements in pBuffer
			//
			virtual IndexBuffer* CreateIndexBuffer(_IN_ void* pBuffer, EUINT64 indicesCount) final;


			//
			// Release IndexBuffer
			// pIndexBuffer -
			//
			virtual EUINT32 ReleaseIndexBuffer(_IO_ IndexBuffer* pIndexBuffer) final;



			// set vertex and index buffers on graphics device
			// pVB - 
			// vertexSize -
			// pIndexBuffer -
			PF_INLINE virtual EUINT32 SetBuffers(_IN_ VertexBuffer* pVB, _IN_ IndexBuffer* pIndexBuffer) final;

			//
			// Set Vertex Buffer
			//
			PF_INLINE virtual void SetVertexBuffer(_IN_ VertexBuffer* pVB) final;

			//
			// Set Index Buffer
			//
			PF_INLINE virtual void SetIndexBuffer(_IN_ IndexBuffer* pIB) final;



		//
		// Load shader from file 
		//
		private:
		
			// initialize Shader on graphics device
			virtual EUINT32 InitializeShader(_IO_ Shader* pShader) final;


		public:

			//
			// Release shader
			//

			virtual EUINT32 ReleaseShader(_IO_ Shader* pShader) final;

			//
			// SetShader - set shader on graphics card
			//
			PF_INLINE virtual EUINT32 SetShader(_IO_ Shader* pShader, ShaderFlagSet renderSettings = ShaderFlagSet::ALL) final;




			// -------------------------------------------------------------------------------
			// Methods-Textures and Samplers and Render Targets and 
			// -------------------------------------------------------------------------------

	
			//
			// Create Texture2D using data from pTexture
			//
			virtual EUINT32 CreateTexture(_IO_ Texture2D* pTexture) final;

			//
			// Release Texture2D
			//
			virtual EUINT32 ReleaseTexture(_IO_ Texture2D* pTexture) final;


			virtual EUINT32 CreateSampler(_IN_ ShaderSamplerState* pSampler) final;


			// Set default sampler for fegister registers S(x)
			PF_INLINE virtual EUINT32 SetSampler(EUINT32 regNo, SamplerType typy) final;

			// Set user defined sampler
			PF_INLINE virtual EUINT32 SetSampler(EUINT32 regNo) final;

			virtual RenderTarget* CreateRenderTarget(
				GraphicsFormat RTFormat, GraphicsFormat DepthFormat, EUINT32 width, EUINT32 height) final;

			virtual EUINT32 ReleaseRenderTarget(_IN_ RenderTarget* pRT) final;

			// set default render target
			virtual EUINT32 SetDefaultRenderTarget() = 0;

			// Set Render Target. Also set DepthBuffer assocated in RenderTarget object
			virtual EUINT32 SetRenderTarget(EUINT32 No, _IN_ RenderTarget* RT) final;

			// Set 1 RenderTargets. Depth Buffer will be get from RTDepth.
			virtual EUINT32 SetRenderTargets(
				_OUT_ RenderTarget* RT0,
				_OUT_ RenderTarget* RTDepth, EBOOL clearDepth = true) final;

			// Set 2 RenderTargets. Depth Buffer will be get from RTDepth.
			virtual EUINT32 SetRenderTargets(
				_OUT_ RenderTarget* RT0, _OUT_ RenderTarget* RT1,
				_OUT_ RenderTarget* RTDepth, EBOOL clearDepth = true) final;

			// Set 3 RenderTargets. Depth Buffer will be get from RTDepth.
			virtual EUINT32 SetRenderTargets(
				_OUT_ RenderTarget* RT0, _OUT_ RenderTarget* RT1, _OUT_ RenderTarget* RT2,
				_OUT_ RenderTarget* RTDepth, EBOOL clearDepth = true) final;

			// Set 4 RenderTargets. Depth Buffer will be get from RTDepth.
			virtual EUINT32 SetRenderTargets(
				_OUT_ RenderTarget* RT0, _OUT_ RenderTarget* RT1, _OUT_ RenderTarget* RT2, _OUT_ RenderTarget* RT3,
				_OUT_ RenderTarget* RTDepth, EBOOL clearDepth = true) final;


			// -------------------------------------------------------------------------------
			//	Rasterizer functions
			// -------------------------------------------------------------------------------

			// Turn on/off dept buffer - comparing depths
			virtual EUINT32 TurnDepthBuffer(bool flag) final;

			// Turn on/ff Alpha Blending
			virtual EUINT32 TurnAlphaBlending(bool flag) final;

			// Set Rasterizer Mode using defines PF_GI_*
			virtual EUINT32 RasterizerSetMode(EUINT32 flag) final;

			// Set Stencil State
			virtual EUINT32 StencilSetState(EUINT32 flag) final;

			// set blending mode
			virtual EUINT32 BlendSetState(EUINT32 flag) final;


			// -------------------------------------------------------------------------------
			//	Data helper
			// -------------------------------------------------------------------------------

			//
			virtual void GetResourceData(_IN_ RenderTarget* pResource, _IN_ Vector2D& uv, _OUT_ Vector4D* pOut) final;




			// ======================================================================================================
			//	Queue functions
			// ======================================================================================================
		private:




			// ======================================================================================================
			//	Device functions
			// ======================================================================================================

		protected:

			virtual EUINT32 Device_Initialize(int screenWidth, int screenHeight, void* hwnd, EUINT32 settings) = 0;
			virtual EUINT32 Device_Release() = 0;
			virtual EUINT32 Device_Update() = 0;

			PF_INLINE virtual void Device_Render(EUINT64 indicesCount) = 0;


			// -------------------------------------------------------------------------------
			// Buffers
			// -------------------------------------------------------------------------------

			//
			// CreateVertexBuffer -
			//
			virtual EUINT32 Device_CreateVertexBuffer(_IO_ void* ppOutputBuffer, _IN_ void* pVertices, EUINT32 vertexSize, EUINT32 verticesCount) = 0;

			//
			// ReleaseVertexBuffer -
			//
			virtual EUINT32 Device_ReleaseVertexBuffer(_IO_ void* ppBuffer) = 0;

			//
			// CreateIndexBuffer -
			//
			virtual EUINT32 Device_CreateIndexBuffer(_IO_ void* ppOutputBuffer, _IN_ void* pIndices, EUINT64 indicesCount) = 0;

			//
			// ReleaseIndexBuffer -
			//
			virtual EUINT32 Device_ReleaseIndexBuffer(_IO_ void*  ppIndexBuffer) = 0;


			//
			// Set buffers on the graphics device
			//
			PF_INLINE virtual EUINT32 Device_SetBuffers(_IN_ void* ppVertexBuffer, EUINT32 vertexSize, _IN_ void* ppIndexBuffer) = 0;

			//
			// Set vertex buffer on the graphics device
			//
			PF_INLINE virtual void Device_SetVertexBuffer(_IN_ void* pVertexBuffer, EUINT32 vertexSize) = 0;

			//
			// Set index buffer on the graphics device
			//
			PF_INLINE virtual void Device_SetIndexBuffer(_IN_ void* pIndexBuffer) = 0;

			// -------------------------------------------------------------------------------
			// Methods-Shader
			// -------------------------------------------------------------------------------

			// load vertex shader from TXT file
			// pShader - set and used by graphic API (compiled shader)
			// filename - path to file with the shader
			// functionName - function to compile
			virtual EUINT32 Device_LoadVertexShader(
				_OUT_ void* ppShader, const char* filename, const char* functionName,
				_IN_ ShaderLayout* pInputLayout, EUINT32 layoutFieldsCount, _OUT_ void* ppLayout) = 0;

			// load pixel shader from TXT file
			// pShader - set and used by graphic API (compiled shader)
			// filename - path to file with the shader
			// functionName - function to compile
			virtual EUINT32 Device_LoadPixelShader(_OUT_ void* pShader, const char* filename, const char* functionName) = 0;


			//
			// release Vertex Shader
			//
			virtual EUINT32 Device_ReleaseVertexShader(_OUT_ void* pShader) = 0;

			//
			// release Vertex Shader
			//
			virtual EUINT32 Device_ReleasePixelShader(_OUT_ void* pShader) = 0;


			virtual EUINT32 Device_SetVertexShader(_IN_ void* pShader) = 0;
			virtual EUINT32 Device_SetPixelShader(_IN_ void* pShader) = 0;
			virtual EUINT32 Device_SetInputLayout(_IN_ void* ppLayout) = 0;

			// -------------------------------------------------------------------------------
			// Methods-CBuffer
			// -------------------------------------------------------------------------------

			private:

			// create CBuffer
			virtual EUINT32 Device_CreateCBuffer(_OUT_ void* ppBuffer, EUINT32 bufferSize) = 0;

			// set shader parametrs - for general purposes
			// ppBuffer - address of CBuffer
			// offset - offset in ppBufer where data should be copied
			// pIn - pointer to the data to transfer
			// size - how much bytes should be copied from pIn to ppBuffer
			PF_INLINE virtual EUINT32 Device_SetCBuffer(_IN_ void* ppBuffer, EUINT32 offset, _IN_ void* pIn, EUINT32 inputSize) = 0;

			// set shader parametrs
			PF_INLINE virtual EUINT32 Device_SetCBuffer(_IN_ void* ppBuffer, _IN_ ShaderParametr** ppParametrs, EUINT32 parametrsCount) = 0;

			// set shader parametrs stored in parametrsBuffer of size bufferSize
			PF_INLINE virtual EUINT32 Device_SetCBuffer(_IN_ void* ppBuffer, _IN_ void* pParametrsBuffer, EUINT32 bufferSize) = 0;

			

			// release CBuffer
			virtual EUINT32 Device_ReleaseCBuffer(_IN_ void* ppBuffer) = 0;


			// -------------------------------------------------------------------------------
			// Methods-Shader
			// -------------------------------------------------------------------------------

			// set shader resources
			virtual EUINT32 Device_SetResources(_IN_ ShaderParametr** ppParametrs, EUINT32 parametrsCount) = 0;

			virtual EUINT32 Device_CreateResource(_OUT_ void* ppBuffer, _IN_ const char* filename) = 0;

			virtual EUINT32 Device_LoadTexture(_OUT_ void* ppBuffer, _IN_ const char* filename) = 0;

			//
			// Create Texture using data located in memory
			// pTextureData - texture data
			// width
			// height
			// format
			// pBuffer - output pointer pointing to the resource located on graphics device
			//
			virtual EUINT32 Device_CreateTexture(
				_IN_ void* pTextureData, EUINT32 width, EUINT32 height, GraphicsFormat format, _OUT_ void* ppBuffer, _OUT_ void* ppBufferType) = 0;

			//
			// Release texture
			//
			virtual EUINT32 Device_ReleaseTexture(_IO_ void* ppBuffer, _IO_ void* ppFormat) = 0;



			virtual EUINT32 Device_CreateSampler(
					_OUT_ void* ppBuffer, 
					SamplerFilter filter,
					SamplerAddress addressU,
					SamplerAddress addressV,
					SamplerAddress addressW,
					EFLOAT32 mipLODBias,
					EUINT32 maxAnisotropy,
					SamplerComparsion comparisonFunc) = 0;

			
			virtual EUINT32 Device_SetSampler(_IN_ void* ppBuffer, EUINT32 regNo) = 0;

			virtual EUINT32 Device_ReleaseSampler(_IN_ void* ppBuffer) = 0;

			virtual EUINT32 Device_SamplerSetDefault(EUINT32 regNo, SamplerType type) = 0;

			virtual EUINT32 Device_CreateRenderTarget(
				_OUT_ void* ppBufferTexture, _OUT_ void* ppBufferView, _OUT_ void* ppBufferResource,
				GraphicsFormat textureFormat, EUINT32 width, EUINT32 height) = 0;

			virtual EUINT32 Device_ReleaseRenderTarget(_IO_ void* ppBufferTexture, _IO_ void* ppBufferView, _IO_ void* ppBufferResource) = 0;

			virtual EUINT32 Device_CreateDepthBuffer(
				_OUT_ void* ppDepthBuffer, void* ppDepthView,
				EUINT32 textureFormat, EUINT32 width, EUINT32 height) = 0;
			
			// set 1 RenderTarget
			virtual EUINT32 Device_SetRenderTarget(
				EUINT32 No, _IN_ void* pViewportBuffer,
				_IN_ void* pRTBufferFormat, _IN_ void* pDepthBufferFormat, EFLOAT32 depthValue
				) = 0;



			virtual EUINT32 Device_SetRenderTargets(
				EUINT32 Count, _IN_ void* pViewportBuffer,
				_IN_ void** ppRTBufferFormat, _IN_ void** ppDepthBufferFormat, 
				_OUT_ void* pDepthStencilView, EBOOL clearDepth, EFLOAT32 depthValue
				) = 0;

			// 
			// set depth buffer to some value
			virtual EUINT32 Device_RenderTargetsSetDepth(_OUT_ void* pDepthStencilView, EFLOAT32 depthValue) = 0;


			// -------------------------------------------------------------------------------
			//Rasterizer functions
			// -------------------------------------------------------------------------------

			// Turn on/off dept buffer - comparing depths
			virtual EUINT32 Device_TurnDepthBuffer(bool flag) = 0;

			// Turn on/ff Alpha Blending
			virtual EUINT32 Device_TurnAlphaBlending(bool flag) = 0;


			// Set Rasterizer Mode using defines PF_GI_*
			virtual EUINT32 Device_RasterizerSetMode(EUINT32 flag) = 0;

			// Set Stencil State
			virtual EUINT32 Device_StencilSetState(EUINT32 flag) = 0;

			// Set Blending State
			virtual EUINT32 Device_BlendSetState(EUINT32 flag) = 0;


			////////////////////////////////////////////////////////////
		public:

			// get data from RenderTarget - Get rectangle of size (x,y) to (x+width, y+height)
			// out represents float color 4 components - each 32bits
			virtual void GetResourceData1x1(EUINT32 X, EUINT32 Y, _IN_ RenderTarget* pRenderTarget, _OUT_ Vector4D* out) = 0;

			// get data from RenderTarget - Get rectangle of size (x,y) to (x+width, y+height)
			// out represents float color 4 components - each 32bits


			// read data from 512x512 texture - out must be 512x512 size.
			virtual void GetResourceData512x512(EUINT32 sx, EUINT32 sy, EUINT32 width, EUINT32 height, _IN_ RenderTarget* pRenderTarget, _OUT_ Vector4D* out) = 0;

			virtual void GetResourceData512x512(_IN_ RenderTarget* pRenderTarget, _OUT_ Vector4D* out) = 0;

			virtual void GetResourceData1920x1080(_IN_ RenderTarget* pRenderTarget, _OUT_ Vector4D* out) = 0;
			
			virtual void GetResourceData2560x1440(_IN_ RenderTarget* pRenderTarget, _OUT_ Vector4D* out) = 0;

			// get data from RenderTarget - Get rectangle of size (x,y) to (x+width, y+height)
			// out represents float color 4 components - each 32bits
			virtual void GetResourceData(EUINT32 X, EUINT32 Y, EUINT32 width, EUINT32 height, _IN_ RenderTarget* pResource, _OUT_ Vector4D* out) = 0;






		//
		// Members
		//
		private:


			// is GraphisDevice Locked
			EBOOL m_isLocked;


			// internal graphics card reference
			void* m_device;

			// internal graphics card context
			void* m_deviceContext;

			// internal graphics card instance
			void* m_deviceInstance;

			// internal device interface - NOT USED in DX
			void* m_deviceInterface;

			// internal device state - NOT USED in DX
			void* m_deviceState;

			// internal cached CPU buffer for device purposes
			EBYTE* m_deviceBuffer;

			// points to unused space in m_deviceBuffer
			EUINT32 m_deviceBufferPosition;


			// created vertex buffers
			std::list<VertexBuffer*> m_vertexBuffers;

			// created index buffers
			std::list<IndexBuffer*> m_indexBuffers;

			// created shaders
			std::list<Shader*> m_shaders;

			// created textures
			std::list<Texture2D*> m_textures;

			// created render targets
			std::list<RenderTarget*> m_renderTargets;


			// current VertexBuffer
			VertexBuffer* m_currentVertexBuffer;

			// current IndexBuffer
			IndexBuffer* m_currentIndexBuffer;

			// current Shader
			Shader* m_currentShader;


			// current RenderTargets
			RenderTarget* m_currentRenderTargets[PF_GI_RENDERTARGETSCOUNT];

			// how many redertargets were set
			EUINT32 m_currentRenderTargetsCount;

			// current rasterizer state
			EUINT32 m_currentRasterizerState;

			// current stencil state
			EUINT32 m_currentStencilState;

			// current blend mode
			EUINT32 m_currentBlendState;


			//
			GraphicsQueue m_queue;

			std::thread m_queueThread;


		//	-----------------------------------------------------------------------
		//		Debug data and profiling
		//	-----------------------------------------------------------------------
		public:

			
		protected:

			// return reference to internal graphics device instance
			inline void* GetDevice();

			// return reference to internal graphics device instance
			inline void* GetDeviceContext();

			// return reference to internal graphics device instance
			inline void* GetDeviceInstance();

			// 
			inline void* GetDeviceInterface();

			// 
			inline void* GetDeviceState();


			template<typename T>
			T* GetDeviceBuffer()
			{
				if (m_deviceBufferPosition + sizeof(T) < 512)
				{
					return (T*)m_deviceBuffer[m_deviceBufferPosition];
					m_deviceBufferPosition += sizeof(T);
				}
				else
				{
					return NULLPTR;
				}
			}




		//
		// Get debug data
		//
		public:
			EUINT32 GetFrameCounter();


		
			//	--------------------------------------------------------------------
			//		Profiler
			//	--------------------------------------------------------------------

			struct ProfilerData
			{
				EUINT32 UsedGPUMemory;
				EUINT32 DrawCallsCount;
				EUINT32 RenderedVerticesCount;
				EUINT32 RenderedTrianglesCount;
				EUINT32 SetShaderCount;
				EUINT32 SentDataSize;
				EUINT32 FPS;
				EUINT32 FrameCounter;
				EUINT32 FrameTimeBegin;
				EUINT32 FrameTimePrevious;
				EUINT32 FrameTimeCurrent;

			} profilerData;


			//ProfileGraphicsDevice* GetProfileTrack();


		};

	}
}








#endif