Texture2D shaderTexture[3];

SamplerState SampleTypePoint : register(s0);

cbuffer LightBuffer
{ 
	float4 lightAmb;
	float4 lightDiff;
	float4 lightSpec;
	float3 lightDirection;
    float specPower;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 PS(PixelInputType input) : SV_TARGET
{
    float4 colors;
    float4 normals;
	float4 cameraDirs;
    float3 lightDir;
    float4 totalIntensity;
    float4 outputColor;
	float4 viewDir = float4(0.0, -1.0, 0.0, 0.0);
	float3 reflection;

    colors = shaderTexture[0].Sample(SampleTypePoint, input.tex);
    normals = shaderTexture[1].Sample(SampleTypePoint, input.tex);
	cameraDirs = shaderTexture[2].Sample(SampleTypePoint, input.tex);
	cameraDirs = cameraDirs * 2.0f - 1.0f;

    lightDir = -lightDirection;
	
	float diffIntensity = saturate(dot(normals.xyz, lightDir));
	
	float4 ambPart = lightAmb;
	float4 diffPart = lightDiff * diffIntensity;
	float4 specPart = float4(0.0, 0.0, 0.0, 0.0); 
	
	if(diffIntensity>0.0f)
	{
		reflection = normalize(2 * diffIntensity * normals - lightDirection);
		specPart = lightSpec * pow(saturate(dot(reflection, -cameraDirs)), specPower);
	}

	totalIntensity = saturate(lightAmb + specPart);
	//totalIntensity = saturate(lightAmb + diffPart);
	
	outputColor = colors * totalIntensity;
	//outputColor = cameraDirs;
	//outputColor = totalIntensity;
    return outputColor;
}