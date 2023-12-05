#pragma once
#include "component.h"
#include "vector.h"
class Cameracm :public Component
{
private:
    Vector3 m_Target{};
    D3DXMATRIX m_ViewMatrix{};

public:
    void Init()override;
    void Uninit()override;
    void Update()override;
    void Draw()override;

    template<class Archive>
    void serialize(Archive& archive)
    {
    }
};
CEREAL_REGISTER_TYPE(Cameracm)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Cameracm)
