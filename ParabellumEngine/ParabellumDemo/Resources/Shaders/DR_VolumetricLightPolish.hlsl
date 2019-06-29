//	------------------------------------------------------------------
//	Includes
//	------------------------------------------------------------------


//	------------------------------------------------------------------
//	Globals
//	------------------------------------------------------------------

float4x4 g_cameraProjection;
float4 g_cameraDirectionFar;				// xyz - camera direction, w - far plane
float4 g_lightDirectionFactor;				// xyz - light direction, w - blur factor


Texture2D g_RTVolumetricLight:register(t0);

SamplerState SamplerT0:register(s0);

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


	// transfrom light direction to camera view
	float4 projectedLightDirection = float4(normalize(g_lightDirectionFactor.xyz + g_cameraDirectionFar.xyz), 1.0f);
	projectedLightDirection = mul(projectedLightDirection, g_cameraProjection);

	projectedLightDirection.x /= 1920.0f * 0.25f;
	projectedLightDirection.y /= 1080.0f * 0.25f;

	float2 sampledUV;
	float4 tempColor;
	int samplesCount = 0;
	
	for (int i = 0; i < 8; ++i)
	{
		sampledUV = input.UV + (i - 4) * projectedLightDirection.xy;
	
		tempColor = g_RTVolumetricLight.Sample(SamplerT0, sampledUV);
	
		output.Buffer0 += tempColor;
		samplesCount++;
	
	}
	
	output.Buffer0 /= samplesCount;


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