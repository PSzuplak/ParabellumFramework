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

float4 g_gradL0;
float4 g_gradL1;

float4 g_sunPositionSize;	// XY - position, Z - size, W - reserved
float4 g_sunGrad0;
float4 g_sunGrad1;


//	------------------------------------------------------------------
//	Resources
//	------------------------------------------------------------------

Texture2D g_TDiffuseMap:register(t0);		//	RGBA - Color

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
	float4 Position			: SV_POSITION;
	float2 ScreenPosition	: TEXCOORD0;
	float2 UV				: TEXCOORD1;
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
	output.ScreenPosition.xy = input.Position.xy;
	output.UV = input.UV;

	return output;
}



//	------------------------------------------------------------------
//	Function for Pixel Shader
//	------------------------------------------------------------------
PixelShaderOutput PixelShaderFunction(VertexShaderOutput input) : SV_TARGET
{
	PixelShaderOutput output;

	output.Buffer0 = float4(0.0f, 0.0f, 0.0f, 0.5f);


	float4 skyColor = 0.0f;
	float4 gradColor = 0.0f;
	float lenSky = 0.0f;
	float factor = 0.0f;
	float4 sunColor = 0.0f;

	float4 sunLightNear = 0.0f;
	float4 sunLightAll = 0.0f;

	float4 whiteColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

	//float4 worldPosition = input.WorldPosition;
	//float angle = dot(normalize(g_cameraPositionFar.xyz - worldPosition.xyz), float3(0.0f, -1.0f, 0.0f));
	//float3 target = normalize(g_cameraPositionFar.xyz - worldPosition.xyz);
	//float2 currentUV = float2(0.5f, 0.5f) + target.xz * 0.5f;
	//output.Buffer0 = (g_gradL1 * angle) + ( (1.0f - angle) * g_gradL0);

	// sky gradient
	lenSky = length(input.ScreenPosition.xy);
	gradColor = (g_gradL1 * lenSky) + ((1.0f - lenSky) * g_gradL0);

	skyColor = gradColor;

	//
	// add sun
	if (
		(g_sunPositionSize.x > input.ScreenPosition.x - g_sunPositionSize.z) &&
		(g_sunPositionSize.x < input.ScreenPosition.x + g_sunPositionSize.z) &&
		(g_sunPositionSize.y < input.ScreenPosition.y + g_sunPositionSize.z) &&
		(g_sunPositionSize.y > input.ScreenPosition.y - g_sunPositionSize.z) &&
		(length(g_sunPositionSize.xy - input.ScreenPosition.xy) < g_sunPositionSize.z)
		)
	{
		factor = length(g_sunPositionSize.xy - input.ScreenPosition.xy) / g_sunPositionSize.z;

		sunColor = (g_sunGrad1 * factor) + ((1.0f - factor) * g_sunGrad0);

		skyColor = (gradColor * sunColor.a) + ((1.0f - sunColor.a) * sunColor);
	}
	// add light from sun
	else
	{
		// light near the sun
		factor = length(g_sunPositionSize.xy - input.ScreenPosition.xy);//*1.5f;
		sunLightNear = (gradColor * factor) + ((1.0f - factor) * g_sunGrad1);

		// som other light
		//factor = 2.0f - length(g_sunPositionSize.xy - input.ScreenPosition.xy);
		//sunLightAll = (g_sunGrad1 * factor) + ((1.0f - factor) * gradColor);


		skyColor *= sunLightNear;
		//skyColor *= sunLightAll;
	}

	

		



	output.Buffer0 = skyColor;

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