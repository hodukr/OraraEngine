#pragma once
#include "component.h"
#include <string>
#include "cereal/types/string.hpp"
class Texture : public Component
{
private:
    ID3D11Buffer* m_VertexBuffer{};
    int m_TextureNum = -1;
    D3DXCOLOR m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    std::string m_Texturefile;
public:
    Texture() {
        m_DrawLayer = GAME_OBJECT_DRAW_LAYER_2D;
        m_Texturefile = "field004.jpg";
    }

    void DrawInspector()
    {
        SET_NEXT_FOLDER("asset\\texture", ".");
        if (SET_DATE_STATE(m_Texturefile, CASTOMDRAWSTATE_STRING_FOLDER))
        {
            SetTexture();
        }
    }
    void Init();
    void Uninit();
    void EditorUpdate();
    void Update();
    void Draw();

    void SetTexture();

    template<class Archive>
    void serialize(Archive& archive)
    {
        try
        {
            archive(
                cereal::make_nvp("fileName", m_Texturefile));
        }
        catch (const std::exception&)
        {

        }

    }
};

