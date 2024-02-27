#pragma once
struct VERTEX_3D
{
    D3DXVECTOR3 Position;
    D3DXVECTOR3 Normal;
    D3DXVECTOR4 Diffuse;
    D3DXVECTOR2 TexCoord;
};

struct MATERIAL
{
    D3DXCOLOR	Ambient;
    D3DXCOLOR	Diffuse;
    D3DXCOLOR	Specular;
    D3DXCOLOR	Emission;
    float		Shininess;
    BOOL		TextureEnable;
    float		Dummy[2];
};

struct LIGHT
{
    BOOL		Enable;
    BOOL		Dummy[3];
    D3DXVECTOR4	Direction;
    D3DXCOLOR	Diffuse;
    D3DXCOLOR	Ambient;
    D3DXMATRIX  ViewMatrix;       //追加 ライトカメラ行列
    D3DXMATRIX  ProjectionMatrix; //追加 ライトプロジェクション行列
};

struct PARAMETER
{
    D3DXVECTOR4 Hitpoint;
    D3DXCOLOR	BaseColor;
    D3DXCOLOR	LostColor;
    D3DXCOLOR	DiffColor;

    float dissolveThreshold;
    float dissolveRange;
    float dummy[2];
};

struct WATER
{
    D3DXVECTOR4 Pos;
    float WaveAmplitude;  //振幅 
    float WaveFrequency;  //周期 
    float Time;           //時間 
    float Speed;          //速度
};

struct PRATICLE
{
    D3DXCOLOR Color[3];
    bool IsGradation;
    D3DXVECTOR3 Statposition;
    float Size;
    float Dummy[3];
};
