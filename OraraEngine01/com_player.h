#ifndef player_H
#define player_H

#include "component.h"

class Player:public Component
{
private:
    float m_JumpPower{};
    Vector3 m_Velocity{};
    int m_IsGround = 0;
    class BoxCollision* m_Collision{};
    unordered_map<BoxCollision*, bool> m_IsOldUpHit{};
    class ChangeScene* m_ChangeScene{};
    bool m_IsDead = false;
    int m_DeadWave = 0;
    Vector3 m_DeadPosition{};
    int m_HItTime{};
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
