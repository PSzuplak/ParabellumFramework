//	------------------------------------------------------------------
//	Includes
//	------------------------------------------------------------------

#include "GBufferFunctions.hlsl"
#include "NormalCode.hlsl"
//	------------------------------------------------------------------
//	Globals
//	------------------------------------------------------------------

float4 g_settings;

Texture2D g_RTGBufferMipMap:register(t0);

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
	float4 Position : SV_POSITION;
	float2 UV : TEXCOORD0;
};


struct PixelShaderOutput
{
	float4 Buffer0 : SV_Target0; // r - , g - , b - , w - minimal ViewZ
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
	output.Buffer0 = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float4 packedData = g_RTGBufferMipMap.Sample(SamplerT0, input.UV);
	uint flags = f32tof16(packedData.b);

	// average color
	output.Buffer0.r = packedData.r;



	// parallax 
	if (flags & (1 << 10))
		output.Buffer0.g = 1.0f;
	else
		output.Buffer0.g = 0.0f;


	// reflection

	output.Buffer0.a = packedData.g;

	// view Z
	output.Buffer0.a = packedData.a;

	// volumetric light
	output.Buffer0.a = packedData.w;

	//output.Buffer0.r = packedData.w;
	//output.Buffer0.g = packedData.w;
	//output.Buffer0.b = packedData.w;

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