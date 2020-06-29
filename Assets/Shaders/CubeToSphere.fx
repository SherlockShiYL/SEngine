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

    output.dirToView = normalize(ViewPosition.xyz - worldPosition.xyz); // ???????

	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 n = normalize(input.normal);
    //float3x3 TBNW = float3x3(normalize(input.tangent), normalize(input.binormal), n);
    //float4 normalColor = normalMap.Sample(textureSampler, input.texCoord);
    //float3 sampledNormal = float3((2.0f * normalColor.xy) - 1.0f, normalColor.z);
    //n = mul(sampledNormal, TBNW);

    float3 dtv = normalize(input.dirToView);
    float3 r = reflect(-dtv, n);
	
    float dotu = dot(n, float3(0.0f, 1.0f, 0.0f));
    float dotd = dot(n, float3(0.0f, -1.0f, 0.0f));
    float dotl = dot(n, float3(-1.0f, 0.0f, 0.0f));
    float dotr = dot(n, float3(1.0f, 0.0f, 0.0f));
    float dotf = dot(n, float3(0.0f, 0.0f, 1.0f));
    float dotb = dot(n, float3(0.0f, 0.0f, -1.0f));

    if (dotu >= dotd && dotu >= dotl && dotu >= dotr && dotu >= dotf && dotu >= dotb)
        return UMap.Sample(textureSampler, input.texCoord);
    if (dotd >= dotu && dotd >= dotl && dotd >= dotr && dotd >= dotf && dotd >= dotb)
        return DMap.Sample(textureSampler, input.texCoord);
    if (dotl >= dotu && dotl >= dotd && dotl >= dotr && dotl >= dotf && dotl >= dotb)
        return LMap.Sample(textureSampler, input.texCoord);
    if (dotr >= dotu && dotr >= dotd && dotr >= dotl && dotr >= dotf && dotr >= dotb)
        return RMap.Sample(textureSampler, input.texCoord);
    if (dotf >= dotu && dotf >= dotd && dotf >= dotl && dotf >= dotr && dotf >= dotb)
        return FMap.Sample(textureSampler, input.texCoord);
    if (dotb >= dotu && dotb >= dotd && dotb >= dotl && dotb >= dotr && dotb >= dotf)
        return BMap.Sample(textureSampler, input.texCoord);
    return 1.0f;
}
