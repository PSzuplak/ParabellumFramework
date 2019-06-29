//	------------------------------------------------------------------
//	Includes
//	------------------------------------------------------------------

#include "GBufferFunctions.hlsl"

//	------------------------------------------------------------------
//	Globals
//	------------------------------------------------------------------

float4 g_settings;

Texture2D g_RTParticlesFrontWorldZ:register(t0);
Texture2D g_RTParticlesFrontDiffuse:register(t1);

Texture2D g_RTParticlesBackWorldZ:register(t2);

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
	float4 Buffer0 : SV_Target0;
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

	float4 positionFront = g_RTParticlesFrontWorldZ.Sample(SamplerT0, input.UV);
	float4 positionBack = g_RTParticlesBackWorldZ.Sample(SamplerT1, input.UV);

	output.Buffer0 = float4(0.0f, 0.0f, 0.0f, 0.5f);

	//output.Buffer0.rgb = (positionBack.a - positionFront.a) / 10.0f;

	output.Buffer0.rgb = 1.0f - positionFront.a / 20.0f;

	output.Buffer0 *= g_RTParticlesFrontDiffuse.Sample(SamplerT0, input.UV);

	float4 colorInt = g_RTParticlesFrontDiffuse.Sample(SamplerT0, input.UV);

	output.Buffer0.rgba = UnpackFloatToFloat4(colorInt.r);

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