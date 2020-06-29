cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 lDirection;
	float4 lAmbient;
	float4 lDiffuse;
	float4 lSpecular;
	float4 mAmbient;
	float4 mDiffuse;
	float4 mSpecular;
	float4 mPower;
	
	float4 ViewPosition;
}

Texture2D diffuseMap : register(t0);
SamplerState textureSampler : register(s0);

// Structs

struct VS_INPUT
{
	float4 position	: POSITION;
	float3 normal : NORMAL;
	float2 texCoord	: TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position	: SV_POSITION;
	float2 texCoord	: TEXCOORD;
	float4 color : COLOR;
};

// Vertex Shader

// This file is not correct

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	output.texCoord = input.texCoord;

	float4 worldPosition = mul(input.position, World);
	output.position = mul(worldPosition, View);
	output.position = mul(output.position, Projection);

	float3 n = normalize(mul(input.normal, (float3x3)World));
	float3 l = normalize(-lDirection.xyz);
	float3 v = normalize(ViewPosition.xyz - worldPosition.xyz);

	float nl = saturate(dot(n, l));
	float3 r = reflect(lDirection.xyz, n);
	float3 rNor = normalize(r);
	
	float4 Ta = lAmbient * mAmbient;
	float4 Td = lDiffuse * mDiffuse * nl;
	float4 Ts = lSpecular * mSpecular * pow(saturate(dot(rNor, v)), mPower.x);
	
	output.color = Ta + Td + Ts;

	return output;
}

// Pixel Shader

float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 textureColor = diffuseMap.Sample(textureSampler, input.texCoord);
	float4 lightingColor = input.color;
	return textureColor * lightingColor	;
}
