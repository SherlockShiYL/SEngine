cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
    float4 ViewPosition;
}

Texture2D UMap : register(t0);
Texture2D DMap : register(t1);
Texture2D LMap : register(t2);
Texture2D RMap : register(t3);
Texture2D BMap : register(t4);
Texture2D FMap : register(t5);
Texture2D normalMap : register(t6);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
	float4 position	: POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
	float2 texCoord	: TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position	: SV_POSITION;
	float2 texCoord	: TEXCOORD;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 dirToView : TEXCOORD1;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
    float4 worldPosition = mul(input.position, World);
    output.position = mul(worldPosition, View);
	output.position = mul( output.position, Projection );

    output.normal = mul(input.normal, (float3x3) World);
    output.tangent = mul(input.tangent, (float3x3) World);
    output.binormal = cross(output.normal, output.tangent);
    output.texCoord = input.texCoord;

    output.dirToView = normalize(ViewPosition.xyz - worldPosition.xyz);

	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 n = normalize(input.normal);
    /*float3x3 TBNW = float3x3(normalize(input.tangent), normalize(input.binormal), n);
    float4 normalColor = normalMap.Sample(textureSampler, input.texCoord);
    float3 sampledNormal = float3((2.0f * normalColor.xy) - 1.0f, normalColor.z);
    n = mul(sampledNormal, TBNW);*/

    float3 dtv = normalize(input.dirToView);
    float3 r = reflect(-dtv, n);
    if (abs(r.x) >= abs(r.y) && abs(r.x) >= abs(r.z))
    {
        if (r.x > 0.0f)
        {
            r.yz *= 1.0f / r.x;
            return RMap.Sample(textureSampler, float2(1.0f - (r.z + 1.0f) * 0.5f, 1.0f - (r.y + 1.0f) * 0.5f));
        }
        else
        {
            r.yz *= -1.0f / r.x;
            return LMap.Sample(textureSampler, float2((r.z + 1.0f) * 0.5f, 1.0f - (r.y + 1.0f) * 0.5f));
        }
    }
    else if (abs(r.y) >= abs(r.x) && abs(r.y) >= abs(r.z))
    {
        if (r.y > 0.0f)
        {
            r.xz *= 1.0f / r.y;
            return UMap.Sample(textureSampler, float2((r.x + 1.0f) * 0.5f, (r.z + 1.0f) * 0.5f));
        }
        else
        {
            r.xz *= -1.0f / r.y;
            return DMap.Sample(textureSampler, float2((r.x + 1.0f) * 0.5f, 1.0f - (r.z + 1.0f) * 0.5f));
        }
    }
    else if (abs(r.z) >= abs(r.x) && abs(r.z) >= abs(r.y))
    {
        if (r.z > 0.0f)
        {
            r.xy *= 1.0f / r.z;
            return FMap.Sample(textureSampler, float2((r.x + 1.0f) * 0.5f, 1.0f - (r.y + 1.0f) * 0.5f));
        }
        else
        {
            r.xy *= -1.0f / r.z;
            return BMap.Sample(textureSampler, float2(1.0f - (r.x + 1.0f) * 0.5f, 1.0f - (r.y + 1.0f) * 0.5f));
        }
    }
    return 1.0f;
}
