#pragma once
#include "component.h"
#include "vector.h"
class InputSystem :public Component
{
private:
    float  m_speed{0.3f};
public:
    void DrawInspector()
    {
        SET_DATE(m_speed);
    }

    void Init()override;
    void Uninit()override;
    void Update()override;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(CEREAL_NVP(m_speed));
    }
};

