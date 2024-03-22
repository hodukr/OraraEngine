#pragma once
#include "component.h"
#include "cereal/types/string.hpp"

class MoveTarget :public Component
{
private:
    class GameObject* m_Target{};
    string m_TargetName{};
    float m_Speed{};
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
            archive(CEREAL_NVP(m_TargetName), CEREAL_NVP(m_Speed));
        }
        catch (const exception&)
        {

        }
    }
};

