#include "main.h"
#include "renderer.h"
#include "sphereCollisionDraw.h"

void SphereCollisionDraw::Init()
{
    //頂点色が何故かDiffuseで変えられなかったためシェーダーで変える
    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "shader\\boxCollisionVS.cso");

    Renderer::CreatePixelShader(&m_PixelShader,
        "shader\\boxCollisionPS.cso");

    //計算用  
    int num = VERTEX_NUM - 1;

    //球  
    VERTEX_3D vertex[VERTEX_NUM];

    float size = 1.5;

    //XZの円 
    for (int i = 0; i < num; ++i)
    {
        float theta = 2.0f * D3DX_PI * i / num;
        float x = size * cosf(theta);
        float z = size * sinf(theta);

        vertex[i].Position = D3DXVECTOR3(x, 0.0f, z);
    }
    //最後の点から最初の点をつなぐ 
    vertex[num].Position = D3DXVECTOR3(size * cosf(0.0f), 0.0f, size * sinf(0.0f));

    //頂点バッファ生成 
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VERTEX_3D) * VERTEX_NUM;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;


    D3D11_SUBRESOURCE_DATA sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.pSysMem = vertex;

    Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer[0]);

    //ZYの円 
    for (int i = 0; i < num; ++i)
    {
        float theta = 2.0f * D3DX_PI * i / num;
        float y = size * cosf(theta);
        float z = size * sinf(theta);

        vertex[i].Position = D3DXVECTOR3(0.0f, y, z);
    }
    //最後の点から最初の点をつなぐ  
    vertex[num].Position = D3DXVECTOR3(0.0f, size * cosf(0.0f), size * sinf(0.0f));

    ZeroMemory(&sd, sizeof(sd));
    sd.pSysMem = vertex;

    Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer[1]);

    //XYの円 
    for (int i = 0; i < num; ++i)
    {
        float theta = 2.0f * D3DX_PI * i / num;
        float x = size * cosf(theta);
        float y = size * sinf(theta);

        vertex[i].Position = D3DXVECTOR3(x, y, 0.0f);
    }

    //最後の点から最初の点をつなぐ 
    vertex[num].Position = D3DXVECTOR3(size * cosf(0.0f), size * sinf(0.0f), 0.0f);

    ZeroMemory(&sd, sizeof(sd));
    sd.pSysMem = vertex;

    Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer[2]);
}

void SphereCollisionDraw::Uninit()
{
    m_VertexLayout->Release();
    m_VertexShader->Release();
    m_PixelShader->Release();

    for (int i = 0; i < 3; i++)
    {
        m_VertexBuffer[i]->Release();
    }
}
void SphereCollisionDraw::Update()
{

}

void SphereCollisionDraw::Draw()
{
    //入力レイアウト設定
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

    //シェーダー設定
    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

    //マトリクス設定
    D3DXMATRIX world, scale, rot, trans;
    D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
    D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
    D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
    world = scale * rot * trans;
    Renderer::SetWorldMatrix(&world);

    //頂点バッファ設定 
    for (int i = 0; i < 3; i++)
    {
        UINT stride = sizeof(VERTEX_3D);
        UINT offset = 0;
        Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer[i], &stride, &offset);

        //プリミティブトポロジ設定
        Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

        //ポリゴン描画
        Renderer::GetDeviceContext()->Draw(VERTEX_NUM, 0);
    }
}


