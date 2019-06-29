#ifndef _BOUNDINGVOLUMES_H_
#define _BOUNDINGVOLUMES_H_


//
//	Predefs
//
#include "DLLIE.h"
#include "TypesC.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix4x4.h"
#include "IntersectCodes.h"
#include "SystemCodes.h"

#include <list>
#include <vector>

namespace ParabellumFramework
{
		class BoundingBox;
		class BoundingSphere;

		class XYZ_API BoundingSphere
		{

		public:
			BoundingSphere();
			BoundingSphere(BoundingSphere& src);
			~BoundingSphere();

		public:

			EUINT32 MakeBoundingSphere(_IN_ Vector3D* pCenter, EFLOAT32 ray);
			EUINT32 MakeBoundingSphere(EFLOAT32 px, EFLOAT32 py, EFLOAT32 pz, EFLOAT32 ray);
			EUINT32 MakeFromBoundingBox(_IN_ BoundingBox* pBox);
			EUINT32 MakeFromPoints(_IN_ Vector3D* pPoints, EUINT32 pointsCount);

			IntersectCodes Intersects(_IN_ BoundingBox* pVolume);
			IntersectCodes Intersects(_IN_ BoundingSphere* pVolume);
			IntersectCodes Intersects(_IN_ Vector3D* pVolume);

			// merge this bounding box with Vector3D
			void Merge(_IN_ Vector3D* pVolume);

		//
		// Members
		//
		private:
			Vector3D m_center;
			EFLOAT32 m_ray;

			union
			{
				struct
				{
					Vector3D m_center;
					EFLOAT32 m_ray;
				};

				struct
				{
					// XYZ - center, W - ray length
					Vector4D centerRay;
				};

			};

		//
		// Gets and sets
		//
		public:
			void SetCenter(_IN_ Vector3D& center);
			void SetRayLength(EFLOAT32 rayLength);

			Vector3D* GetCenter();
			EFLOAT32 GetRayLength();

		//
		// Operators
		//
		public:
			inline BoundingSphere& operator=(const BoundingSphere& right)
			{
				this->m_center = right.m_center;
				this->m_ray = right.m_ray;

				return *this;
			}
		};

		//
		//
		//	AABB
		//
		//
		class XYZ_API BoundingBox
		{
		public:
			BoundingBox();
			BoundingBox(BoundingBox& src);
			~BoundingBox();


		public:
		
			EUINT32 MakeFromPoints(_IN_ Vector3D* min, _IN_ Vector3D* max);
			EUINT32 MakeFromPoints(EFLOAT32 x0, EFLOAT32 y0, EFLOAT32 z0, EFLOAT32 x1, EFLOAT32 y1, EFLOAT32 z1);
			EUINT32 MakeFromPoints(_IN_ Vector3D* pPoints, EUINT32 pointsCount);
			EUINT32 MakeFromSphere(_IN_ BoundingSphere * pSphere);
			EUINT32 MakeCube(EFLOAT32 length);

			IntersectCodes Intersects(_IN_ BoundingBox* pVolume);
			IntersectCodes Intersects(_IN_ BoundingSphere* pVolume);
			IntersectCodes Intersects(_IN_ Vector3D* pVolume);

			// merge this bounding box with another BoundingBox
			void Merge(_IN_ BoundingBox* pVolume);

			// merge this bounding box with Vector3D
			void Merge(_IN_ Vector3D* pVolume);

		//
		// Members
		//
		private:
			Vector3D m_min;
			Vector3D m_max;

		//
		// gets and sets
		//
		public:

			// get the center point of the BoundingBox
			void GetCenter(_OUT_ Vector3D* pVec);

			Vector3D* GetMin();
			Vector3D* GetMax();

			EFLOAT32 GetWidth();
			EFLOAT32 GetHeight();
			EFLOAT32 GetDepth();

			// get corners of the BoundingBox - pVector must be an array of 8 size
			void GetCorners(_OUT_ Vector3D* pVectors);			

			void GetMinMax(_OUT_ Vector3D& minv, _OUT_ Vector3D& maxv);


		//
		// Operators
		//
		public:
			inline BoundingBox& operator=(const BoundingBox& right)
			{
				this->m_min = right.m_min;
				this->m_max = right.m_max;

				return *this;
			}
		};



		//
		//
		//	Extended BoundingBox - can be rotated
		//
		//
		class XYZ_API OrientedBoundingBox
		{
		public:
			OrientedBoundingBox();
			OrientedBoundingBox(OrientedBoundingBox& src);
			~OrientedBoundingBox();


		public:
			void Make(_IN_ BoundingBox* pBox, _IN_ Vector3D* scale, _IN_ Vector3D* rotation, _IN_ Vector3D* position);
			void Make(_IN_ Vector3D* min, _IN_ Vector3D* max, _IN_ Matrix4x4* pWorld);


		//
		// Members
		//
		private:
			Vector3D m_points[8];


		//
		// gets and sets
		//
		public:

			// get corners of the BoundingBox - pVector must be an array of 8 size
			void GetCorners(_OUT_ Vector3D* pVectors);

			// get corners of the BoundingBox - pVector must be an array of 8 size
			Vector3D* GetCorners();

		//
		// Operators
		//
		public:
			inline OrientedBoundingBox& operator=(const OrientedBoundingBox& right)
			{
				this->m_points[0] = right.m_points[0];
				this->m_points[1] = right.m_points[1];
				this->m_points[2] = right.m_points[2];
				this->m_points[3] = right.m_points[3];
				this->m_points[4] = right.m_points[4];
				this->m_points[5] = right.m_points[5];
				this->m_points[6] = right.m_points[6];
				this->m_points[7] = right.m_points[7];

				return *this;
			}
		};



		//
		//
		//	Extended BoundingBox - can be rotated
		//
		//





}

#endif