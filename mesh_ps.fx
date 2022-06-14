
Texture2D shaderTexture[5];
SamplerState SampleType;

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 posPixel : POSITION;
};

struct PixelOutputType
{
    float4 color : SV_Target0;
    float4 normal : SV_Target1;
	float4 posPixel : SV_Target2;
	float4 specular : SV_Target3;
};

PixelOutputType PS(PixelInputType input) : SV_TARGET
{
    float4 textureAlbedo;
	float4 textureNormal;
	float4 textureRough;
	float4 textureMetal;
	float4 textureAo;
	float3 bumpNormal;
	
	PixelOutputType output;

	textureAlbedo = shaderTexture[0].Sample(SampleType, input.tex);
	textureNormal = shaderTexture[1].Sample(SampleType, input.tex);
	textureRough = shaderTexture[2].Sample(SampleType, input.tex);
	textureMetal = shaderTexture[3].Sample(SampleType, input.tex);
	textureAo = shaderTexture[4].Sample(SampleType, input.tex);
	
	textureNormal = (2.0f * textureNormal) - 1.0f;
	bumpNormal = (textureNormal.x * input.tangent) + (textureNormal.y * input.binormal) + (textureNormal.z * input.normal);
	bumpNormal = normalize(bumpNormal);
	
    output.color = textureAlbedo;
	output.color.a = textureAo.r;
	
	output.normal = float4(bumpNormal, 1.0);
	
	output.posPixel = float4(input.posPixel, 1.0f);
	
	output.specular.r = textureRough.r;
	output.specular.g = textureMetal.r;

    return output;
}