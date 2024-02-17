#pragma once
#include "component.h"
#include "vector.h"
class SceneCamera :public Component
{
private:
    Vector3 m_Position{};
    Vector3 m_Rotation{};
    Vector3 m_Target{};
    D3DXMATRIX m_ViewMatrix{};
    D3DXMATRIX m_ProjectionMatrix{};

    Vector2 m_LeftClickMousePos;
    Vector3 m_LeftClickCameraRot;
    bool	m_IsMouseUp;

public:
    void Init()override;
    void Uninit()override;
    void Update()override;
    void Draw()override;

    Vector3 GetForward()
    {
        D3DXMATRIX rot;
        D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
        Vector3 forward;
        forward.x = rot._31;
        forward.y = rot._32;
        forward.z = rot._33;

        return forward;
    }

    D3DXMATRIX GetViewMatrix() { return m_ViewMatrix; }
    D3DXMATRIX GetProjectionMatrix() { return m_ProjectionMatrix; }
    Vector3 GetWorldMousePosition(float x, float y);
};

