#pragma once

#include "gameObject.h"

class BoxCollisionDraw : public GameObject
{
private:
    ID3D11VertexShader* m_VertexShader{};
    ID3D11PixelShader* m_PixelShader{};
    ID3D11InputLayout* m_VertexLayout{};

    ID3D11Buffer* m_VertexBuffer{};

    D3DXVECTOR3 m_Size{};

    void SetVertex(VERTEX_3D* vertex);
public:
    void Init();
    void Uninit();
    void Update();
    void Draw();

    void SetSize(D3DXVECTOR3 size) { m_Size = size; }
};
