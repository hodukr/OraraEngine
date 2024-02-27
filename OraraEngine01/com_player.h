#ifndef player_H
#define player_H

#include "component.h"

class Player:public Component
{
private:
    float m_JumpPower{};
    Vector3 m_Velocity{};
    bool m_IsGround = false;
    class BoxCollision* m_Collision{};
    class WaterSurface* m_WaterSurface{};
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
    void serialize(Archive & archive)
    {
        try
        {
            //archive(CEREAL_NVP());
        }
        catch (const std::exception&)
        {

        }
    }
};

#endif /* player_H */
