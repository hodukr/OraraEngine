#ifndef player_H
#define player_H

#include "component.h"

class Player:public Component
{
private:
    int m_IsGround = 0;
    int m_DeadWave = 0;
    int m_HItTime{};
    float m_JumpPower{};
    bool m_IsDead = false;
    Vector3 m_Velocity{};
    Vector3 m_DeadPosition{};
    Vector3 m_BaseSize{};
    unordered_map<BoxCollision*, bool> m_IsOldUpHit{};
    class BoxCollision* m_Collision{};
    class ChangeScene* m_ChangeScene{};
    class WaterSurface* m_WaterSurface{};
    class ElapsedTimeTracker* m_Timer{};
    class Audio* m_Sound{};

public:
    void DrawInspector()
    {
        SET_DATE(m_Velocity);
        SET_DATE(m_IsGround);
    }
    void Init()override;
    void Uninit()override;
    void EditorUpdate()override;
    void Update()override;
    void Draw()override;

    template<class Archive>
    void serialize(Archive& archive)
    {

        Component::serialize<Archive>(archive);
        try
        {
            //archive(CEREAL_NVP());
        }
        catch (const exception&)
        {

        }
    }
private:
    void Dead();
};

#endif /* player_H */
