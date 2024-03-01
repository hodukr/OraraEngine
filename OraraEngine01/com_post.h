#pragma once
#include "component.h"

class ComponentPost :public Component
{
private:
    bool m_IsPost;
    bool m_Debug;
public:
    ComponentPost(bool post = false,bool debug = false) :m_IsPost(post),m_Debug(debug) {};

    void Update()override;
    void EditorUpdate()override;

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(CEREAL_NVP(m_IsPost), CEREAL_NVP(m_Debug));
    }

    void DrawInspector()override 
    { 
        SET_DATE(m_IsPost);
        SET_DATE(m_Debug);
    }
};
