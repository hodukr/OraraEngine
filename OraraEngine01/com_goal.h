#pragma once
#include "component.h"

class Goal :public Component
{
private:
    bool  m_IsGoal = false;
    class InputSystem* m_HitObj{};
    class Cloth* m_Cloth{};
    class BoxCollision* m_Collision{};
    class ChangeScene* m_ChangeScene{};
    std::string m_HitObjName{""};
    std::string m_ClothName{""};
public:
    void DrawInspector();
    void Init()override;
    void Update()override;

    template<class Archive>
    void serialize(Archive& archive)
    {
        try
        {
            archive(CEREAL_NVP(m_HitObjName), CEREAL_NVP(m_ClothName));
        }
        catch (const std::exception&)
        {

        }
    }
};