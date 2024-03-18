#pragma once
#include "component.h"
class Camera :public Component
{
private:
    Vector3 m_Target{};
    Vector3 m_Offset{};
    D3DXMATRIX m_ViewMatrix{};
    D3DXMATRIX m_ProjectionMatrix{};

public:
    Camera();
    void DrawInspector()override
    {
        SET_DATE(m_Target);
    }
    void Init()override;
    void Uninit()override;
    void Update()override;
    void Draw()override;

    void SetOffset(Vector3 vec) { m_Offset = vec; }
    D3DXMATRIX GetViewMatrix() { return m_ViewMatrix;}

    template<class Archive>
    void serialize(Archive& archive)
    {
    }
};

