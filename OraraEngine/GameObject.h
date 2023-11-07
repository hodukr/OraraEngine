#pragma once
#include "vector.h"
class GameObject
{
protected:
    int m_ModelIndex = -1;
    Vector3 m_Position;
    Vector3 m_Rotation;
    Vector3 m_Scale;

public:
    void Init();
    void Uninit();
    void Update();
    void Draw();


};
