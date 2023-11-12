#pragma once

#include "gameObject.h"



class SphereCollisionDraw : public GameObject
{
private:
    #define VERTEX_NUM  31

    ID3D11VertexShader* m_VertexShader{};
    ID3D11PixelShader* m_PixelShader{};
    ID3D11InputLayout* m_VertexLayout{};

    ID3D11Buffer* m_VertexBuffer[3]{};
public:

    void Init();
    void Uninit();
    void Update();
    void Draw();
};
