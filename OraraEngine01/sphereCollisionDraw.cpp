#include "main.h"
#include "renderer.h"
#include "sphereCollisionDraw.h"

#include "input.h"

void SphereCollisionDraw::Init()
{
    m_Size = 1.5f;

    VERTEX_3D vertex[VERTEX_NUM];

    SetVertex(vertex, 0);

    //頂点バッファ生成 
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(VERTEX_3D) * VERTEX_NUM;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


    D3D11_SUBRESOURCE_DATA sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.pSysMem = vertex;

    Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer[0]);

    SetVertex(vertex, 1);

    ZeroMemory(&sd, sizeof(sd));
    sd.pSysMem = vertex;

    Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer[1]);

    SetVertex(vertex, 2);

    ZeroMemory(&sd, sizeof(sd));
    sd.pSysMem = vertex;

    Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer[2]);

    //頂点色が何故かDiffuseで変えられなかったためシェーダーで変える
    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "shader\\boxCollisionVS.cso");

    Renderer::CreatePixelShader(&m_PixelShader,
        "shader\\boxCollisionPS.cso");
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
    if (Input::GetKeyPress(VK_UP))
    {
        m_Size += 0.1f;
        m_Transform->Translate(Vector3::Up() * 0.1f);
    }

    if (Input::GetKeyPress(VK_LEFT))
    {
        m_Transform->Translate(-Vector3::Right() * 0.1f);
    }
}

void SphereCollisionDraw::Draw()
{
    //入力レイアウト設定
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);


    //シェーダー設定
    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

    //ポジションのみマトリクスに設定
    D3DXMATRIX trans;
    D3DXVECTOR3 Position = m_Transform->GetRotation().dx();
    D3DXMatrixTranslation(&trans, Position.x, Position.y, Position.z);
    Renderer::SetWorldMatrix(&trans);

    
    //頂点バッファ設定 
    for (int i = 0; i < 3; i++)
    {
        //頂点データ書き換え
        D3D11_MAPPED_SUBRESOURCE msr;
        Renderer::GetDeviceContext()->Map(m_VertexBuffer[i], 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

        VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

        SetVertex(vertex, i);

        Renderer::GetDeviceContext()->Unmap(m_VertexBuffer[i], 0);

        UINT stride = sizeof(VERTEX_3D);
        UINT offset = 0;
        Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer[i], &stride, &offset);

        //プリミティブトポロジ設定
        Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

        //ポリゴン描画
        Renderer::GetDeviceContext()->Draw(VERTEX_NUM, 0);
    }
}

//円の形に頂点をセットする
void SphereCollisionDraw::SetVertex(VERTEX_3D* vertex,int index)
{
    int num = VERTEX_NUM - 1;

    switch (index)
    {
    case 0:
        //XZの円 
        for (int i = 0; i < num; ++i)
        {
            float theta = 2.0f * D3DX_PI * i / num;
            float x = m_Size * cosf(theta);
            float z = m_Size * sinf(theta);

            vertex[i].Position = D3DXVECTOR3(x, 0.0f, z);
        }
        //最後の点から最初の点をつなぐ 
        vertex[num].Position = D3DXVECTOR3(m_Size * cosf(0.0f), 0.0f, m_Size * sinf(0.0f));
        break;
    case 1:
        //ZYの円 
        for (int i = 0; i < num; ++i)
        {
            float theta = 2.0f * D3DX_PI * i / num;
            float y = m_Size * cosf(theta);
            float z = m_Size * sinf(theta);

            vertex[i].Position = D3DXVECTOR3(0.0f, y, z);
        }
        //最後の点から最初の点をつなぐ  
        vertex[num].Position = D3DXVECTOR3(0.0f, m_Size * cosf(0.0f), m_Size * sinf(0.0f));
        break;
    case 2:
        //XYの円 
        for (int i = 0; i < num; ++i)
        {
            float theta = 2.0f * D3DX_PI * i / num;
            float x = m_Size * cosf(theta);
            float y = m_Size * sinf(theta);

            vertex[i].Position = D3DXVECTOR3(x, y, 0.0f);
        }
        //最後の点から最初の点をつなぐ 
        vertex[num].Position = D3DXVECTOR3(m_Size * cosf(0.0f), m_Size * sinf(0.0f), 0.0f);
        break;
    default:
        break;
    }
}
