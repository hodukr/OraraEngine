#pragma once

#include "gameObject.h"

class BoxCollisionDraw : public GameObject
{
private:
    ID3D11VertexShader* m_VertexShader{};
    ID3D11PixelShader* m_PixelShader{};
    ID3D11InputLayout* m_VertexLayout{};

    ID3D11Buffer* m_VertexBuffer{};
public:

    void Init();
    void Uninit();
    void Update();
    void Draw();
};
