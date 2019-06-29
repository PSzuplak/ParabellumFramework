//--------------------------------------------------------------------------------
//
//	Volume - created from many vertices - used for precise collision detection	
//
//	Example:
//		Volume volume;
//		Matrix4x4 tran0, tran1;
//		tran0.MakeTranslation(100.0f, 200.0f, 300.0f);
//		tran1.MakeTranslation(500.0f, 600.0f, 700.0f);
//		Vector3D vertices[3];
//		EUINT32 indices[3];
//		vertices[0].Set(1.0f, 1.0f, 1.0f);
//		vertices[1].Set(2.0f, 2.0f, 2.0f);
//		vertices[2].Set(3.0f, 3.0f, 3.0f);
//		indices[0] = 0;
//		indices[1] = 0;
//		indices[2] = 0;
//		volume.InitializeCopy(3, vertices, 3, indices);
//		volume.Print();
//		volume.Transform(&tran0);
//		volume.Print();
//		volume.Transform(&tran1);
//		volume.Print();
//	
//
//--------------------------------------------------------------------------------

#ifndef _MESH_H_
#define _MESH_H_


//
//
//
#include <memory>
#include <vector>

//
//	Predefs
//
#include "DLLIE.h"

//
//
//
#include "TypesC.h"
#include "IntersectCodes.h"
#include "MathHelper.h"
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "Triangle.h"
#include "BoundingVolumes.h"

namespace ParabellumFramework
{
	//
	//	Cloud of vertices which define a volume
	//
	class XYZ_API Volume
	{

	//
	// Constructors and destructors
	//
	public:
		Volume();
		Volume(_IN_ _ARRAY_ Vector3D* vertices, EUINT32 verticesCount, _IN_ _ARRAY_ EUINT32* indices, EUINT32 indicesCount);
		Volume(Volume& src) = delete;
		~Volume();


	//
	// Methods
	//
	private:


		void Initialize(_IN_ _ARRAY_ Vector3D* vertices, EUINT32 verticesCount, _IN_ _ARRAY_ EUINT32* indices, EUINT32 indicesCount);

		void Release();

	public:



		// check intersection with another Volume
		IntersectCodes Intersects(_IN_ Volume* pMesh);

		// transform all vertices to the original position (before last TransformOriginal), and then transfrom it by pTransfom
		void TransformOriginal(_IN_ Matrix4x4* pTransform);

		void Print()
		{
			printf("\n Volume : \n");
			for (EUINT32 i = 0; i < m_verticesCount; ++i)
			{
				printf("\n -> %s", m_vertices[i].ToString().c_str());
			}
		}

	protected:

		virtual void TransformData(_IN_ Matrix4x4* pTransform) {};

	//
	// Members
	//
	public:

		// this contains inverded matrix of this one passed in Transform()
		// eWorld = emodel->GetWorldMatrix();
		// Matrix4x4 transform = ...
		// transformInv = Matrix4x4::Invert(transform);
		// eWorldo = transform * eWorld;
		// eWorldo = transformInv * eWorldo;
		Matrix4x4 m_inverse;

	private:

	
		EUINT32 m_verticesCount;

		Vector3D* m_vertices;

		EUINT32 m_indicesCount;

		EUINT32* m_indices;

	protected:

	//
	// Gets and Sets
	//
	public:

		Vector3D* GetVertex(EUINT32 vertexNo);
		EUINT32 GetVerticesCount();

		void GetFace(EUINT32 triangleNo, _OUT_ Triangle& triangle);

	};


}




#endif