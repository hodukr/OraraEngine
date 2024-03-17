#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureDissolve : register(t1c);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//法線を正規化して格納する
    float4 normal = normalize(In.Normal);

	//ランバート
    float light = -dot(Light.Direction.xyz, normal.xyz);
    light = saturate(light);

	//テクスチャから色をサンプリングする
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse.rgb *= light;
    outDiffuse.a *= In.Diffuse.a;
   
	//ブリンフォン鏡面反射
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev);
    float3 halfv = eyev + Light.Direction.xyz;
    halfv = normalize(halfv);

    float specular = -dot(halfv, normal.xyz);
    saturate(specular);
    specular = pow(specular, 30);
    outDiffuse.rgb = saturate(outDiffuse.rgb + specular);
    
    //背景色を設定
    //outDiffuse.rgb = float3(0.0f, 0.0f, 0.0f);
	
	//ワイプ用テクスチャからサンプリング(rgbaからrだけを抽出する)(輝度)
    float dissolveValue = g_TextureDissolve.Sample(g_SamplerState, In.TexCoord);
	
    float threshold = Param.dissolveThreshold * (1.0f + Param.dissolveRange) - Param.dissolveRange;
	
    float rate = saturate((dissolveValue - threshold) / Param.dissolveRange);
   
    outDiffuse.a = rate;
    
    outDiffuse.rgb = lerp(outDiffuse.rgb, float3(0.0f, 0.0f, 1.0f), 1 - pow(rate, 5));

}
