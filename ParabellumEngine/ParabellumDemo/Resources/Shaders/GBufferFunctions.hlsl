float GetLinearDepth(float ViewZ, float Znear, float Zfar)
{
	return ViewZ / Zfar;
}


float3 GetViewPositionFromLinearDepth(float2 UV, float Depth, float Zfar, float4x4 CameraInverseProjection)
{
	float4 position = 1.0f;
	position.x = UV.x * 2.0f - 1.0f;
	position.y = -(UV.y * 2.0f - 1.0f);
	position.z = Depth;

	float3 viewPosition = mul(position, CameraInverseProjection).xyz;
	float3 viewRay = float3(viewPosition.xy * (Zfar / viewPosition.z), Zfar);
	viewPosition = viewRay * Depth;
	return viewPosition;
}


//
//
// Reconstructs world position by inverting matrices
//
//
float3 GetWorldPositionFromLinearDepth(float2 UV, float Depth, float Zfar, float4x4 CameraInverseProjection, float4x4 CameraInverseView)
{
	float4 position = 1.0f;
	position.x = UV.x * 2.0f - 1.0f;
	position.y = -(UV.y * 2.0f - 1.0f);
	position.z = Depth;

	float3 viewPosition = mul(position, CameraInverseProjection).xyz;
	float3 viewRay = float3(viewPosition.xy * (Zfar / viewPosition.z), Zfar);
	viewPosition = viewRay * Depth;

	position = mul(float4(viewPosition, 1.0f), CameraInverseView);

	return position;
}



//
//
// Reconstructs world position using frustum corners (world position)
//
//
float3 GetWorldFromFrustumCorners(
	float2 UV, float depth, float3 cameraPosition,
	float3 ctl, float3 ctr, float3 cbr, float3 cbl
	)
{
	float3 position = 1.0f;

	// camera to corners
	float3 c2tl, c2tr, c2bl;

	// top left to top right and top left to bottom left
	float3 tl2tr, tl2bl, tl2br;

	// camera to far
	float3 c2f;

	c2tl = ctl - cameraPosition;
	c2tr = ctr - cameraPosition;
	c2bl = cbl - cameraPosition;

	tl2tr = c2tr - c2tl;
	tl2bl = c2bl - c2tl;

	tl2tr *= UV.x;
	tl2bl *= UV.y;

	c2f = tl2tr + tl2bl;
	c2f += c2tl;
	c2f *= depth;

	c2f += cameraPosition;

	position.xyz = c2f;


	return position;
}



//
//
// Reconstructs world position using frustum corners (world position)
//
// ctl - camera to top left
// ctl2ctr - camera to top left corner TO camera to top right corner
// ctl2cbl - camera to top left corner TO camera to bottom left
//
float3 GetWorldFromFrustumCorners(
	float2 UV, float depth, float3 cameraPosition,
	float3 c2tl, float3 ctl2ctr, float3 ctl2cbl
	)
{
	float3 position = 1.0f;

	// camera to far
	float3 c2f;

	float3 Nctl2ctr = ctl2ctr * UV.x;
	float3 Nctl2cbl = ctl2cbl * UV.y;

	c2f = Nctl2ctr + Nctl2cbl;
	c2f += c2tl;
	c2f *= depth;

	c2f += cameraPosition;

	position.xyz = c2f;


	return position;
}




uint C4U8toU32(uint r, uint g, uint b, uint a)
{
	uint result = 0;

	result = (r << 24) | (g << 16) | (b << 8) | a;

	return result;
}

uint4 U32toC4U8(uint input)
{
	uint4 result = uint4(0,0,0,0);

	result.r = (input & 0xFF000000) >> 24;
	result.g = (input & 0x00FF0000) >> 16;
	result.b = (input & 0x0000FF00) >> 8;
	result.a = (input & 0x000000FF);

	return result;
}



float4 C4U8toC4NF32(uint4 c4u32)
{
	float4 result;

	result.r = c4u32.r / 255.0f;
	result.g = c4u32.g / 255.0f;
	result.b = c4u32.b / 255.0f;
	result.a = c4u32.a / 255.0f;

	return result;
}

float4 C4U8toC4NF32(uint r, uint g, uint b, uint a)
{
	float4 result;

	result.r = r / 255.0f;
	result.g = g / 255.0f;
	result.b = b / 255.0f;
	result.a = a / 255.0f;

	return result;
}



//
//
// Convert float4 to uint2
//
//
uint2 PackFloat4toU2(float4 input)
{
	uint2 result = uint2(0, 0);

	float fR = input.r;
	float fG = input.g;
	float fB = input.b;
	float fA = input.a;

	result.r = (f32tof16(fR) << 16) | f32tof16(fG);
	result.g = (f32tof16(fB) << 16) | f32tof16(fA);

	return result;
}


//
//
// Pack float4 in float2
//
//
float2 PackFloat4toFloat2(float4 input)
{
	float2 result = float2(0.0f, 0.0f);

	uint2 packedu = PackFloat4toU2(input);

	result.r = asfloat(packedu.r);
	result.g = asfloat(packedu.g);

	return result;
}


//
//
// Convert input contains uint2 created by float4toU2()
//
//
float4 u2toFloat4(uint2 input)
{
	float4 result = float4(0.0f, 0.0f, 0.0f, 0.0f);

	uint iRG = asuint(input.r);
	uint iBA = asuint(input.g);

	result.r = f16tof32(iRG >> 16);
	result.g = f16tof32(iRG);

	result.b = f16tof32(iBA >> 16);
	result.a = f16tof32(iBA);


	return result;
}

//
//
// Convert compressed by float4toU2 float2 to float4
//
//

float4 UnpackFloat2toFloat4(float2 input)
{
	float4 result = float4(0.0f, 0.0f, 0.0f, 0.0f);

	uint iRG = asuint(input.r);
	uint iBA = asuint(input.g);

	result.r = f16tof32(iRG >> 16);
	result.g = f16tof32(iRG);

	result.b = f16tof32(iBA >> 16);
	result.a = f16tof32(iBA);

	return result;
}



//
//
// Pack float2 to float
//
//
float PackFloat2ToFloat(float2 input)
{
	float result;
	uint valu;

	valu = (f32tof16(input.r) << 16) | f32tof16(input.g);
	result = asfloat(valu);

	return result;
}


//
//
// Unpack float to float2
//
//
float2 UnpackFloatToFloat2(float input)
{
	float2 result;

	uint valu = asuint(input);

	result.r = f16tof32(valu >> 16);
	result.g = f16tof32(valu);

	return result;
}


//
//
// Pack float4 (with normalized value 0.0f-1.0f) to float
//
//
float PackFloat4ToFloat(float4 input)
{
	float result;
	uint valu;
	uint4 au;
	
	au.r = (uint)(input.r * 255) & 0x000000FF;
	au.g = (uint)(input.g * 255) & 0x000000FF;
	au.b = (uint)(input.b * 255) & 0x000000FF;
	au.a = (uint)(input.a * 255) & 0x000000FF;

	valu = (au.r << 24) | (au.g << 16) | (au.b << 8) | (au.a);

	result = asfloat(valu);

	return result;
}

//
//
// Pack float4 (with normalized value 0.0f-1.0f) to float
//
//
float4 UnpackFloatToFloat4(float input)
{
	float4 result;
	uint valu = asuint(input);
	uint4 au;

	au.r = (valu >> 24);
	au.g = (valu >> 16) & 0x000000FF;
	au.b = (valu >> 8) & 0x000000FF;
	au.a = valu & 0x000000FF;

	result.r = (float)au.r / 255.0f;//* 0.00392156862f;
	result.g = (float)au.g / 255.0f;//* 0.00392156862f;
	result.b = (float)au.b / 255.0f;//* 0.00392156862f;
	result.a = (float)au.a / 255.0f;//* 0.00392156862f;

	return result;
}
