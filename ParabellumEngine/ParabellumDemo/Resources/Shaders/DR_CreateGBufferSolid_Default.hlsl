//	------------------------------------------------------------------
//
//	Default shader for Deferred Rendering solid models
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
float4 g_cameraPositionFar;		// XYZ - camera position, W - camera Z Far
float4 g_objectProperties;		// X - (unsigned int) object ID, YZW -

//	------------------------------------------------------------------
//	Resources
//	------------------------------------------------------------------

Texture2D g_TDiffuseMap:register(t0);		//	R - color
Texture2D g_TNSMap:register(t1);			//	RG - XY Normal vector ( UP 'z coord' always 1), BA - reversed
Texture2D g_TTGEMap:register(t2);			//	R - Translucency, G - Glow, B - emissive , A - reversed
Texture2D g_THGRRMap:register(t3);			//	R - height factor, G - glow, B - reflection, A - refraction


SamplerState SamplerT0:register(s0);
SamplerState SamplerT1:register(s1);
SamplerState SamplerT2:register(s2);
SamplerState SamplerT3:register(s3);

//	------------------------------------------------------------------
//	Shader Structs
//	------------------------------------------------------------------
struct VertexShaderInput
{
	float3 Position : POSITION0;
	float2 UV : TEXCOORD0;
	float3 Normal : NORMAL0;
	float3 Tangent : TANGENT0;
	float3 Binormal : BINORMAL0;
};

struct VertexShaderOutput
{
	float4 Position			: SV_POSITION;
	float4 ViewPosition		: TEXCOORD0;
	float2 UV				: TEXCOORD1;
	float3 NormalWorld		: TEXCOORD2;
	float3x3 TBN			: TEXCOORD3;
};


struct PixelShaderOutput
{
	float4 Buffer0 : SV_Target0;		//	
	float4 Buffer1 : SV_Target1;		//	
	float4 Buffer2 : SV_Target2;		// 
};




//	------------------------------------------------------------------
//	Function for Vertex Shader
//	------------------------------------------------------------------
VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output;

	output.UV = input.UV;

	float4x4 worldView = mul(g_world, g_cameraView);

	// set position
	float4 viewPosition = mul(float4(input.Position.xyz, 1.0f), worldView);
	output.Position = mul(viewPosition, g_cameraProjection);
	output.ViewPosition = viewPosition;

	output.ViewPosition.w = GetLinearDepth(viewPosition.z, 1.0f, g_cameraPositionFar.w);

	// set  worldView TBN matrix
	output.TBN[0] = input.Tangent;
	output.TBN[1] = input.Binormal;
	output.TBN[2] = mul(input.Normal, (float3x3)worldView);
	output.NormalWorld = mul(input.Normal, (float3x3)g_world);

	return output;
}

//	------------------------------------------------------------------
//	Function for Pixel Shader
//	------------------------------------------------------------------
PixelShaderOutput PixelShaderFunction(VertexShaderOutput input) : SV_TARGET
{
	PixelShaderOutput output;

	float4 sampleTHGRRMap;
	float4 sampleDiffuseMap;
	float4 sampleTTGEMap;
	float viewPositionZ;
	uint materialProperties = 0;

	output.Buffer0 = float4(0.0f, 0.0f, 0.0f, 0.0f);
	output.Buffer1 = float4(0.0f, 0.0f, 0.0f, 0.5f);
	output.Buffer2 = float4(0.0f, 0.0f, 0.0f, 0.5f);

	// read height buffer
	sampleTHGRRMap = g_THGRRMap.Sample(SamplerT3, input.UV);

	// calculate depth
	viewPositionZ = input.ViewPosition.w;
	//viewPositionZ += (1.0f - sampleTHGRRMap.r) / g_cameraPositionFar.w;

	// read diffuse map
	sampleDiffuseMap = g_TDiffuseMap.Sample(SamplerT2, input.UV);

	// discard pixel if it is transparent
	//if (sampleDiffuseMap.a < 0.008f)
	//	discard;

	// sample TGEMap
	sampleTTGEMap = g_TTGEMap.Sample(SamplerT0, input.UV);

	// ------------------------------------------------------------
	// set Buffer0

	output.Buffer0.r = PackFloat4ToFloat(sampleDiffuseMap);
	output.Buffer0.b = g_objectProperties.x;
	output.Buffer0.a = viewPositionZ;

	//uint diffuser = C4U8toU32(240, 240, 0, 128);
	//uint diffuser = asuint( g_TDiffuseMap.Sample(SamplerT2, input.UV).r );
	//uint4 ucolor = U32toC4U8(diffuser);
	//float4 fcolor = C4U8toC4NF32(ucolor);
	//output.Buffer0 = fcolor;

	// ------------------------------------------------------------
	// set Buffer1

	float4 normalMapData = g_TNSMap.Sample(SamplerT1, input.UV);
	normalMapData = (normalMapData * 2.0f) - 1.0f;

	float3 normalC;
	normalC.x = normalMapData.x;
	normalC.y = normalMapData.z * (sampleTHGRRMap.r + 0.1f);
	normalC.z = normalMapData.y;
	normalC = normalize(normalC);

	// calculte bump vector
	const float3 right = float3(1.0f, 0.0f, 0.0f);
	const float3 up = float3(0.0f, 1.0f, 0.0f);
	const float3 forward = float3(0.0f, 0.0f, 1.0f);

	float cosXa = 0.0f - dot(normalC, right);
	float cosYa = 1.0f - dot(normalC, up);
	float cosZa = 0.0f - dot(normalC, forward);

	float cosXb = dot(input.NormalWorld,   right);
	float cosYb = dot(input.NormalWorld,      up);	
	float cosZb = dot(input.NormalWorld, forward);
													
	float vx = cosXb - cosXa;
	float vy = cosYb - cosYa;
	float vz = cosZb - cosZa;


	float3 normal = NormalEncode( normalize(float3(vx, vy, vz)) );


	// normal vector and RESERVED
	output.Buffer1.rg = PackFloat4toFloat2(
		float4(
			normal.xyz,		// bumped normal
			0.0f			// 
			)
		);


	// packed Specular intensty, Specular power, Roughtness, Opacity(translucency) as Float4 to Float
	output.Buffer1.b = PackFloat4ToFloat(
		float4(
				1.0f,	// Specular intensty
				0.2f,	// Specular power
				1.0f,	// Roughtness
				0.9f
			)
		);

	// packed Thickness/Translucency, Glow, Emissive, RESERVED as Float4 to Float
	output.Buffer1.a = PackFloat4ToFloat(
		float4(
				0.0f,	// Translucency
				0.0f,	// Glow
				0.0f,	// Emissive
				0.0f	// RESERVED
			)
		);


	// ------------------------------------------------------------
	// set Buffer3
	output.Buffer2.rg = PackFloat4toFloat2( float4(NormalEncode(input.TBN[2]), sampleTHGRRMap.r) );

	output.Buffer2.ba = PackFloat4toFloat2( float4(NormalEncode(input.TBN[0]), sampleTHGRRMap.r) );

	
	// ------------------------------------------------------------
	// Properties in output.Buffer0.b
	//

	// parallax mapping
	if (sampleTHGRRMap.r < 0.95f)
		materialProperties = materialProperties | (1 << 10);

	// reflection

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