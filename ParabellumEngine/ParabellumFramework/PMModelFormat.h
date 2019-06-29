#ifndef _PMMODELFORMAT_H_
#define _PMMODELFORMAT_H_


#include "TypesC.h"
#include "Vector2D.h"
#include "Vector3D.h"

namespace ParabellumFramework
{
	//namespace Resources
	//{
		//	-------------------------------------------------------------
		//		Enums
		//	-------------------------------------------------------------

		//
		//
		//
		enum PEM_OBJETC_TYPE
		{
			MODEL = 0,
			MESH,
			TEXTURE,
			ANIMATION_K,
			ANIMATION_S,
			LIGHT,
			CAMERA,
		};

		//
		//
		//
		enum PEM_FLAG
		{
			COORINATESYSTEM_LH = 0,
			COORINATESYSTEM_RH,
			UNIT_MM,				// units milimetrs
			UNIT_CM,				// units centimetrs
			UNIT_DCM,				// units decimetrs
			UNIT_M					// units metrs
		};


		//	-------------------------------------------------------------
		//		Types
		//	-------------------------------------------------------------
		struct PEMVertexType
		{
			Vector3D position;
			Vector2D texture;
			Vector3D normal;
		};


		//	-------------------------------------------------------------
		//		File Stricts
		//	-------------------------------------------------------------

		//
		//
		//
		struct PEM_Header
		{
			EUINT32	Version;				// format version
			EUINT64	Checksum;				// checksum for file
			EUINT64	Data;					// data file creation
		};

		//
		//
		//
		struct PEM_Properties_3D
		{
			EUINT32	CoordinateSystem;		// coordiante system - val from PEM_FLAG
			EUINT32	Unit;					// unit system (milimetrs/centimetrs/...)
		};

		//
		//
		//
		struct PEM_Objects
		{
			EUINT64 	PointersCount;
			EUINT64*	Pointers;		// pointers to PEM_OBJECT
		};

		//
		//
		//
		struct PEM_Object
		{
			EUINT32	Id;				// identyficator
			EUINT32	Type;			// Object Type
			EUINT64	Pointer;		// Pointer to Object
		};

		//
		//
		//
		struct PEM_Object_Model
		{
			EUINT64	MeshCount;
			EUINT64* PtrMeshes;

			EUINT32	TexturesCount;
			EUINT64* PtrTextures;
		};


		//
		//
		//
		struct PEM_Object_Texture
		{
			EUINT32	PathLength;
			ESTR Path;
		};

		//
		//
		//
		struct PEM_Mesh_Material
		{
			EUINT16			Reversed;		// reversed 16 bits
			bool			Transparent;	// is mesh transparent
			EFLOAT32		AlphaBias;		// texels with alpha less than AlphaBias will be cut
											// works only if Transparent is set to false
		};

		//
		//
		//
		struct PEM_Object_Mesh
		{
			EUINT64		VerticesCount;
			Vector3D*	Vertices;
			Vector3D*	Normals;
			Vector2D*	TexCoords;

			EUINT32* BoneNo;				// count - VerticesCount // Each vertice contain number of bone

			EUINT32	IndicesCount;
			EUINT32* Indices;

			EUINT32 TexturesCount;
			PEM_Object_Texture* Textures;

			PEM_Mesh_Material Material;
		};

		//
		//
		//
		struct PEM_Mesh_Bone
		{
			EUINT32 No;			// bone id
			Vector3D Position;	// bone positon in Local Coordinates
			EUINT32 Parent;		// number of bone which is connected with this bone
		};


		class PEM_Operator
		{
		};


	//}

}

#endif