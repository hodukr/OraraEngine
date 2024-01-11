cbuffer WorldBuffer : register(b0)
{
	matrix World;
}
cbuffer ViewBuffer : register(b1)
{
	matrix View;
}
cbuffer ProjectionBuffer : register(b2)
{
	matrix Projection;
}

struct MATERIAL
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emission;
	float  Shininess;
	bool   TextureEnable;
	float2 Dummy;
};

cbuffer MaterialBuffer : register(b3)
{
	MATERIAL Material;
}

struct LIGHT
{
	bool Enable;
	bool3 Dummy;
	float4 Direction;
	float4 Diffuse;
	float4 Ambient;
    matrix View; //���C�g�J�����̃r���[�s��
    matrix Projection; //���C�g�J�����̃v���W�F�N�V�����s��
};

cbuffer LightBuffer : register(b4)
{
	LIGHT Light;
}

struct PARAMETER
{
    float4 Hp;
    float4 BaseColor;
    float4 LostColor;
    float4 DiffColor;
};

cbuffer ParamBuffer : register(b5)
{
    PARAMETER Param;
}

struct PRATICLE
{
    float4 Color1;
    float4 Color2;
    float4 Color3;
    bool Gradation;
    float3 pos;
    float Size;
    float3 Dummy;
};

cbuffer PraticleBuffer : register(b6)
{
    PRATICLE Praticle;
}

cbuffer CameraBuffer : register(b7)
{
    float4 CameraPosition;
}

struct WATER
{
    float4 Pos;
    float  WaveAmplitude; //�U�� 
    float  WaveFrequency; //���� 
    float  Time; //���� 
    float  Speed; //���x
};

cbuffer WaterBuffer : register(b8)
{
    WATER Water;
}

struct VS_IN
{
	float4 Position		: POSITION0;
	float4 Normal		: NORMAL0;
	float4 Diffuse		: COLOR0;
	float2 TexCoord		: TEXCOORD0;
};


struct PS_IN
{
	float4 Position		 : SV_POSITION;
    float4 WorldPosition : POSITION0;
    float4 Normal        : NORMAL0;
	float4 Diffuse		 : COLOR0;
	float2 TexCoord		 : TEXCOORD0;
    
    float4 ShadowPosition : POSITION1; //���C�g�J�������猩���s�N�Z���̍��W
};
