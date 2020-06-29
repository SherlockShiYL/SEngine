cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	matrix SView;
	matrix SProjection;
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

Texture2D aoMap : register(t0);
Texture2D diffuseMap : register(t1);
Texture2D specularMap : register(t2);
Texture2D shadowMap : register(t3);
SamplerState textureSampler : register(s0);

// Structs

struct VS_INPUT
{
	float4 position	: POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texcoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float3 dirToLight : TEXCOORD1;
	float3 dirToView : TEXCOORD2;
	float2 texcoord : TEXCOORD0;
	float3 position2 : TEXCOORD3;
};

// Vertex Shader

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 worldPosition = mul(input.position, World);
	output.position = mul(worldPosition, View);
	output.position = mul(output.position, Projection);

	output.normal = mul(input.normal, (float3x3)World);
	output.dirToLight = -lDirection.xyz;
	output.dirToView = normalize(ViewPosition.xyz - worldPosition.xyz);
	output.texcoord = input.texcoord;

	float4 positionRelativeToLight = mul(worldPosition, SView);
	positionRelativeToLight = mul(positionRelativeToLight, SProjection);

	output.position2 = positionRelativeToLight.xyz / positionRelativeToLight.w;

	return output;
}

// Pixel Shader

float4 PS(VS_OUTPUT input) : SV_Target
{
	float3 n = normalize(input.normal);

	float3 l = normalize(input.dirToLight);
	float3 v = normalize(input.dirToView);

	float nl = saturate(dot(n, l));
	float3 r = reflect(lDirection.xyz, n);
	float3 rNor = normalize(r);

	float4 Ta = aoMap.Sample(textureSampler, input.texcoord);
	float4 Td = lDiffuse * mDiffuse * nl;
	float4 Ts = lSpecular * mSpecular * pow(saturate(dot(rNor, v)), mPower.x);

	float4 textureColor = diffuseMap.Sample(textureSampler, input.texcoord);

	return (Ta + Td) * textureColor + Ts;
}
