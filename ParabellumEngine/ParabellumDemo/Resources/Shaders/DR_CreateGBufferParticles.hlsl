//	------------------------------------------------------------------
//	GBuffer for solid models
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
float4x4 g_cameraView;
float4x4 g_cameraProjection;
float4x4 g_world;
float4x4 g_worldViewIT;
float4 g_cameraPositionFar;		// XYZ - camera position, W - camera Z Far
float4 g_particleProperties;
float4 g_wind;					// XYZ - direction, A - strength

//	------------------------------------------------------------------
//	Resources
//	------------------------------------------------------------------

Texture2D g_TDiffuseMap:register(t0);		//	RGBA - Color

SamplerState SampleType:register(s0);



//	------------------------------------------------------------------
//	Shader Structs
//	------------------------------------------------------------------
struct VertexShaderInput
{
	float4 Position : POSITION0;
	float2 UV : TEXCOORD0;
	float3 Normal : NORMAL0;
	float3 DeltaDirection : POSITION1;
	float3 DeltaRotation : POSITION2;
	float3 Properties : POSITION3;
	float3 Sprite : POSITION4;
};

struct VertexShaderOutput
{
	float4 Position			: SV_POSITION;
	float4 WorldPosition	: TEXCOORD0;
	float2 UV				: TEXCOORD1;
	float2 Properties		: TEXCOORD2;		// R - alpha, G - reversed
};


struct PixelShaderOutput
{
	float4 Buffer0 : SV_Target0;		// RGB - world position XYZ, A - view position Z
	float4 Buffer1 : SV_Target1;		// RGB - color, A - reversed
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
	output.Properties = float2(1.0f, 1.0f);


	input.Position.w = 1.0f;

	
	int spritesCount = 16;

	int timeCurrent = g_particleProperties.x;

	int timeParticle = input.Properties.x;


	float normalizedTime = (float)(timeCurrent%timeParticle) / (float)timeParticle;

	int spriteRow = 4;
	int spriteCol = 4;

	int spriteNo = (timeCurrent / 50) % spritesCount;

	//spriteNo = 14;

	float2 spriteUV = float2(0.0f, 0.0f);



	// set texture coords
	output.UV = input.UV;

	spriteUV.x =  ((spriteNo % spriteRow) / input.Sprite.x) + (input.UV.x / input.Sprite.x);
	spriteUV.y = ( ((int)((float)spriteNo / (float)spriteRow)) / input.Sprite.y) + (input.UV.y / input.Sprite.y);

	//output.UV = spriteUV;
	float sint = sin(g_particleProperties.x * input.DeltaRotation.x);
	float cost = cos(g_particleProperties.x * input.DeltaRotation.x);

	float nx = input.Position.x * cost - input.Position.y * sint;
	float ny = input.Position.x * sint + input.Position.y * cost;
	float nz = input.Position.z;//input.Position.x * sint + input.Position.y * cost;

	input.Position.x = nx;
	input.Position.y = ny;
	input.Position.z = nz;


	input.Position.xyz += input.DeltaDirection.xyz * (((int)g_particleProperties.x) % ((int)input.Properties.x));//(((int)input.MovDirLife.w) % ((int)input.Properties.x));

	input.Position.xyz += g_wind.xyz * (normalizedTime * 5.0f);

	input.Position.xyz += float3(5.0f, 1.0f, 5.0f);

	// set position
	float4 worldPosition = mul(input.Position, g_world);
	float4 viewPosition = mul(worldPosition, g_cameraView);
	output.Position = mul(viewPosition, g_cameraProjection);
	
	output.WorldPosition = worldPosition;

	// set alha blending
	output.Properties.r = 1.0f - normalizedTime;

	output.Properties.g = viewPosition.z;

	return output;
}



//	------------------------------------------------------------------
//	Function for Pixel Shader
//	------------------------------------------------------------------
PixelShaderOutput PixelShaderFunction(VertexShaderOutput input) : SV_TARGET
{
	PixelShaderOutput output;


	output.Buffer0 = input.WorldPosition;
	output.Buffer0.a = GetLinearDepth(input.Properties.g, 1.0f, 1000.0f);

	output.Buffer1 = float4(1.0f, 1.0f, 0.0f, 1.0f);
	output.Buffer1 = g_TDiffuseMap.Sample(SampleType, input.UV);

	if (output.Buffer1.a < 0.5f)
		discard;

	output.Buffer1.a = input.Properties.r;


	float4 colort = output.Buffer1;

	output.Buffer1.r = PackFloat4ToFloat(colort.rgba);


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