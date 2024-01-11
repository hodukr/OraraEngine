
#include "main.h"
#include "renderer.h"
#include "waterSurface.h"
#include "textureManager.h"
#include "imgui/imgui.h"
#include "gameObject.h"

void WaterSurface::Init()
{
    m_WavePitch = 1.0f;
    m_Amplitude = 10.0f;
    m_WaveLength = 14.0f;
    m_WaveCycle = 7.0f;

    m_Time =0.0f;
     
    // 頂点バッファ生成
    {
        for (int x = 0; x <= NUM_VERTEX - 1; x++)
        {
            for (int z = 0; z <= NUM_VERTEX - 1; z++)
            {
                m_Vertex[x][z].Position = D3DXVECTOR3((x - 10) * m_WavePitch, 0.0f, (z - 10) * -m_WavePitch);
                m_Vertex[x][z].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);//法線ベクトル
                m_Vertex[x][z].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
                m_Vertex[x][z].TexCoord = D3DXVECTOR2(x * WATER_UV, z * WATER_UV);
            }
        }

        //法線ベクトル算出
        for (int x = 1; x <= NUM_VERTEX - 2; x++)
        {
            for (int z = 1; z <= NUM_VERTEX - 2; z++)
            {
                D3DXVECTOR3 vx, vz, vn;
                vx = m_Vertex[x + 1][z].Position - m_Vertex[x - 1][z].Position;
                vz = m_Vertex[x][z - 1].Position - m_Vertex[x][z + 1].Position;

                D3DXVec3Cross(&vn, &vz, &vx);//外積
                D3DXVec3Normalize(&vn, &vn);//正規化
                m_Vertex[x][z].Normal = vn;
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
            for (int z = 0; z < NUM_VERTEX; z++)
            {
                index[i] = x * NUM_VERTEX + z;
                i++;

                index[i] = (x + 1) * NUM_VERTEX + z;
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

    m_TexNum = TextureManager::LoadTexture("asset\\texture\\water.jpg");
    
    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

    Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");
}


void WaterSurface::Uninit()
{
    m_VertexBuffer->Release();
    m_IndexBuffer->Release();
    
    m_VertexLayout->Release();
    m_VertexShader->Release();
    m_PixelShader->Release();
}


void WaterSurface::Update()
{
    for (int x = 0; x <= NUM_VERTEX - 1; x++)
    {
        for (int z = 0; z <= NUM_VERTEX - 1; z++)
        {
            // 振動原点からの距離を求める
            float length = sqrtf((m_Vertex[x][z].Position.x - m_Vertex[NUM_VERTEX / 2][NUM_VERTEX / 2].Position.x) * (m_Vertex[x][z].Position.x - m_Vertex[NUM_VERTEX / 2][NUM_VERTEX / 2].Position.x)
                + (m_Vertex[x][z].Position.z - m_Vertex[NUM_VERTEX / 2][NUM_VERTEX / 2].Position.z) * (m_Vertex[x][z].Position.z - m_Vertex[NUM_VERTEX / 2][NUM_VERTEX / 2].Position.z));

            // 距離を元に振動の式から変位（波高）を求める
            m_Vertex[x][z].Position.y = m_Amplitude * sinf(2 * D3DX_PI * (length / m_WaveLength - m_Time / m_WaveCycle));
        }
    }
    m_Time += 0.01f;
    m_WaveTime++;

//#ifdef _DEBUG
//    ImGui::Begin("WaterSaface", 0, ImGuiWindowFlags_NoScrollbar);
//
//    ImGui::SliderFloat("Amplitude", &m_Amplitude, 1.0f, 50.0f);
//    ImGui::SliderFloat("WaveLength", &m_WaveLength, 1.0f, 50.0f);
//    ImGui::SliderFloat("WaveCycle", &m_WaveCycle, 1.0f, 50.0f);
//
//    ImGui::End();
//#endif //_DEBUG
}


void WaterSurface::Draw()
{
    //頂点データ書き換え
    D3D11_MAPPED_SUBRESOURCE msr;
    Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

    VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

    for (int x = 0; x <= NUM_VERTEX - 1; x++)
    {
        for (int z = 0; z <= NUM_VERTEX - 1; z++)
        {
            int index = x * NUM_VERTEX + z;
            vertex[index].Position = D3DXVECTOR3(m_Vertex[x][z].Position.x, m_Vertex[x][z].Position.y, m_Vertex[x][z].Position.z);
            vertex[index].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);//法線ベクトル
            vertex[index].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
            vertex[index].TexCoord = D3DXVECTOR2(x * WATER_UV, z * WATER_UV);
        }
    }

    //法線ベクトル算出
    for (int x = 1; x <= NUM_VERTEX - 2; x++)
    {
        for (int z = 1; z <= NUM_VERTEX - 2; z++)
        {
            int index = x * NUM_VERTEX + z; // 一次元のインデックス計算
            D3DXVECTOR3 vx, vz, vn;
            vx = vertex[index + NUM_VERTEX].Position - vertex[index - NUM_VERTEX].Position;
            vz = vertex[index - 1].Position - vertex[index + 1].Position;

            D3DXVec3Cross(&vn, &vz, &vx);
            D3DXVec3Normalize(&vn, &vn);
            vertex[index].Normal = vn;
        }
    }

    Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

    // 入力レイアウト設定
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

    // シェーダ設定
    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


    // マトリクス設定
    D3DXMATRIX world, scale, rot, trans;
    D3DXVECTOR3 Scale = m_GameObject->m_Transform->GetScale().dx();
    D3DXVECTOR3 Position = m_GameObject->m_Transform->GetPosition().dx();
    D3DXVECTOR3 Rotation = m_GameObject->m_Transform->GetRotation().dx();

    D3DXMatrixScaling(&scale, Scale.x, Scale.y, Scale.z);
    D3DXMatrixRotationYawPitchRoll(&rot, Rotation.y, Rotation.x, Rotation.z);
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
    Renderer::SetMaterial(material);

    // テクスチャ設定
    Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, TextureManager::GetTexture(m_TexNum));
    //Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, Renderer::GetCubeReflectShaderResourceView());

    // プリミティブトポロジ設定
    Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

  /*  PARAMETER param;
    ZeroMemory(&param, sizeof(param));
    param.pos = D3DXVECTOR4(m_Position.x, 0.0f, m_Position.z, 1.0f);
    param.waveAmplitude = 0.1f;
    param.waveFrequency = 100.0f;
    param.speed = 0.01f;
    param.time = m_WaveTime;

    Renderer::SetParameter(param);*/

    // ポリゴン描画
    Renderer::GetDeviceContext()->DrawIndexed(((NUM_VERTEX + 1) * 2) * (NUM_VERTEX - 1) - 2, 0, 0);

}


