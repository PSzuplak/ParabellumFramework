#ifndef _RAY_H_
#define _RAY_H_

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
	// Ray
	//
	class XYZ_API Ray
	{
	public:


	//
	//	Constructors and destructors
	//
	public:
		Ray();
		Ray(_IN_ Vector3D& vbeg, _IN_ Vector3D& vdir);
		Ray(const Ray&);
		~Ray();


	//
	// Methods
	//
	public:

		// make a ray from 2 points
		void MakeFromPoints(_IN_ Vector3D& v0, _IN_ Vector3D& v1);

		// make a ray using a begin vector and a direction vector.
		void MakeParametric(_IN_ Vector3D& vbeg, _IN_ Vector3D& vdir);

	//
	// Members
	//
	public:

		// begin of the ray
		Vector3D begin;

		// direction of the ray - must be a normal vector
		Vector3D direction;

	//
	// Gets and Sets
	//
	public:

	};
}


#endif