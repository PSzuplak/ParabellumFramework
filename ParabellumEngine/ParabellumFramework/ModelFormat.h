#ifndef _MODELFORMAT_H_
#define _MODELFORMAT_H_


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

		// basic types of ParabellumFramework Model
		enum class PFMTypes
		{
			// Undefinied type - needs to read size of type
			VOIDN = 1,

			INT8,
			INT16,
			INT32,
			INT64,
			UINT8,
			UINT16,
			UINT32,
			UINT64,
			FLOAT32,
			FLOAT64,
			CHAR8,
			CHAR16,
			CHAR32,

			VECTOR2D,
			VECTOR3D,
			VECTOR4D,
			MATRIX4X4,

			// PFMArray
			ARRAY,

			// PFMComplexType
			COMPLEXTYPE,

			OBJECT,
			OBJECT3D,
			MODEL,

			// (4Bytes)type + (4Bytes)vertexFormatSize + (4Bytes)verticesCount +  (4Bytes * verticesCount)verticesCount * vertexFormatSize
			VERTICES,

			// (4Bytes)type + (4Byte no of VERTICES - offets index) + (4Bytes)indicesCount + (4Bytes * indicesCount)indicesCount * sizeof(EUINT32)
			GROUP,

			INDICES,

			MESH,
			BONES,
			DIRECTLIGHT,
			SPOTLIGHT,
			POINTLIGHT,
			CAMERA,
			KEYFRAMES,
			TEXTURE2D,
			MATERIAL,

			// Reference to object in PFMObjects. Can be used only in ComplexType and PFMArray
			// If it sets in ComplexType it means that PFMComplexType.var stores index to PFMObjects.offsets
			// f.ex. Model can store many references to the same mesh (f.ex. 10 windows)
			OBJECTREF,

			//
			BOUNDINGBOX,

			//
			BOUNDINGSPHERE
		};


		enum class PFMChecksum
		{
			Id = 48361527,
		};

		enum class PFMFormatType
		{
			TEXT = 1,
			BINARY = 2
		};

		enum class PFMUnitType
		{
			MILLIMETERE = 1,
			CENTIMETERE = 2,
			DECIMETERE = 3,
			METERE = 4,
			UNDEFINIED = 5
		};

		enum class PFMCooridnateSystem
		{
			LEFTHANDED = 1,
			RIGHTHANDED = 2,
			UNDEFINIED = 3
		};

		enum class PFMUPUNIT
		{
			UPX = 1,
			UPY = 2,
			UPZ = 3,
			UNDEFINIED = 4
		};


		struct PFMComplexType
		{
			EUINT32	varType;
			EVOID*	var;
		};


		struct PFMArray
		{
			EUINT32	elementSize;
			EUINT32	elementsCount;
			EVOID*	var;
		};

		struct PFMHeader
		{
			EUINT32 	checksum;
			EUINT32 	fileFormat;
			EUINT32 	versionMajor;
			EUINT32 	versionMinor;
			EUINT32 	dateCreated;
		};

		struct PFMEnvironmentSettings
		{
			EFLOAT32	scale;
			EUINT32		units;
			EUINT32		coordinateSystem;
		};

		struct PFMObject3D
		{
			EUINT32		parent;
			Matrix4x4	translate;
			Matrix4x4	rotate;
			Matrix4x4	scale;

			// Model, PointLight, SpotLight, Camera, etc
			EUINT32		objectNo;
		};

		struct PFMData
		{
			EUINT32					objectsCount;
			_ARRAY_ EUINT32*		offsets;
			_ARRAY_ PFMObject3D*	objects;
		};


		struct PFMRoot
		{
			PFMComplexType root;
		};

		struct PFMFileStruct
		{
			PFMHeader					header;
			PFMEnvironmentSettings		environmentSettings;
			PFMData						objects;
			PFMRoot						root;
		};


		
		struct PFMMesh
		{
			EUINT32				vertexFormatSize;
			EUINT32				verticesCount;
			_ARRAY_ EVOID*		verties;
			EUINT32				indicesCount;
			_ARRAY_ EUINT32*	indices;
		};


		struct PFM_T_MODEL
		{
			EUINT32 offsetIndex;
		};


		struct PFM_Scene
		{
			EUINT32 objectsCount;
		};

		struct PFM_Scene_Mesh
		{
			EUINT32 type;
			EUINT32 parent;
			EUINT32 vno;
			EUINT32 ino;
		};



	}
}








#endif