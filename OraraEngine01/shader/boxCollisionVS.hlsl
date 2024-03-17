#include "common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);

    Out.Position = mul(In.Position, wvp);
    Out.Diffuse = In.Diffuse;
    Out.TexCoord = float2(0.0f, 0.0f);
    Out.WorldPosition = float4(0.0f, 0.0f, 0.0f, 0.0f);
    Out.Normal = float4(0.0f, 0.0f, 0.0f, 0.0f);
    Out.ShadowPosition = float4(0.0f, 0.0f, 0.0f, 0.0f);
}
