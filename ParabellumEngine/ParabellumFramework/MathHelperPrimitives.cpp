#include "stdafx.h"
#include "MathHelperPrimitives.h"





//	------------------------------------------------------------------------------------------------
ParabellumFramework::EFLOAT32
ParabellumFramework::Math::Distance(_IN_ Vector3D* pVec, _IN_ BoundingBox* pBoundingBox)
{
	Vector3D bbcenter, fit, vec2fit, bbcenter2fit;
	EFLOAT32 d_vec2bbcenter, d_vec2fit;

	EFLOAT32 tan;

	pBoundingBox->GetCenter(&bbcenter);

	fit = *pVec;
	fit.Y = bbcenter.Y;

	vec2fit = (*pVec) - fit;
	bbcenter2fit = bbcenter - fit;

	tan = vec2fit.Length() / bbcenter2fit.Length();




	//d_vec2bbcenter = ((*pVec) - bbcenter).Length();
	//d_vec2fit = ((*pVec) - fit).Length();

	return 0.0f;

}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::EFLOAT32
ParabellumFramework::Math::Distance(_IN_ Ray* pRay, _IN_ Vector3D* pPoint)
{
	return Distance(&pRay->begin, &pRay->direction, pPoint);
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::EFLOAT32
ParabellumFramework::Math::Distance(_IN_ Ray* pRay, _IN_ Vector3D* pPoint, _OUT_ Vector3D* pOnRay)
{
	Math::NearestPointOnRay(&pRay->begin, &pRay->direction, pPoint, pOnRay);

	return ( (*pOnRay) - (*pPoint) ).Length();
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::EFLOAT32
ParabellumFramework::Math::Distance(_IN_ Ray* pRay, _IN_ BoundingSphere* pBoundingsphere)
{
	return Distance(&pRay->begin, &pRay->direction, pBoundingsphere->GetCenter()) - pBoundingsphere->GetRayLength();
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::EFLOAT32
ParabellumFramework::Math::Distance(_IN_ BoundingSphere* pBoundingsphere1, _IN_ BoundingSphere* pBoundingsphere2)
{
	Vector3D sub;

	sub = (*pBoundingsphere1->GetCenter()) - (*pBoundingsphere2->GetCenter());

	return ( sub.Length() - pBoundingsphere1->GetRayLength() - pBoundingsphere2->GetRayLength() );
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::EFLOAT32
ParabellumFramework::Math::Distance(_IN_ BoundingSphere* pBoundingsphere, _IN_ BoundingBox* pBoundingBox)
{
	Vector3D bbmin, bbmax, bbcenter;
	Vector3D bscenter;

	pBoundingBox->GetCenter(&bbcenter);
	pBoundingBox->GetMinMax(bbmin, bbmax);
	bscenter = *pBoundingsphere->GetCenter();



	return 0.0f;

}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::IntersectCodes
ParabellumFramework::Math::Intersects(_IN_ Vector3D* pVol1, _IN_ BoundingSphere* pVol2)
{
	Vector3D d, sphereCenter;
	d = (*pVol1) - (*pVol2->GetCenter());

	if (Vector3D::SquareLength(d) < (pVol2->GetRayLength() * pVol2->GetRayLength()))
		return IntersectCodes::CONTAIN;

	return IntersectCodes::DISJOINT;
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::IntersectCodes
ParabellumFramework::Math::Intersects(_IN_ Vector3D* pVol1, _IN_ BoundingBox* pVol2)
{
	return pVol2->Intersects(pVol1);
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::IntersectCodes
ParabellumFramework::Math::Intersects(_IN_ BoundingSphere* pVol1, _IN_ BoundingSphere* pVol2)
{
	return pVol1->Intersects(pVol2);
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::IntersectCodes
ParabellumFramework::Math::Intersects(_IN_ BoundingSphere* pVol1, _IN_ BoundingBox* pVol2)
{
	return pVol2->Intersects(pVol1);
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::IntersectCodes
ParabellumFramework::Math::Intersects(_IN_ Ray* pRay, _IN_ BoundingSphere* pBoundingSphere)
{
	if (Distance(pRay, pBoundingSphere) < pBoundingSphere->GetRayLength())
		return IntersectCodes::INTERSECT;
	return IntersectCodes::DISJOINT;
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::IntersectCodes
ParabellumFramework::Math::Intersects(_IN_ Ray* pRay, _IN_ BoundingSphere* pBoundingSphere, _OUT_ Vector3D* pIntersectionPointIn, _OUT_ Vector3D* pIntersectionPointOut)
{
	return Intersects(&pRay->begin, &pRay->direction, pBoundingSphere->GetCenter(), pBoundingSphere->GetRayLength(), pIntersectionPointIn, pIntersectionPointOut);
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::IntersectCodes
ParabellumFramework::Math::Intersects(_IN_ Ray* pRay, _IN_ BoundingBox* pBoundingBox)
{
	Vector4D bbmin, bbmax, rbeg, rdir, irdir;
	Vector4D d0, d1, v0, v1;

	irdir.X = 1.0f / rdir.X;
	irdir.Y = 1.0f / rdir.Y;
	irdir.Z = 1.0f / rdir.Z;
	irdir.W = 1.0f;

	d0 = (bbmin - rbeg) * irdir;
	d1 = (bbmax - rbeg) * irdir;

	Vector4D::Min(&d0, &d1, &v0);
	Vector4D::Max(&d0, &d1, &v0);

	return IntersectCodes::DISJOINT;
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::IntersectCodes
ParabellumFramework::Math::Intersects(_IN_ Ray* pVol1, _IN_ Triangle* pVol2)
{
	Vector3D u, v, n;		// u,v, normal of a triangle

	Vector3D rb, rd;		// ray begin
	Vector3D con;			// ray begin

	pVol2->GetU(u);
	pVol2->GetV(v);
	Vector3D::Cross(&n, &u, &v);

	return IntersectCodes::DISJOINT;
}
//	------------------------------------------------------------------------------------------------
ParabellumFramework::IntersectCodes
ParabellumFramework::Math::Intersects(_IN_ Volume* pVol1, _IN_ BoundingSphere* pVol2)
{
	IntersectCodes ic;

	for (EUINT32 i = 0; i < pVol1->GetVerticesCount(); ++i)
	{
		ic = Math::Intersects(pVol1->GetVertex(i), pVol2);

		if (ic == IntersectCodes::CONTAIN)
			return IntersectCodes::CONTAIN;
	}

	return IntersectCodes::DISJOINT;
}
//	------------------------------------------------------------------------------------------------