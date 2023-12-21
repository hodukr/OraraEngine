#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s1);

static const float pi = 3.1415926535f;

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float2 uv = In.TexCoord;

    //波紋の中心をとる
    uv.x -= 0.5f;
    uv.y -= 0.5f;
   
    float dist = sqrt(uv.x * uv.x + uv.y * uv.y);
    float angle = atan2(uv.y, uv.x);
 
    uv.x = 0.5f + cos(angle) * (dist * (1.0 + sin(dist * Water.WaveFrequency + -(Water.Time * Water.Speed) * pi) * Water.WaveAmplitude));
    uv.y = 0.5f + sin(angle) * (dist * (1.0 + sin(dist * Water.WaveFrequency + -(Water.Time * Water.Speed) * pi) * Water.WaveAmplitude));

    outDiffuse = g_Texture.Sample(g_SamplerState, uv);
    //outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse.a = 1.0f;
}

