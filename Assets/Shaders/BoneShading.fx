cbuffer ConstantBuffer : register(b0)
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

cbuffer BonesConstantBuffer : register(b1)
{
	matrix Bone[100];
}

Texture2D diffuseMap : register(t0);
SamplerState textureSampler : register(s0);

// Structs

struct VS_INPUT
{
	float4 position	: POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texcoord : TEXCOORD;	
    int4 boneindex : BLENDINDICES;
    float4 boneweight : BLENDWEIGHT;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
	float3 dirToLight : TEXCOORD1;
	float3 dirToView : TEXCOORD2;
};

// Vertex Shader

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

    output.position = mul(Bone[input.boneindex.x], input.position) * input.boneweight.x;
    output.position += mul(Bone[input.boneindex.y], input.position) * input.boneweight.y;
    output.position += mul(Bone[input.boneindex.z], input.position) * input.boneweight.z;
    output.position += mul(Bone[input.boneindex.w], input.position) * input.boneweight.w;

    output.normal = mul(Bone[input.boneindex.x], float4(input.normal, 0.0f)) * input.boneweight.x;
    output.normal += mul(Bone[input.boneindex.y], float4(input.normal, 0.0f)) * input.boneweight.y;
    output.normal += mul(Bone[input.boneindex.z], float4(input.normal, 0.0f)) * input.boneweight.z;
    output.normal += mul(Bone[input.boneindex.w], float4(input.normal, 0.0f)) * input.boneweight.w;

	float4 worldPosition = mul(input.position, World);
    output.position = mul(output.position, World);
    output.position = mul(output.position, View);
	output.position = mul(output.position, Projection);

	output.normal = mul(output.normal, (float3x3)World);
    output.tangent = mul(input.tangent, (float3x3) World);
	output.dirToLight = -lDirection.xyz;
	output.dirToView = normalize(ViewPosition.xyz - worldPosition.xyz);
	output.texcoord = input.texcoord;

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

	float4 Ta = lAmbient * mAmbient;
	float4 Td = lDiffuse * mDiffuse * nl;
	float4 Ts = lSpecular * mSpecular * pow(saturate(dot(rNor, v)), mPower.x);

	float4 textureColor = diffuseMap.Sample(textureSampler, input.texcoord);
	float4 lColor = (Ta + Td) * textureColor + Ts;

	return lColor * textureColor;
}
