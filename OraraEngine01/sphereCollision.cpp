#include "main.h"
#include "renderer.h"
#include "sphereCollision.h"
#include "boxCollision.h"
#include "input.h"

void SphereCollision::Init()
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

void SphereCollision::Uninit()
{
    m_VertexLayout->Release();
    m_VertexShader->Release();
    m_PixelShader->Release();

    for (int i = 0; i < 3; i++)
    {
        m_VertexBuffer[i]->Release();
    }
}
void SphereCollision::Update()
{
    m_Position = m_Object->m_Transform->GetPosition();
    m_Position += m_Offset;
}

void SphereCollision::Draw()
{
    //入力レイアウト設定
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);


    //シェーダー設定
    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

    //ポジションのみマトリクスに設定 
    D3DXMATRIX trans;
    D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
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
void SphereCollision::SetVertex(VERTEX_3D* vertex,int index)
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

bool SphereCollision::CollideWith(BoxCollision* other)
{
    // Sphere と Box の当たり判定ロジック
    float distanceSquared = 0.0f;

    // X軸方向の距離
    float dx = std::max(0.0f, std::abs(m_Position.x - other->GetPosition().x) - other->GetSize().x);
    distanceSquared += dx * dx;

    // Y軸方向の距離
    float dy = std::max(0.0f, std::abs(m_Position.y - other->GetPosition().y) - other->GetSize().y);
    distanceSquared += dy * dy;

    // Z軸方向の距離
    float dz = std::max(0.0f, std::abs(m_Position.z - other->GetPosition().z) - other->GetSize().z);
    distanceSquared += dz * dz;

    // Sphereの半径との比較
    if (distanceSquared < m_Size * m_Size)
    {
        if (m_Trigger || other->GetTrigger())
            return true;

        // 重なっている場合の補正
        float distance = std::sqrt(distanceSquared);
        Vector3 normal = (m_Position - other->GetPosition()) / distance;  // 最短距離ベクトルの正規化

        // 補正ベクトルの計算（ボックスの頂点から球への最短距離ベクトル）
        Vector3 vec = normal * (m_Size - distance);

        // ポジションの補正
        Vector3 pos = m_Position - m_Offset + vec;
        m_Object->m_Transform->SetPosition(pos);
        m_Position = m_Object->m_Transform->GetPosition() + m_Offset;
        return true;
    }

    return false;
}

bool SphereCollision::CollideWith(SphereCollision* other)
{
    D3DXVECTOR3 vec = m_Position.dx() - other->GetPosition().dx();
    //Sphere と Sphere の当たり判定ロジック
    float distanceSquared = D3DXVec3LengthSq(&vec);

    // 2つの球が重なっているかどうかを判定
    if (distanceSquared < (m_Size + other->GetSize()) * (m_Size + other->GetSize()))
    {
        if (m_Trigger || other->GetTrigger())
            return true;

        float distance = std::sqrt(distanceSquared);

        // 重なりの量を計算
        float overlapDistance = distance - (m_Size + other->GetSize());

        // 重なりの方向を計算（正規化されたベクトル）
        Vector3 overlapDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        if (distanceSquared > 0.0f)
        {
            overlapDirection = vec / distance;
        }

        // 補正を適用して位置を調整
        Vector3 pos = (m_Position - m_Offset) - overlapDirection * overlapDistance * 0.5f;
        m_Object->m_Transform->SetPosition(pos);
        m_Position = m_Object->m_Transform->GetPosition() + m_Offset;

        return true;
    }
    return false;
}
