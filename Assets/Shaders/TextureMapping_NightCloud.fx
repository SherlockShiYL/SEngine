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

Texture2D diffuseMap : register(t0);
Texture2D ndiffuseMap : register(t1);
Texture2D specularMap : register(t2);
Texture2D bumpMap : register(t3);
Texture2D normalMap : register(t4);
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
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 dirToLight : TEXCOORD1;
	float3 dirToView : TEXCOORD2;
	float2 texcoord : TEXCOORD0;
};

// Vertex Shader

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float displacement = bumpMap.SampleLevel(textureSampler, input.texcoord, 0).r;
	input.position += displacement * float4(input.normal, 0.0f) * ViewPosition.w;

	float4 worldPosition = mul(input.position, World);
	output.position = mul(worldPosition, View);
	output.position = mul(output.position, Projection);

	output.normal = mul(input.normal, (float3x3)World);
	output.tangent = mul(input.tangent, (float3x3)World);
	output.binormal = cross(output.normal, output.tangent);
	output.dirToLight = -lDirection.xyz;
	output.dirToView = normalize(ViewPosition.xyz - worldPosition.xyz);
	output.texcoord = input.texcoord;

	return output;
}

// Pixel Shader

float4 PS(VS_OUTPUT input) : SV_Target
{
	float3 n = normalize(input.normal);
	float3x3 TBNW = float3x3(normalize(input.tangent), normalize(input.binormal), n);
	
	float4 normalColor = normalMap.Sample(textureSampler, input.texcoord);
	float3 sampledNormal = float3((2.0f * normalColor.xy) - 1.0f, normalColor.z);
	n = mul(sampledNormal, TBNW);

	float3 l = normalize(input.dirToLight);
	float3 v = normalize(input.dirToView);

	float nl = saturate(dot(n, l));
	float nnl = saturate(dot(n, -l));
	float3 r = reflect(lDirection.xyz, n);
	float3 rNor = normalize(r);

	float4 Ta = lAmbient * mAmbient;
	float4 Td = lDiffuse * mDiffuse * nl;
	float4 nTd = lDiffuse * mDiffuse * nnl;
	float4 Ts = lSpecular * mSpecular * pow(saturate(dot(rNor, v)), mPower.x);

	float4 textureColor = diffuseMap.Sample(textureSampler, input.texcoord);
	float4 ntextureColor = ndiffuseMap.Sample(textureSampler, input.texcoord);
	float4 specularColor = specularMap.Sample(textureSampler, input.texcoord);

	float4 lColor = (Ta + Td) * textureColor + nTd * ntextureColor + Ts * specularColor;

	return lColor;
}
