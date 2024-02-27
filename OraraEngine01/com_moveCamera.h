#ifndef moveCamera_H
#define moveCamera_H

#include "component.h"

class MoveCamera:public Component
{
private:
    class GameObject* m_Target = nullptr;
    std::string TargetName{""};
public:
    void DrawInspector()
    {
        SET_DATE_STATE(TargetName, CASTOMDRAWSTATE_STRING_GAMEOBJECT);
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

#endif /* moveCamera_H */
