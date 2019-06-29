//	------------------------------------------------------------------
//	Includes
//	------------------------------------------------------------------

#include "GBufferFunctions.hlsl"
#include "NormalCode.hlsl"
#include "LightFunctions.hlsl"

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

Texture2D g_TDiffuseMap:register(t0);		//	RGBA - Color
Texture2D g_TNSMap:register(t1);			//	RG - Normal vector ('z' always 1.0), BA - Specular power/intensity
Texture2D g_THGRRMap:register(t2);			//	R = height factor, G - glow, B - reflection, A - emissive/refraction


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
	float2 UV : TEXCOORD0;
	float3 Depth : TEXCOORD1;
	float3 Normal : TEXCOORD2;
	float3x3 TBN : TEXCOORD3;
	float4 ScreenUV : TEXCOORD6;
};


struct PixelShaderOutput
{
	float4 Color0 : SV_Target0;
	float4 Color1 : SV_Target1;
	float4 Color2 : SV_Target2;
	float4 Color3 : SV_Target3;
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

	// set depth
	output.Depth = float3(1.0f, 1.0f, 1.0f);
	output.Depth.x = output.Position.z;
	output.Depth.y = output.Position.w;
	output.Depth.z = viewPosition.z;

	// set screen (projecion) coordinates
	output.ScreenUV = output.Position;

	// set TBN matrix
	output.TBN[0] = normalize(mul(input.Tangent, (float3x3)g_worldViewIT));
	output.TBN[1] = normalize(mul(input.Binormal, (float3x3)g_worldViewIT));
	output.TBN[2] = normalize(mul(input.Normal, (float3x3)g_worldViewIT));

	// transform normal to local coodinates
	output.Normal = mul(input.Normal, output.TBN);

	return output;
}



//	------------------------------------------------------------------
//	Function for Pixel Shader
//	------------------------------------------------------------------
PixelShaderOutput PixelShaderFunction(VertexShaderOutput input) : SV_TARGET
{
	PixelShaderOutput output;

	//.output.Color = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//output.Color.r = input.Color.w;
	//output.Color.g = input.Color.w;
	//output.Color.b = input.Color.w;
	output.Color0 = float4(1.0f, 0.0f, 0.0f, 0.0f);
	output.Color1 = float4(0.0f, 0.0f, 0.0f, 0.0f);
	output.Color2 = float4(0.0f, 0.0f, 1.0f, 0.0f);
	output.Color3 = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	output.Color0 = g_TDiffuseMap.Sample(SampleType, input.UV);

	// set depth
	output.Color1.r = GetLinearDepth(input.Depth.z, 1.0f, g_cameraPositionFar.w);

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