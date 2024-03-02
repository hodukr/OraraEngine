
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

    Out.Diffuse = In.Diffuse * MaterialModel.Diffuse * light * Light.Diffuse * Material.Diffuse;
    Out.Diffuse += In.Diffuse * MaterialModel.Ambient * Light.Ambient;
    Out.Diffuse += MaterialModel.Emission;
    Out.Diffuse.a = In.Diffuse.a * MaterialModel.Diffuse.a;

	Out.Position = mul( In.Position, wvp );
	Out.TexCoord = In.TexCoord;

}

