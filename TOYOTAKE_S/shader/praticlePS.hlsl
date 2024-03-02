#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);

    float4 resalt = float4(0.0f,0.0f,0.0f,0.0f);
    for (int y = -2; y < 3; y++)
    {
        for (int x = -2; x < 3; x++)
        {
            resalt += g_Texture.Sample(g_SamplerState, In.TexCoord + float2(x, y) / 200.0f);

        }
        
    }
    resalt /= 9.0f;
    outDiffuse = resalt;
    outDiffuse *= In.Diffuse;
    

    //if (outDiffuse.a == 0.0f)
    //{
    //    outDiffuse.rgba = 0.0f;
    //}
    //outDiffuse *= float4(1.0f, 0.0f,0.0f, 1.0f);

}
