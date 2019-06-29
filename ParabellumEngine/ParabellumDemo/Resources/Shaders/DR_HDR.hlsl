//	------------------------------------------------------------------
//	Includes
//	------------------------------------------------------------------

#include "GBufferFunctions.hlsl"
#include "ImageProcessing.hlsl"

//	------------------------------------------------------------------
//	Globals
//	------------------------------------------------------------------

float4 g_settings;	// x - average luminance


Texture2D g_RTGbuffer0:register(t0);
Texture2D g_RTBufferLight:register(t1);

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
			
	float4 sampleGbuffer0 = g_RTGbuffer0.Sample(SamplerT0, input.UV);
	float4 sampleLight = g_RTBufferLight.Sample(SamplerT0, input.UV);


	//
	float4 color = UnpackFloatToFloat4(sampleGbuffer0.r);




	// = LumPix - LumMin
	float4 lowerLight = sampleLight - g_settings.x;
	
	// = basics for exponent function
	float4 basics = g_settings.z - g_settings.x;

	//
	float4 middleGery = 1.0f + (g_settings.z - g_settings.y);
	float4 lum = sampleLight * (1.8f / g_settings.x);

	//if(input.UV.x > 0.5f)
	//	output.Buffer0 = lum / (1.0f + lum);
	//else
		output.Buffer0 = color;

	//output.Buffer0 *= 1.0f + g_settings.z;

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