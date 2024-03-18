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

    void DeadPlayer();

    template<class Archive>
    void serialize(Archive & archive)
    {
        try
        {
            //archive(CEREAL_NVP());
        }
        catch (const exception&)
        {

        }
    }
};

#endif /* player_H */
