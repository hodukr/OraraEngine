#include "main.h"
#include "renderer.h"
#include "com_uiText.h"
#include "com_sprite.h"
#include "shaderManager.h"
#include "pass_depthShadow.h"


void Text::Init()
{
    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "shader\\unlitTextureVS.cso");

    Renderer::CreatePixelShader(&m_PixelShader,
        "shader\\unlitTexturePS.cso");

    //Sprite* sprite = AddComponent<Sprite>();
    //sprite->Init(100.0f, 100.0f, 500.0f, 500.0f, "asset\\texture\\kizuna.jpg");

    //AddComponent<Sprite>()->Init(0.0f, 0.0f, 200.0f, 200.0f, "asset\\texture\\kizuna.jpg");
    VERTEX_3D vertex[4];
    float x = 300.0f;
    float y = 100.0f;
    float width = 600.0f;
    float height = 400.0f;


    vertex[0].Position = D3DXVECTOR3(x, y, 0.0f);
    vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

    vertex[1].Position = D3DXVECTOR3(x + width, y, 0.0f);
    vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

    vertex[2].Position = D3DXVECTOR3(x, y + height, 0.0f);
    vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

    vertex[3].Position = D3DXVECTOR3(x + width, y + height, 0.0f);
    vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

    //頂点バッファ生成
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VERTEX_3D) * 4;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.pSysMem = vertex;

    Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

    //テクスチャ読み込み 
    D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
        "asset\\texture\\kyanpu.png",
        NULL,
        NULL,
        &m_Texture,
        NULL);
    assert(m_Texture);
    //AddComponent<Sprite>()->Init(300.0f,300.0f,200.0f,200.0f, "asset\\texture\\kizuna.jpg");

}

void Text::Uninit()
{
    m_VertexLayout->Release();
    m_VertexShader->Release();
    m_PixelShader->Release();
    m_VertexBuffer->Release();
    m_Texture->Release();
}
void Text::Update()
{
}

void Text::Draw()
{
    //入力レイアウト設定 
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

    //シェーダー設定 
    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

    //マトリクス設定 
    Renderer::SetWorldViewProjection2D();

    //頂点バッファ設定 
    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

    //マテリアル設定 
    MATERIAL material;
    ZeroMemory(&material, sizeof(material));
    material.Diffuse = m_Color;
    material.TextureEnable = true;
    Renderer::SetMaterial(material);

    //テクスチャ設定 
    //Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
    DepthShadow* shadow = ShaderManager::Instance().GetPass<DepthShadow>(SHADER_SHADOW);
    Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, shadow->GetDepthShadowTexture());

    //プリミティブトポロジ設定 
    Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    //ポリゴン描画 
    Renderer::GetDeviceContext()->Draw(4, 0);
}
