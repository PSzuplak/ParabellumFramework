#ifndef _MODELCONVERTER_H_
#define _MODELCONVERTER_H_


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

#include "Model.h"
#include "ModelFormat.h"

#include "CommonMaterialType.h"

#include "Vector3D.h"

#include <iostream>
#include <string>

using namespace ParabellumFramework::Graphics;

namespace ParabellumFramework
{
	class XYZ_API ModelConverter
	{
		struct FaceType;
		struct OBJGroup;
		struct Common3DObject;


	public:
		ModelConverter();
		~ModelConverter();

	public:

		void ConvertOBJ2PFM(const char* in, const char* out);

		// calculate TBN
		void CalculateTBN(CommonVertexType* vertices, EUINT32 verticesCount, EUINT32* indices, EUINT32 indicesCount);


	private:

		std::vector<Common3DObject> m_objects;

	//
	// data
	//
	private:


		struct FaceType
		{
			int vIndex1, vIndex2, vIndex3;
			int tIndex1, tIndex2, tIndex3;
			int nIndex1, nIndex2, nIndex3;
		};

		struct OBJGroup
		{
			std::vector<FaceType> faces;

			CommonMaterialType meterial;
		};

		struct Common3DObject
		{
			std::vector<Vector3D> vertices;
			std::vector<Vector3D> normals;
			std::vector<Vector3D> coords;
			std::vector<FaceType> faces;			// all faces


			std::vector<OBJGroup> groups;			//grouped faces
		};

	};

}








#endif