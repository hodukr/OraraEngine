#pragma once
#include "component.h"
#include <string>
#include "cereal/types/string.hpp"
class Texture : public Component
{
private:
    ID3D11Buffer* m_VertexBuffer{};
    int m_TextureNum = -1;
    FolderPass m_TextureFolder;
    D3DXCOLOR m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
public:
    Texture() {
        m_DrawLayer = GAME_OBJECT_DRAW_LAYER_2D;
        m_TextureFolder.Date = "field004.jpg";
        m_TextureFolder.Pass = "asset\\texture";
        
        SETDATE(m_TextureFolder);
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
                cereal::make_nvp("FolderPass", m_TextureFolder.Pass),
                cereal::make_nvp("fileName", m_TextureFolder.Date));
        }
        catch (const std::exception&)
        {

        }

    }
};

