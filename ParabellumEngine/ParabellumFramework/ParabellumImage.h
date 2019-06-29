#ifndef _PARABELLUMIMAGE_H_
#define _PARABELLUMIMAGE_H_

#include "TypesC.h"
#include "GraphicsTypes.h"

namespace ParabellumFramework
{


	enum class PFTCompressionType
	{
		BASIC_RLE = 109
	};




	struct PFTHeader
	{
		//
		EUINT32 hash;

		// data
		EUINT32 data;

		// values from ParabellumFramework::Graphics::GraphicsFormat
		EUINT32 graphicsFormat;

		//
		EUINT32 compressionType;

		//
		EUINT32 mipmapLevel;
	};


	struct PFTData
	{
		EUINT32 width;
		EUINT32 height;
		//
		EUINT32 channelsCount;
	};

	struct PFTChannel
	{

		EUINT32 minVal;
		EUINT32 maxVal;

		EUINT32 width;
		EUINT32 height;


		// size in bytes to read
		EUINT32 size;
	};


	

	

	


	

}

#endif