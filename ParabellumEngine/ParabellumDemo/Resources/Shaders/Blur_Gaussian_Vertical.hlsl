//	------------------------------------------------------------------
//	Includes
//	------------------------------------------------------------------


//	------------------------------------------------------------------
//	Globals
//	------------------------------------------------------------------

float4 g_cameraPositionFar;		// XYZ - camera position, W - camera Z Far
float4 g_settings;				// x - screen size (depends on dir - for vertical can be 1920 or 800), y - samples count, z - direction X, w - direction Y


Texture2D g_TBuffer:register(t0);

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
	const float weight[] = 
	{
		0.2270270270f, 0.1945945946f, 0.1216216216f ,0.0540540541f, 0.0162162162f
	};


	PixelShaderOutput output;

	output.Buffer0 = float4(0.0f, 0.0f, 0.0f, 0.0f);


	float4 middleSample = g_TBuffer.Sample(SamplerT0, input.UV);

	if(middleSample.a > 0.9f)
	{
		output.Buffer0 = float4(0.0f, 0.0f, 0.0f, 1.0f);
		return output;
	}

	float2 hp = g_settings.zw / g_settings.x;

	float4 sum = middleSample * weight[0];

	//for (int i = 1; i < (int)(g_settings.y * 0.5f); ++i)
	for (int i = 1; i < 5; ++i)
	{
		sum += g_TBuffer.Sample(SamplerT0, input.UV + hp*i) * weight[i] + g_TBuffer.Sample(SamplerT0, input.UV - hp*i) * weight[i];
	}

	//sum /= 8;//(int)(g_settings.y);

	output.Buffer0 = sum;
	output.Buffer0.a = middleSample.a;
	

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