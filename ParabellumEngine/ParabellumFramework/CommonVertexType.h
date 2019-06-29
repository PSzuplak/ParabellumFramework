#ifndef _MESHTYPES_H_
#define _MESHTYPES_H_

#include <list>

#include "TypesC.h"
#include "Vector3D.h"


namespace ParabellumFramework
{
	//
	// Common used VertexType
	//
	struct CommonVertexType
	{
		union
		{
			struct
			{
				EFLOAT32 positionl[3];
				EFLOAT32 texturel[2];		
				//EFLOAT32 propertiesl[2];	// X - no of mesh (for "Insdancing" hehe), Y - reversed
				EFLOAT32 normall[3];
				EFLOAT32 tangentl[3];
				EFLOAT32 binormall[3];
			};

			struct
			{
				Vector3D position;
				Vector2D texture;	
				//Vector2D properties;
				Vector3D normal;
				Vector3D tangent;
				Vector3D binormal;
			};
		};

		CommonVertexType()
		{
			//propertiesl[0] = 0.0f;
			//propertiesl[1] = 0.0f;
		}

		~CommonVertexType()
		{
		}


	};

	//
	// For instancing model. Struct has only 1 vector3 field.
	// properties.x - number of drawed ModelMesh 
	// properties.y - reversed
	// properties.z - reversed
	//
	struct CommonInstanceType
	{
		//D3DXVECTOR3 properties;
		EFLOAT32 properties[3];
	};







}

#endif