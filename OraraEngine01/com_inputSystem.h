#pragma once
#include "component.h"
#include "vector.h"
class InputSystem :public Component
{
private:
    float  m_Speed{0.3f};
    bool m_IsDirection = true;
public:
    void DrawInspector()
    {
        SET_DATE(m_Speed);
        SET_DATE(m_IsDirection);
    }

    void Init()override;
    void Uninit()override;
    void Update()override;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(CEREAL_NVP(m_Speed), CEREAL_NVP(m_IsDirection));
    }
};

