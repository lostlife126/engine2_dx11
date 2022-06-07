
Texture2D shaderTexture;
SamplerState SampleType;

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD;
	float fogFactor : FOG;
};

struct PixelOutputType
{
    float4 color : SV_Target0;
    float4 normal : SV_Target1;
	float4 roughness : SV_Target2;
};

PixelOutputType PS(PixelInputType input) : SV_Target
{
    float4 textureColor;
	float4 fogColor = float4(0.5f, 0.5f, 0.5f, 0.5f);
	PixelOutputType finalColor;

	textureColor = shaderTexture.Sample(SampleType, input.tex);

    finalColor.color = fogColor; // input.fogFactor * textureColor + (1.0 - input.fogFactor) * fogColor;
	return finalColor;
}