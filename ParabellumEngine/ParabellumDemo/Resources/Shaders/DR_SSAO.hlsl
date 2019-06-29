//	------------------------------------------------------------------
//	SSAO - :)
//	
//
//
//
//	------------------------------------------------------------------


//	------------------------------------------------------------------
//	Includes
//	------------------------------------------------------------------

#include "GBufferFunctions.hlsl"
#include "NormalCode.hlsl"
#include "LightFunctions.hlsli"

//	------------------------------------------------------------------
//	Globals
//	------------------------------------------------------------------

float4x4 g_cameraView;
float4x4 g_cameraInverseView;
float4x4 g_cameraProjection;
float4x4 g_cameraInverseProjection;
float4x4 g_cameraInverseViewProjection;
float4 g_cameraPositionFar;				// RGB - camera position, A - z Far
float4 g_IRBA;							// intensity/radious/bias/iterations

float4 g_ctl;					// camera position to top left frustum corner
float4 g_ctl2ctr;				// camera position to top left frustum corner TO camera position to top rigt frustum corner
float4 g_ctl2cbl;				// camera position to top left frustum corner TO camera position to bottom left frustum corner


//	------------------------------------------------------------------
//	Resources
//	------------------------------------------------------------------

Texture2D g_RTGBuffer00:register(t0);
Texture2D g_RTGBuffer01:register(t1);

SamplerState SamplerT0:register(s0);
SamplerState SamplerT1:register(s1);

//	------------------------------------------------------------------
//	Shader Structs
//	------------------------------------------------------------------
struct VertexShaderInput
{
	float4 Position : POSITION0;
	float2 UV : TEXCOORD0;
};

struct VertexShaderOutput
{
	float4 Position : SV_POSITION;
	float2 UV : TEXCOORD0;
};


struct PixelShaderOutput
{
	float4 BufferSSAO : SV_Target0;
};




//	------------------------------------------------------------------
//	Function for Vertex Shader
//	------------------------------------------------------------------
VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output;

	input.Position.w = 1.0f;
	output.Position = input.Position;
	output.UV = input.UV;
	return output;
}



//	------------------------------------------------------------------
//	Function for Pixel Shader
//	------------------------------------------------------------------
PixelShaderOutput PixelShaderFunction(VertexShaderOutput input) : SV_TARGET
{
	PixelShaderOutput output;
	output.BufferSSAO = float4(1.0f, 1.0f, 1.0f, 1.0f);

	float4 worldZ;
	float4 position;

	float depth;
	float4 rtNormal;
	float3 normal;


	float3 target;
	float lengthTarget;
	float4 randomVector;
	float shading = 0.0f;
	float2 currentUV;				// sample texture coordinates
	float4 samplePositionViewZ;
	float4 samplePositionWorld;
	float angle;					// angle between current pixel position and sample positon
	float3 actualToCurrent;			// vector from current pixel to sample pixel

	// directions for sample vectors
	const float2 sampleUV[8] =	{	
									float2(0.5f, 0), float2(-0.5f, 0), float2(0, 0.5f), float2(0, -0.5f),
									float2(0.5f, 0.5f), float2(-0.5f, -0.5f), float2(-0.5f, 0.5f), float2(0.5f, -0.5f)
								};


	// read world positon and viewZ
	worldZ = g_RTGBuffer00.Sample(SamplerT0, input.UV);


	//if (worldZ.w > 0.1f)
	//{
	//	output.BufferSSAO = float4(1.0f, 1.0f, 1.0f, 1.0f);
	//	return output;
	//}


	position.xyz = GetWorldFromFrustumCorners(
		input.UV, worldZ.w, g_cameraPositionFar.xyz,
		g_ctl.xyz, g_ctl2ctr.xyz, g_ctl2cbl.xyz
		);

	position.w = worldZ.w;

	target = position.xyz - g_cameraPositionFar.xyz;
	lengthTarget = length(target);


	// read and decode normal vector
	rtNormal = g_RTGBuffer01.Sample(SamplerT1, input.UV);

	// read and decode normal vector
	float4 unpackedNormal = UnpackFloat2toFloat4(rtNormal.rg);

	normal = NormalDecode(unpackedNormal.xyz);
	normal = normalize(normal);



	//
	// SSAO
	// ssao
	for (int i = 0; i < 8; ++i)
	{
		float4 randomVector = float4(0.0f, 0.0f, 0.0f, 0.0f);

		// calculate coords which are related to random vector
		currentUV = normalize( sampleUV[i % 8] + randomVector.xy ) *  g_IRBA.y;

		// div coords to keep perspective
		currentUV /= lengthTarget;
		currentUV += input.UV;

		// process ssao only if there is no big difference between
		// position of sample and position of current pixel
		samplePositionViewZ = g_RTGBuffer00.Sample(SamplerT0, currentUV);

		samplePositionWorld.xyz = GetWorldFromFrustumCorners(
			currentUV, samplePositionViewZ.w, g_cameraPositionFar.xyz,
			g_ctl.xyz, g_ctl2ctr.xyz, g_ctl2cbl.xyz
			);

		samplePositionWorld.w = samplePositionViewZ.w;

		// get vector from actual processed pixel to sample pixel
		actualToCurrent = samplePositionWorld.xyz - position.xyz;
		float lenActualToCurrent = length(actualToCurrent);

		if (lenActualToCurrent < g_IRBA.z)
		{
			// calculate angle between actual processed pixel and sample pixel
			angle = dot(normal, normalize(actualToCurrent));

			// calculate shading factor
			if ( (angle > g_IRBA.a) && (angle < 0.99f) )
				shading += angle * (1.0f / (1.0f + lenActualToCurrent)) * g_IRBA.x;
		}
	}

	shading /= 8.0f;

	shading = clamp(shading, 0.0f, 1.0f);

	output.BufferSSAO.rgba = 1.0f - float4(shading, shading, shading, shading);


	return output;
}




//	------------------------------------------------------------------
//	Techniques
//	------------------------------------------------------------------
technique11 ShaderTechnique
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_5_0, VertexShaderFunction()));
		SetPixelShader(CompileShader(ps_5_0, PixelShaderFunction()));
		SetGeometryShader(NULL);
	}
}