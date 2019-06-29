//****************************************************************************************************/
//
//	Filename : Float32_3.h
//
//	Descritpion : 3 dimension vector of 32bit floats
//
//					
//
//
//****************************************************************************************************/

#ifndef __PRAGMAONCE_PARABELLUMUTILS_FLOAT32_3_H__
#define __PRAGMAONCE_PARABELLUMUTILS_FLOAT32_3_H__

#include "Configuration.h"
#include "Fixes.h"
#include "Types.h"

#include "Float32_2.h"

namespace ParabellumUtils
{

	// 4 dimension vector of 32bit floats
	class Float32_3
	{
	public:

		union
		{
			struct
			{
				EFLOAT32 X;
				EFLOAT32 Y;
				EFLOAT32 Z;
			};


			// 16 Bytes variable - for shaders parametrs purpose
			struct
			{
				EFLOAT32 data[3];
			} DATA;

	
			struct
			{
				Float32_2 XY;
				EFLOAT32 Z;
			};

			struct
			{
				EFLOAT32 X;
				Float32_2 XY;		
			};


		};


	};





}



#endif