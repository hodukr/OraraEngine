#pragma once
#include "component.h"

class Mesh :public Component
{
private:
    int m_ModelNum{};
    std::string m_Modelpas;
    bool m_IsSet = false;
public:
    Mesh(std::string pas = "box.obj") :m_Modelpas(pas)
    {
        m_DrawLayer = GAME_OBJECT_DRAW_LAYER_3D;
    }
    void DrawInspector()override
    {
        SET_NEXT_FOLDER("asset\\model", ".obj");
        if (SET_DATE_STATE(m_Modelpas, CASTOMDRAWSTATE_STRING_FOLDER))
        {
            SetModel(m_Modelpas);
        }

    }
    void SetModel(std::string pas);
    
    void Init() override;
    void Uninit() override;
    void Update() override;
    void Draw()override;

    const std::string GetModelpas() { return m_Modelpas; }
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(CEREAL_NVP(m_Modelpas));
    }

};


