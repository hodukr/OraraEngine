#pragma once
#include "component.h"
#include <cereal/types/list.hpp>
#include <cereal/types/string.hpp>
#include <stack>
#include <list>
#include "timeProvider.h"
#include <unordered_map>

enum class COLORVARIATION
{
    None,
    DefaultColor,
    GradationColor
};

struct PARTICLE
{
    Vector3 Position;
    D3DXCOLOR Color;
    float Rotation;
    Vector3 Velocity;
    float TimeToLife;
    ElapsedTimeTracker Timer;
};


struct PARTICLEEMITTER {
    Vector3 StartPosition;//開始位置 
    D3DXCOLOR StartColor;//開始カラー 
    float StartRotation;
    float StartLifetime;//生存時間 
    float Speed;
    float EmissionRate = 0;//発生頻度 
    int MaxParticles = 0;
    Vector3 AddStateVelocity;//初期生成時のベクトル加算  
    Vector3 AddVelocity;//移動方向の補正 
    D3DXCOLOR GradationColor[3];
    bool IsGradation = false;
    float Fluctuation = 0.0f;//揺らぎ
    FolderPass TexName;
    float GradationLength = 2.0f;
    bool IsFade = false;
    float StartFadeTime = 1.0f;
    float DeclineFade = 0.9f;
};
template<class Archive>
void serialize(Archive& archive, D3DXCOLOR& color)
{
    archive(cereal::make_nvp("r", color.r),
    cereal::make_nvp("g", color.g),
    cereal::make_nvp("b", color.b),
    cereal::make_nvp("a", color.a));
}


template<class Archive>
void serialize(Archive& archive, PARTICLEEMITTER& emltter)
{
    archive(cereal::make_nvp("StartColor", emltter.StartColor),
        cereal::make_nvp("StartRotation", emltter.StartRotation),
        cereal::make_nvp("StartLifetime", emltter.StartLifetime),
        cereal::make_nvp("EmissionRate", emltter.EmissionRate),
        cereal::make_nvp("Speed", emltter.Speed),
        cereal::make_nvp("MaxParticles", emltter.MaxParticles),
        cereal::make_nvp("AddStateVelocity", emltter.AddStateVelocity),
        cereal::make_nvp("AddVelocity", emltter.AddVelocity),
        cereal::make_nvp("GradationColor", emltter.GradationColor),
        cereal::make_nvp("IsGradation", emltter.IsGradation),
        cereal::make_nvp("Fluctuation", emltter.Fluctuation),
        cereal::make_nvp("TexName", emltter.TexName.Date),
        cereal::make_nvp("GradationLength", emltter.GradationLength),
        cereal::make_nvp("Fade", emltter.IsFade),
        cereal::make_nvp("StartFadeTime", emltter.StartFadeTime),
        cereal::make_nvp("declineFade", emltter.DeclineFade));
}


class PraticleSystem :public Component
{
private:
    ID3D11Buffer* m_VertexBuffer{};

    ID3D11VertexShader* m_VertexShader{};
    ID3D11PixelShader* m_PixelShader{};
    ID3D11InputLayout* m_VertexLayout{};
    int m_TexNum;
    float m_Scale;
    int m_NumPraticle;
    class Camera* m_Camera;

    PARTICLEEMITTER m_Emitter;
    std::stack<PARTICLE*> m_StackParticle;
    std::list<PARTICLE*> m_ParticleList;
    ElapsedTimeTracker m_Timer;
public:
    PraticleSystem();
    void Init()override;
    void Uninit()override;
    void Update()override;
    void Draw()override;

    PARTICLEEMITTER* GetEmitter() { return &m_Emitter; }
private:
    void InitParticle(PARTICLE* partiale);
    void AddPraticle();
    bool DeletPraticle();
public:
    template<class Archive>
    void serialize(Archive& archive)
    {
            archive(CEREAL_NVP(m_Emitter));
    }
};

