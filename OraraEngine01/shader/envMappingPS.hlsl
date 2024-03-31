
#include "common.hlsl"

Texture2D g_Texture : register(t0);
TextureCube g_TextureEnvCube : register(t1);
SamplerState g_SamplerState : register(s0);

static const float pi = 3.1415926535f;

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    outDiffuse.rgb = g_Texture.Sample(g_SamplerState, In.TexCoord);
    
    // 法線を正規化して格納する
    float4 normal = normalize(In.Normal);

    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev);

    //波紋表現の時仕様
    //float2 uv = In.TexCoord;

    // 波紋の中心をとる
    //uv.x -= 0.5f;
    //uv.y -= 0.5f;

    //float dist = sqrt(uv.x * uv.x + uv.y * uv.y);
    //float angle = atan2(uv.y, uv.x);

    //uv.x = 0.5f + cos(angle) * (dist * (1.0 + sin(dist * Water.WaveFrequency + -(Water.Time * Water.Speed) * pi) * Water.WaveAmplitude));
    //uv.y = 0.5f + sin(angle) * (dist * (1.0 + sin(dist * Water.WaveFrequency + -(Water.Time * Water.Speed) * pi) * Water.WaveAmplitude));

    float3 refv = reflect(eyev, normal.xyz); // 修正点
    refv = normalize(refv);

    // テクスチャから色をサンプリングする                       /*float3(refv.xy + uv.xy, refv.z)*/
    float4 envColor = g_TextureEnvCube.Sample(g_SamplerState, refv);

    outDiffuse.rgb *= envColor.rgb;
    outDiffuse.a = In.Diffuse.a;
}
