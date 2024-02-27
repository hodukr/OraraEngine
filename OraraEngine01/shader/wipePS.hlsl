
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureWipe : register(t1);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	//�w�i�F��ݒ�
    outDiffuse.rgb = g_Texture.Sample(g_SamplerState, In.TexCoord);
	
	//���C�v�p�e�N�X�`������T���v�����O(rgba����r�����𒊏o����)
    float dissolveValue = g_TextureWipe.Sample(g_SamplerState, In.TexCoord);
	
    float threshold = Param.dissolveThreshold * (1.0f + Param.dissolveRange) - Param.dissolveRange;
	
    float rate = saturate((dissolveValue - threshold) / Param.dissolveRange);
   
    outDiffuse.a = rate;
    
    //step(x,y) ���@x <= y�Ȃ�1��Ԃ�  �����łȂ��Ȃ�0��Ԃ�
    outDiffuse.a = step(Param.dissolveThreshold, dissolveValue);
    
    outDiffuse.a *= 1.0f;
}
