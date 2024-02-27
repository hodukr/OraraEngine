#ifndef moveCamera_H
#define moveCamera_H

#include "component.h"
#include "cereal/types/string.hpp"

class MoveCamera:public Component
{
private:
    class GameObject* m_Target = nullptr;
    std::string m_TargetName{""};
    Vector3 m_Offset{};
public:
    void DrawInspector()
    {
        if (SET_DATE_STATE(m_TargetName, CASTOMDRAWSTATE_STRING_GAMEOBJECT))
        {
            SetTarget();
        }
        SET_DATE(m_Offset);
    }
    void Init()override;
    void Uninit()override;
    void EditorUpdate()override;
    void Update()override;
    void Draw()override;

    void SetTarget();
    template<class Archive>
    void serialize(Archive & archive)
    {
        archive(CEREAL_NVP(m_TargetName), CEREAL_NVP(m_Offset));
    }
};

#endif /* moveCamera_H */
