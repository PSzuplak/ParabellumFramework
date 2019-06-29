#ifndef _PLANE_H_
#define _PLANE_H_

//
//	Predefs
//
#include "DLLIE.h"
#include "TypesC.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "MathHelper.h"


namespace ParabellumFramework
{

		//
		//
		//
		//
		//
		class XYZ_API Plane
		{
		public:
			union
			{
				// 16 bytes data
				struct
				{
					EFLOAT32 m_data[4];
				} DATA;

				// parametrs of plane equation ax + by + cz + dw = 0. 
				// abc - normal; d - distance from the point (0.0f, 0.0f, 0.0f)
				struct
				{
					EFLOAT32 a;
					EFLOAT32 b;
					EFLOAT32 c;
					EFLOAT32 d;
				};

				struct
				{
					EFLOAT32 m_parametrs[4];
				};

				struct
				{
					Vector4D m_V4D;
				};

			};


		//
		//	Constructors and destructors
		//
		public:
			Plane();
			Plane(const Plane&);
			~Plane();


		//
		// Methods
		//
		public:

			// plane normalization
			inline void Normalize();			


			inline void MakeFromPoints(_IN_ Vector3D& v1, _IN_ Vector3D& v2, _IN_ Vector3D& v3);


			static inline void Intersect(_IN_ Plane* Plane1, _IN_ Plane* Plane2, _IN_ Plane* Plane3, _OUT_ Vector3D* pOut);

		//
		//	Gets and Sets
		//
		public:

			void GetPlane(_OUT_ Vector4D* pOut);

			inline void GetNormal(_OUT_ Vector3D* pOut);	// get normal
			inline EFLOAT32  GetPlaneDistance();			// get distance from the point (0,0,0)

			inline EFLOAT32 GetDistance(_IN_ Vector3D* pVector);	// get distace vector form the plane
																	// if its lower than 0 it means that vector
																	// is placed "behind" plane (depent of plane normal)


			inline EFLOAT32 GetSquareDistance(_IN_ Vector3D* pVector);	// get square distance from the plane
																		// a bit faster than GetDistance

		};
}


#endif