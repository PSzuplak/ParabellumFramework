#ifndef _INDEXBUFFER_H_
#define _INDEXBUFFER_H_


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
		//	IndexBuffer - keeps index buffer
		//
		class XYZ_API IndexBuffer
		{
			//
			// friendzone
			//
			friend class XYZ_API GraphicsDevice;

			private:
				IndexBuffer(const IndexBuffer&) = delete;

				// IndexBuffer must be created by GraphicsDevice::CreateIndexBuffer
				IndexBuffer(EUINT64 indicesCount);

				// Destructor - IndexBuffer objects can be destroyed only throught GraphicsDevice
				~IndexBuffer();

			private:
				void* m_buffer;				// pointer/value of the buffer
				EUINT64 m_indicesCount;		// number of indices


			private:
				// Get pointer to the buffer. Dereference it if You want to get buffer
				void* GetBuffer();
			public:
				// Get number of indices
				EUINT64 GetIndicesCount();

		};

	}
}








#endif