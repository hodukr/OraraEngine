#pragma once

class Light
{
public:
    BOOL		Enable;
    BOOL		Dummy[3];
    D3DXVECTOR4	Direction;
    D3DXCOLOR	Diffuse;
    D3DXCOLOR	Ambient;

    void Init();
    void SetLight(Light light);
};
