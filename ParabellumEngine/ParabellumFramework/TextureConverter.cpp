#include "stdafx.h"
#include "TextureConverter.h"



//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumFramework::TextureConverter::TextureConverter()
{

}


ParabellumFramework::TextureConverter::~TextureConverter()
{

}



//	--------------------------------------------------------------------
//		Methods
//	--------------------------------------------------------------------

void 
ParabellumFramework::TextureConverter::convertBMP2PFT(const char* in, const char* out)
{
	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PFGI", "T2DC", "convertBMP2PFT(" << in << "," << out << ") : start" << in);
	#endif


	FILE* fout = fopen(in, "rb");

	EUINT32 i;
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, fout);	// read the 54-byte header

													// extract image height and width from header
	EUINT32 width = *(EUINT32*)&info[18];
	EUINT32 height = *(EUINT32*)&info[22];
	EINT16 bitdepth = *(EINT16*)&info[28];

	EUINT32 size = 0;
	EUINT32 step = 0;

	//printf("\n\n%s", out);
	//printf("\nbits = %i", bitdepth);

	if (bitdepth == 32)
	{
		size = 4 * width * height;
		step = 4;
	}
	else if (bitdepth == 24)
	{
		size = 3 * width * height;
		step = 3;
	}
	else
		assert(!"BMP - wrong bits per pixel");


	unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
	fread(data, sizeof(unsigned char), size, fout); // read the rest of the data at once


	unsigned char* dataR = new unsigned char[4 * width * height];

	//std::reverse(data, &data[size - 1]);

	EUINT32 min[4] = { 255 };
	EUINT32 max[4] = { 0 };

	EUINT32 counter = 0;

	for (i = 0; i < size; i += step)
	{
		unsigned char r, g, b, a;



		if (bitdepth == 32)
		{
			r = data[i + 3];
			g = data[i + 2];
			b = data[i + 1];
			a = data[i + 0];
		}
		else if (bitdepth == 24)
		{
			r = data[i + 2];
			g = data[i + 1];
			b = data[i + 0];
			a = 255;
		}

		dataR[0 * width * height + counter] = r;
		dataR[1 * width * height + counter] = g;
		dataR[2 * width * height + counter] = b;
		dataR[3 * width * height + counter] = a;

		counter++;
	}


	delete[] data;
	fclose(fout);



	//
	// Save PFC
	//
	fout = fopen(out, "wb");

	// save header
	PFTHeader pic_header;

	pic_header.hash = 0;
	pic_header.data = 0;
	pic_header.graphicsFormat = (EUINT32)ParabellumFramework::Graphics::GraphicsFormat::FORMAT_R8G8B8A8_INT;
	pic_header.compressionType = (EUINT32)PFTCompressionType::BASIC_RLE;
	pic_header.mipmapLevel = 0;

	fwrite(&pic_header, sizeof(PFTHeader), 1, fout);


	// save data info
	PFTData pic_data;

	pic_data.channelsCount = 4;
	pic_data.width = width;
	pic_data.height = height;


	fwrite(&pic_data, sizeof(PFTData), 1, fout);


	//
	// Sae channels

	for (EUINT32 i = 0; i < pic_data.channelsCount; ++i)
	{
		PFTChannel pic_channel;

		pic_channel.height = width;
		pic_channel.width = height;
		pic_channel.size = 1;
		pic_channel.minVal = 0;
		pic_channel.maxVal = 255;

		// save channel data
		fwrite(&pic_channel, sizeof(PFTChannel), 1, fout);

		// save data
		fwrite(&dataR[i* width * height], pic_channel.size, pic_channel.width*pic_channel.height, fout);
	}


	// cleanup
	delete dataR;

	fclose(fout);
}





void 
ParabellumFramework::TextureConverter::convertFourBMP2PFT(const char* in0, const char* in1, const char* in2, const char* in3, const char* out)
{
	#ifdef DEBUG_LOGLEVEL_1
	PFDLOG("PFGI", "T2DC", "convertFourBMP2PFT(" << in0 << "," << in1 << "," << in2 << "," << in3 << "," << out << ") : start");
	#endif

	FILE* fout = fopen(in0, "rb");

	EUINT32 i;
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, fout);	// read the 54-byte header

													// extract image height and width from header
	EUINT32 width = *(EUINT32*)&info[18];
	EUINT32 height = *(EUINT32*)&info[22];
	EINT16 bitdepth = *(EINT16*)&info[28];

	EUINT32 size = 0;
	EUINT32 step = 0;

	//printf("\n\n%s", out);
	//printf("\nbits = %i", bitdepth);

	if (bitdepth == 32)
	{
		size = 4 * width * height;
		step = 4;
	}
	else if (bitdepth == 24)
	{
		size = 3 * width * height;
		step = 3;
	}
	else
		assert(!"BMP - wrong bits per pixel");


	unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
	fread(data, sizeof(unsigned char), size, fout); // read the rest of the data at once


	EUINT32* dataR = new EUINT32[4 * width * height];

	//std::reverse(data, &data[size - 1]);

	EUINT32 min[4] = { 255 };
	EUINT32 max[4] = { 0 };

	EUINT32 counter = 0;

	for (i = 0; i < size; i += step)
	{
		unsigned char r, g, b, a;

		if (bitdepth == 32)
		{
			r = data[i + 3];
			g = data[i + 2];
			b = data[i + 1];
			a = data[i + 0];
		}
		else if (bitdepth == 24)
		{
			r = data[i + 2];
			g = data[i + 1];
			b = data[i + 0];
			a = 255;
		}

		dataR[0 * width * height + counter] = Math::UnitHelper::U8toU32(r,g,b,250);
		dataR[1 * width * height + counter] = Math::UnitHelper::U8toU32(240,0,0,128);
		dataR[2 * width * height + counter] = Math::UnitHelper::U8toU32(240,0,0,128);
		dataR[3 * width * height + counter] = Math::UnitHelper::U8toU32(240,0,0,128);

		counter++;
	}


	delete[] data;
	fclose(fout);



	//
	// Save PFC
	//
	fout = fopen(out, "wb");

	// save header
	PFTHeader pic_header;

	pic_header.hash = 0;
	pic_header.data = 0;
	pic_header.graphicsFormat = (EUINT32)ParabellumFramework::Graphics::GraphicsFormat::FORMAT_R32G32B32A32_INT;
	pic_header.compressionType = (EUINT32)PFTCompressionType::BASIC_RLE;
	pic_header.mipmapLevel = 0;

	fwrite(&pic_header, sizeof(PFTHeader), 1, fout);


	// save data info
	PFTData pic_data;

	pic_data.channelsCount = 4;
	pic_data.width = width;
	pic_data.height = height;


	fwrite(&pic_data, sizeof(PFTData), 1, fout);


	//
	// Sae channels

	for (EUINT32 i = 0; i < pic_data.channelsCount; ++i)
	{
		PFTChannel pic_channel;

		pic_channel.height = width;
		pic_channel.width = height;
		pic_channel.size = 4;
		pic_channel.minVal = 0;
		pic_channel.maxVal = 255;

		// save channel data
		fwrite(&pic_channel, sizeof(PFTChannel), 1, fout);

		// save data
		fwrite(&dataR[i* width * height], pic_channel.size, pic_channel.width*pic_channel.height, fout);
	}


	// cleanup
	delete dataR;

	fclose(fout);

}










//	--------------------------------------------------------------------
//		Gets and Sets
//	--------------------------------------------------------------------

