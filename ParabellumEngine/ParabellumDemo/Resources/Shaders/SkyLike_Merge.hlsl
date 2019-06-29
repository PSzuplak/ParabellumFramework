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
#include "MathHelper.hlsl"
#include "ImageProcessing.hlsl"

//	------------------------------------------------------------------
//	Globals
//	------------------------------------------------------------------

float4 g_position;	// XY - begin coords, ZW - width,height


//	------------------------------------------------------------------
//	Resources
//	------------------------------------------------------------------
Texture2D g_TSun:register(t0);
Texture2D g_TClouds:register(t1);


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
	output.ScreenPosition.xy = input.Position;
	output.UV = input.UV;

	return output;
}



//	------------------------------------------------------------------
//	Function for Pixel Shader
//	------------------------------------------------------------------
PixelShaderOutput PixelShaderFunction(VertexShaderOutput input) : SV_TARGET
{
	PixelShaderOutput output;

	// Save clouds as polar coordinates
	//float2 xxx = GIMP_PolarCoordiantes(input.UV.xy, 4096, 4096, false, true);
	//float2 cuUV = input.ScreenPosition.xy * 10.0f;
	//float2 polarUV = CartesianToPolar2D(input.ScreenPosition.xy);
	//float2 cuUV = float2(0.5f, 0.5f) + (polarUV);	
	//float2 middleUV = polarUV;
	//float2 polarUV = 0.0f;	
	//float t = atan2(cuUV.y, cuUV.x);
	//float r = sqrt(cuUV.x*cuUV.x + cuUV.y*cuUV.y);	
	//polarUV.x = t;
	//polarUV.y = r;
	////polarUV = SSToUV(polarUV);
	//float2 fUV = float2(0.5f, 0.5f) + (polarUV);
	//output.Buffer0 = g_TClouds.Sample(SamplerT1, xxx);
	

	output.Buffer0 = g_TClouds.Sample(SamplerT1, input.UV);

	
	//float4 sunColor = g_TSun.Sample(SamplerT0, input.UV);
	//float4 clouds = g_TClouds.Sample(SamplerT1, GIMP_PolarCoordiantes(input.UV.xy, 4096, 4096, false, true) );
	//float4 mergedSunClouds = (clouds * clouds.a) + ((1.0f - clouds.a) * sunColor);
	//output.Buffer0 = mergedSunClouds;

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