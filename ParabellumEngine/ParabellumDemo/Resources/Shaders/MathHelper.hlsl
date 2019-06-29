//
// Some math function
//
//


float2 SSToUV(float2 ss)
{
	float2 uv;
	uv.x = (ss.x + 1.0f) * 0.5f;
	uv.y = (-ss.y + 1.0f) * 0.5f;
	return uv;
}


float2 UVToSS(float2 uv)
{
	float2 ss;
	ss.x = uv.x * 2.0f - 1.0f;
	ss.y = -(uv.y * 2.0f - 1.0f);
	return ss;
}




float3 CartesianToSpherical(float3 cart)
{
	float ro = sqrt(cart.x * cart.x + cart.y * cart.y + cart.z * cart.z);
	float teta = acos(cart.z / ro);
	float fi = atan2(cart.y, cart.x);

	return float3(ro, teta, fi);
}


float2 CartesianToPolar2D(float2 cart)
{
	float2 polar = 0.0f;
	float angle = 0.0f;

	polar.x = length(cart);

	if ((cart.x > 0.0f) && (cart.y >= 0.0f))
	{
		polar.y = atan(cart.y / cart.x);
	}
	else if ((cart.x > 0.0f) && (cart.y < 0.0f))
	{
		polar.y = atan(cart.y / cart.x) + 1.57079632679f;
	}
	else if (cart.x < 0.0f)
	{
		polar.y = atan(cart.y / cart.x) + 3.14159265359f;
	}
	else if (cart.y > 0.0f)
	{
		polar.y = 1.57079632679f;
	}
	else if (cart.y < 0.0f)
	{
		polar.y = 3.14159265359f * 1.5f;
	}


	return polar;
}


float DistanceToRay(float3 rayBegin, float3 rayDirection, float3 pointToFind)
{
	float3 sub, cross_sub_vd;

	sub = rayBegin - pointToFind;
	cross_sub_vd = cross(sub, rayDirection);

	return length(cross_sub_vd) / length(rayDirection);
}


//
// pn - plane normal vector
// pd - plande distance // 'd' in ax+by+cz+d=0 equation
// ptf - point to find
//
float DistanceToPlane(float3 pn, float pd, float3 ptf)
{
	//return (pn.x * ptf.x + pn.y * ptf.y + pn.z * ptf.z + pd) / sqrt(pn.x*pn.x + pn.y*pn.y + pn.z*pn.z);
	return ( dot(pn, ptf) + pd) / sqrt( dot(pn, pn) );
}

//
// does not calculate a distance but it works if we want to compare 2 points from the same plane
//
float DistanceCrap(float3 pn, float pd, float3 ptf)
{
	return dot(pn, ptf);
}


//
// Plane equation r*x + g*y + b*z + a = 0
//
// Plane parametrs RGB must be normalized and distance parametr 'a' must be equal to a distance 
// to the top left corner of a orthographic box
// Example:
//		planeTop.XYZ.Normalize();
//		planeTop.W = Math::Distance(&light00_plane_top.XYZ, 0.0f, boxTopLeft corner);
//
// planeTop
// planeLeft
// boxSize - vertical and horizonal size of orthographic box
// ptf - point to find
//
float2 GetUVForOrthographicProjection(float4 planeTop, float4 planeLeft, float2 boxSize, float3 ptf)
{
	float2 result;

	result.x = DistanceToPlane(planeLeft.xyz, planeLeft.w, ptf.xyz);
	result.y = DistanceToPlane(planeTop.xyz, planeTop.w, ptf.xyz);
	result.xy /= boxSize.xy;

	return result;
}


//
//
//
//
//	result.X = Screen position X (-1.0f, 1.0f)
//	result.Y = Screen position Y (-1.0f, 1.0f)
//	result.Z = Screen position Y (-1.0f, 1.0f)
//	result.W = distance from the near plane divided by 'zFar'
float4 PerspectiveProjectionFromPlanes(float4 planeNear, float4 planeTop, float4 planeLeft, float zFar, float3 ptf)
{
	float4 result;

	result = float4(0.0f, 0.0f, 0.0f, 0.0f);

	result.z = DistanceToPlane(planeNear.xyz, planeNear.w, ptf.xyz) / zFar;

	return result;
}

// if point 'ptf' lises inside AABB definied by min, max
bool Intersect(float3 bbmin, float3 bbmax, float3 ptf)
{
	if ((ptf.x >= bbmin.x) && (ptf.y >= bbmin.y) && (ptf.z >= bbmin.z) &&
		(ptf.x <= bbmax.x) && (ptf.y <= bbmax.y) && (ptf.z <= bbmax.z)
		)
		return true;

	return false;
}



// are coordinates in non-wrapped uv cooridnates 
bool InUVRange(float2 coords)
{
	if ( (coords.x < 1.0f) && (coords.x > 0.0f) && (coords.y < 1.0f) && (coords.y > 0.0f) )
		return true;

	return false;
}