
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraBuffer
{
	matrix lightViewMatrix;
    float3 cameraPos;
};

cbuffer FogBuffer
{
    float fogStart;
	float fogEnd;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 posPixel : POSITION;
	float4 posLightView : TEXCOORD1;
	float fogFactor: FOG;
};

PixelInputType VS(VertexInputType input)
{
    PixelInputType output;
	
	float4 camPos;
    
    input.position.w = 1.0f;

    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
	camPos = output.position;
    output.position = mul(output.position, projectionMatrix);
    
	output.posLightView = mul(input.position, worldMatrix);
    output.posLightView = mul(output.posLightView, lightViewMatrix);
    output.posLightView = mul(output.posLightView, projectionMatrix);
	
    output.tex = input.tex;
	
	output.posPixel = mul(input.position, worldMatrix);

    output.normal = mul(input.normal, (float3x3)worldMatrix);
    output.normal = normalize(output.normal);
	
	output.binormal = mul(input.binormal, (float3x3)worldMatrix);
    output.binormal = normalize(output.binormal);
	
	output.tangent = mul(input.tangent, (float3x3)worldMatrix);
    output.tangent = normalize(output.tangent);

	output.fogFactor = saturate(camPos.z * 0.01f);
	
	return output;
}