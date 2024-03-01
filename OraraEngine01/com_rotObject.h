#pragma once
#pragma once
#include "component.h"

class RotObject :public Component
{
private:
    float m_Speed = 0.1f;
    Vector3 m_Axis{0.0f,0.0f,0.0f};
public:
    void DrawInspector()
    {
        SET_DATE(m_Speed);
        SET_DATE(m_Axis);
    }

    void Update()override;

    template<class Archive>
    void serialize(Archive& archive)
    {
        try
        {
            archive(CEREAL_NVP(m_Speed), CEREAL_NVP(m_Axis));
        }
        catch (const std::exception&)
        {

        }
    }
};
