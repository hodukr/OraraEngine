#pragma once
#include "component.h"

class Goal :public Component
{
private:
    bool  m_IsGoal = false;
    bool  m_IsUp = false;
    bool  m_IsDissolve = false;
    class GameObject* m_HitObj{};
    class Cloth* m_Cloth{};
    class BoxCollision* m_Collision{};
    class ChangeScene* m_ChangeScene{};
    class Audio* m_Audio{};
    string m_HitObjTag{""};
    string m_ClothName{""};
    float m_ThresholdSpeed{};
    Vector3 m_StartPosition{};
public:
    void DrawInspector();
    void Init()override;
    void Update()override;

    template<class Archive>
    void serialize(Archive& archive)
    {
        try
        {
            archive(CEREAL_NVP(m_HitObjTag), CEREAL_NVP(m_ClothName),CEREAL_NVP(m_ThresholdSpeed));
        }
        catch (const exception&)
        {

        }
    }
};