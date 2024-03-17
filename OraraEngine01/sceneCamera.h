#pragma once
#include "component.h"
#include "vector.h"

class EditorCamera
{
private:
    Vector3 m_Position{};
    Vector3 m_Rotation{};
    Vector3 m_Target{};
    D3DXMATRIX m_ViewMatrix{};
    D3DXMATRIX m_ProjectionMatrix{};

    Vector2 m_LeftClickMousePos{};
    Vector3 m_LeftClickCameraRot{};
    bool	m_IsMouseUp{};

public:
    void Init();
    void Uninit();
    void Update();
    void Draw();

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

    Vector3 GetRight()
    {
        D3DXMATRIX rot;
        D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

        Vector3 right;
        right.x = rot._11;
        right.y = rot._12;
        right.z = rot._13;

        return right;
    }

    Vector3 GetUp()
    {
        D3DXMATRIX rot;
        D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
        Vector3 up;
        up.x = rot._21;
        up.y = rot._22;
        up.z = rot._23;

        return up;
    }
    D3DXMATRIX GetViewMatrix() { return m_ViewMatrix; }
    D3DXMATRIX GetProjectionMatrix() { return m_ProjectionMatrix; }
};

