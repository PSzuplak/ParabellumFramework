//	------------------------------------------------------------------
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
float4 g_cameraPositionFar;		// XYZ - camera position, W - camera Z Far

float4x4 g_world00;
float4x4 g_world01;
float4x4 g_world02;
float4x4 g_world03;
float4x4 g_world04;
float4x4 g_world05;
float4x4 g_world06;
float4x4 g_world07;



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
	float4 ScreenUV			: TEXCOORD3;
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

	float4x4 world;

	if ( (input.Properties.x > 0.1f) && (input.Properties.x < 0.9f) )
	{
		world = g_world00;
	}
	else if ( (input.Properties.x > 1.1f) && (input.Properties.x < 1.9f) )
	{
		world = g_world01;
	}
	else if ((input.Properties.x > 2.1f) && (input.Properties.x < 2.9f))
	{
		world = g_world02;
	}
	else if ((input.Properties.x > 3.1f) && (input.Properties.x < 3.9f))
	{
		world = g_world03;
	}
	else if ((input.Properties.x > 4.1f) && (input.Properties.x < 4.9f))
	{
		world = g_world04;
	}
	else if ((input.Properties.x > 5.1f) && (input.Properties.x < 5.9f))
	{
		world = g_world05;
	}
	else if ((input.Properties.x > 6.1f) && (input.Properties.x < 6.9f))
	{
		world = g_world06;
	}
	else if ((input.Properties.x > 7.1f) && (input.Properties.x < 7.9f))
	{
		world = g_world07;
	}


	// set texture coords
	output.UV = input.UV;

	// set position
	float4 worldPosition = mul(float4(input.Position.xyz, 1.0f), world);
	float4 viewPosition = mul(worldPosition, g_cameraView);
	output.Position = mul(viewPosition, g_cameraProjection);
	output.ViewPosition = viewPosition;


	// set screen (projecion) coordinates
	output.ScreenUV = output.Position;



	// transform normal to local coodinates
	//output.Normal = mul(input.Normal, output.TBN);
	output.Normal = normalize(mul(input.Normal, (float3x3)world));

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

	float viewPositionZ = GetLinearDepth(input.ViewPosition.z, 1.0f, g_cameraPositionFar.w);
	
	float4 textureColor = g_TDiffuseMap.Sample(SamplerT0, input.UV);

	// propertices for channel "B" for Buffer1
	uint effectsProperties = 0;

	//
	// set Buffer0

	//output.Buffer0 = PackFloat4ToFloat(textureColor);

	output.Buffer0 = PackFloat4ToFloat(textureColor);

	output.Buffer0.a = viewPositionZ;


	//
	// set Buffer1

	float4 normalMapData = g_TNSMap.Sample(SamplerT0, input.UV);
	normalMapData.x = 0.0f;
	normalMapData.y = 0.0f;

	float3 normal = NormalEncode(input.Normal);

	//normal = NormalEncode(input.Normal); // get world space normal


	// normal vector and RESERVED
	output.Buffer1.rg = PackFloat4toFloat2(float4(normal.xyz, 0.0f));

	// light properties
	output.Buffer1.b = PackFloat4ToFloat(float4(1.0f, 0.2f, 1.0f, 0.9f));

	//
	// set Buffer3

	float4 hgrrData = 0.5f;

	float4 vb;
	vb.rgb = NormalEncode(float3(0.0f, 0.0f, 0.0f));
	vb.a = 0.0f;
	
	float4 cvb;

	cvb.rg = PackFloat4toFloat2(vb.rgba);

	output.Buffer2.rg = vb.rg;
	output.Buffer2.b = vb.b;
	output.Buffer2.a = vb.a;


	//
	// Properties in output.Buffer0.b
	//

	effectsProperties = effectsProperties | (1 << 10);
	

	output.Buffer0.b = f16tof32(effectsProperties);


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