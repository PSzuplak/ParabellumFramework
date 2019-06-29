//	------------------------------------------------------------------
//	Create Depth buffer - for shadow mapping
//
//
//	Return
//		Buffer0.r - packed RGBA for solid models
//		Buffer0.g - solid model view Z component from the light perspective
//		Buffer0.b - packed RGBA for transparent models
//		Buffer0.a - transparent model view Z component from the light perspective
//	------------------------------------------------------------------





//	------------------------------------------------------------------
//	Includes
//	------------------------------------------------------------------

#include "GBufferFunctions.hlsl"
#include "NormalCode.hlsl"
#include "MathHelper.hlsl"

//	------------------------------------------------------------------
//	Globals
//	------------------------------------------------------------------
float4x4 g_world;
float4x4 g_cameraView;
float4x4 g_cameraProjection;

//	------------------------------------------------------------------
//	Resources
//	------------------------------------------------------------------

Texture2D g_TDiffuseMap:register(t0);		//	RGBA - Color
SamplerState SampleType:register(s0);



//	------------------------------------------------------------------
//	Shader Structs
//	------------------------------------------------------------------
struct VertexShaderInput
{
	float4 Position : POSITION0;
	float2 UV : TEXCOORD0;
	float3 Normal : NORMAL0;
	float3 Tangent : TANGENT0;
	float3 Binormal : BINORMAL0;
	float2 Properties : TEXCOORD1;
};

struct VertexShaderOutput
{
	float4 Position : SV_POSITION;
	float4 WorldPosition : TEXCOORD0;
	float2 UV : TEXCOORD1;
};


struct PixelShaderOutput
{
	float4 Buffer0 : SV_Target0;
};




//	------------------------------------------------------------------
//	Function for Vertex Shader
//	------------------------------------------------------------------
VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output;

	input.Position.w = 1.0f;

	// set texture coords
	output.UV = input.UV;

	// set position
	float4 worldPosition = mul(float4(input.Position.xyz, 1.0f), g_world);
	float4 viewPosition = mul(worldPosition, g_cameraView);
	output.Position = mul(viewPosition, g_cameraProjection);

	output.WorldPosition = worldPosition;
	//output.WorldPosition.w = DistanceToPlane(g_lightPlaneNear.xyz, g_lightPlaneNear.w, worldPosition.xyz);

	// set depth
	output.WorldPosition.w = GetLinearDepth(viewPosition.z, 1.0f, 1000.0f);

	return output;
}



//	------------------------------------------------------------------
//	Function for Pixel Shader
//	------------------------------------------------------------------
PixelShaderOutput PixelShaderFunction(VertexShaderOutput input) : SV_TARGET
{
	PixelShaderOutput output;

	output.Buffer0 = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	output.Buffer0 = input.WorldPosition;
	output.Buffer0.w = input.WorldPosition.w;

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