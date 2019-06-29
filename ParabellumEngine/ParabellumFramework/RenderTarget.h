#ifndef _RENDERTARGET_H_
#define _RENDERTARGET_H_


//
//	Predefs
//
#include "DLLIE.h"


//
//	Includes
//

#include "TypesC.h"
#include "SystemCodes.h"
#include "DebugManager.h"

#include "Vector4D.h"


namespace ParabellumFramework
{

	namespace Graphics
	{
		//
		//	RenderTarget
		//
		class XYZ_API RenderTarget
		{

			friend class GraphicsDevice;
			friend class ShaderParametr;

		private:
			RenderTarget();
			RenderTarget(const RenderTarget&) = delete;
			~RenderTarget();


		public:
			void* m_rtBufferTexture;
			void* m_rtBufferFormat;
			void* m_rtBufferResource;


			EUINT32 m_rtFormat;

			void* m_depthBuffer;
			void* m_depthBufferFormat;

			EUINT32 m_depthFormat;

			EUINT32 m_width;

			EUINT32 m_height;

			float m_viewportBuffer[6];

			Vector4D m_viewport;

		private:
			void* GetRTBufferResource();

		//
		// Gets and Sets
		//
		public:
			EUINT32 GetWidth();

			EUINT32 GetHeight();

			//
			// operators
			//
			private:

				RenderTarget& operator=(const RenderTarget& right) = delete;
		};

	}
}








#endif