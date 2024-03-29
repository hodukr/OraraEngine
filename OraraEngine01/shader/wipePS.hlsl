
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureWipe : register(t1);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//背景色を設定
    outDiffuse.rgb = g_Texture.Sample(g_SamplerState, In.TexCoord);
    	
	//ワイプ用テクスチャからサンプリング(rgbaからrだけを抽出する)
    float dissolveValue = g_TextureWipe.Sample(g_SamplerState, In.TexCoord);
	
    float threshold = Param.dissolveThreshold * (1.0f + Param.dissolveRange) - Param.dissolveRange;
	
    float rate = saturate((dissolveValue - threshold) / Param.dissolveRange);
   
    outDiffuse.a = rate;
    
    //step(x,y) →　x <= yなら1を返す  そうでないなら0を返す
    outDiffuse.a = step(Param.dissolveThreshold, dissolveValue);
    
    if (outDiffuse.a <= 0.1f)
    {
        outDiffuse.rgb = In.Diffuse.rgb;
        outDiffuse.a = 1.0f;
    }

}
