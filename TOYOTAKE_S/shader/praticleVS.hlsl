
#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    Out.Position = mul(In.Position, wvp);
    
    Out.Diffuse = In.Diffuse;
    Out.TexCoord = In.TexCoord;
    
    //Out.Diffuse = Praticle.Color1;
    Out.Diffuse = In.Diffuse * Material.Diffuse;

    if (Praticle.Gradation)
    {
        float3 worldPositon = mul(In.Position, World);
        float3 vec = worldPositon - Praticle.pos;
        float len = length(vec);
        float size = Praticle.Size;
        if (len < size)
        {
            Out.Diffuse.rgb *= (Praticle.Color1 * (1.0f - (len / size)) + Praticle.Color2 * (len / size)).rgb;
        }
        else
        {
            Out.Diffuse.rgb *= (Praticle.Color2 * (size - (len / size)) + Praticle.Color3 * ((len / size) - 1.0f)).rgb;
            Out.Diffuse.a *= In.Diffuse.a;

        }
    }
  
}


