//	------------------------------------------------------------------
//	Includes
//	------------------------------------------------------------------


//	------------------------------------------------------------------
//	Globals
//	------------------------------------------------------------------

float4 g_settings;

Texture2D g_RTGBufferSolid:register(t0);
Texture2D g_RTVolumetricLight:register(t1);

SamplerState SamplerT0:register(s0);
SamplerState SamplerT1:register(s1);

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
			
	float4 buffer01 = g_RTGBufferSolid.Sample(SamplerT0, input.UV);
	float4 buffer02 = g_RTVolumetricLight.Sample(SamplerT1, input.UV);


	output.Buffer1.r = buffer01.r;
	output.Buffer1.g = buffer02.r;
	output.Buffer1.b = buffer01.b;
	output.Buffer1.a = buffer01.a;

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