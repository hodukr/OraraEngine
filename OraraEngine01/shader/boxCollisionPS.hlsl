
#include "common.hlsl"


Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
        
   // ボックスの色
    float4 boxColor = float4(1, 0, 0, 1); // 例: 赤い色

    // ボックスのサイズ
    float3 boxSize = float3(1, 1, 1); // 例: 幅、高さ、奥行き

    // ボックスの中心座標
    float3 boxCenter = float3(0, 0, 0); // 例: ワールド座標の中心

    // ボックスの各辺に対するラインの太さ
    float lineWidth = 2.0f; // 例: ラインの太さ

    // ボックスのローカル座標
    float3 localPos = abs((In.WorldPosition / In.WorldPosition.w).xyz - boxCenter) / boxSize;

    // 各辺に対する線の描画ロジック
    float3 edgeLines = saturate(fwidth(localPos) - 1.0 / lineWidth);
    float edgeFactor = min(min(edgeLines.x, edgeLines.y), edgeLines.z);

    // ボックスの線を描画
    outDiffuse = lerp(boxColor, float4(0, 0, 0, 1), smoothstep(0, lineWidth * 0.5, edgeFactor));
}
