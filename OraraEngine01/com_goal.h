#pragma once
#include "component.h"

class Goal :public Component
{
private:
    bool  m_IsGoal = false;
    class BoxCollision* m_Collision{};
    class ChangeScene* m_ChangeScene{};
    std::string m_HitObjName{""};
public:
    void DrawInspector()
    {
        SET_DATE_STATE(m_HitObjName, CASTOMDRAWSTATE_STRING_GAMEOBJECT);
    }
    void Init()override;
    void Update()override;

    template<class Archive>
    void serialize(Archive& archive)
    {
        try
        {
            archive(CEREAL_NVP(m_HitObjName));
        }
        catch (const std::exception&)
        {

        }
    }
};