#ifndef _MODEL_H_
#define _MODEL_H_

//
//	Predefs
//
#include "DLLIE.h"

#include "TypesC.h"
#include "IOCodes.h"

#include "Resource.h"
#include "ModelPart.h"

#include "DebugManager.h"
#include "Settings.h"

#include "PMModelFormat.h"
#include "ModelFormat.h"
#include "CommonVertexType.h"
#include "CommonMaterialType.h"

#include "BoundingVolumes.h"


namespace ParabellumFramework
{
	namespace Graphics
	{
		struct MeshLayout
		{
			EFLOAT32 position[3];
			EFLOAT32 normal[4];
			EFLOAT32 tangent[4];
		};

		//
		//
		// Model (Object3D) is a parent of ALL ModelPart objects which were loaded from a file
		//
		//
		class XYZ_API Model : public ParabellumFramework::Resources::Resource
		{
			// friendhip zone
			friend class XYZ_API ParabellumFramework::Resources::ResourceManager;

			// declaration
			struct DataVertices;
			struct DataIndices;
			
		//
		// STATIC DATA
		//
		public:

			// how many unique vertices GROUPS have been loaded
			static EUINT32 ModelVerticesGroupsCount;

			// how many indices GROUPS have been loaded
			static EUINT32 ModelIndicesGroupsCount;


			// types of objects stored in ModelObjects[]
			static EUINT32 ModelObjectsType[PF_MODEL_MAXOBJECTS];

			// pointers to Model Objects like - vertex buffers, index buffers, lights, cameras etc.
			static void** ModelObjects[PF_MODEL_MAXOBJECTS];

			static void* GetModelObject(EUINT32 no);


			static EUINT32 LoadedObjectsCount;
			static EUINT32 GetLoadedObjectsCount();

			// bounding boxes for meshes
			//static BoundingBox ModelsBoundingBox[PF_MODEL_MAXOBJECTS];

			// bounding boxes for meshes
			//static BoundingSphere ModelsBoundingSphere[PF_MODEL_MAXOBJECTS];

		private:
			Model();
			Model(const Model&) = delete;
			~Model();

		private:
			EUINT32 Initialize(_IN_ GraphicsDevice* pDevice);

		public:
			EUINT32 Release();

		private:
			// load model from PFM file
			EUINT32 Parse(const char* filePath);


		//
		// members
		//
		public:

			// data vertices
			DataVertices* m_dataVertices;

			// data indices
			DataIndices* m_dataIndices;



			//
			// Single mesh data
			//


			// vertices from file
			VertexBuffer** m_vertexBuffers;

			// indices from file
			IndexBuffer** m_indexBuffers;

			// array of indices of ModelUniqueVertexBuffers[]
			EUINT32* m_vBIds;

			// array of indices of ModelUniqueIndexBuffers[]
			EUINT32* m_iBIds;


			// materials
			CommonMaterialType* m_materials;


			EUINT32 m_vertexBuffersCount;

			EUINT32 m_indexBuffersCount;



			// array of indices of ModelUniqueIndexBuffers[]
			EUINT32* m_materialIds;



			// model parts
			ModelPart* m_modelParts;

			EUINT32 m_modelPartsCount;


			// boundingbox
			BoundingBox m_boundingBox;

			//bounding sphere
			BoundingSphere m_boundingSphere;


		//
		// Gets and Sets
		//
		public:

			EUINT32 GetModelPartsCount();

			ModelPart* GetModelPart(EUINT32 no);

			VertexBuffer* GetVertexBuffer(EUINT32 no);

			IndexBuffer* GetIndexBuffer(EUINT32 no);
			
		//
		// operators
		//
		private:

			Model& operator=(const Model& right) = delete;

		private:

			struct DataVertices
			{
				void* vertices;
				EUINT32 verticesCount;
				EUINT32 vertexSize;


				DataVertices()
				{
					vertices = NULLPTR;
					verticesCount = 0;
					vertexSize = 0;
				}
			};


			struct DataIndices
			{
				EUINT32* indices;
				EUINT32 indicesCount;
				CommonMaterialType material;

				BoundingSphere bsphere;
				BoundingBox bbox;

				DataIndices()
				{
					indices = NULLPTR;
					indicesCount = 0;
				}
			};











		};
	}

}


#endif