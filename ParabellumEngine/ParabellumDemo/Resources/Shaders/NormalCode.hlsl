float3 NormalEncode(float3 n)
{
	//n = normalize(n);
	n.xyz = 0.5f * (n.xyz + 1.0f);
	return n;
}

float3 NormalDecode(float3 enc)
{
	return (2.0f * enc.xyz - 1.0f);
}

float3 NormalEncodeX(float3 n, float range)
{
	//n = normalize(n);
	n.xyz = (0.5f * range) * (n.xyz + (1.0f*range) );
	return n;
}



float2  NormalToSpherical( float3 normal )
{
	return normalize( normal.xy ) * ( sqrt( -normal.z * 0.5f + 0.5f ));
}

float3 NormalToCartesian( float2 encoded )
{
	float4 nn = float4(  encoded, 1, -1 );
	float l = dot( nn.xyz, -nn.xyw );
	nn.z = l;
	nn.xy *= sqrt(l);
	return nn.xyz * 2 + float3( 0, 0, -1 ); 
}