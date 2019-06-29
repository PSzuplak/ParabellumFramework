#ifndef _VERTEXBUFFER_H_
#define _VERTEXBUFFER_H_


//
//	Predefs
//
#include "DLLIE.h"


//
//	Includes
//
#include "TypesC.h"
#include "SystemCodes.h"


namespace ParabellumFramework
{
	namespace Graphics
	{
		
		//
		//	VertexBuffer - keeps vertex buffer. VertexBuffer should be created by GraphicsDevice
		//	
		//
		class XYZ_API VertexBuffer
		{
			//
			// friendzone
			//
			friend class XYZ_API GraphicsDevice;

			private:
				//VertexBuffer();
				VertexBuffer(const VertexBuffer&) = delete;

				// VertexBuffer must be created by GraphicsDevice::CreateVertexBuffer
				VertexBuffer(EUINT32 vertexSize, EUINT32 verticesCount);

				// Destructor
				~VertexBuffer();

			private:
				void* m_buffer;				// pointer/value of the buffer (located on a Graphics Card)
				EUINT32 m_vertexSize;		// size of vertex in bytes
				EUINT32 m_verticesCount;	// vertices count

			private:

				// Get reference to the buffer
				void* GetAddressOfBuffer();

			public:
				// Get size of ONE vertex
				EUINT32 GetVertexSize();

				// Get number of vertices
				EUINT32 GetVerticesCount();

				// Get buffer size - VertexSize*VerticesCount
				EUINT32 GetBufferSize();
		};

	}
}








#endif