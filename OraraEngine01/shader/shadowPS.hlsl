
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureShadowDepth : register(t1);
SamplerState g_SamplerState : register(s1);

// シャドウマップのサイズとフィルターサイズ
static const float shadowMapSizeX = 1280.0f;
static const float shadowMapSizeY = 720.0f;
static const float filterSize = 1.0f; // フィルターサイズ

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    //日向での色 = 普通のテクスチャ * 頂点色を作成しておく
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse *= In.Diffuse;

    In.ShadowPosition.xyz /= In.ShadowPosition.w; //正規化デバイス座標に変換
    In.ShadowPosition.x = In.ShadowPosition.x * 0.5f + 0.5f; //テクスチャ座標に変換
    In.ShadowPosition.y = -In.ShadowPosition.y * 0.5f + 0.5f; //

    float shadow = 0.0;
    
    for (float x = -filterSize; x <= filterSize; x += 1.0)
    {
        for (float y = -filterSize; y <= filterSize; y += 1.0)
        {
            float depth = g_TextureShadowDepth.Sample(g_SamplerState, float2(In.ShadowPosition.x + x / shadowMapSizeX, In.ShadowPosition.y + y / shadowMapSizeY)).r;
            
            if (depth < In.ShadowPosition.z - 0.001f)
            {
                shadow += 0.5f;
            }
        }
    }
    
	 //サンプルの平均を計算
    shadow /= ((2.0 * filterSize + 1.0) * (2.0 * filterSize + 1.0));
    
    // シャドウ値を考慮して色を暗くする
    outDiffuse.rgb *= (1.0f - shadow);
}
