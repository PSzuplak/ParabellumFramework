//	------------------------------------------------------------------
//	Includes
//	------------------------------------------------------------------

#include "GBufferFunctions.hlsl"
#include "NormalCode.hlsl"
//	------------------------------------------------------------------
//	Globals
//	------------------------------------------------------------------

float4 g_settings;

Texture2D g_RTtoShrink:register(t0);

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

	// changed uvs
	float2 uvs[4];

	// sampled packed data from GBUffers
	float4 rawData[4];

	// unpacked data from rawData[4]
	float unpackedData[4];

	// properties from 'output.Buffer0.b = f16tof32(effectsProperties);'
	uint effectsProperties = 0;

	// minimal Z from 4 pixels to shrink
	float minViewZ = 0.0f;

	// maximal Z from 4 pixels to shrink
	float maxViewZ = 0.0f;

	float avgVolLight = 1.0f;

	float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);

	bool isBlindSpot = false;
	
	uvs[0] = input.UV + float2(+g_settings.z, +g_settings.w);
	uvs[1] = input.UV + float2(-g_settings.z, +g_settings.w);
	uvs[2] = input.UV + float2(+g_settings.z, -g_settings.w);
	uvs[3] = input.UV + float2(-g_settings.z, -g_settings.w);


	uvs[0] = clamp(uvs[0], 0.0f, 1.0f-g_settings.z);
	uvs[1] = clamp(uvs[1], 0.0f, 1.0f-g_settings.z);
	uvs[2] = clamp(uvs[2], 0.0f, 1.0f-g_settings.z);
	uvs[3] = clamp(uvs[3], 0.0f, 1.0f-g_settings.z);

	for (int i = 0; i < 4; ++i)
	{
		// sample
		rawData[i] = g_RTtoShrink.Sample(SamplerT0, uvs[i]);

		if(minViewZ < rawData[i].a)
			minViewZ = rawData[i].a;

		if (maxViewZ < rawData[i].a)
			maxViewZ = rawData[i].a;

		color += UnpackFloatToFloat4(rawData[i].r);

		unpackedData[i] = UnpackFloatToFloat4(rawData[i]);

		effectsProperties = effectsProperties | f32tof16(rawData[i].b);
	
		if (rawData[i].b < 0.5f)
			isBlindSpot = true;
	}

	color *= 0.25f;

	output.Buffer0.r = g_RTtoShrink.Sample(SamplerT0, input.UV).b; // object ID must be first in order to sort it correctly(faster) on CPU later
	
	if(isBlindSpot == true)
		output.Buffer0.g = 1.0f;
	else
		output.Buffer0.g = 0.0f;

	//output.Buffer0.b = f16tof32(effectsProperties);
	output.Buffer0.b = PackFloat4ToFloat(color);

	output.Buffer0.a = minViewZ;

	//output.Buffer0.r = g_RTtoShrink.Sample(SamplerT0, input.UV).a;

	//output.Buffer0.a = 0.456f;

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