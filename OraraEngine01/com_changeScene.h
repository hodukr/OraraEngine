#pragma once
#include "component.h"
#include "cereal/types/string.hpp"

class ChangeScene :public Component
{
private:
    std::string m_SceneName{""};
public:
    void DrawInspector()
    {
        SET_NEXT_FOLDER("asset\\scene", ".");
        if (SET_DATE_STATE(m_SceneName, CASTOMDRAWSTATE_STRING_FOLDER))
        {
            SetScene(m_SceneName);
        }

    }
    void Init()override;
    void Uninit()override;
    void EditorUpdate()override;
    void Update()override;
    void Draw()override;

    void SetScene(std::string name) { m_SceneName = name; }
    void SceneChange(std::string name);

    template<class Archive>
    void serialize(Archive& archive)
    {
        try
        {
            archive(cereal::make_nvp("SceneName", m_SceneName));
        }
        catch (const std::exception&)
        {

        }

    }
};
