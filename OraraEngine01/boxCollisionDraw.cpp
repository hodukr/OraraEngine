#include "main.h"
#include "renderer.h"
#include "boxCollisionDraw.h"

#include "input.h"



void BoxCollisionDraw::Init()
{
    m_Size = D3DXVECTOR3(1.5,1.5f,1.5f);
   
    //立方体
    VERTEX_3D vertex[16];

    SetVertex(vertex);

    //頂点バッファ生成
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(VERTEX_3D) * 16;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    D3D11_SUBRESOURCE_DATA sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.pSysMem = vertex;

    Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

    //頂点色が何故かDiffuseで変えられなかったためシェーダーで変える
    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "shader\\boxCollisionVS.cso");

    Renderer::CreatePixelShader(&m_PixelShader,
        "shader\\boxCollisionPS.cso");
}

void BoxCollisionDraw::Uninit()
{
    m_VertexLayout->Release();
    m_VertexShader->Release();
    m_PixelShader->Release();

    m_VertexBuffer->Release();

}
void BoxCollisionDraw::Update()
{
    if (Input::GetKeyPress(VK_UP))
    {
        m_Size.y += 0.1f;
       // m_Position.y += 0.1f;
    }

    if (Input::GetKeyPress(VK_RIGHT))
    {
        m_Size.x += 0.1f;
       // m_Position.x += 0.1f;
    }

    if (Input::GetKeyPress(VK_LEFT))
    {
        m_Size.z += 0.1f;
        //m_Position.z -= 0.1f;
    }
}

void BoxCollisionDraw::Draw()
{
    //頂点データ書き換え
    D3D11_MAPPED_SUBRESOURCE msr;
    Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

    VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

    SetVertex(vertex);

    Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

    //入力レイアウト設定
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

    //シェーダー設定
    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

    //マトリクス設定
    D3DXMATRIX world, scale, rot, trans;
    D3DXVECTOR3 Scale = m_Transform->GetPosition().dx();
    D3DXVECTOR3 Rotation = m_Transform->GetRotation().dx();
    D3DXVECTOR3 Position = m_Transform->GetRotation().dx();

    D3DXMatrixScaling(&scale, Scale.x, Scale.y, Scale.z);
    D3DXMatrixRotationYawPitchRoll(&rot, Rotation.y, Rotation.x, Rotation.z);
    D3DXMatrixTranslation(&trans, Position.x, Position.y, Position.z);
    world = scale * rot * trans;
    world = rot * trans;
    Renderer::SetWorldMatrix(&world);

    //頂点バッファ設定
    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

    //プリミティブトポロジ設定
    Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

    //ポリゴン描画
    Renderer::GetDeviceContext()->Draw(16, 0);
}

void BoxCollisionDraw::SetVertex(VERTEX_3D* vertex)
{
    //底面
    vertex[0].Position = D3DXVECTOR3(-m_Size.x, -m_Size.y, -m_Size.z);
    vertex[1].Position = D3DXVECTOR3(-m_Size.x, -m_Size.y, m_Size.z);
    vertex[2].Position = D3DXVECTOR3(m_Size.x, -m_Size.y, m_Size.z);
    vertex[3].Position = D3DXVECTOR3(m_Size.x, -m_Size.y, -m_Size.z);
    vertex[4].Position = D3DXVECTOR3(-m_Size.x, -m_Size.y, -m_Size.z);

    // 上面
    vertex[5].Position = D3DXVECTOR3(-m_Size.x, m_Size.y, -m_Size.z);
    vertex[6].Position = D3DXVECTOR3(-m_Size.x, m_Size.y, m_Size.z);
    vertex[7].Position = D3DXVECTOR3(m_Size.x, m_Size.y, m_Size.z);
    vertex[8].Position = D3DXVECTOR3(m_Size.x, m_Size.y, -m_Size.z);
    vertex[9].Position = D3DXVECTOR3(-m_Size.x, m_Size.y, -m_Size.z);

    //左面
    vertex[10].Position = D3DXVECTOR3(-m_Size.x, m_Size.y, m_Size.z);
    vertex[11].Position = D3DXVECTOR3(-m_Size.x, -m_Size.y, m_Size.z);

    //背面
    vertex[12].Position = D3DXVECTOR3(m_Size.x, -m_Size.y, m_Size.z);
    vertex[13].Position = D3DXVECTOR3(m_Size.x, m_Size.y, m_Size.z);

    //右面
    vertex[14].Position = D3DXVECTOR3(m_Size.x, m_Size.y, -m_Size.z);
    vertex[15].Position = D3DXVECTOR3(m_Size.x, -m_Size.y, -m_Size.z);
}
