#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureToon : register(t1);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//法線を正規化して格納する
    float4 normal = normalize(In.Normal);
    
	//テクスチャから色をサンプリングする
    if (MaterialModel.TextureEnable)
    {
        outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
        outDiffuse.a *= In.Diffuse.a;
    }
    else
    {
        outDiffuse = In.Diffuse;
    }
    
    //光源計算
    float light = -dot(Light.Direction.xyz, normal.xyz);
       
	//プログラム版　明るさをセル塗り風に変換  if文でlightの値を3段階に調整
    if (light > 0.6f)
        light = 1.0f;
    else if (light > 0.3f)
        light = 0.8f;
    else
        light = 0.6f;
    
    //テクスチャ * 頂点色 * 明るさ
    outDiffuse.rgb *= saturate(In.Diffuse.rgb * light);

    ////エッジの作成
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev);
    
    float d = dot(eyev, normal.xyz);
    
    if (d > -0.3f)
    {
        outDiffuse.rgb *= 0.2f;
    }
    
}
