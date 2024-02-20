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

    VERTEX_3D					m_Vertex[NUM_VERTEX][NUM_VERTEX]{};

    float m_WavePitch{};   //波の幅　
    float m_Amplitude{};   //波の振幅  
    float m_WaveLength{};  //波長 
    float m_WaveCycle{};   //波の間隔 

    float m_Time{};
    float m_WaveTime{};
    CustomFloat m_WavePitchDate;
    CustomFloat m_AmplitudeDate;
    CustomFloat m_WaveLengthDate;
    CustomFloat m_WaveCycleDate;
public:
    WaterSurface()
    {
        m_WavePitchDate.Date = &m_WavePitch;
        m_WavePitchDate.Max = 10.0f;

        m_AmplitudeDate.Date = &m_Amplitude;
        m_AmplitudeDate.Max = 20.0f;

        m_WaveLengthDate.Date = &m_WaveLength;
        m_WaveLengthDate.Max = 30.0f;

        m_WaveCycleDate.Date = &m_WaveCycle;
        m_WaveCycleDate.Max = 20.0f;

        SetDateList("WavePitch", &m_WavePitchDate);
        SetDateList("Amplitude", &m_AmplitudeDate);
        SetDateList("WaveLength", &m_WaveLengthDate);
        SetDateList("WaveCycle", &m_WaveCycleDate);
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
