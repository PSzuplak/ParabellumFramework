//	------------------------------------------------------------------
//	Shader for Deferred Rendering solid models
//	The difference with DR_CreateGBufferSolid_Default is that
//	calculations are being made directly in the shader
//	Is thould be used for models which small number of vertices
//	and models WITHOUT parallax mapping!
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
float4x4 g_world;


//	------------------------------------------------------------------
//	Resources
//	------------------------------------------------------------------

Texture2D g_TDiffuseMap:register(t0);		//	R - color
Texture2D g_TNSMap:register(t1);			//	RG - XY Normal vector ( UP 'z coord' always 1), BA - reversed
Texture2D g_TTGEMap:register(t2);			//	R - Translucency, G - Glow, B - emissive , A - reversed
Texture2D g_THGRRMap:register(t3);			//	R - height factor, G - glow, B - reflection, A - refraction


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

	output.UV = input.UV;


	// set texture coords
	output.UV = input.UV;

	// set position
	float4 worldPosition = mul( float4(input.Position.xyz, 1.0f), g_world);
	float4 viewPosition = mul(worldPosition, g_cameraView);
	output.Position = mul(viewPosition, g_cameraProjection);
	output.ViewPosition = viewPosition;

	// set screen (projecion) coordinates
	output.ScreenUV = output.Position;

	// transform normal to local coodinates
	//output.Normal = mul(input.Normal, output.TBN);
	output.Normal = normalize(mul(input.Normal, (float3x3)g_world));

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
	output.Buffer1.rg = PackFloat4toFloat2(
		float4(
			normal.xyz,		// bumped normal
			0.0f			// 
			)
		);

	// light properties
	output.Buffer1.b = PackFloat4ToFloat( float4(1.0f, 0.2f, 1.0f, 0.9f) );


	output.Buffer1.a = PackFloat4ToFloat(
		float4(
			1.0f,	//Translucency
			0.2f,	// Glow
			0.5f,	// Emissive
			0.9f	//
			)
		);


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

	//effectsProperties = effectsProperties | (1 << 10);
	//output.Buffer0.b = f16tof32(effectsProperties);


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