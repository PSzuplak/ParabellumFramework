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

float4 g_windForce;
float4 g_windProperties;
float4 g_timeline;


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
	float2 Properties : TEXCOORD1;
};

struct VertexShaderOutput
{
	float4 Position : SV_POSITION;
	float4 WorldPosition : TEXCOORD0;
	float2 UV : TEXCOORD1;
	float2 Properties : TEXCOORD2;
	float3 Depth : TEXCOORD3;
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
	output.Position = input.Position;


	output.UV = input.UV;
	output.Properties = input.Properties;


	// set position
	float4 worldPosition = mul(input.Position, g_world);


	// animation
	if (output.UV.y > 0.0f)
	{
		float4 delta = g_windForce;
		delta.w = 0.0f;

		float x = g_timeline.x;
			
		//delta += sin(x) * sin(x * 2) * cos(x * 5) * tan(x) * g_windForce.w;
		float hj = cos(x*5) * cos(x*9) * sin(x*x) * g_windForce.w;

		worldPosition += delta * hj * output.UV.y * output.UV.y;
	}

	float4 viewPosition = mul(worldPosition, g_cameraView);
	output.Position = mul(viewPosition, g_cameraProjection);
	output.WorldPosition = worldPosition;

	// remove from rasterization if LOD
	float4 distance = abs(g_cameraPositionFar - worldPosition);


	if (input.Properties.x < (length(distance.xyz) / 10.0f))
	{
		output.Position.w = 0.0f;
		return output;
	}


	// set depth
	output.Depth = float3(1.0f, 1.0f, 1.0f);
	output.Depth.x = output.Position.z;
	output.Depth.y = output.Position.w;
	output.Depth.z = viewPosition.z;

	return output;
}



//	------------------------------------------------------------------
//	Function for Pixel Shader
//	------------------------------------------------------------------
PixelShaderOutput PixelShaderFunction(VertexShaderOutput input) : SV_TARGET
{
	PixelShaderOutput output;

	float4 bB = float4(0.0f, 0.25f, 0.0f, 0.0f);
	float4 gT = float4(0.0f, input.Properties.y, 0.0f, 0.0f);
	float4 gM = float4(0.3f, 0.3f, 0.0f, 0.0f);
	float4 gE = float4(input.Properties.y - 0.25f, input.Properties.y - 0.25f, 0.0f, 0.0f);



	float4 val;

	val = bB + (gT - bB) * input.UV.y;

	val += gE + (gM - gE) * input.UV.y;

	float4 color = val;

	float4 viewPositionZ = GetLinearDepth(input.Depth.z, 1.0f, g_cameraPositionFar.w);

	// set Buffer0
	output.Buffer0 = input.WorldPosition;
	output.Buffer0.a = viewPositionZ;

	// set Buffer1
	output.Buffer1 = color;


	output.Buffer2 = color;
	output.Buffer3 = color;


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