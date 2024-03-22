#ifndef moveObject_H
#define moveObject_H

#include "component.h"

class MoveObject:public Component
{
private:
    Vector3 m_Vel{};
    Vector3 m_Target{};
public:
    void DrawInspector()
    {
        SET_DATE(m_Target);
    }
    void Init()override;
    void Uninit()override;
    void EditorUpdate()override;
    void Update()override;
    void Draw()override;

    template<class Archive>
    void serialize(Archive & archive)
    {
        //archive(CEREAL_NVP());
    }
};

#endif /* moveObject_H */
