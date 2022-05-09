
Texture2D shaderTexture : register(t0);
SamplerState SampleTypeWrap : register(s0);

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelOutputType
{
    float4 color : SV_Target0;
    float4 normal : SV_Target1;
};

PixelOutputType PS(PixelInputType input) : SV_TARGET
{
	PixelOutputType output;

	output.color = shaderTexture.Sample(SampleTypeWrap, input.tex);

	output.normal = float4(input.normal, 1.0f);

    return output;
}