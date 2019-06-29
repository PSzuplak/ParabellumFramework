//	------------------------------------------------------------------
//	Globals
//	------------------------------------------------------------------
float4 g_Position;	// XY - top left position
float4 g_Rotation;	// X - angle x axis, Y - angle y axis
float4 g_Size;		// X - width, Y - height
float4 g_bgUV;
float4 g_fgUV;
float4 g_viewPortArea;
float4 g_color;
float4 g_borderSize;
float4 g_borderColor;

//	------------------------------------------------------------------
//	Resources
//	------------------------------------------------------------------
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
	float4 Position : SV_Position;
	float2 ScreenPos : TEXCOORD0;
	float2 AbsolutePos : TEXCOORD1;
	float2 Size : TEXCOORD2;
	float2 UVBG : TEXCOORD3;
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

	float4 position = g_Position ;

	output.Position.x = (position.x * 2.0f - 1.0f) + (input.Position.x * 2.0f * g_Size.x);
	output.Position.y = (1.0f - position.y * 2.0f) - (input.Position.y * 2.0f * g_Size.y);
	
	output.ScreenPos.xy = (output.Position.xy + 1.0f) * 0.5f;
	output.ScreenPos.y = 1.0f - output.ScreenPos.y;

	output.AbsolutePos = float4(0.0f, 0.0f, 0.0f, 0.0f);

	output.Position.z = 0.0f;
	output.Position.w = 1.0f;

	output.Size = g_Size.xy;

	output.UVBG = input.UV;

	if (output.UVBG.x < 0.01f)
	{
		output.UVBG.x = g_bgUV.x;
		output.AbsolutePos.x = 0.0f;
	}

	if (output.UVBG.y < 0.01f)
	{
		output.UVBG.y = g_bgUV.y;
		output.AbsolutePos.y = 0.0f;
	}

	if (output.UVBG.x > 0.99f)
	{
		output.UVBG.x = g_bgUV.z;
		output.AbsolutePos.x = g_Size.x;
	}

	if (output.UVBG.y > 0.99f)
	{
		output.UVBG.y = g_bgUV.w;
		output.AbsolutePos.y = g_Size.y;
	}

	return output;
}

//	------------------------------------------------------------------
//	Function for Pixel Shader
//	------------------------------------------------------------------
PixelShaderOutput PixelShaderFunction(VertexShaderOutput input) : SV_TARGET
{
	PixelShaderOutput output;
	float4 bg;
	
	// discard outsiders
	if (
		(input.ScreenPos.x < g_viewPortArea.x) ||
		(input.ScreenPos.x > g_viewPortArea.z) ||
		(input.ScreenPos.y < g_viewPortArea.y) ||
		(input.ScreenPos.y > g_viewPortArea.w)
		)
		discard;
	

	// border
	if (
		(input.AbsolutePos.x < g_borderSize.x) ||
		(input.AbsolutePos.y < g_borderSize.y) ||
		(input.AbsolutePos.x > (g_Size.x - g_borderSize.z)) ||
		(input.AbsolutePos.y >(g_Size.y - g_borderSize.w))
		)
	{
		output.Color.rgba = g_borderColor;
	}
	else
	{
		bg = g_TBackground.Sample(SampleType, input.UVBG.xy);

		output.Color.rgb = bg.rgb;
		output.Color.rgb *= g_color.rgb;
		output.Color.a = bg.a;
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