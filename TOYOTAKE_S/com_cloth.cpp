#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "com_cloth.h"
#include "imgui/imgui.h"
#include "gameObject.h"
#include "shaderManager.h"

void Cloth::Init()
{
    for (int x = 0; x <= NUM_VERTEX - 1; x++)
    {
        for (int y = 0; y <= NUM_VERTEX - 1; y++)
        {
            m_Velocity[x][y] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            m_Acceleration[x][y] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            m_Resultant[x][y] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            m_Gravity[x][y] = D3DXVECTOR3(0.0f, m_SpringMass * -9.8f, 0.0f);
            // 特定の粒子はロック状態へ 
            m_OnLock[x][y] = (x == 0 && (y == 0 || y == NUM_VERTEX - 1));	// ロック 
        }
    }

    // 頂点バッファ生成
    {
        for (int x = 0; x <= NUM_VERTEX - 1; x++)
        {
            for (int y = 0; y <= NUM_VERTEX - 1; y++)
            {
                m_Vertex[x][y].Position = D3DXVECTOR3((x - 10) * 5.0f, (y - 10) * -5.0f,0.0f);
                m_Vertex[x][y].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);//法線ベクトル
                m_Vertex[x][y].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
                m_Vertex[x][y].TexCoord = D3DXVECTOR2(x * CLOTH_UV, y * CLOTH_UV);
            }
        }

        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(VERTEX_3D) * NUM_VERTEX * NUM_VERTEX;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        D3D11_SUBRESOURCE_DATA sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.pSysMem = m_Vertex;

        Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
    }

    // インデックスバッファ生成
    {
        unsigned int index[((NUM_VERTEX + 1) * 2) * (NUM_VERTEX - 1) - 2];

        int i = 0;
        for (int x = 0; x < (NUM_VERTEX - 1); x++)
        {
            for (int y = 0; y < NUM_VERTEX; y++)
            {
                index[i] = x * NUM_VERTEX + y;
                i++;

                index[i] = (x + 1) * NUM_VERTEX + y;
                i++;
            }

            if (x == NUM_VERTEX - 2)
                break;

            //縮退ポリゴン
            index[i] = (x + 1) * NUM_VERTEX + (NUM_VERTEX - 1);
            i++;

            index[i] = (x + 1) * NUM_VERTEX;
            i++;
        }

        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(unsigned int) * (((NUM_VERTEX + 1) * 2) * (NUM_VERTEX - 1) - 2);
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        D3D11_SUBRESOURCE_DATA sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.pSysMem = index;

        Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
    }

    m_TexNum = TextureManager::LoadTexture((char*)"asset\\texture\\dissolve.png");

    // バネの初期化
    int count = 0;
    D3DXVECTOR3	len;				// 粒子間の距離
    for (int x = 0; x <= NUM_VERTEX ; x++)
    {
        for (int y = 0; y <= NUM_VERTEX; y++)
        {
            // バネ参照インデックスの設定
            if (x < NUM_VERTEX)
            {
                m_Spring[count].Praticle1.x = x;		// １個目の参照粒子インデックスを設定
                m_Spring[count].Praticle1.y = y;
                m_Spring[count].Praticle2.x = x + 1;	// ２個目の参照粒子インデックスを設定
                m_Spring[count].Praticle2.y = y;
                // ２個の粒子間の距離を求め、バネの自然長とする
                len =  m_Vertex[x][y].Position - m_Vertex[x + 1][y].Position;
                m_Spring[count].Length = D3DXVec3Length(&len);
                count++;
            }
            if (y < NUM_VERTEX)
            {
                m_Spring[count].Praticle1.x = x;
                m_Spring[count].Praticle1.y = y;
                m_Spring[count].Praticle2.x = x;
                m_Spring[count].Praticle2.y = y + 1;
                len = m_Vertex[x][y].Position - m_Vertex[x][y + 1].Position;
                m_Spring[count].Length = D3DXVec3Length(&len);
                count++;
            }
            if (x < NUM_VERTEX && y < NUM_VERTEX)
            {
                m_Spring[count].Praticle1.x = x;
                m_Spring[count].Praticle1.y = y;
                m_Spring[count].Praticle2.x = x + 1;
                m_Spring[count].Praticle2.y = y + 1;
                len = m_Vertex[x][y].Position - m_Vertex[x + 1][y + 1].Position;
                m_Spring[count].Length = D3DXVec3Length(&len);
                count++;
            }
            if (x > 0 && y < NUM_VERTEX)
            {
                m_Spring[count].Praticle1.x = x;
                m_Spring[count].Praticle1.y = y;
                m_Spring[count].Praticle2.x = x - 1;
                m_Spring[count].Praticle2.y = y + 1;
                len = m_Vertex[x][y].Position - m_Vertex[x - 1][y + 1].Position;
                m_Spring[count].Length = D3DXVec3Length(&len);
                count++;
            }
        }
    }
    
    m_Parameter = new PARAMETER;
    m_Parameter->dissolveThreshold = 0.0f;
    m_Parameter->dissolveRange = 0.1f;
}


void Cloth::Uninit()
{
    m_VertexBuffer->Release();
    m_IndexBuffer->Release();
    delete m_Parameter;
}


void Cloth::Update()
{
    D3DXVECTOR3	resistance = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 抵抗力ベクトル 
    D3DXVECTOR3	windforce = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 風力ベクトル 
    for (int x = 0; x <= NUM_VERTEX - 1; x++)
    {
        for (int y = 0; y <= NUM_VERTEX - 1; y++)
        {
            // 合力を0にする 
            m_Resultant[x][y] = D3DXVECTOR3(0.0f,0.0f, 0.0f);

            if (!m_OnLock[x][y])
            {
                // 重力を求める 
                m_Gravity[x][y] = D3DXVECTOR3(0.0f, m_SpringMass * -0.98f, 0.0f);

                // 抵抗力を求める   
                resistance = -m_Velocity[x][y] * m_AttCoefficient;

                // 風力を求める 
                if (m_IsWind)
                {//風ONの場合　乱数で風力を作る 
                    windforce.x = m_WindForce.x + (float)(rand() % 30 - 15);
                    windforce.y = m_WindForce.y + (float)(rand() % 30 - 15);
                    windforce.z = m_WindForce.z + (float)(rand() % 30 - 15);
                }
                else
                {
                    windforce = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                }

                // 合力＝重力＋抵抗力＋風力 
                m_Resultant[x][y].x = m_Gravity[x][y].x + resistance.x + windforce.x;
                m_Resultant[x][y].y = m_Gravity[x][y].y + resistance.y + windforce.y;
                m_Resultant[x][y].z = m_Gravity[x][y].z + resistance.z + windforce.z;
            }
        }
    }

    int		x1, y1, x2, y2;			// バネ端にある粒子のインデックス（横、縦別２個分）  
    float	force1, force2;		    // バネが粒子に与える力（２種） 
    D3DXVECTOR3	vec_spr;		    // 粒子２点間のベクトル  
    D3DXVECTOR3	lenForce1;			// 長さからバネに発生する力  
    D3DXVECTOR3	lenForce2;          // 長さからバネに発生する力 
    float	Length;					// 粒子間の距離  
    // バネの処理  
    for (int i = 0; i < SPRING_NUMS; i++)
    {
        // バネの参照粒子（２個）を取得   
        x1 = m_Spring[i].Praticle1.x; 
        y1 = m_Spring[i].Praticle1.y;
        x2 = m_Spring[i].Praticle2.x;
        y2 = m_Spring[i].Praticle2.y;
        // ２個の粒子間のベクトルを求める  
        vec_spr = m_Vertex[x1][y1].Position - m_Vertex[x2][y2].Position;
        // 粒子間の距離を求める  
        Length = D3DXVec3Length(&vec_spr);
        // 距離、自然長、バネ係数からかかる力を求める（２つ目は逆方向） 
        force1 = -m_SpringCoefficient * (Length - m_Spring[i].Length);
        force2 = -force1;
        // ベクトルの成分に力をかける  
        lenForce1 = force1 * (vec_spr / Length);
        lenForce2 = force2 * (vec_spr / Length);
        
        // 求めた力を合力に加える  
        if (!m_OnLock[x1][y1])
        {
            m_Resultant[x1][y1] += lenForce1;
        }
        if (!m_OnLock[x2][y2])
        {
            m_Resultant[x2][y2] += lenForce2;
        }
    }

    for (int x = 0; x <= NUM_VERTEX - 1; x++)
    {
        for (int y = 0; y <= NUM_VERTEX - 1; y++)
        {
           // 固定されていればスキップ  
           if (m_OnLock[x][y]) continue;

           // 合力と質量から加速度を求める  
           m_Acceleration[x][y] = m_Resultant[x][y] / m_SpringMass;

            // 速度に加速度を加える  
           m_Velocity[x][y] += m_Acceleration[x][y] * m_deltaTime;
  
            // 速度から座標を移動 
           m_Vertex[x][y].Position += m_Velocity[x][y] * m_deltaTime;
        }
    }
}


void Cloth::Draw()
{
    //頂点データ書き換え 
    D3D11_MAPPED_SUBRESOURCE msr;
    Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

    VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

    for (int x = 0; x <= NUM_VERTEX - 1; x++)
    {
        for (int y = 0; y <= NUM_VERTEX - 1; y++)
        {
            int index = x * NUM_VERTEX + y;
            vertex[index].Position = D3DXVECTOR3(m_Vertex[x][y].Position.x, m_Vertex[x][y].Position.y, m_Vertex[x][y].Position.z);
            vertex[index].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);//法線ベクトル 
            vertex[index].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
            vertex[index].TexCoord = D3DXVECTOR2(x * CLOTH_UV, y * CLOTH_UV);
        }
    }

    Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);


    // マトリクス設定 
    D3DXMATRIX world, scale, rot, trans;
    D3DXVECTOR3 Scale = m_GameObject->m_Transform->GetScale().dx();
    D3DXVECTOR3 Position = m_GameObject->m_Transform->GetPosition().dx();
    D3DXVECTOR3 Rotation = m_GameObject->m_Transform->GetRotation().dx();

    D3DXMatrixScaling(&scale, Scale.x, Scale.y, Scale.z);
    D3DXMatrixRotationYawPitchRoll(&rot, Rotation.y,Rotation.x, Rotation.z);
    D3DXMatrixTranslation(&trans, Position.x, Position.y, Position.z);
    world = scale * rot * trans;
    Renderer::SetWorldMatrix(&world);


    // 頂点バッファ設定 
    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

    // インデックスバッファ設定 
    Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // マテリアル設定 
    MATERIAL material;
    ZeroMemory(&material, sizeof(material));
    material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    material.TextureEnable = true;
    Renderer::SetMaterialModel(material);

    // テクスチャ設定
    if (m_GameObject->GetMaterial()->GetTextureNum() > -1)
        Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, TextureManager::GetTexture(m_GameObject->GetMaterial()->GetTextureNum()));


    if (ShaderManager::Instance().GetShader(m_GameObject->GetMaterial()->GetShaderNum())->GetFile() == "dissolve")
    {
        Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, TextureManager::GetTexture(m_TexNum));
        Renderer::SetParameter(*m_Parameter);
    }
    // プリミティブトポロジ設定 
    Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // ポリゴン描画 
    Renderer::GetDeviceContext()->DrawIndexed(((NUM_VERTEX + 1) * 2) * (NUM_VERTEX - 1) - 2, 0, 0);
}


