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
float4x4 g_worldViewProjection;
float4 g_cameraPositionFar;		// XYZ - camera position, W - camera Z Far

//	------------------------------------------------------------------
//	Resources
//	------------------------------------------------------------------

Texture2D g_TCloudBilboards:register(t0);		//	RGBA - Color


SamplerState SamplerT0:register(s0);



//	------------------------------------------------------------------
//	Shader Structs
//	------------------------------------------------------------------
struct VertexShaderInput
{
	float4 Position : POSITION0;
	float2 UV : TEXCOORD0;
	float2 CloudProperties : TEXCOORD1;
};

struct VertexShaderOutput
{
	float4 Position			: SV_POSITION;
	float2 UV				: TEXCOORD0;
	float2 CloudProperties	: TEXCOORD1;
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

	// properties
	output.CloudProperties = input.CloudProperties;



	// set position
	output.Position = mul(input.Position, g_worldViewProjection);


	return output;
}



//	------------------------------------------------------------------
//	Function for Pixel Shader
//	------------------------------------------------------------------
PixelShaderOutput PixelShaderFunction(VertexShaderOutput input) : SV_TARGET
{
	PixelShaderOutput output;

	float density = input.CloudProperties.x;

	float4 bilboardSample = g_TCloudBilboards.Sample(SamplerT0, input.UV);

	if (bilboardSample.a < 0.1f)
		discard;
	
	output.Buffer0 = bilboardSample;//float4(0.0f, 1.0f, 1.0f, 0.5f);

	output.Buffer0 = float4(density, density, density, 0.1f);

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