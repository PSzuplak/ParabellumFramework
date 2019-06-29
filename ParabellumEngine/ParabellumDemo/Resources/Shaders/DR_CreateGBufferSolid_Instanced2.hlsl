//	------------------------------------------------------------------
//	Default shader for Deferred Rendering solid models
//	Use it for nearest models.
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

float4x4 g_worldView00;
float4x4 g_worldViewIT00;

float4x4 g_worldView01;
float4x4 g_worldViewIT01;

float4 g_cameraPositionFar;		// XYZ - camera position, W - camera Z Far

//	------------------------------------------------------------------
//	Resources
//	------------------------------------------------------------------

Texture2D g_TDiffuseMap:register(t0);		//	R - color
Texture2D g_TNSMap:register(t1);			//	RG - XY Normal vector ( UP 'z coord' always 1), BA - Specular power/intensity
Texture2D g_THGRRMap:register(t2);			//	R - height factor, G - glow, B - reflection, A - emissive/refraction


SamplerState SamplerT0:register(s0);



//	------------------------------------------------------------------
//	Shader Structs
//	------------------------------------------------------------------
struct VertexShaderInput
{
	float3 Position : POSITION0;
	float2 UV : TEXCOORD0;
	float2 Properties : TEXCOORD1;
	float3 Normal : NORMAL0;
	float3 Tangent : TANGENT0;
	float3 Binormal : BINORMAL0;
	
	
};

struct VertexShaderOutput
{
	float4 Position			: SV_POSITION;
	float4 ViewPosition		: TEXCOORD0;
	float2 UV				: TEXCOORD1;
	float3 Normal			: TEXCOORD2;
	float3x3 TBN			: TEXCOORD3;
	float4 ScreenUV			: TEXCOORD6;
};


struct PixelShaderOutput
{
	float4 Buffer0 : SV_Target0;		//	RGB - world position, A - view position Z 
	float4 Buffer1 : SV_Target1;		//	R - color.rgba as float32 - assuming 8btis per channel G - Reflection, Height Factor, RES, RES; 
	float4 Buffer2 : SV_Target2;		// 
};




//	------------------------------------------------------------------
//	Function for Vertex Shader
//	------------------------------------------------------------------
VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output;

	float4x4 g_worldView = g_worldView00;
	float4x4 g_worldViewIT = g_worldViewIT00;

	if ( (input.Properties.x > 0.1f) && (input.Properties.x < 0.9f) )
	{
		g_worldView = g_worldView00;
		g_worldViewIT = g_worldViewIT00;
	}
	else if ( (input.Properties.x > 1.1f) && (input.Properties.x < 1.9f) )
	{
		g_worldView = g_worldView01;
		g_worldViewIT = g_worldViewIT01;
	}


	// set texture coords
	output.UV = input.UV;

	// set position
	float4 worldPosition = float4(input.Position.xyz, 1.0f);
	float4 viewPosition = mul(worldPosition, g_worldView);
	output.Position = mul(viewPosition, g_cameraProjection);
	output.ViewPosition = viewPosition;


	// set screen (projecion) coordinates
	output.ScreenUV = output.Position;

	// set TBN matrix
	output.TBN[0] = normalize(mul(input.Tangent, (float3x3)g_worldViewIT));
	output.TBN[1] = normalize(mul(input.Binormal, (float3x3)g_worldViewIT));
	output.TBN[2] = normalize(mul(input.Normal, (float3x3)g_worldViewIT));



	// transform normal to local coodinates
	//output.Normal = mul(input.Normal, output.TBN);
	output.Normal = normalize(mul(input.Normal, (float3x3)g_worldViewIT));

	//output.TBN[2] = input.Normal;



	return output;
}



//	------------------------------------------------------------------
//	Function for Pixel Shader
//	------------------------------------------------------------------
PixelShaderOutput PixelShaderFunction(VertexShaderOutput input) : SV_TARGET
{
	PixelShaderOutput output;

	output.Buffer0 = float4(0.0f, 0.0f, 0.0f, 0.0f);
	output.Buffer1 = float4(0.0f, 0.0f, 0.0f, 0.5f);
	output.Buffer2 = float4(0.0f, 0.0f, 0.0f, 0.5f);



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