//	------------------------------------------------------------------
//	Deferred Lighting
//	Perform only basic directional lighting - for far pixels
//
//
//
//	------------------------------------------------------------------


//	------------------------------------------------------------------
//	Includes
//	------------------------------------------------------------------

#include "GBufferFunctions.hlsl"
#include "NormalCode.hlsl"
#include "LightFunctions.hlsli"

//	------------------------------------------------------------------
//	Globals
//	------------------------------------------------------------------

float4x4 g_cameraInverseView;
float4 g_lightDirectionAmbient;	// RGB - direction, A - unused
float4 g_lightPosition;			// unused
float4 g_lightColorIntensity;	// RGB - color, A - light intensity
float4 g_ambientColor;			// ambient folor


Texture2D g_RTNormal:register(t0);


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
	float4 BufferLight : SV_Target0;	// RGB - color; A transculency, ... - as Float4 to Float
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

	float4 rtNormal;
	float4 unpackedNormal;
	float4 lightProperties;
	float3 decodedNormal;
	float3 normal;
	float nl;

	output.BufferLight = g_ambientColor;

	rtNormal = g_RTNormal.Sample(SamplerT0, input.UV);

	unpackedNormal = UnpackFloatToFloat4(rtNormal.r);
	lightProperties = UnpackFloatToFloat4(rtNormal.g);

	decodedNormal = NormalDecode(unpackedNormal.xyz);

	normal = decodedNormal.xyz;
	normal = normalize(normal);
	normal = mul(normal, g_cameraInverseView);

	nl = dot(normal, -g_lightDirectionAmbient.xyz);


	if (nl > 0.0f)
	{
		output.BufferLight.xyz += nl * g_lightColorIntensity.xyz;

		output.BufferLight *= g_lightColorIntensity.w;
		output.BufferLight *= lightProperties.b;
	}

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