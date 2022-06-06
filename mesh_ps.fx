
Texture2D shaderTexture[3];
SamplerState SampleType;

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct PixelOutputType
{
    float4 color : SV_Target0;
    float4 normal : SV_Target1;
	float4 roughness : SV_Target2;
};

PixelOutputType PS(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
	float4 bumpMap;
	float3 bumpNormal;
	float4 roughness;
	
	PixelOutputType output;

	textureColor = shaderTexture[0].Sample(SampleType, input.tex);
	bumpMap = shaderTexture[1].Sample(SampleType, input.tex);
	roughness = shaderTexture[2].Sample(SampleType, input.tex);
	bumpMap = (2.0f * bumpMap) - 1.0f;
	bumpNormal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal);
	bumpNormal = normalize(bumpNormal);
	
    output.color = textureColor;
	output.normal = float4(bumpNormal, 1.0);
	output.roughness = roughness;

    return output;
}