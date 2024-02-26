#pragma once
#include "component.h"

class Enemy :public Component
{
private:
    class GameObject* m_Player{};
    class BoxCollision* m_Collision{};
    float m_Speed = 0.1f;
    float m_Range = 7.0f;
public:
    void DrawInspector()
    {
        SET_DATE(m_Speed);
        SET_DATE(m_Range);
    }
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
            archive(CEREAL_NVP(m_Speed), CEREAL_NVP(m_Range));
        }
        catch (const std::exception&)
        {

        }
    }
};
