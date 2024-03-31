#pragma once
#include "component.h"
#include "shaderResource.h"

#define NUM_VERTEX 21
#define SPRING_NUMS				(NUM_VERTEX*(NUM_VERTEX+1) + \
								 NUM_VERTEX*(NUM_VERTEX+1) + \
								 NUM_VERTEX*NUM_VERTEX*2)	// スプリング総数

#define CLOTH_UV (1.0f / ((float)NUM_VERTEX - 1.0f))   

struct SPRING
{
    D3DXVECTOR2	Praticle1;					// 接続されている粒子１ 
    D3DXVECTOR2	Praticle2;					// 接続されている粒子２ 
    float	Length;				    // 自然長 
};

struct PARAMETER;

class Cloth : public Component
{
private:
    ID3D11Buffer* m_VertexBuffer{};
    ID3D11Buffer* m_IndexBuffer{};

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
    bool m_IsWind = false;                                       //風が吹いているか?　
    Vector3 m_WindForce = D3DXVECTOR3(4.0f, 6.0f, 0.0f);         //風力 
    SPRING		m_Spring[SPRING_NUMS]{};                         //頂点間のバネ　

    int m_TexNum{};
    PARAMETER* m_Parameter{};
    class Material* m_Material{};
public:
    Cloth(){}
    void DrawInspector()override
    {
        SET_NEXT_SLIDER(0.0, 10.0f);
        SET_DATE_STATE(m_SpringMass, CASTOMDRAWSTATE_FLOAT_SLIDER);
        SET_NEXT_SLIDER(0.1f, 5.0f);
        SET_DATE_STATE(m_AttCoefficient, CASTOMDRAWSTATE_FLOAT_SLIDER);
        SET_NEXT_SLIDER(0.1f, 50.0f);
        SET_DATE_STATE(m_SpringCoefficient, CASTOMDRAWSTATE_FLOAT_SLIDER);
        SET_NEXT_SLIDER(0.1f, 5.0f);
        SET_DATE_STATE(m_deltaTime, CASTOMDRAWSTATE_FLOAT_SLIDER);
        SET_DATE(m_IsWind);
        SET_DATE(m_WindForce);
    }
    void Init() override;
    void Uninit() override;
    void Update() override;
    void Draw() override;

    void SetTexNum(int num) { m_TexNum = num;}
    void SetIsWind(bool wind) { m_IsWind = wind;}

    //ディゾルブの処理用とりあえずここに記述
    PARAMETER* GetParameter() { return m_Parameter; }

    template<class Archive>
    void serialize(Archive& archive)
    {
        try
        {
            archive(CEREAL_NVP(m_SpringMass),
                CEREAL_NVP(m_AttCoefficient),
                CEREAL_NVP(m_SpringCoefficient),
                CEREAL_NVP(m_deltaTime),
                CEREAL_NVP(m_IsWind),
                CEREAL_NVP(m_WindForce));
        }
        catch (const exception&)
        {

        }
    }

};
