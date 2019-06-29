//	------------------------------------------------------------------
//	Blur LightMap/ShadowMap which stores blur/AA factor in Alpha channel 
//	
//
//
//
//	------------------------------------------------------------------

//	------------------------------------------------------------------
//	Includes
//	------------------------------------------------------------------
#include "PPFunctions.hlsli"

//	------------------------------------------------------------------
//	Globals
//	------------------------------------------------------------------

float4 g_settings;

Texture2D g_TBackground:register(t0);

SamplerState SampleType:register(s0);

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
	float4 Color : SV_Target0;
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


	float4 sum = 0;
	float4 tmpColor = g_TBackground.Sample(SampleType, input.UV);
	float x, y;

	for(y = -4.5f; y <= 4.5f; y += 1.0f)
		for(x = -4.5f; x <= 4.5f; x += 1.0f)
		{
			sum += g_TBackground.Sample(SampleType, input.UV + float2(x / 800,y / 600));
		}

	sum /= 64.0f;

	output.Color = sum;

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