#include "common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	float4 worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);

	float light = -dot(Light.Direction.xyz, worldNormal.xyz);
	light = saturate(light);

	Out.Diffuse = In.Diffuse * Material.Diffuse * light * Light.Diffuse;
	Out.Diffuse += In.Diffuse * Material.Ambient * Light.Ambient;
	Out.Diffuse += Material.Emission;
	Out.Diffuse.a = In.Diffuse.a * Material.Diffuse.a;


	Out.Position = mul( In.Position, wvp );

    Out.TexCoord = In.TexCoord;

    //float4 wPos = mul(In.Position, World);
    //float4 wCenterPos = Param.pos;
    //// 追加: 波紋の計算
    //float dist = distance(wPos.xz, Param.pos.xz);
    ////float length = sqrt((wPos.x - wCenterPos.x) * (wPos.x - wCenterPos.x) + (wPos.z - wCenterPos.z) * (wPos.z - wCenterPos.z));
    //wPos.y += Param.waveAmplitude * sin(2 * 3.141592653 * (dist / 140.0f - Param.time / Param.waveFrequency));
    //Out.Position = mul(wPos,wvp);
}

