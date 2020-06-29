cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	matrix ProjView;
	matrix ProjProjection;
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
Texture2D selectionMap : register(t1);
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
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 dirToLight : TEXCOORD1;
	float3 dirToView : TEXCOORD2;
	float4 projection : TEXCOORD3;
};

// Vertex Shader

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    float4 worldPosition = mul(input.position, World);
    output.position = mul(worldPosition, View);
    output.position = mul(output.position, Projection);

    output.normal = mul(input.normal, (float3x3) World);
    output.dirToLight = -lDirection.xyz;
    output.dirToView = normalize(ViewPosition.xyz - worldPosition.xyz);
    output.texcoord = input.texcoord;
	
    output.projection = mul(worldPosition, ProjView);
    output.projection = mul(output.projection, ProjProjection);
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

	float2 projectionUV = input.projection.xy / input.projection.w;
	projectionUV = (projectionUV + 1.0f) * 0.5f;
	projectionUV.y = 1.0f - projectionUV.y;
	if (saturate(projectionUV.x) == projectionUV.x &&
	saturate(projectionUV.y) == projectionUV.y)
	{
		lColor += selectionMap.Sample(textureSampler, projectionUV) /**float4(0.0f, 0.5f, 0.3f, 1.0f)*/;
	}
	return lColor * textureColor;
}