cbuffer ConstantBuffer : register(b0)
{
	float4 randomDistance;
}

Texture2D textureMap : register(t0);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
	float4 position : POSITION;
	float2 texcoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};

// =================================================
float dimX;
float dimY;
float dimZ;

float4 GetJpgSample(float2 uv, float4 keyColor)
{
	float4 result = textureMap.Sample(textureSampler, uv);
	if (distance(result.rgb, keyColor.rgb) == 0.0)
	{
		result = 0.0;
	}
	return result;
}

float2 Scale(float2 uv, float factor)
{
	return uv * factor;
}

float2 Translate(float2 uv, float dx, float dy)
{
	return uv + float2(dx, dy);
}

float2 Rotate(float2 origin, float angle)
{
	float2 result;

	float cos;
	float sin;
	sincos(angle*100.0f, sin, cos);

	float2 uv = origin + float2(cos, sin)*0.01f;
	result = uv;

	return result;
}

float4 Negative(float4 sample)
{
	float4 result = sample;
	result.rgb = 1.0 - result.rgb;
	result.a = 1.0;
	return result;
}

float4 HighPassFilter(float4 sample, float filter)
{
	float4 result = sample;
	if (length(result.rgb) > filter)
	{
		result = 0.0;
	}
	return result;
}

float4 LowPassFilter(float4 sample, float filter)
{
	float4 result = sample;
	if (length(result.rgb) < filter)
	{
		result = 0.0;
	}
	return result;
}

float2 Deform(float x, float y)
{
	float2 result;
	float2 uv = { x,y };
	float2 dxy = 0.0;

	dxy.y = sin(uv.x*4.0);
	dxy *= distance(float2(.5, .5), float2(x, y));
	uv += dxy;

	result = uv;
	return result;
}

float2 Ripple(float2 xy)
{
	float2 result;

	float2 cp = { dimX,dimY };
	float2 v1 = cp - xy;
	float d = length(v1);
	v1 = normalize(v1);
	result = v1 * (cos((d*6.28) + dimZ) * d);

	return frac(result);
}
// =================================================

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.position = input.position;
	output.texcoord = input.texcoord;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	/*float offset[5] = float(0.0f, 1.0f, 2.0f, 3.0f, 4.0f);
	float weight[5] = float(0.2270270270f, 0.1945945946f, 0.1216216216f, 0.0540540541f, 0.0162162162f);*/

	// Blur
	{
        const float weight[] = { 0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162 };
        float4 textureColor = 0.0f;
        int range = 4;
        float2 tex = 0.0f;
        if (randomDistance.w == 0.0f)
        {
            textureColor += textureMap.Sample(textureSampler, input.texcoord) * weight[0];
            for (int i = 1; i <= range; ++i)
            {
                tex.x = i / 1280.0f;
                textureColor += textureMap.Sample(textureSampler, saturate(input.texcoord + -tex)) * weight[i];
                textureColor += textureMap.Sample(textureSampler, saturate(input.texcoord + tex)) * weight[i];
            }
        }
        else
        {
            textureColor += textureMap.Sample(textureSampler, input.texcoord) * weight[0];
            for (int i = 1; i <= range; ++i)
            {
                tex.y += i / 720.0f;
                textureColor += textureMap.Sample(textureSampler, saturate(input.texcoord + -tex)) * weight[i];
                textureColor += textureMap.Sample(textureSampler, saturate(input.texcoord + tex)) * weight[i];
            }
        }
        return textureColor;
    }
	/*{
		const float weight[] = {0.2270270270, 0.1945945946, 0.1216216216,0.0540540541, 0.0162162162};
		float4 textureColor = 0.0f;
		if (randomDistance.w == 0.0f)
		{
			float2 tex = 0.0f;
			tex.x = 1.0f / 1280.0f;
			textureColor += textureMap.Sample(textureSampler, input.texcoord + (-4.0f * tex)) * weight[4];
            textureColor += textureMap.Sample(textureSampler, input.texcoord + (-3.0f * tex)) * weight[3];
            textureColor += textureMap.Sample(textureSampler, input.texcoord + (-2.0f * tex)) * weight[2];
            textureColor += textureMap.Sample(textureSampler, input.texcoord + (-1.0f * tex)) * weight[1];
			textureColor += textureMap.Sample(textureSampler, input.texcoord)*weight[0];
			textureColor += textureMap.Sample(textureSampler, input.texcoord + (1.0f * tex)) * weight[1];
            textureColor += textureMap.Sample(textureSampler, input.texcoord + (2.0f * tex)) * weight[2];
            textureColor += textureMap.Sample(textureSampler, input.texcoord + (3.0f * tex)) * weight[3];
            textureColor += textureMap.Sample(textureSampler, input.texcoord + (4.0f * tex)) * weight[4];
        }
		else
		{
			float2 tex = 0.0f;
			tex.y = 1.0f / 720.0f;
			textureColor += textureMap.Sample(textureSampler, input.texcoord + -4.0f * tex)*weight[4];
			textureColor += textureMap.Sample(textureSampler, input.texcoord + -3.0f * tex)*weight[3];
			textureColor += textureMap.Sample(textureSampler, input.texcoord + -2.0f * tex)*weight[2];
			textureColor += textureMap.Sample(textureSampler, input.texcoord + -1.0f * tex)*weight[1];
			textureColor += textureMap.Sample(textureSampler, input.texcoord)*weight[0];
			textureColor += textureMap.Sample(textureSampler, input.texcoord + 1.0f * tex)*weight[1];
			textureColor += textureMap.Sample(textureSampler, input.texcoord + 2.0f * tex)*weight[2];
			textureColor += textureMap.Sample(textureSampler, input.texcoord + 3.0f * tex)*weight[3];
			textureColor += textureMap.Sample(textureSampler, input.texcoord + 4.0f * tex)*weight[4];
		}
		return textureColor;
	}*/

	/*float2 temp = Rotate(input.texcoord, length(input.texcoord - randomDistance.yz) + randomDistance.x);
	float4 textureColor = textureMap.Sample(textureSampler, temp);
	return textureColor;*/
}