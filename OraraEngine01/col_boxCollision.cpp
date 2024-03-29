﻿#include "main.h"
#include "renderer.h"
#include "col_boxCollision.h"
#include "col_sphereCollision.h"
#include "gameObject.h"
#include "input.h"
#include "imgui/imgui.h"
#include "collisionManager.h"



void BoxCollision::Init()
{
    CollisionManager::SetShape(this);

    //m_Size = D3DXVECTOR3(1.5,1.5f,1.5f);
   
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

void BoxCollision::Uninit()
{
    CollisionManager::DeletShape(this);

    m_VertexLayout->Release();
    m_VertexShader->Release();
    m_PixelShader->Release();

    m_VertexBuffer->Release();

}
void BoxCollision::EditorUpdate()
{
    m_Position = m_GameObject->m_Transform->GetPosition();
    m_Position += m_Offset;


    m_Size = m_GameObject->m_Transform->GetScale();
    m_Size *= m_Scale;
}
void BoxCollision::Update()
{
    m_Position = m_GameObject->m_Transform->GetPosition();
    m_Position += m_Offset;

    m_Size = m_GameObject->m_Transform->GetScale();
    m_Size *= m_Scale;
}

void BoxCollision::EditorDraw()
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

    //ポジションのみマトリクスに設定  
    D3DXMATRIX  trans;
    D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
    Renderer::SetWorldMatrix(&trans);

    //頂点バッファ設定 
    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

    //プリミティブトポロジ設定 
    Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

    //ポリゴン描画 
    Renderer::GetDeviceContext()->Draw(16, 0);
}

void BoxCollision::Draw()
{
   
}


void BoxCollision::SetVertex(VERTEX_3D* vertex)
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

bool BoxCollision::CollideWith(BoxCollision* other)
{
    if (!m_Dynamic && !other->GetDynamic())return false;
    // Box と Box の当たり判定ロジック
    float minXA = m_Position.x - m_Size.x;
    float maxXA = m_Position.x + m_Size.x;
    float minYA = m_Position.y - m_Size.y;
    float maxYA = m_Position.y + m_Size.y;
    float minZA = m_Position.z - m_Size.z;
    float maxZA = m_Position.z + m_Size.z;

    Vector3 pos = other->GetPosition();
    Vector3 size = other->GetSize();

    float minXB = pos.x -  size.x;
    float maxXB = pos.x +  size.x;
    float minYB = pos.y -  size.y;
    float maxYB = pos.y +  size.y;
    float minZB = pos.z -  size.z;
    float maxZB = pos.z +  size.z;

    if (minXB < maxXA && maxXB > minXA &&
        minYA < maxYB && maxYA > minYB &&
        minZB < maxZA && maxZB > minZA)
    {
        if (m_Trigger || other->GetTrigger())
            return true;
        float difference[6];
        difference[0] = minYA - maxYB;
        difference[1] = maxYA - minYB;
        difference[2] = minXA - maxXB;
        difference[3] = maxXA - minXB;
        difference[4] = minZA - maxZB;
        difference[5] = maxZA - minZB;
        float diffmin = fabs(difference[0]);
        int index = 0;
        for (int i = 1; i < 6; i++)
        {
            if (diffmin > fabs(difference[i]))
            {
                diffmin = fabs(difference[i]);
                index = i;
            }
        }

        if (m_Dynamic)
        {
            Vector3 posvec = m_Position - pos;
            //ポジション計算用 
            pos = m_GameObject->m_Transform->GetPosition();
            float difference = posvec.y - m_Size.y - size.y;
            // 補正
            //if (maxYB <= GetOldPosition().y - m_Size.y && minYA <= maxYB)
            if (index == 0 && minYA <= maxYB)
            {
                float difference = posvec.y - m_Size.y - size.y;
                pos.y -= difference;
                m_Directions[other] = BOXHITDIRECTION_DOWN;
                other->SetHitDirection(this,BOXHITDIRECTION_UP);
            }
            //else if (minYB >= GetOldPosition().y + m_Size.y && maxYA >= minYB)
            else if (index == 1 && maxYA >= minYB)
            {
                float difference = posvec.y + m_Size.y + size.y;
                pos.y -= difference;
                m_Directions[other] = BOXHITDIRECTION_UP;
                other->SetHitDirection(this, BOXHITDIRECTION_DOWN);
            }
            //else if (maxXB <= GetOldPosition().x - m_Size.x && minXA <= maxXB)
            else if (index == 2 && minXA <= maxXB)
            {
                pos.x = m_GameObject->m_Transform->GetOldePosition().x;
                m_Directions[other] = BOXHITDIRECTION_LEFT;
                other->SetHitDirection(this, BOXHITDIRECTION_RIGHT);
            }
            //else if (minXB >= GetOldPosition().x + m_Size.x && maxXA >= minXB)
            else if (index == 3 && maxXA >= minXB)
            {
                pos.x = m_GameObject->m_Transform->GetOldePosition().x;
                m_Directions[other] = BOXHITDIRECTION_RIGHT;
                other->SetHitDirection(this, BOXHITDIRECTION_LEFT);

            }
            //else if (maxZB <= GetOldPosition().z - m_Size.z && minZA <= maxZB)
            else if (index == 4 && minZA <= maxZB)
            {
                pos.z = m_GameObject->m_Transform->GetOldePosition().z;
                m_Directions[other] = BOXHITDIRECTION_BACK;
                other->SetHitDirection(this, BOXHITDIRECTION_FORWARD);
            }
            else
            {
                pos.z = m_GameObject->m_Transform->GetOldePosition().z;
                m_Directions[other] = BOXHITDIRECTION_FORWARD;
                other->SetHitDirection(this, BOXHITDIRECTION_BACK);
            }

            m_GameObject->m_Transform->SetPosition(pos);
            m_Position = pos + m_Offset;

            return true;
        }
        else
        {
            //ポジション計算用 
            Vector3 posvec = pos - m_Position;
            Vector3 pos = other->m_GameObject->m_Transform->GetPosition();
            // 補正
            if (index == 1 && minYB <= maxYA)
            {
                float difference = posvec.y - m_Size.y - size.y;
                pos.y -= difference;
                other->SetHitDirection(this, BOXHITDIRECTION_DOWN);
                m_Directions[other] = BOXHITDIRECTION_UP;
            }
            else if (index == 0 && maxYB >= minYA)
            {
                float difference = posvec.y + m_Size.y + size.y;
                pos.y -= difference;
                other->SetHitDirection(this, BOXHITDIRECTION_UP);
                m_Directions[other] = BOXHITDIRECTION_DOWN;
            }
            else if (index == 3  && minXB <= maxXA)
            {
                pos.x = other->m_GameObject->m_Transform->GetOldePosition().x;
                other->SetHitDirection(this, BOXHITDIRECTION_LEFT);
                m_Directions[other] = BOXHITDIRECTION_RIGHT;
            }
            else if (index == 2 && maxXB >= minXA)
            {
                pos.x = other->m_GameObject->m_Transform->GetOldePosition().x;
                other->SetHitDirection(this, BOXHITDIRECTION_RIGHT);
                m_Directions[other] = BOXHITDIRECTION_LEFT;

            }
            else if (index == 4 && maxZB >= minZA)
            {
                pos.z = other->m_GameObject->m_Transform->GetOldePosition().z;
                other->SetHitDirection(this, BOXHITDIRECTION_BACK);
                m_Directions[other] = BOXHITDIRECTION_FORWARD;
            }
            else
            {
                pos.z = other->m_GameObject->m_Transform->GetOldePosition().z;
                other->SetHitDirection(this, BOXHITDIRECTION_FORWARD);
                m_Directions[other] = BOXHITDIRECTION_BACK;
            }
          
            other->m_GameObject->m_Transform->SetPosition(pos);
            other->m_Position = pos + other->m_Offset;
            return true;
        }
    }

    return false;
}

bool BoxCollision::CollideWith(SphereCollision* other)
{
    if (!m_Dynamic && !other->GetDynamic()) return false;

    // Box と Sphere の当たり判定ロジック
    // BoxとSphereが重なっているかどうかを判定
    float distanceSquared = 0.0f;

    // X軸方向の距離
    float dx = max(0.0f, abs(m_Position.x - other->GetPosition().x) - m_Size.x);
    distanceSquared += dx * dx;

    // Y軸方向の距離
    float dy = max(0.0f, abs(m_Position.y - other->GetPosition().y) - m_Size.y);
    distanceSquared += dy * dy;

    // Z軸方向の距離
    float dz = max(0.0f, abs(m_Position.z - other->GetPosition().z) - m_Size.z);
    distanceSquared += dz * dz;

    // Sphereの半径との比較
    if (distanceSquared < other->GetSize() * other->GetSize())
    {
        if (m_Trigger || other->GetTrigger())
            return true;

        if (m_Dynamic)
        {
            // 重なっている場合の補正
            float distance = sqrt(distanceSquared);

            Vector3 normal = (m_Position - other->GetPosition()) / distance;  // 最短距離ベクトルの正規化
            if (normal.x == INFINITY)
                normal.x = 0.0f;
            if (normal.y == INFINITY)
                normal.y = 0.0f;
            if (normal.z == INFINITY)
                normal.z= 0.0f;

            // 補正ベクトルの計算（ボックスの頂点から球への最短距離ベクトル）
            Vector3 vec = normal * (other->GetSize() - distance);

            // ポジションの補正
            Vector3 pos = m_Position - m_Offset + vec;
            m_GameObject->m_Transform->SetPosition(pos);
            m_Position = m_GameObject->m_Transform->GetPosition() + m_Offset;

            return true;
        }
        else
        {
            // 重なっている場合の補正
            float distance = sqrt(distanceSquared);
            Vector3 normal = (other->GetPosition() - m_Position) / distance;  // 最短距離ベクトルの正規化

            if (normal.x == INFINITY)
                normal.x = 0.0f;
            if (normal.y == INFINITY)
                normal.y = 0.0f;
            if (normal.z == INFINITY)
                normal.z = 0.0f;

            // 補正ベクトルの計算（ボックスの頂点から球への最短距離ベクトル）
            Vector3 vec = normal * (other->GetSize() - distance);

            // ポジションの補正
            Vector3 pos = other->GetPosition() - other->GetOffset() + vec;
            other->GetGameObejct()->m_Transform->SetPosition(pos);
            other->GetPosition() = other->GetGameObejct()->m_Transform->GetPosition() + other->GetOffset();

            return true;
        }
    }

    return false;
}
