#pragma once
#include "component.h"
class Camera :public Component
{
private:
    Vector3 m_Target{};
    Vector3 m_Offset{};
    D3DXMATRIX m_ViewMatrix{};
    D3DXMATRIX m_ProjectionMatrix{};
    std::string m_TargetObjectName;
    class GameObject* m_TargetObject = nullptr;
public:
    Camera();
    void DrawInspector()override;
    
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

