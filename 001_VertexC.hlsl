#include "000_Header.hlsl"

struct VertexInput //버텍스 쉐이더로 넘어감
{
    float4 position : POSITION0; //받을 값
	float4 color : COLOR0;
};

struct PixelInput
{
    float4 position : SV_POSITION; //픽셀 쉐이더로 넘길 값
    float4 color : COLOR0;
};

PixelInput VS(VertexInput input) //버텍스 쉐이더 부분
{
    PixelInput output;
    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);
	
	output.color = input.color;
	
    return output;
}

float4 PS(PixelInput input) : SV_TARGET //픽셀 쉐이더 부분
{
    return input.color;
}