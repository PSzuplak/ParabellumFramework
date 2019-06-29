//	------------------------------------------------------------------
//	Deferred Lighting, shadows and SSAO 
//	in one shader for higher performance
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

float4x4 g_cameraViewProjection;
float4 g_cameraPositionFar;
float4 g_ctl;					// camera position to top left frustum corner
float4 g_ctl2ctr;				// camera position to top left frustum corner TO camera position to top rigt frustum corner
float4 g_ctl2cbl;				// camera position to top left frustum corner TO camera position to bottom left frustum corner
float4 g_plPositionRay;			// XYZ - point light position - W - ray length
float4 g_plColorIntensity;		// RGB - point light color - W - intensity

Texture2D g_RTGBuffer00:register(t0);
Texture2D g_RTGBuffer01:register(t1);

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
	float4 ScreenUV	: TEXCOORD1;
};


struct PixelShaderOutput
{
	float4 Buffer0 : SV_Target0;	// RGB - color ; W - alpha blending factor // if W is lower then bigger intensity 
};




//	------------------------------------------------------------------
//	Function for Vertex Shader
//	------------------------------------------------------------------
VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output;

	input.Position.w = 1.0f;

	float4 worldPosition = float4(input.Position.xyz, 1.0f);
	worldPosition.xyz *= g_plPositionRay.w;
	worldPosition.xyz += g_plPositionRay.xyz;

	output.Position = mul(worldPosition, g_cameraViewProjection);

	// set uv
	output.UV = input.UV;

	// set screen (projecion) coordinates
	output.ScreenUV = output.Position;

	return output;
}



//	------------------------------------------------------------------
//	Function for Pixel Shader
//	------------------------------------------------------------------
PixelShaderOutput PixelShaderFunction(VertexShaderOutput input) : SV_TARGET
{
	PixelShaderOutput output;

	float4 sampleGBuffer0;
	float4 sampleGBuffer1;

	float4 position;
	float intensity = 0.0f;
	float distanceFromLight = 0.0f;
	float normalizedDistance = 0.0f; // distance from the light in 0.0f - 1.0f range (from 0 to RayLenght)

	float4 unpackedNormal;
	float4 decodedNormal;
	float3 normal;
	float4 lightProperties;
	float3 lightDirection;

	// default output
	output.Buffer0 = float4(1.0f, 1.0f, 1.0f, 0.0f);

	// normalize screen position
	input.ScreenUV /= input.ScreenUV.w;
	input.ScreenUV.xy = (input.ScreenUV.xy + float2(1.0f, 1.0f)) * 0.5f;
	input.ScreenUV.y = 1.0f - input.ScreenUV.y;

	// read buffers
	sampleGBuffer0 = g_RTGBuffer00.Sample(SamplerT0, input.ScreenUV.xy);


	position.xyz = GetWorldFromFrustumCorners(
		input.ScreenUV.xy, sampleGBuffer0.w, g_cameraPositionFar.xyz,
		g_ctl.xyz, g_ctl2ctr.xyz, g_ctl2cbl.xyz
		);



	distanceFromLight = length(position.xyz - g_plPositionRay.xyz);
	normalizedDistance = distanceFromLight / g_plPositionRay.w;

	intensity = 1.0f - normalizedDistance * normalizedDistance;
	intensity *= g_plColorIntensity.w;
	intensity = clamp(intensity, 0.0f, 1.0f);

	lightDirection = normalize(position.xyz - g_plPositionRay.xyz);

	//
	// calculate point light if pixel is in range
	if (distanceFromLight < g_plPositionRay.w)
	{
		sampleGBuffer1 = g_RTGBuffer01.Sample(SamplerT1, input.ScreenUV.xy);

		unpackedNormal = UnpackFloat2toFloat4(sampleGBuffer1.rg);
		decodedNormal.xyz = NormalDecode(unpackedNormal.xyz);
		normal = decodedNormal.xyz;
		normal = normalize(normal);
		lightProperties = UnpackFloatToFloat4(sampleGBuffer1.b);


		output.Buffer0 = Phong(
			g_cameraPositionFar.xyz, position.xyz, normal.xyz, lightDirection,
			g_plColorIntensity, intensity,
			lightProperties.r * 1.5f, lightProperties.g * 255.0f, 0.5f);

		output.Buffer0 = clamp(output.Buffer0, 0.0f, 1.0f);

		output.Buffer0 *= intensity;
		output.Buffer0.w = intensity;
	}
	else
	{
		discard;
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