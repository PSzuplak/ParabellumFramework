#include "stdafx.h"
#include "Ray.h"


//	------------------------------------------------------------
//		Constructors and destructors
//	------------------------------------------------------------
ParabellumFramework::Ray::Ray()
{
}

ParabellumFramework::Ray::Ray(_IN_ Vector3D& vbeg, _IN_ Vector3D& vdir)
{
	MakeParametric(vbeg, vdir);
}

ParabellumFramework::Ray::Ray(const Ray& src)
{
}


ParabellumFramework::Ray::~Ray()
{
}


//	------------------------------------------------------------
//		Methods
//	------------------------------------------------------------

void 
ParabellumFramework::Ray::MakeFromPoints(_IN_ Vector3D& v0, _IN_ Vector3D& v1)
{
	begin = v0;
	direction = v1 - v0;
}

void
ParabellumFramework::Ray::MakeParametric(_IN_ Vector3D& vbeg, _IN_ Vector3D& vdir)
{
	begin = vbeg;
	direction = vdir;
}

//	------------------------------------------------------------
//		Gets And Sets
//	------------------------------------------------------------
