//	------------------------------------------------------------------
//	Includes
//	------------------------------------------------------------------

#include "GBufferFunctions.hlsl"
#include "NormalCode.hlsl"

//	------------------------------------------------------------------
//	Globals
//	------------------------------------------------------------------

float4 g_settings;

Texture2D g_RTGBufferSolid0:register(t0);
Texture2D g_RTGBufferSolid1:register(t1);
Texture2D g_RTParticlesFrontWorldZ:register(t2);
Texture2D g_RTParticlesFrontDiffuse:register(t3);
Texture2D g_RTParticlesBackWorldZ:register(t4);

SamplerState SamplerT0:register(s0);
SamplerState SamplerT1:register(s1);
SamplerState SamplerT2:register(s2);
SamplerState SamplerT3:register(s3);
SamplerState SamplerT4:register(s4);


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

	output.Buffer0 = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float4 GBufferSolid0 = g_RTGBufferSolid0.Sample(SamplerT0, input.UV);
	float4 GBufferSolid1 = g_RTGBufferSolid1.Sample(SamplerT1, input.UV);
	float4 ParticlesFrontWorldZ = g_RTParticlesFrontWorldZ.Sample(SamplerT2, input.UV);
	float4 ParticlesFrontDiffuse = g_RTParticlesFrontDiffuse.Sample(SamplerT3, input.UV);
	float4 ParticlesBackWorldZ = g_RTParticlesBackWorldZ.Sample(SamplerT4, input.UV);

	float visibility = (ParticlesBackWorldZ.w - ParticlesFrontWorldZ.w) * 2000.0f;
	visibility *= 1.0f - abs(-log(ParticlesFrontDiffuse.a));
	visibility = clamp(visibility, 0.0f, 1.0f);
	//visibility = 0.8f;

	if ((GBufferSolid0.w < ParticlesFrontWorldZ.w) || (ParticlesFrontWorldZ.w > 0.5f) || (ParticlesBackWorldZ.w > 0.5f))
		output.Buffer0 = GBufferSolid1;
	else
		//output.Buffer0 = ParticlesFrontDiffuse;
		output.Buffer0 = (1.0f - visibility)*GBufferSolid1 + ParticlesFrontDiffuse * visibility;

	//output.Buffer0 = ParticlesFrontDiffuse

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