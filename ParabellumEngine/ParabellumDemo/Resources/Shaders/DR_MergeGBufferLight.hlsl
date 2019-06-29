//	------------------------------------------------------------------
//	Includes
//	------------------------------------------------------------------

#include "GBufferFunctions.hlsl"
#include "NormalCode.hlsl"
#include "ImageProcessing.hlsl"

//	------------------------------------------------------------------
//	Globals
//	------------------------------------------------------------------

float4 g_settings;

Texture2D g_RTBuffer:register(t0);
Texture2D g_RTLightMap:register(t1);
Texture2D g_RTSSAO:register(t2);
Texture2D g_RTVol:register(t3);

SamplerState SamplerT0:register(s0);
SamplerState SamplerT1:register(s1);
SamplerState SamplerT2:register(s2);
SamplerState SamplerT3:register(s3);

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

	float4 sampleBuffer = g_RTBuffer.Sample(SamplerT0, input.UV);
	float4 sampleLightMap = g_RTLightMap.Sample(SamplerT1, input.UV);

	float4 sampleSSAO = g_RTSSAO.Sample(SamplerT2, input.UV);
	float4 sampleVol = g_RTVol.Sample(SamplerT3, input.UV);

	float4 unpSampleBuffer = UnpackFloatToFloat4(sampleBuffer.r);

	


	float4 mergedColor =  sampleLightMap * clamp(sampleSSAO * 0.5f + 0.5f, 0.0f, 1.0f);
	mergedColor = unpSampleBuffer * mergedColor;
	//mergedColor = UnpackFloatToFloat4(sampleBuffer.r) * sampleLightMap;//sampleLightMap;
	//mergedColor = sampleLightMap;
	
	mergedColor = unpSampleBuffer * sampleLightMap;
	mergedColor += sampleVol.b;

	output.Buffer0.r = PackFloat4ToFloat(mergedColor);

	output.Buffer0.gba = sampleBuffer.gba;
	
	output.Buffer0 = mergedColor;

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