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
float4x4 g_world00;
float4x4 g_world01;
float4x4 g_world02;
float4x4 g_world03;
float4x4 g_world04;
float4x4 g_world05;
float4x4 g_world06;
float4x4 g_world07;

float4 g_cameraPositionFar;		// XYZ - camera position, W - camera Z Far

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
	float3 NormalWorld		: TEXCOORD2;
	float3x3 TBN			: TEXCOORD3;
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



	float4x4 world = g_world00;
	float4x4 worldView = mul(g_world00, g_cameraView);


	if ((input.Properties.x > 0.1f) && (input.Properties.x < 0.9f))
	{
		world = g_world00;
		worldView = mul(g_world00, g_cameraView);
	}	
	else if ((input.Properties.x > 1.1f) && (input.Properties.x < 1.9f))
	{
		world = g_world01;
		worldView = mul(g_world01, g_cameraView);
	}
	else if ((input.Properties.x > 2.1f) && (input.Properties.x < 2.9f))
	{
		world = g_world02;
		worldView = mul(g_world02, g_cameraView);
	}
	else if ((input.Properties.x > 3.1f) && (input.Properties.x < 3.9f))
	{
		world = g_world03;
		worldView = mul(g_world03, g_cameraView);
	}
	else if ((input.Properties.x > 4.1f) && (input.Properties.x < 4.9f))
	{
		world = g_world04;
		worldView = mul(g_world04, g_cameraView);
	}
	else if ((input.Properties.x > 5.1f) && (input.Properties.x < 5.9f))
	{
		world = g_world05;
		worldView = mul(g_world05, g_cameraView);
	}
	else if ((input.Properties.x > 6.1f) && (input.Properties.x < 6.9f))
	{
		world = g_world06;
		worldView = mul(g_world06, g_cameraView);
	}
	else if ((input.Properties.x > 7.1f) && (input.Properties.x < 7.9f))
	{
		world = g_world07;
		worldView = mul(g_world07, g_cameraView);
	}

	// set texture coords
	output.UV = input.UV;

	// set position
	float4 viewPosition = mul(float4(input.Position.xyz, 1.0f), worldView);
	output.Position = mul(viewPosition, g_cameraProjection);
	output.ViewPosition = viewPosition;


	// set  worldView TBN matrix
	output.TBN[0] = input.Tangent;
	output.TBN[1] = input.Binormal;
	output.TBN[2] = normalize(mul(input.Normal, (float3x3)worldView));

	output.NormalWorld = normalize(mul(input.Normal, (float3x3)world));




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
	float viewPositionZ;
	
	
	float4 sampleTTGEMap;
	
	output.Buffer0 = float4(0.0f, 0.0f, 0.0f, 0.0f);
	output.Buffer1 = float4(0.0f, 0.0f, 0.0f, 0.5f);
	output.Buffer2 = float4(0.0f, 0.0f, 0.0f, 0.5f);
	
	
	
	// read height buffer
	sampleTHGRRMap = g_THGRRMap.Sample(SamplerT3, input.UV);
	
	
	// set depth and sub height factor
	viewPositionZ = GetLinearDepth(input.ViewPosition.z, 1.0f, g_cameraPositionFar.w);
	//viewPositionZ += (1.0f - sampleTHGRRMap.r) / g_cameraPositionFar.w;
	
	
	sampleDiffuseMap = g_TDiffuseMap.Sample(SamplerT0, input.UV);
	
	
	if (sampleDiffuseMap.a < 0.008f)
		discard;
	
	
	// sample TGEMap
	sampleTTGEMap = g_TTGEMap.Sample(SamplerT0, input.UV);
	
	
	
	
	
	
	// propertices for channel "B" for Buffer1
	uint effectsProperties = 0;
	
	
	
	//
	// set Buffer0
	
	//output.Buffer0 = PackFloat4ToFloat(sampleDiffuseMap);
	sampleDiffuseMap = float4(1.0f, 0.0f, 0.0f, 0.5f);
	output.Buffer0.r = PackFloat4ToFloat(sampleDiffuseMap);
	
	output.Buffer0.a = viewPositionZ;
	
	
	//
	// set Buffer1
	
	float4 normalMapData = g_TNSMap.Sample(SamplerT1, input.UV);
	normalMapData = (normalMapData * 2.0f) - 1.0f;
	
	float3 normalC;
	normalC.x = normalMapData.x;
	normalC.y = normalMapData.z * (sampleTHGRRMap + 0.1f);	//for parallax maping 
	normalC.z = normalMapData.y;
	
	normalC = normalize(normalC);
	
	
	// calculte bump vector
	float3 right = float3(1.0f, 0.0f, 0.0f);
	float3 up = float3(0.0f, 1.0f, 0.0f);
	float3 forward = float3(0.0f, 0.0f, 1.0f);
	
	float cosXa = 0.0f - dot(normalC, right);
	float cosYa = 1.0f - dot(normalC, up);
	float cosZa = 0.0f - dot(normalC, forward);
	
	float cosXb = dot(input.NormalWorld,   right);
	float cosYb = dot(input.NormalWorld,      up);
	float cosZb = dot(input.NormalWorld, forward);
	
	float vx = cosXb - cosXa;
	float vy = cosYb - cosYa;
	float vz = cosZb - cosZa;
	
	input.NormalWorld = normalize(float3(vx, vy, vz));
	
	float3 normal = NormalEncode(normalize(input.NormalWorld));
	
	
	// normal vector and RESERVED
	output.Buffer1.rg = PackFloat4toFloat2(
		float4(
			normal.xyz,		// bumped normal
			0.0f			// 
			)
		);
	
	// light properties
	
	
	
	output.Buffer1.b = PackFloat4ToFloat(float4(1.0f, 0.2f, 1.0f, 0.9f));
	
	
	//output.Buffer1.a = PackFloat4ToFloat(
	//	float4(
	//		sampleTTGEMap.r,	// Translucency
	//		sampleTTGEMap.g,	// Glow
	//		sampleTTGEMap.b,	// Emissive
	//		sampleTTGEMap.a		//
	//		)
	//	);
	
	output.Buffer1.a = PackFloat4ToFloat(
		float4(
			0.0f,	// Translucency
			0.0f,	// Glow
			0.0f,	// Emissive
			0.0f		//
			)
		);
	
	
	//
	// set Buffer3
	float4 buffer2RG;
	buffer2RG.rgb = NormalEncode(input.TBN[2]);
	buffer2RG.a = sampleTHGRRMap.r;
	
	float4 cvb;
	
	output.Buffer2.rg = PackFloat4toFloat2(float4(buffer2RG.rgb, sampleTHGRRMap.r));
	
	output.Buffer2.b = buffer2RG.b;
	output.Buffer2.a = buffer2RG.a;
	
	//
	// Properties in output.Buffer0.b
	//
	
	//
	// set propery for parallax mapping
	
	if (sampleTHGRRMap.r < 0.95f)
	{
		effectsProperties = effectsProperties | (1 << 10);
	}
	
	
	
	//
	// set propery for reflection
	
	//
	// save properties
	
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