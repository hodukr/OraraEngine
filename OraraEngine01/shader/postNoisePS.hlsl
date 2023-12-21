
#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

float Rand(float2 coord)
{
	//frac(x) = xの小数部を返す乱数は0.0f <= rand < 1.0fの範囲
	return frac(sin(dot(coord.xy, float2(12.9898f, 78.233f)))*43758.5453f);
}
void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float4 Tex = g_Texture.Sample(g_SamplerState, In.TexCoord);
	
	//float noise = clamp(0.2f, 0.5f, Rand(In.TexCoord * Param.hp.x));

	//Tex.rgb += noise;

	outDiffuse.rgb = saturate(Tex.rgb)* In.Diffuse.rgb;
	outDiffuse.a = Tex.a * In.Diffuse.a;
	

	//////グレースケール変換
 //   float gray;
 //   gray = 0.299 * outDiffuse.r + 0.587 * outDiffuse.g + 0.114 * outDiffuse.b;
 //   outDiffuse.r = gray;
 //   outDiffuse.g = gray;
 //   outDiffuse.b = gray;

  
    
	//セピア変換
    //float4 sepia = float4(0.960, 0.784, 0.580, 1.0);
    //outDiffuse *= sepia;

    outDiffuse.a = 1.0f;
    //    discard;
    
}
