//	------------------------------------------------------------------
//	ParallaxMapping
//	
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

float4x4 g_cameraView;
float4x4 g_cameraProjection;
float4x4 g_cameraViewProjection;
float4x4 g_cameraInverseView;
float4x4 g_cameraInverseProjection;
float4x4 g_cameraInverseViewProjection;
float4 g_cameraPositionFar;
float4 g_cameraDirectionNear;
float4 g_areaProperties;

float4 g_ctl;					// camera position to top left frustum corner
float4 g_ctl2ctr;				// camera position to top left frustum corner TO camera position to top rigt frustum corner
float4 g_ctl2cbl;				// camera position to top left frustum corner TO camera position to bottom left frustum corner


//	------------------------------------------------------------------
//	Resources
//	------------------------------------------------------------------

Texture2D g_RTGBuffer00:register(t0);
Texture2D g_RTGBuffer01:register(t1);
Texture2D g_RTGBuffer02:register(t2);

SamplerState SamplerT0:register(s0);
SamplerState SamplerT1:register(s1);
SamplerState SamplerT2:register(s2);

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
	float4 Buffer0 : SV_Target0;
	float4 Buffer1 : SV_Target0;
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

	output.Buffer0 = float4(0.0f, 0.0f, 0.0f, 0.0f);
	output.Buffer1 = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// get normal to face
	float4 packedHeightData = g_RTGBuffer02.Sample(SamplerT1, input.UV);
	float4 heightData = UnpackFloat2toFloat4(packedHeightData.rg);

	float3 decodedNormal = NormalDecode(heightData.rgb);
	float3 normal = normalize(decodedNormal);

	float3 changedNormal = normal;

	// read world positon and viewZ
	float4 worldZ = g_RTGBuffer00.Sample(SamplerT0, input.UV);

	float4 position;

	position.xyz = GetWorldFromFrustumCorners(
		input.UV, worldZ.w, g_cameraPositionFar.xyz,
		g_ctl.xyz, g_ctl2ctr.xyz, g_ctl2cbl.xyz
		);

	position.w = worldZ.w;


	// calculate view direction
	float3 viewDirection = g_cameraPositionFar.xyz - position.xyz;

	// get thle length of normal to face (length is height factor)
	float height = heightData.a;

	float2 currentUV = float2(0.0f, 0.0f);
	float currentSampleHeight = 0.0f;

	float4 g_HIBI = float4(0.1f, 0.0f, 0.0f, 0.0f);


	normal.y = -normal.y;	// reverse due to inverse screen space coordiantes

	float heightLevel = g_HIBI.x / length(viewDirection);// / (1 + dot(normal, -normalize(viewDirection)));

	float2 dirUV = normal * heightLevel;

	float stepSize = 1.0f / 16.0f;

	float currentRaySize = 1.0f;
	float previousRaySize = 1.0f;

	float2 previousUV = float2(0.0f, 0.0f);

	float prevSampleHeight = 1.0f;
	currentSampleHeight = 1.0f;


	bool isEndLoop = false;

	//
	if (height < 0.996f)
	{

		//
		// find poor coordinates interval first
		for (int i = 0; i < 32; ++i)
		{
			if (isEndLoop == false)
			{

				heightData = UnpackFloat2toFloat4( g_RTGBuffer02.Sample(SamplerT1, input.UV + currentUV).rg );
				currentSampleHeight = heightData.a;

				if (currentSampleHeight > currentRaySize)
				{
					isEndLoop = true;
				}
				else
				{
					previousRaySize = currentRaySize;
					currentRaySize -= stepSize;

					previousUV = currentUV;
					currentUV += stepSize * dirUV;

					prevSampleHeight = currentSampleHeight;
				}

			}
		}


		//
		// repeat the alghoritm on calculated interval
		isEndLoop = false;

		float2 beginUV = previousUV;
		float2 endUV = currentUV;

		float beginRaySize = previousRaySize;
		float endRaySize = currentRaySize;

		float beginSampleHeight = prevSampleHeight;
		float endSampleHeight = currentSampleHeight;

		currentUV = beginUV;
		currentRaySize = beginRaySize;
		stepSize /= 8.0f;

		
		for (int i = 0; i < 5; ++i)
		{
			if (isEndLoop == false)
			{
				heightData = UnpackFloat2toFloat4(g_RTGBuffer02.Sample(SamplerT1, input.UV + currentUV).rg);
				currentSampleHeight = heightData.a;

				if (currentSampleHeight > currentRaySize)
				{
					isEndLoop = true;
				}
				else
				{
					previousRaySize = currentRaySize;
					currentRaySize -= stepSize;

					previousUV = currentUV;
					currentUV += stepSize * dirUV;

					prevSampleHeight = currentSampleHeight;
				}

			}
		}
		
		// binary ?
		// currentUV = (endUV + beginUV) * 0.5f





	} // end if height


	float4 PM_worldZ = g_RTGBuffer00.Sample(SamplerT0, input.UV + currentUV);

	if ( abs(position.w - PM_worldZ.w) < 0.002f)
	{
		float4 changed = g_RTGBuffer00.Sample(SamplerT0, input.UV + currentUV);

		output.Buffer0 = changed;

		//output.Buffer0 = UnpackFloatToFloat4(changed);
		//changed.w += currentSampleHeight/1000.0f;

		//
		// normal buffer
		float4 rtNormal = g_RTGBuffer01.Sample(SamplerT1, input.UV + currentUV);

		output.Buffer1 = rtNormal;
	}
	else
	{
		output.Buffer0 = g_RTGBuffer00.Sample(SamplerT0, input.UV);
		output.Buffer1 = g_RTGBuffer01.Sample(SamplerT1, input.UV);
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