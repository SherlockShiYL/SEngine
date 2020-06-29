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
	float4 color : COLOR;
};

struct VS_OUTPUT
{
	float4 position	: SV_POSITION;
	float4 color : COLOR0;
};

// Vertex Shader

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	float4 worldPosition = mul( input.position, World );
	output.position = mul(worldPosition, View);
	output.position = mul( output.position, Projection );
	
	float3 n = normalize(mul(input.normal, (float3x3)World));
	float3 l = normalize(-lDirection.xyz);
	float3 v = normalize(ViewPosition.xyz - worldPosition.xyz);
	
	float nl = saturate(dot(n, l));
	float3 r = reflect(lDirection.xyz, n);
	float3 rNor = normalize(r);
	
	float4 Ta = lAmbient * mAmbient;
	float4 Td = lDiffuse * mDiffuse * nl;
	float4 Ts = lSpecular * mSpecular * pow(saturate(dot(rNor, v)), mPower.x);
	
	output.color = (Ta + Td) * input.color + Ts;

	return output;
}

// Pixel Shader

float4 PS(VS_OUTPUT input) : SV_Target
{
	return input.color;
}
