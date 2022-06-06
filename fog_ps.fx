
Texture2D shaderTexture;
SamplerState SampleType;

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD;
	float fogFactor : FOG;
};

float4 PS(PixelInputType input) : SV_Target1
{
    float4 textureColor;
	float4 fogColor = float4(0.5f, 0.5f, 0.5f, 0.5f);
	float4 finalColor;

	textureColor = shaderTexture.Sample(SampleType, input.tex);

    finalColor = fogColor; // input.fogFactor * textureColor + (1.0 - input.fogFactor) * fogColor;
	return finalColor;
}