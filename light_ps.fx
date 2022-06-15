Texture2D shaderTexture[4];

SamplerState SampleTypePoint : register(s0);

cbuffer LightBuffer
{ 
	float4 lightAmb;
	float4 lightDiff;
	float4 lightSpec;
	float3 lightDirection;
	//float3 lightPos;
    float specPower;
};

cbuffer CameraBuffer
{
    float3 cameraPos;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

static const float PI = 3.14159265f;

float distributionGGX(float3 n, float3 h, float roughness)
{
    float a = roughness * roughness;
	float a2 = a*a;
	float nDotH = max(dot(n, h), 0.0);
	float nDotH2 = nDotH * nDotH;
	float num = a2;
	float denom = (nDotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;
	return num / denom;
}

float geometrySchlickGGX(float nDotV, float roughness)
{
    float r = roughness + 1.0;
	float k = (r * r) / 8.0;
	float num = nDotV;
	float denom = nDotV * (1.0 - k) + k;
	return num / denom;
}

float geometrySmith(float3 n, float3 v, float3 l, float roughness)
{
    float nDotV = max(dot(n, v), 0.0);
	float nDotL = max(dot(n, l), 0.0);
	float ggx2 = geometrySchlickGGX(nDotV, roughness);
	float ggx1 = geometrySchlickGGX(nDotL, roughness);
	return ggx1 * ggx2;
}

float3 fresnelSchlick(float cosTheta, float3 f0)
{
    return f0 + (1.0 - f0) * pow(1.0 - cosTheta, 5.0);
}


float4 PS(PixelInputType input) : SV_TARGET
{
    float4 textureAlbedo;
    float4 textureNormal;
	float4 texturePixelPos;
	float4 textureSpecular;
    float4 outputColor;

    textureAlbedo = shaderTexture[0].Sample(SampleTypePoint, input.tex);
    textureNormal = shaderTexture[1].Sample(SampleTypePoint, input.tex);
	texturePixelPos = shaderTexture[2].Sample(SampleTypePoint, input.tex);
	textureSpecular = shaderTexture[3].Sample(SampleTypePoint, input.tex);
	
	float roughness = textureSpecular.r;
	float metalness = textureSpecular.g;
	
	float3 n = normalize(textureNormal.xyz);
	float3 v = normalize(cameraPos - texturePixelPos.xyz);
	float3 f0 = float3(0.04, 0.04, 0.04);
	float3 lo = float3(0.0, 0.0, 0.0);
	
	for(int i = 0; i < 1; i++)
	{
	    float3 l = normalize(-lightDirection);
		float3 h = normalize(v + l);
		float dist  = 1.0;
		float attenuation = 1.0 / (dist * dist);
		float3 radiance = lightDiff * attenuation;
		
		float ndf = distributionGGX(n, h, roughness);
		float g = geometrySmith(n, v, l, roughness);
		float3 f = fresnelSchlick(max(dot(h,v), 0.0), f0);
		
		float3 kS = f;
		float3 kD = float3(1.0, 1.0, 1.0) - kS;
		kD *= 1.0 - metalness;
		
		float3 numerator = ndf * g * f;
		float denominator = 4.0 * max(dot(n, v), 0.0) * max(dot(n, l), 0.0);
		float3 specular = numerator / max(denominator, 0.001);
		
		float nDotL = max(dot(n,l), 0.0);
		lo += (kD * textureAlbedo.rgb / PI + specular) * radiance * nDotL;
		//lo = f;
		//lo = float3(g, g, g);
	}
	
	float3 ambient = 0.002 * textureAlbedo.a * textureAlbedo.rgb ;
	float3 color = ambient + lo;
	color = color / (color + float3(1.0, 1.0, 1.0));
	color = pow (color, (1.0/2.2));
	
	outputColor = float4(color, 1.0);
    return outputColor;
}