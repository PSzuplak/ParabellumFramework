#ifndef _MODELPART_H_
#define _MODELPART_H_

//
//	Predefs
//
#include "DLLIE.h"

#include "TypesC.h"
#include "IOCodes.h"

#include "GraphicsDevice.h"

#include "Texture2D.h"
#include "CommonMaterialType.h"

#include "BoundingVolumes.h"


namespace ParabellumFramework
{
	namespace Graphics
	{
		//
		//
		// Containt only ONE mesh. Model is build from ModelPart objects
		//
		//
		class XYZ_API ModelPart
		{
			friend class Model;

		public:
			ModelPart();
			ModelPart(const ModelPart&);
			~ModelPart();

		private:


		//
		// Members
		//
		public:


			// index in Model::ModelObjects
			EUINT32 m_vbIndex;

			// index in Model::ModelObjects
			EUINT32 m_ibIndex;

			// model part material
			CommonMaterialType m_material;

			// boundingbox
			BoundingBox m_boundingBox;

			//bounding sphere
			BoundingSphere m_boundingSphere;

		};
	}

}


#endif