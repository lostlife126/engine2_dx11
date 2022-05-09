Texture2D colorTexture : register(t0);
Texture2D normalTexture : register(t1);

SamplerState SampleTypePoint : register(s0);

cbuffer LightBuffer
{
    
	float4 lightAmb;
	float4 lightDiff;
	float3 lightDirection;
    float padding;
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
    float3 lightDir;
    float4 lightIntensity;
    float4 outputColor;

    colors = colorTexture.Sample(SampleTypePoint, input.tex);
    normals = normalTexture.Sample(SampleTypePoint, input.tex);

    lightDir = lightDirection;

    //lightIntensity = saturate(dot(normals.xyz, lightDir));
	lightIntensity = (lightAmb + lightDiff * dot(normals.xyz, lightDir));

	outputColor = (colors * lightIntensity);

    return outputColor;
}