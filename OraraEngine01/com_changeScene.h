#pragma once
#include "component.h"
#include "cereal/types/string.hpp"

class ChangeScene :public Component
{
private:
    bool m_UseButton = false;
    bool m_IsFadeOut = false;
    bool m_IsFadeIn = false;
    bool m_StartFadeIn = false;
    std::string m_SceneName{""};
public:
    void DrawInspector()
    {
        SET_DATE(m_UseButton);
        SET_NEXT_FOLDER("asset\\scene", ".");
        if (SET_DATE_STATE(m_SceneName, CASTOMDRAWSTATE_STRING_FOLDER))
        {
            SetScene(m_SceneName);
        }

    }
    void Init()override;
    void Update()override;

    void SetIsFadeOut(bool flag) { m_IsFadeOut = flag; }
    void SetScene(std::string name) { m_SceneName = name; }
    void SceneChange(std::string name);

    template<class Archive>
    void serialize(Archive& archive)
    {
        try
        {
            archive(cereal::make_nvp("SceneName", m_SceneName), CEREAL_NVP(m_UseButton));
        }
        catch (const std::exception&)
        {

        }

    }
};
