#ifndef moveObject_H
#define moveObject_H

#include "component.h"

class MoveObject:public Component
{
private:
    Vector3 m_Vel{};
    Vector3 m_Target{};
    Vector3 m_NewTarget{};
    Vector3 m_StatePos{};
    float m_Speed{1.0f};
    bool m_IsLoop{false};
public:
    void DrawInspector()
    {
        SET_DATE(m_Target);
        SET_DATE(m_Speed);
        SET_DATE(m_IsLoop);
        
    }
    void Init()override;
    void Uninit()override;
    void EditorUpdate()override;
    void Update()override;
    void Draw()override;

    template<class Archive>
    void serialize(Archive & archive)
    {
        Component::serialize<Archive>(archive);
        try
        {
            archive(CEREAL_NVP(m_Target), CEREAL_NVP(m_Speed), CEREAL_NVP(m_IsLoop));
        }
        catch (const std::exception&)
        {

        }
    }
};

#endif /* moveObject_H */
