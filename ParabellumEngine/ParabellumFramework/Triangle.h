#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

//
//	Predefs
//
#include "DLLIE.h"
#include "TypesC.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "MathHelper.h"
#include "MathHelperExtended.h"
#include "Plane.h"

namespace ParabellumFramework
{

	//
	// Triangle - point must be passed counterclocwork
	//
	class XYZ_API Triangle
	{
	public:


	//
	//	Constructors and destructors
	//
	public:
		Triangle();
		Triangle(const Triangle& src);
		~Triangle();


	//
	// Methods
	//
	public:

		void GetMiddle(_OUT_ Vector3D& out);

		void GetNormal(_OUT_ Vector3D& out);

		EFLOAT32 Area();


	//
	// Members
	//
	public:

		union
		{
			struct
			{
				Vector3D a;
				Vector3D b;
				Vector3D c;
			};

			struct
			{
				Vector3D vertices[3];
			};
		};

	//
	// Gets And Sets
	//
	public:

		// Get V1 - V0 vector
		void GetU(_OUT_ Vector3D& out);

		// Get V2 - V0 vector
		void GetV(_OUT_ Vector3D& out);
	};
}


#endif