//	------------------------------------------------------------------
//	GBuffer for solid models
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
float4 g_lightDirectionAmbient;

//	------------------------------------------------------------------
//	Resources
//	------------------------------------------------------------------

Texture2D g_TDiffuseMap:register(t0);		//	RGBA - Color
Texture2D g_TNSMap:register(t1);			//	RG - Normal vector ('z' always 1.0), BA - Specular power/intensity
Texture2D g_THGRRMap:register(t2);			//	R = height factor, G - glow, B - reflection, A - emissive/refraction


SamplerState SamplerT0:register(s0);
SamplerState SamplerT1:register(s1);
SamplerState SamplerT2:register(s2);;



//	------------------------------------------------------------------
//	Shader Structs
//	------------------------------------------------------------------
struct VertexShaderInput
{
	float4 Position : POSITION0;
	float2 AUV : TEXCOORD0;
	float2 RUV : TEXCOORD1;
	float3 Normal : NORMAL0;
	float3 Tangent : TANGENT0;
	float3 Binormal : BINORMAL0;
};

struct VertexShaderOutput
{
	float4 Position : SV_POSITION;
	float4 ViewPosition	: TEXCOORD0;
	float2 AUV : TEXCOORD1;
	float2 RUV : TEXCOORD2;
	float3 Depth : TEXCOORD3;
	float3 NormalBumped : TEXCOORD4;
	float3x3 TBN : TEXCOORD5;
	float4 ScreenUV : TEXCOORD8;
};


struct PixelShaderOutput
{
	float4 Buffer0 : SV_Target0;		// RGB - world position XYZ, A - view position Z
	float4 Buffer1 : SV_Target1;		// RGB - bump normal vector DIV emmisive factor ; A - specular power
	float4 Buffer2 : SV_Target2;		// R - view position Z, G - reflection, B - height factor, A - specular Intensity
	float4 Buffer3 : SV_Target3;		// RGB - face normal DIV height factor, A - reversed
};




//	------------------------------------------------------------------
//	Function for Vertex Shader
//	------------------------------------------------------------------
VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output;

	input.Position.w = 1.0f;

	float4x4 worldView = mul(g_world, g_cameraView);

	/*
	uint2 pos_xy = { (int)(input.AUV.x*255), (int)(input.AUV.y * 255) };
	float4 texelColor = g_TDiffuseMap[pos_xy];

	input.Position.y = sin(input.Position.x) * cos(input.Position.y);

	input.Position.y = texelColor.r * 5.0f;
	*/
	// propertices for channel "B" for Buffer1
	uint effectsProperties = 0;

	// set position
	float4 viewPosition = mul(float4(input.Position.xyz, 1.0f), worldView);
	output.Position = mul(viewPosition, g_cameraProjection);
	output.ViewPosition = viewPosition;

	// set screen (projecion) coordinates
	output.ScreenUV = output.Position;

	// set depth
	output.Depth = float3(1.0f, 1.0f, 1.0f);
	output.Depth.x = output.Position.z;
	output.Depth.y = output.Position.w;
	output.Depth.z = viewPosition.z;

	// set  worldView TBN matrix
	output.TBN[0] = input.Tangent;
	output.TBN[1] = input.Binormal;
	output.TBN[2] = normalize(mul(input.Normal, (float3x3)worldView));

	output.NormalBumped = normalize(mul(input.Normal, (float3x3)g_world));

	// save texure coords
	output.RUV = input.RUV;
	output.AUV = input.AUV;


	return output;
}



//	------------------------------------------------------------------
//	Function for Pixel Shader
//	------------------------------------------------------------------
PixelShaderOutput PixelShaderFunction(VertexShaderOutput input) : SV_TARGET
{
	PixelShaderOutput output;

	float4 hgrrData;
	float4 textureColor;
	float viewPositionZ;

	// propertices for channel "B" for Buffer1
	uint effectsProperties = 0;

	output.Buffer0 = float4(1.0f, 0.0f, 0.0f, 0.0f);
	output.Buffer1 = float4(0.0f, 0.0f, 0.0f, 0.0f);
	output.Buffer2 = float4(0.0f, 0.0f, 0.0f, 0.0f);
	output.Buffer3 = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// set depth and sub height factor
	viewPositionZ = GetLinearDepth(input.ViewPosition.z, 1.0f, g_cameraPositionFar.w);

	textureColor = g_TDiffuseMap.Sample(SamplerT0, input.AUV);
	textureColor = float4(1.0f, 1.0f, 1.0f, 0.0f);

	//
	// set Buffer0

	output.Buffer0.r = PackFloat4ToFloat(textureColor);
	output.Buffer0.a = viewPositionZ;

	//
	// set Buffer1
	float3 normal = NormalEncode(normalize(input.NormalBumped));

	// normal vector and RESERVED
	output.Buffer1.rg = PackFloat4toFloat2(float4(normal.xyz, 0.0f));

	// light properties
	output.Buffer1.b = PackFloat4ToFloat(float4(1.0f, 0.2f, 1.0f, 0.9f));








	
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