
#include "common.hlsl"

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    // ボックスの線を描画
    outDiffuse = float4(0.3, 1, 0.3, 1);
}
