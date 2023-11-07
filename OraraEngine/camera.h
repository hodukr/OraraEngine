#pragma once
#include "GameObject.h"

class Camera :public GameObject
{
private:

    D3DXVECTOR3 m_Target{};
    D3DXVECTOR3 m_Position{};
    D3DXMATRIX m_ViewMatrix{};
public:
    void Init();
    void Update();
    void Draw();

    D3DXMATRIX GetViewMatrix() { return m_ViewMatrix; }

};
