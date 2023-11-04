#pragma once

class Material
{
public:
    D3DXCOLOR	Ambient;
    D3DXCOLOR	Diffuse;
    D3DXCOLOR	Specular;
    D3DXCOLOR	Emission;
    float		Shininess;
    BOOL		TextureEnable;
    float		Dummy[2];

    void Init();
    void SetMaterial(Material material);
};
