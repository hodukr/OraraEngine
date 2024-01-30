#pragma once
#include "component.h"
#include "vector.h"
class Camera :public Component
{
private:
    Vector3 m_Target{};
    D3DXMATRIX m_ViewMatrix{};
    D3DXMATRIX m_ProjectionMatrix{};

public:
    Camera();
    void Init()override;
    void Uninit()override;
    void Update()override;
    void Draw()override;

    D3DXMATRIX GetViewMatrix() { return m_ViewMatrix;}

    template<class Archive>
    void serialize(Archive& archive)
    {
    }
};

