//
//	Phong 
//
//
// cameraPosition - camera position
// position - wolrd position of the pixel
// normal - normal vector to the surface
// lightDirection - light direction
// lightColor - color of the light
// lightIntensity - light intenisty - result color is multiplicated withthis one
// specularIntensity - specular intensity
// specularPower - specular power
// specularFactor -  factor of WHITE light and Color which will be for specular reflection 
//
// Result - RGB - result color, A - specular
//
float4 Phong(
	float3 cameraPosition, float3 position, float3 normal, 
	float3 lightDirection, float4 lightColor, float lightIntensity,
	float specularIntensity, float specularPower, float specularFactor
	)
{
	float4 result = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 reflected = normalize(reflect(lightDirection, normal));
	float3 target = normalize(cameraPosition - position.xyz);
	float NL = dot(normal, -lightDirection);

	float3 diffuse = NL * lightColor.xyz;
	float specular = specularIntensity * pow(saturate(dot(reflected, target)), specularPower);

	float3 specularColor = (specularFactor * float3(1.0f, 1.0f, 1.0f)) + (1.0f - specularFactor)*lightColor.xyz;

	result = float4(diffuse.rgb, specular) + float4(specularColor.rgb, 0.0f) * specular;
	result.a = specular;

	return result;
}

