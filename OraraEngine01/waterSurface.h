#pragma once
#include "component.h"
#include "shaderResource.h"

#define NUM_VERTEX 21
#define WATER_UV (1.0f / ((float)NUM_VERTEX - 1.0f))

class WaterSurface : public Component
{

private:
    ID3D11Buffer* m_VertexBuffer{};
    ID3D11Buffer* m_IndexBuffer{};
    int m_TexNum{};
    ID3D11VertexShader* m_VertexShader{};
    ID3D11PixelShader* m_PixelShader{};
    ID3D11InputLayout* m_VertexLayout{};

    VERTEX_3D					m_Vertex[NUM_VERTEX][NUM_VERTEX]{};

    float m_WavePitch{};   //波の幅　
    float m_Amplitude{};   //波の振幅  
    float m_WaveLength{};  //波長 
    float m_WaveCycle{};   //波の間隔 

    float m_Time{};
    float m_WaveTime{};

public:
    void Init() override;
    void Uninit() override;
    void Update() override;
    void Draw() override;

    //float GetHeight(D3DXVECTOR3 position);
};
