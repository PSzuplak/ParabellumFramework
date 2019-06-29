//	------------------------------------------------------------------
//	Skydome
//
//
//
//	------------------------------------------------------------------





//	------------------------------------------------------------------
//	Includes
//	------------------------------------------------------------------

#include "GBufferFunctions.hlsl"
#include "NormalCode.hlsl"

//	------------------------------------------------------------------
//	Globals
//	------------------------------------------------------------------
float4x4 g_cameraView;
float4x4 g_cameraProjection;
float4x4 g_world;
float4x4 g_worldViewIT;
float4 g_cameraPositionFar;		// XYZ - camera position, W - camera Z Far


//	------------------------------------------------------------------
//	Resources
//	------------------------------------------------------------------

Texture2D g_TSkyMerged:register(t0);		//	RGBA - Color


SamplerState SamplerT0:register(s0);



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
	float4 Position			: SV_POSITION;
	float4 WorldPosition	: TEXCOORD0;
	float2 ScreenPosition	: TEXCOORD1;
	float2 UV				: TEXCOORD2;
};


struct PixelShaderOutput
{
	float4 Buffer0 : SV_Target0;		// RGB - world position XYZ, A - view position Z
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

	input.Position.w = 1.0f;


	// set texture coords
	output.UV = input.UV;

	// set position
	float4 worldPosition = mul(input.Position, g_world);
	float4 viewPosition = mul(worldPosition, g_cameraView);
	output.Position = mul(viewPosition, g_cameraProjection);
	output.WorldPosition = worldPosition;



	return output;
}



//	------------------------------------------------------------------
//	Function for Pixel Shader
//	------------------------------------------------------------------
PixelShaderOutput PixelShaderFunction(VertexShaderOutput input) : SV_TARGET
{
	PixelShaderOutput output;

	output.Buffer0 = float4(1.0f, 1.0f, 1.0f, 1.0f);

	// sample text which has polar coordinates
	//float3 camToWorld = input.WorldPosition.xyz - float3(0.0f, 0.0f, 0.0f);//g_cameraPositionFar.xyz;
	//float angle = dot(normalize(g_cameraPositionFar.xyz - input.WorldPosition.xyz), float3(0.0f, -1.0f, 0.0f));
	//float3 target = normalize(camToWorld);
	//float2 currentUV = float2(0.5f, 0.5f) - target.xz * 0.5f;
	//output.Buffer0 = g_TSkyMerged.Sample(SamplerT0, currentUV);



	//float angle = dot(normalize(g_cameraPositionFar.xyz - input.WorldPosition.xyz), float3(0.0f, -1.0f, 0.0f));
	//float3 camToWorld = input.WorldPosition.xyz - float3(0.0f, 0.0f, 0.0f);//g_cameraPositionFar.xyz	
	//float2 currentUV;
	//float4 gradL0 = float4(1.0f, 1.0f, 1.0f, 0.5f);
	//float4 gradL1 = float4(0.2f, 0.7f, 1.0f, 0.5f);	
	//float lenSky = angle;
	//float4 gradColor = (gradL1 * lenSky) + ((1.0f - lenSky) * gradL0);
	//float dotp = dot(normalize(camToWorld).xz, float2(0.0f, 1.0f));
	//currentUV.x = (dotp + 1.0f) * 0.5f;
	//currentUV.y = normalize(camToWorld).y;
	//float4 cloudColor = g_TSkyMerged.Sample(SamplerT0, currentUV);
	//output.Buffer0 = (cloudColor * cloudColor.a) + ((1.0f - cloudColor.a) * gradColor);
	//output.Buffer0 = g_TSkyMerged.Sample(SamplerT0, input.UV);


	//
	//
	// cylindric clouds
	float3 camToWorld = input.WorldPosition.xyz - g_cameraPositionFar.xyz;

	float3 nC2W = camToWorld;
	nC2W.y = 0.0f;
	nC2W = normalize(nC2W);

	float dotCWZ = dot(nC2W.xz, float2(0.0f, 1.0f));
	float2 currentUV = input.UV;

	currentUV.x = (dotCWZ + 1.0f) * 0.5f;

	output.Buffer0 = g_TSkyMerged.Sample(SamplerT0, currentUV);


	//output.Buffer0.r += input.UV.x;
	//output.Buffer0.r = currentUV.x;






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