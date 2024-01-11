#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
    matrix wvp; //matrix型の変数wvpを定義する
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    Out.Position = mul(In.Position, wvp);

	//頂点の法線をワールド行列で変換する
    float4 worldNormal, normal;

    normal = float4(In.Normal.xyz, 0.0);
    worldNormal = mul(normal, World);
    worldNormal = normalize(worldNormal);
    Out.Normal = worldNormal;

	//光源処理
    float light = -dot(Light.Direction.xyz, worldNormal.xyz);
    light = saturate(light);
    Out.Diffuse = light;
    Out.Diffuse.a = In.Diffuse.a;
    Out.TexCoord = In.TexCoord;

    matrix lightwvp;
    lightwvp = mul(World, Light.View); //ワールド行列 * ライトビュー行列
    lightwvp = mul(lightwvp, Light.Projection); //さらに * ライトプロジェクション行列

    Out.ShadowPosition = mul(In.Position, lightwvp); //ライトカメラから見た頂点座標出力
}
