//	------------------------------------------------------------------
//	Includes
//	------------------------------------------------------------------


//	------------------------------------------------------------------
//	Globals
//	------------------------------------------------------------------

float4x4 g_World;
float4x4 g_CameraView;
float4x4 g_CameraProjection;
float4 g_colorBorder;
float4 g_colorInterior;
float4 g_boxSize;


Texture2D g_TBackground:register(t0);

SamplerState SampleType:register(s0);

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

	// set texture coords
	output.UV = input.UV;

	// set position
	float4 worldPosition = mul(input.Position, g_World);
	float4 viewPosition = mul(worldPosition, g_CameraView);
	output.Position = mul(viewPosition, g_CameraProjection);

	return output;
}



//	------------------------------------------------------------------
//	Function for Pixel Shader
//	------------------------------------------------------------------
PixelShaderOutput PixelShaderFunction(VertexShaderOutput input) : SV_TARGET
{
	PixelShaderOutput output;
		
	if (
		(input.UV.x < 0.01f || input.UV.x > 0.99f) ||
		(input.UV.y < 0.01f || input.UV.y > 0.99f)
		)
	{
		output.Color = float4(g_colorBorder.x * input.UV.x, g_colorBorder.y * input.UV.y, 0.0f, 0.5f);
	}
	else
	{
		discard;
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