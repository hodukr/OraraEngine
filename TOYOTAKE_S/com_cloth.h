#pragma once
#include "component.h"
#include "shaderResource.h"

#define NUM_VERTEX 21
#define SPRING_NUMS				(NUM_VERTEX*(NUM_VERTEX+1) + \
								 NUM_VERTEX*(NUM_VERTEX+1) + \
								 NUM_VERTEX*NUM_VERTEX*2)	// スプリング総数

#define CLOTH_UV (1.0f / ((float)NUM_VERTEX - 1.0f))   

struct INTVECTOR2
{
    int x;
    int y;
};

struct SPRING
{
    INTVECTOR2	Praticle1;					// 接続されている粒子１ 
    INTVECTOR2	Praticle2;					// 接続されている粒子２ 
    float	Length;				    // 自然長 
};

class Cloth : public Component
{
private:
    ID3D11Buffer* m_VertexBuffer{};
    ID3D11Buffer* m_IndexBuffer{};
    ID3D11VertexShader* m_VertexShader{};
    ID3D11PixelShader* m_PixelShader{};
    ID3D11InputLayout* m_VertexLayout{};

    VERTEX_3D				m_Vertex[NUM_VERTEX][NUM_VERTEX]{};

    float m_SpringMass = 5.0f;                                   //ばねの質量　
    float m_AttCoefficient = 2.0f;                               //減衰系数(Attenuation coefficient)  　
    float m_SpringCoefficient = 50.0f;                           //バネ系数  　
    float m_deltaTime = 0.12f;                                   //積分単位時間  　
    D3DXVECTOR3 m_Velocity[NUM_VERTEX][NUM_VERTEX]{};            //速度 
    D3DXVECTOR3 m_Acceleration[NUM_VERTEX][NUM_VERTEX]{};        //加速度   　
    D3DXVECTOR3 m_Resultant[NUM_VERTEX][NUM_VERTEX]{};           //合力 
    D3DXVECTOR3 m_Gravity[NUM_VERTEX][NUM_VERTEX]{};             //重力 
    bool        m_OnLock[NUM_VERTEX][NUM_VERTEX]{};              //ロックされているかいないか 
    bool m_IsWind{};                                             //風が吹いているか?　
    D3DXVECTOR3 m_WindForce{};                                   //風力 
    SPRING		m_Spring[SPRING_NUMS];                           //頂点間のバネ　

    int m_TexNum{};
public:
    void Init() override;
    void Uninit() override;
    void Update() override;
    void Draw() override;

    void SetTexNum(int num) { m_TexNum = num;}
    void SetIsWind(bool wind) { m_IsWind = wind;}

    //template<class Archive>
    //void serialize(Archive& archive)
    //{
       // archive(CEREAL_NVP(m_Modelpas), CEREAL_NVP(m_Material));
    //}

};
