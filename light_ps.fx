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

cbuffer CameraBuffer
{
    float3 cameraDir;
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
	float4 roughness;
    float3 lightDir;
    float4 totalIntensity;
    float4 outputColor;
	float4 viewDir = float4(0.0, -1.0, 0.0, 0.0);
	float3 reflection;

    colors = shaderTexture[0].Sample(SampleTypePoint, input.tex);
    normals = shaderTexture[1].Sample(SampleTypePoint, input.tex);
	

    lightDir = -lightDirection;
	
	float diffIntensity = saturate(dot(normals.xyz, lightDir));
	
	float4 ambPart = lightAmb;
	float4 diffPart = lightDiff * diffIntensity;
	float4 specPart = float4(0.0, 0.0, 0.0, 0.0); 
	
	if(diffIntensity>0.0f)
	{
	    roughness = shaderTexture[2].Sample(SampleTypePoint, input.tex);
		reflection = normalize(2 * diffIntensity * normals - lightDirection);
		specPart = lightSpec * pow(saturate(dot(reflection, -cameraDir)), specPower);
	}

	//totalIntensity = saturate(lightAmb + diffPart + specPart);
	totalIntensity = saturate(lightAmb + diffPart);
	
	outputColor = colors * totalIntensity;
	//outputColor = float4(reflection, 1.0);
	//outputColor = totalIntensity;
    return outputColor;
}