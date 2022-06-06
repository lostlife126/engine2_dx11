
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD;
	float fogFactor : FOG;
};

PixelInputType VS(VertexInputType input)
{
    PixelInputType output;
	float4 cameraPos;
    
    input.position.w = 1.0f;

    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    output.tex = input.tex;
	
	cameraPos = mul(input.position, worldMatrix);
	cameraPos = mul(cameraPos, viewMatrix);
	output.fogFactor = saturate(exp(-cameraPos.z));
	
	return output;
}