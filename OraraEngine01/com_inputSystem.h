#pragma once
#include "component.h"
#include "vector.h"
class InputSystem :public Component
{
private:
    float  m_Speed{0.3f};
    bool m_IsDirection = true;
    float m_angle{};
    Vector3 m_Vel{};
    float m_Acceleration{0.1f};//加速度
    float m_MaxSpeed{1.0f};
public:
    void DrawInspector()
    {
        SET_DATE(m_Speed);
        SET_DATE(m_IsDirection);
        SET_DATE(m_Acceleration);
        SET_DATE(m_MaxSpeed);
        SET_DATE(m_Vel);
    }

    void Init()override;
    void Uninit()override;
    void Update()override;
    template<class Archive>
    void serialize(Archive& archive)
    {
        Component::serialize<Archive>(archive);
        try
        {
            archive(CEREAL_NVP(m_Speed), CEREAL_NVP(m_IsDirection));
            archive(CEREAL_NVP(m_Acceleration), CEREAL_NVP(m_MaxSpeed));

        }
        catch (const std::exception&)
        {

        }
    }
};

