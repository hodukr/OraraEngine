#pragma once
#include "component.h"

class ComponentPost :public Component
{
private:
    bool m_IsPost;
public:
    ComponentPost(bool flag = false) :m_IsPost(flag) {};

    void Update()override;
    void EditorUpdate()override;

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(CEREAL_NVP(m_IsPost));
    }

    void DrawInspector()override { SET_DATE(m_IsPost);}
};
