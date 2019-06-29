#ifndef _TEXTURE2D_H_
#define _TEXTURE2D_H_


//
//	Predefs
//
#include "DLLIE.h"


//
//	Includes
//
#include "DebugManager.h"

#include "TypesC.h"
#include "SystemCodes.h"
#include "GraphicsTypes.h"
#include "UnitHelper.h"

#include "Resource.h"
#include "ParabellumImage.h"

#include <string>

namespace ParabellumFramework
{

	namespace Graphics
	{

		//
		// Texture2D
		//
		class XYZ_API Texture2D : public ParabellumFramework::Resources::Resource
		{
			// friend zone
			friend class XYZ_API GraphicsDevice;
			friend class XYZ_API ParabellumFramework::Resources::ResourceManager;
			friend class XYZ_API ShaderParametr;

		private:			
			Texture2D();
			~Texture2D();
			Texture2D(const Texture2D&) = delete;

		private:
			EUINT32 Release();

			EUINT32 ReleaseImageData();

			EUINT32 Parse(const char* filePath);



		//
		// Members
		//
		private:

			// pointer to the texture on graphics card
			void* m_buffer;

			// type of data stored in m_buffer. In directX its ResourceView
			void* m_bufferType;

			// pointer to the buffor with data
			void* m_data;

			// texture width
			EUINT32 m_width;

			// texturew height
			EUINT32 m_height;

			// format of the texture
			GraphicsFormat m_format;

			// size of one texel in bytes
			EUINT32 m_texelSize;

		//
		// Gets and Sets
		//
		private:

			// get refernce to graphics card resource
			void* GetAddressOfBuffer();

			void* GetAddressOBufferType();

			// Get RAW data of texture. Note that data is deleted after Parse(), because we assume that
			// it will not be needed in the future, so we have to load data from file again.
			void* GetRAWData();

			// get value of the pixel - pOut has to be at lease of m_texelSize/"bits per pixel" size
			void GetPixel(EUINT32 x, EUINT32 y, _OUT_ void* pOut);

		public:

			EUINT32 GetWidth();

			EUINT32 GetHeight();

			GraphicsFormat GetFormat();

			// get texel size in bytes
			EUINT32 GetTexelSize();

			// return pointer to texel at (X,Y) position
			EFLOAT32 GetTexel(EUINT32 x, EUINT32 y);

			// return the value of one channel of texel located at (X,Y)
			// and cast it to type T
			template<typename T>
			T GetChannelVal(EUINT32 x, EUINT32 y, ChannelNo no)
			{
				EUINT32 offset = 0;

				if (no == ChannelNo::R)
					offset = 0;
				else if (no == ChannelNo::G)
					offset = 1;
				else if (no == ChannelNo::B)
					offset = 2;
				else if (no == ChannelNo::A)
					offset = 3;

				if (m_format == GraphicsFormat::FORMAT_R8G8B8A8_INT)
				{
					return (T)((EUINT8*)m_data)[(x + y*m_width) * 4 + offset];
				}

				// throw an exception
				return (T)0;
			}


		//
		// operators
		//
		private:

			Texture2D& operator=(const Texture2D& right) = delete;

		};


	}
}








#endif