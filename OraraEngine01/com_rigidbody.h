#ifndef rigidbody_H
#define rigidbody_H

#include "component.h"

class Rigidbody:public Component
{
private:
    float m_Gravity{};
    Vector3 m_Velocity{};
    class BoxCollision* m_Collision{};
public:
    void DrawInspector()
    {
        SET_DATE(m_Gravity);
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
            archive(CEREAL_NVP(m_Gravity));

        }
        catch (const std::exception&)
        {

        }
    }
};

#endif /* rigidbody_H */
