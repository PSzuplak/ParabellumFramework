#include "stdafx.h"
#include "Texture2D.h"



//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumFramework::Graphics::Texture2D::Texture2D()
{
	PFDLOG("PFGI", "TX2D", "Texture2D[" << (EADDR)this << "]->Texture2D()");

	m_buffer = NULLPTR;
	m_data = NULLPTR;
	m_width = 0;
	m_height = 0;
	m_format = GraphicsFormat::FORMAT_R8G8B8A8_FLOAT;
	m_texelSize = 0;

	PFDLOG("PFGI", "PGT2", "AddressOf(m_buffer) : " << &m_buffer);
	PFDLOG("PFGI", "PGT2", "AddressOf(m_data) : " << &m_data);

}


ParabellumFramework::Graphics::Texture2D::~Texture2D()
{
	PFDLOG("PFGI", "TX2D", "Texture2D[" << (EADDR)this << "]->~Texture2D()");
	// free memory 
	delete[] m_data;
	m_data = NULLPTR;
}



//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------


ParabellumFramework::EUINT32
ParabellumFramework::Graphics::Texture2D::Release()
{
	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PFGI", "TX2D", "Texture2D[" << (EADDR)this << "]->Release()");
	#endif

	ReleaseImageData();

	return SystemCodes::SUCCESS;
}

ParabellumFramework::EUINT32
ParabellumFramework::Graphics::Texture2D::ReleaseImageData()
{
	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PFGI", "TX2D", "Texture2D[" << (EADDR)this << "]->ReleaseImageData()");
	#endif

	if (m_data != NULLPTR)
	{
		#ifdef DEBUG_LOGLEVEL_1
		PFDLOG("PFGI", "TX2D", "deleting m_data");
		#endif

		delete[] m_data;
		m_data = NULLPTR;
	}
	else
	{
		#ifdef DEBUG_LOGLEVEL_1
		PFDLOG("PFGI", "TX2D", "m_data already deleted");
		#endif
	}

	return SystemCodes::SUCCESS;
}


ParabellumFramework::EUINT32 
ParabellumFramework::Graphics::Texture2D::Parse(const char* filePath)
{
	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PFGI", "TX2D", "Texture2D[" << (EADDR)this << "]->Parse (" << filePath << ")");
	#endif

	// open file
	FILE* fin = fopen(filePath, "rb");

	if (!fin)
	{
		#ifdef DEBUG_LOGLEVEL_1
		PFDLOG("PFGI", "TX2D", "Texture2D[" << (EADDR)this << "]->Parse (...) : Can not open file");
		#endif

		return SystemCodes::ERROR_NOFILE;
	}

	// load header
	PFTHeader pic_header;
	fread(&pic_header, sizeof(PFTHeader), 1, fin);

	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PFGI", "TX2D", "Texture2D[" << (EADDR)this << "]->Parse (...) : header:compression=" << pic_header.compressionType << ",data=" << pic_header.data << ",graphics format=" << pic_header.graphicsFormat << ",hash=" << pic_header.hash << ",mipmap level=" << pic_header.mipmapLevel);
	#endif


	// load data
	PFTData pic_data;
	fread(&pic_data, sizeof(PFTData), 1, fin);

	m_width = pic_data.width;
	m_height = pic_data.height;

	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PFGI", "TX2D", "Texture2D[" << (EADDR)this << "]->Parse (...) : data:" << "channels count=" << pic_data.channelsCount << ",width="<< m_width << ",height=" << m_height);
	#endif


	EUINT32 pixelsCount = 0;

	switch (pic_header.graphicsFormat)
	{
		case Graphics::GraphicsFormat::FORMAT_R8G8B8A8_INT: 
			m_format = GraphicsFormat::FORMAT_R8G8B8A8_INT;
			m_texelSize = 4;
			pixelsCount = pic_data.channelsCount * m_width * m_height;
			m_data = new char[pixelsCount];
			break;
		case Graphics::GraphicsFormat::FORMAT_R32G32B32A32_INT: 
			m_format = GraphicsFormat::FORMAT_R32G32B32A32_INT;
			m_texelSize = 16;
			pixelsCount = pic_data.channelsCount * m_width * m_height;
			m_data = new EUINT32[pixelsCount];
			break;
		default: 
			PF_DEBUG_ASSERT("Incorrect Format"); 
			break;
	}
	
	

	for (EUINT32 i = 0; i < pixelsCount; ++i)
		((char*)m_data)[i] = 255;


	// read channels
	for (EUINT32 i = 0; i < pic_data.channelsCount; ++i)
	{
		PFTChannel pic_channel;

		fread(&pic_channel, sizeof(PFTChannel), 1, fin);

		#ifdef DEBUG_LOGLEVEL_1
		PFDLOG("PFGI", "TX2D", "Texture2D[" << (EADDR)this << "]->Parse (...) : Channel no  : " << i << ",min val = " << pic_channel.minVal << ",max val = " << pic_channel.maxVal << ",size = " << pic_channel.size);
		#endif

		

		switch (pic_header.graphicsFormat)
		{
		case Graphics::GraphicsFormat::FORMAT_R8G8B8A8_INT:
			for (EUINT32 j = 0; j < pic_channel.width*pic_channel.height; ++j)
			{
				char pixel;
				fread(&pixel, pic_channel.size, 1, fin);
				((char*)m_data)[i + j*pic_data.channelsCount] = pixel;
			}
			break;
		case Graphics::GraphicsFormat::FORMAT_R32G32B32A32_INT:
			for (EUINT32 j = 0; j < pic_channel.width*pic_channel.height; ++j)
			{
				EUINT32 pixel;
				fread(&pixel, sizeof(EUINT32), 1, fin);
				((EUINT32*)m_data)[i + j * pic_data.channelsCount] = pixel;
			}
			break;
		default:
			PF_DEBUG_ASSERT("Incorrect Format");
			break;
		}
	}

	
	// free
	fclose(fin);

	return SystemCodes::SUCCESS;
}

//	--------------------------------------------------------------------
//		Gets and Sets
//	--------------------------------------------------------------------

void* ParabellumFramework::Graphics::Texture2D::GetAddressOfBuffer()
{
	return &m_buffer;
}

void* ParabellumFramework::Graphics::Texture2D::GetAddressOBufferType()
{
	return &m_bufferType;
}


void* ParabellumFramework::Graphics::Texture2D::GetRAWData()
{
	return m_data;
}

void ParabellumFramework::Graphics::Texture2D::GetPixel(EUINT32 x, EUINT32 y, _OUT_ void* pOut)
{
}


ParabellumFramework::EUINT32
ParabellumFramework::Graphics::Texture2D::GetWidth()
{
	return m_width;
}


ParabellumFramework::EUINT32
ParabellumFramework::Graphics::Texture2D::GetHeight()
{
	return m_height;
}


ParabellumFramework::Graphics::GraphicsFormat
ParabellumFramework::Graphics::Texture2D::GetFormat()
{
	return m_format;
}


ParabellumFramework::EUINT32
ParabellumFramework::Graphics::Texture2D::GetTexelSize()
{
	return m_texelSize;
}


ParabellumFramework::EFLOAT32
ParabellumFramework::Graphics::Texture2D::GetTexel(EUINT32 x, EUINT32 y)
{
	if (m_data == NULLPTR)
		return 0.0f;


	if (m_format == GraphicsFormat::FORMAT_R8G8B8A8_INT)
	{
		EUINT32 i = 0;
		i++;
	}

	return (EFLOAT32)((char*)m_data)[(x + y*m_width) * 4];
}