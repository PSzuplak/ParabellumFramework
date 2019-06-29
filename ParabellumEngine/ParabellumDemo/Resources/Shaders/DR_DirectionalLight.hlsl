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
#include "MathHelper.hlsl"

//	------------------------------------------------------------------
//	Globals
//	------------------------------------------------------------------

float4 g_cameraPositionFar;			// RGB - camera positon, A - camera z Far
float4 g_lightDirectionAmbient;		// RGB - direction, A - unused
float4 g_lightPosition;				// unused
float4 g_lightColorIntensity;		// RGB - color, A - light intensity
float4 g_ambientColor;				// ambient folor

float4 g_ctl;						// camera position to top left frustum corner
float4 g_ctl2ctr;					// camera position to top left frustum corner TO camera position to top rigt frustum corner
float4 g_ctl2cbl;					// camera position to top left frustum corner TO camera position to bottom left frustum corner

float4 g_cameraFrustumSlice00_min;	// slice of camera frustum enlighted/viewed by specific light frustum - in world coordinates
float4 g_cameraFrustumSlice00_max;	// slice of camera frustum enlighted/viewed by specific light frustum - in world coordinates

float4 g_light00_box;				// details of light frusutm ; x - width, y - height, z - distance from camera, w - reserved
float4 g_light01_box;				// 
float4 g_light02_box;				// 
float4 g_light03_box;				// 

float4 g_light00_planeTop;
float4 g_light00_planeLeft;
float4 g_light00_planeNear;

float4 g_light01_planeTop;
float4 g_light01_planeLeft;
float4 g_light01_planeNear;

float4 g_light02_planeTop;
float4 g_light02_planeLeft;
float4 g_light02_planeNear;

float4 g_light03_planeTop;
float4 g_light03_planeLeft;
float4 g_light03_planeNear;



Texture2D g_RTGBuffer00:register(t0);
Texture2D g_RTGBuffer01:register(t1);
Texture2D g_RTGBuffer02:register(t2);
Texture2D g_RTSolidDepth0:register(t3);
Texture2D g_RTSolidDepth1:register(t4);
Texture2D g_RTSolidDepth2:register(t5);
Texture2D g_RTSolidDepth3:register(t6);

SamplerState SamplerT0:register(s0);
SamplerState SamplerT1:register(s1);
SamplerState SamplerT2:register(s2);
SamplerState SamplerT3:register(s3);
SamplerState SamplerT4:register(s4);
SamplerState SamplerT5:register(s5);
SamplerState SamplerT6:register(s6);

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

	float4 rtNormal = g_RTGBuffer01.Sample(SamplerT0, input.UV);

	float4 sampleGBuffer0;
	float4 position;

	float4 packedSolidFrontMaterial;
	float4 solidFrontColor;

	float depth;

	float4 decodedNormal;
	float3 normal;
	float nl;
	float4 light;

	output.BufferLight = g_ambientColor * g_lightColorIntensity.w;


	//
	// read World Position and View Z
	sampleGBuffer0 = g_RTGBuffer00.Sample(SamplerT0, input.UV);


	//output.BufferLight = float4(sampleGBuffer0.w * 50.0f, 0.0f, 0.0f, 0.0f );
	//return output;

	//
	// discard if too far
	if (sampleGBuffer0.w > 0.9f)
	{
		output.BufferLight = float4(1.0f, 1.0f, 1.0f, 1.0f);
		return output;
	}


	packedSolidFrontMaterial = sampleGBuffer0;

	solidFrontColor = UnpackFloatToFloat4(packedSolidFrontMaterial.r);


	position.xyz = GetWorldFromFrustumCorners(
		input.UV, sampleGBuffer0.w, g_cameraPositionFar.xyz,
		g_ctl.xyz, g_ctl2ctr.xyz, g_ctl2cbl.xyz
		);
	
	position.w = sampleGBuffer0.w;

	sampleGBuffer0.xyz = position.xyz;

	// read and decode normal vector
	float4 unpackedNormal = UnpackFloat2toFloat4(rtNormal.rg);

	decodedNormal.xyz = NormalDecode(unpackedNormal.xyz);

	normal = decodedNormal.xyz;
	normal = normalize(normal);

	// light properties
	float4 lightProperties = UnpackFloatToFloat4(rtNormal.b);

	float4 tgeProperties = UnpackFloatToFloat4(rtNormal.a);


	//output.BufferLight.xyz = decodedNormal;
	//return output;
	//
	// translucency
	/*
	float4 rtTrWorldZ = g_RTSolidBackWorldZ.Sample(SamplerT2, input.UV);

	float4 translucencyColor = solidFrontColor;

	float distance = abs(rtTrWorldZ.w - sampleGBuffer0.w) * g_cameraPositionFar.w * lightProperties.a;
	float translucency = (1.0f-clamp( distance, 0.0f, 1.0f)) ;// *lightProperties.a;//lightProperties.a;

	translucency = clamp(translucency, 0.0f, 1.0f) + 1.0f;

	output.BufferLight.a = translucency;
	*/
	//return output;

	//
	// calculate light
	nl = dot(normal, -g_lightDirectionAmbient.xyz);

	light.xyz = float3(nl, nl, nl) + g_ambientColor.xyz;

	if ((nl < 0.01f) || (nl > 0.99f))
	{
		return output;
	}
	else
	{

		output.BufferLight += Phong(
			g_cameraPositionFar.xyz, position.xyz, normal.xyz,
			g_lightDirectionAmbient.xyz, g_lightColorIntensity, g_lightColorIntensity.w,
			lightProperties.r * 1.5f, lightProperties.g * 255.0f, 0.5f);

		output.BufferLight *= g_lightColorIntensity.w;
		output.BufferLight *= lightProperties.b;
	}

	//
	// set blur factor to default value
	output.BufferLight.a = 0.0f;


	//	--------------------------------------------------------
	//	Shadows 
	//	--------------------------------------------------------
	float positionLength = length(g_cameraPositionFar.xyz - position.xyz);
	float2 currentUV;
	float4 currentDepth;
	float4 currentTransparentDepth;
	float ourdepth;

	int bufferNo = -1;	// no of the depth buffer used to shadowmapping

	float depthBias = (1.0f / g_cameraPositionFar.w) * 0.1f;

	//------------------
	// Shadowm map 0

	float d_pos, d_depth;


	//
	// check in which light frustum the pixel is
	currentUV = GetUVForOrthographicProjection(g_light00_planeTop, g_light00_planeLeft, g_light00_box.xy, position.xyz);

	if (InUVRange(currentUV))
	{
		bufferNo = 0;
	}
	else
	{
		currentUV = GetUVForOrthographicProjection(g_light01_planeTop, g_light01_planeLeft, g_light01_box.xy, position.xyz);

		if (InUVRange(currentUV))
		{
			bufferNo = 1;
		}
		else
		{
			currentUV = GetUVForOrthographicProjection(g_light02_planeTop, g_light02_planeLeft, g_light02_box.xy, position.xyz);

			if (InUVRange(currentUV))
			{
				bufferNo = 2;
			}
			else
			{
				currentUV = GetUVForOrthographicProjection(g_light03_planeTop, g_light03_planeLeft, g_light03_box.xy, position.xyz);

				if (InUVRange(currentUV))
				{
					bufferNo = 3;
				}
				else
				{
					bufferNo = -1;
				}

			}

		}
		

	}



	/*
	viewPosition = mul(float4(position.xyz, 1.0f), g_lightView00);
	projectedPosition = mul(float4(position.xyz, 1.0f), g_lightViewProjection00);
	projectedPosition /= projectedPosition.w;

	//
	// buffer 0
	if ((projectedPosition.x < 1.0f) && (projectedPosition.x > -1.0f) &&
		(projectedPosition.y < 1.0f) && (projectedPosition.y > -1.0f))
	{
		bufferNo = 0;

		currentUV = 0.5f * projectedPosition.xy + float2(0.5f, 0.5f);
		currentUV.y = 1.0f - currentUV.y;
		currentDepth.w = g_RTSolidDepth0.Sample(SamplerT0, currentUV).w;
	}
	
	if (bufferNo != -1)
	{
		output.BufferLight.rgb = float3(1.0f, 0.0f, 0.0f);

		if ((currentDepth.w + depthBias) < GetLinearDepth(viewPosition.z, 1.0f, g_cameraPositionFar.w))
		{
			output.BufferLight.rgb = g_ambientColor.rgb * g_lightColorIntensity.w;
			output.BufferLight.a = 1.0f;
		}
	}
	*/

	
	//
	// buffer 0

	if (bufferNo == 0)
	{
		// distance from the light near plane to the sampled point
		d_pos = DistanceCrap(g_light00_planeNear.xyz, g_light00_planeNear.w, position.xyz);
		currentDepth = g_RTSolidDepth0.Sample(SamplerT0, currentUV);
		d_depth = DistanceCrap(g_light00_planeNear.xyz, g_light00_planeNear.w, currentDepth.xyz);
	}
	else
	if (bufferNo == 1)
	{
		d_pos = DistanceCrap(g_light01_planeNear.xyz, g_light01_planeNear.w, position.xyz);
		currentDepth = g_RTSolidDepth1.Sample(SamplerT0, currentUV);
		d_depth = DistanceCrap(g_light01_planeNear.xyz, g_light01_planeNear.w, currentDepth.xyz);
	}
	else
	if (bufferNo == 2)
	{
		d_pos = DistanceCrap(g_light02_planeNear.xyz, g_light02_planeNear.w, position.xyz);
		currentDepth = g_RTSolidDepth2.Sample(SamplerT0, currentUV);
		d_depth = DistanceCrap(g_light02_planeNear.xyz, g_light02_planeNear.w, currentDepth.xyz);
	}
	else
	if (bufferNo == 3)
	{
		d_pos = DistanceToPlane(g_light03_planeNear.xyz, g_light03_planeNear.w, position.xyz);
		currentDepth = g_RTSolidDepth3.Sample(SamplerT0, currentUV);
		d_depth = DistanceToPlane(g_light03_planeNear.xyz, g_light03_planeNear.w, currentDepth.xyz);
	}


	//
	// fill buffer
	if (bufferNo != -1)
	{
		if (d_pos > d_depth)
		{
			if (bufferNo == 0) output.BufferLight.rgba = float4(1.0f, 0.0f, 0.0f, 0.5f);
			if (bufferNo == 1) output.BufferLight.rgba = float4(1.0f, 1.0f, 0.0f, 0.5f);
			if (bufferNo == 2) output.BufferLight.rgba = float4(1.0f, 0.0f, 1.0f, 0.5f);
			if (bufferNo == 3) output.BufferLight.rgba = float4(0.0f, 1.0f, 0.0f, 0.5f);

			output.BufferLight.rgb = g_ambientColor.rgb * g_lightColorIntensity.w;
			output.BufferLight.a = g_lightColorIntensity.w;
		}
	}
	




	return output;

	//===============================================================================================================

	


	//output.BufferLight = (output.BufferLight) + solidFrontColor;

	output.BufferLight = clamp(output.BufferLight, 0.0f, 1.0f);




	//	--------------------------------------------------------
	//	Translucency
	//	If distance from shadowed pixel and lightet pixel
	//	is less than SOME value then light it up
	//	--------------------------------------------------------

	// translucency only for backface
	/*
	if (
		(bufferNo != -1) &&
		( (nl < 0.01f) || (nl > 0.99f) )
		)
	{
		float depthDifference = currentDepth - GetLinearDepth(viewPosition.z, 1.0f, g_cameraPositionFar.w);

		float translucencyFactor = -depthDifference * 500.0f;

		float power = 1.0f;

		power = translucencyFactor;

		if (depthDifference > -0.01f && depthDifference < -0.00001f)
		{
			

			output.BufferLight.rgb = float3(power, 0.0f, 0.0f);
			//output.BufferLight.rgb = float3(1.0f, 0.0f, 0.0f);

			//output.BufferLight.rgb = float3(0.0f, 0.0f, 0.0f);
			//output.BufferLight.r = translucencyFactor;
		}

	}
	*/


	output.BufferLight.rgb += tgeProperties.b;



	//
	// Clamp output
	//output.BufferLight.rgba = clamp(output.BufferLight.rgba, 0.0f, 1.0f);




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