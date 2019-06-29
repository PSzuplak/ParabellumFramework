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
float4x4 g_cameraInverseProjection;
float4x4 g_cameraInverseViewProjection;
float4 g_cameraPosition;
float4 g_cameraNF;						// X - near plane, Y - far plane
float4 g_lightDirection;
float4 g_lightPosition;	
float4 g_lightColorIntensity;
float4x4 g_volumeLightViewProjection;
float4 g_depthMapWHSize;				// x - rt width, y - rt height

float4 g_ctl;					// camera position to top left frustum corner
float4 g_ctl2ctr;				// camera position to top left frustum corner TO camera position to top rigt frustum corner
float4 g_ctl2cbl;				// camera position to top left frustum corner TO camera position to bottom left frustum corner

Texture2D g_RTBufferWorldZ:register(t0);
Texture2D g_RTLightDepthMap:register(t1);
Texture2D g_RTVolumetricLightLowRes:register(t2);

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
	float4 Color : SV_Target0;
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

	float4 worldZ;
	float4 position;

	output.Color = float4(0.0f, 0.0f, 0.0f, 0.5f);

	// Get World
	worldZ = g_RTBufferWorldZ.Sample(SamplerT0, input.UV);

	position.xyz = GetWorldFromFrustumCorners(
		input.UV, worldZ.w, g_cameraPosition.xyz,
		g_ctl.xyz, g_ctl2ctr.xyz, g_ctl2cbl.xyz
		);

	position.w = 1.0f;

	// 
	//if (worldZ.w > 0.9f)
	//	discard;


	//--------------------------------------------------------------------------
	// volume light
	float4 lightingPosition;
	float2 currentUV;
	float4 currentDepth;
	float4 currentTransparentDepth;
	float ourdepth;
	float sampleFactor = 0.0f;

	float4 startPositionVolumeView;
	float2 startUVVolumeView;
	float4 endPositionVolumeView;
	float2 endUVVolumeView;
	float4 sampleVector;
	float stepsCount;
	float4 samplePositionPrev;
	float4 samplePositionCurrent;
	float4 volumeColor = float4(1.0f, 1.0f, 1.0f, 0.0f);
	float samplesCount = 0.0f;
	float sampleVectorLen = 0.0f;

	float4 test = 0.0f;

	//
	// TEST
	//test = g_RTVolumetricLightLowRes.Sample(SamplerT2, input.UV);

	// project camera position to light view and calculate UV
	startPositionVolumeView = mul(float4(g_cameraPosition.xyz, 1.0f), g_volumeLightViewProjection);

	startUVVolumeView = 0.5f * startPositionVolumeView.xy + float2(0.5f, 0.5f);
	currentUV.y = 1.0f - currentUV.y;

	// project current pixel to light view coordinates and calculate UV
	endPositionVolumeView = mul(position, g_volumeLightViewProjection);

	endUVVolumeView = 0.5f * endPositionVolumeView.xy + float2(0.5f, 0.5f);
	currentUV.y = 1.0f - currentUV.y;


	// calculate sampling vector and steps count
	sampleVector = endPositionVolumeView - startPositionVolumeView;
	sampleVector = normalize(sampleVector) * 0.004f;

	stepsCount = length(endPositionVolumeView - startPositionVolumeView);

	

	// set samples positions
	samplePositionPrev = startPositionVolumeView;
	samplePositionCurrent = startPositionVolumeView;

	// volumetric light color
	volumeColor = float4(1.0f, 1.0f, 1.0f, 0.0f);

	// how many samples are lighted
	samplesCount = 0.0f;

	sampleVectorLen = 0.0f;


	//
	// ray tracing - calculate volumetric light intensity

	for (int i = 0; i < 128; ++i)
	{

		if (sampleVectorLen < stepsCount)
		{
			// move along the ray
			startPositionVolumeView += sampleVector;
			sampleVectorLen += length(sampleVector);
			samplePositionPrev = samplePositionCurrent;
			samplePositionCurrent = startPositionVolumeView;

			// calculate current UV
			currentUV = 0.5f * startPositionVolumeView.xy + float2(0.5f, 0.5f);
			currentUV.y = 1.0f - currentUV.y;

			// check if we move outide the screen
			if ((currentUV.x < 1.0f) && (currentUV.x > 0.0f) && (currentUV.y < 1.0f) && (currentUV.y > 0.0f))
			{				
				currentDepth = g_RTLightDepthMap.Sample(SamplerT1, currentUV);

				sampleFactor = (currentDepth.w - startPositionVolumeView.z) * 1.0f;

				if (currentDepth.w > (startPositionVolumeView.z + 0.001f))
				{
					samplesCount += 1.0f;
				}

			} // end if in screen

		}

	}


	// calculate light intensity
	if (samplesCount > 0)
	{
		output.Color = float4(0.0f, 0.0f, 0.0f, 0.5f);
		output.Color.b = samplesCount / 128.0f;
	}
	else
	{
		output.Color = float4(0.0f, 0.0f, 0.0f, 0.5f);
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