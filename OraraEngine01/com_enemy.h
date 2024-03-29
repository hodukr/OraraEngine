#pragma once
#include "component.h"
#include "cereal/types/string.hpp"

class Enemy :public Component
{
private:
    class GameObject* m_Target{};
    class BoxCollision* m_Collision{};
    class Goal* m_Goal;
    float m_Speed = 0.1f;
    float m_Range = 7.0f;
    string m_TargetName;
    class Mesh* m_Mesh = nullptr;
    int m_time = 0;
    int m_ModelState = 0;
public:
    void DrawInspector();
   
    void Init()override;
    void Uninit()override;
    void EditorUpdate()override;
    void Update()override;
    void Draw()override;

    template<class Archive>
    void serialize(Archive& archive)
    {
        try
        {
            archive(CEREAL_NVP(m_Speed), CEREAL_NVP(m_Range), CEREAL_NVP(m_TargetName));
        }
        catch (const exception&)
        {

        }
    }
};
