Texture2D ObjTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
};

struct PixelInputType
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

float4 PS(PixelInputType input) : SV_TARGET
{
    float4 texColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;

    texColor = ObjTexture.Sample( SampleType, input.tex );
    color = ambientColor;
    lightDir = -lightDirection;
    lightIntensity = saturate(dot(input.normal, lightDir));
    if(lightIntensity > 0.0f)
    {
        color += (diffuseColor * lightIntensity);
    }
    color = saturate(color);
    color = color * texColor;
    return color;

}