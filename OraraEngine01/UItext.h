#pragma once
#include "component.h"

class Text : public Component
{
private:
    ID3D11VertexShader* m_VertexShader{};
    ID3D11PixelShader* m_PixelShader{};
    ID3D11InputLayout* m_VertexLayout{};
    ID3D11Buffer* m_VertexBuffer{};
    ID3D11ShaderResourceView* m_Texture{};

    D3DXCOLOR m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
public:
    void Init();
    void Uninit();
    void Update();
    void Draw();
};

