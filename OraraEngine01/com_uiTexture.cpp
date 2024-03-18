#include "main.h"
#include "renderer.h"
#include "com_uiTexture.h"
#include "com_sprite.h"
#include "gameObject.h"
#include "textureManager.h"
#include "shaderManager.h"
#include "pass_depthShadow.h"


void Texture::Init()
{

    VERTEX_3D vertex[4];
    float x = 300.0f;
    float y = 100.0f;
    float width = 600.0f;
    float height = 400.0f;

    Vector3 pos = m_GameObject->m_Transform->GetPosition();
    Vector3 size = m_GameObject->m_Transform->GetScale();

    vertex[0].Position = D3DXVECTOR3(pos.x, pos.y, 0.0f);
    vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

    vertex[1].Position = D3DXVECTOR3(pos.x + size.x, pos.y, 0.0f);
    vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

    vertex[2].Position = D3DXVECTOR3(pos.x, pos.y + size.y, 0.0f);
    vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

    vertex[3].Position = D3DXVECTOR3(pos.x + size.x, pos.y + size.y, 0.0f);
    vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

    //頂点バッファ生成
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(VERTEX_3D) * 4;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    D3D11_SUBRESOURCE_DATA sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.pSysMem = vertex;

    Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
    SetTexture(m_Texturefile);
    //テクスチャ読み込み 
    //AddComponent<Sprite>()->Init(300.0f,300.0f,200.0f,200.0f, "asset\\texture\\kizuna.jpg");

}

void Texture::Uninit()
{
    m_VertexBuffer->Release();
}
void Texture::EditorUpdate()
{

}
void Texture::Update()
{
}

void Texture::Draw()
{
    D3D11_MAPPED_SUBRESOURCE msr;
    Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

    VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;
    Vector3 pos = m_GameObject->m_Transform->GetPosition();
    Vector3 size = m_GameObject->m_Transform->GetScale() * 200.0f;
    vertex[0].Position = D3DXVECTOR3(pos.x, pos.y, 0.0f);
    vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
    vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[0].TexCoord = D3DXVECTOR2(0.0f,0.0f);

    vertex[1].Position = D3DXVECTOR3(pos.x + size.x, pos.y, 0.0f);
    vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
    vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[1].TexCoord = D3DXVECTOR2(1.0f,0.0f);

    vertex[2].Position = D3DXVECTOR3(pos.x, pos.y + size.y, 0.0f);
    vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
    vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[2].TexCoord = D3DXVECTOR2(0.0f,1.0f);

    vertex[3].Position = D3DXVECTOR3(pos.x + size.x, pos.y + size.y, 0.0f);
    vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
    vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[3].TexCoord = D3DXVECTOR2(1.0f,1.0f);

    Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

    //マトリクス設定 
    Renderer::SetWorldViewProjection2D();

    //頂点バッファ設定 
    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

    //テクスチャ設定 
    Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, TextureManager::GetTexture(m_TextureNum));

    //プリミティブトポロジ設定 
    Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    //ポリゴン描画 
    Renderer::GetDeviceContext()->Draw(4, 0);
}

void Texture::SetTexture(string file)
{
    m_Texturefile = file;
    string pass = "asset\\texture\\" + m_Texturefile;
    m_TextureNum = TextureManager::LoadTexture(pass.c_str());

}
