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

    VERTEX_3D m_Vertex[NUM_VERTEX][NUM_VERTEX]{};

    float m_WavePitch{5.0f};   //波の幅　
    float m_Amplitude{0.8f};   //波の振幅  
    float m_WaveLength{0.165f};  //波長 
    float m_WaveCycle{1.17f};   //波の間隔 

    float m_Time{};
    float m_WaveTime{};
public:
    WaterSurface(){ }
    void DrawInspector()
    {
        SET_NEXT_SLIDER(0.1f, 20.0f);
        SET_DATE_STATE(m_WavePitch, CASTOMDRAWSTATE_FLOAT_SLIDER);
        SET_NEXT_SLIDER(0.1f, 10.0f);
        SET_DATE_STATE(m_Amplitude, CASTOMDRAWSTATE_FLOAT_SLIDER);
        SET_NEXT_SLIDER(0.1f, 30.0f);
        SET_DATE_STATE(m_WaveLength, CASTOMDRAWSTATE_FLOAT_SLIDER);
        SET_NEXT_SLIDER(0.1f, 20.0f);
        SET_DATE_STATE(m_WaveCycle, CASTOMDRAWSTATE_FLOAT_SLIDER);
    }

    void Init() override;
    void Uninit() override;
    void Update() override;
    void Draw() override;

    template<class Archive>
        void serialize(Archive & archive)
    {
        try
        {
            archive(CEREAL_NVP(m_WavePitch),
                CEREAL_NVP(m_Amplitude),
                CEREAL_NVP(m_WaveLength),
                CEREAL_NVP(m_WaveCycle));
        }
        catch (const std::exception&)
        {

        }
    }
};
