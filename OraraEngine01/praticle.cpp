#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "camera.h"
#include "gameObject.h"
#include "praticle.h"
#include "material.h"
#include "textureManager.h"
#include "randomNumberGenerator.h"

static RandomNumberGenerator randm;

PraticleSystem::PraticleSystem()
{
    m_Emitter.StartColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    m_Emitter.StartLifetime = 2.0f;
    m_Emitter.StartRotation = 0.0f;
    m_Emitter.StartSize = 1.0f;
    m_Emitter.MaxParticles = 20;
    m_Emitter.EmissionRate = 0.5f;
    m_Emitter.Speed = 0.1f;
    m_Emitter.AddVelocity = Vector3::Zero();
    m_NumPraticle = 0;
    m_Emitter.GradationColor[0] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
    m_Emitter.GradationColor[1] = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
    m_Emitter.GradationColor[2] = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
    m_Emitter.TexName = "praticle.dds";

    m_Emitter.IsGradation = false;
}

void PraticleSystem::Init()
{
    VERTEX_3D vertex[4];
    D3DXVECTOR3 position = m_GameObject->m_Transform->GetPosition().dx();
    vertex[0].Position = position - D3DXVECTOR3(-1.0f, 1.0f,0.0f);
    vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
    vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

    vertex[1].Position = position - D3DXVECTOR3(1.0f, 1.0f, 0.0f);
    vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
    vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

    vertex[2].Position = position - D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
    vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
    vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

    vertex[3].Position = position - D3DXVECTOR3(1.0f, -1.0f, 0.0f);
    vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
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

    std::string name = "asset\\texture\\" + m_Emitter.TexName;

    m_TexNum = TextureManager::LoadTexture(name.c_str());

    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "shader\\praticleVS.cso");

    Renderer::CreatePixelShader(&m_PixelShader,
        "shader\\praticlePS.cso");

    m_Emitter.StartPosition = m_GameObject->m_Transform->GetPosition();


    Scene* scene = Manager::GetScene();
    m_Camera = nullptr;
    for (auto& obj : scene->GetList()[1])
    {
        m_Camera = obj.get()->GetComponent<Camera>();
        if (m_Camera)
            break;
    }

    for (int i = 0; i < m_Emitter.MaxParticles; i++)
    {
        AddPraticle();
    }
}

void PraticleSystem::Uninit()
{
    m_VertexBuffer->Release();;

    m_VertexLayout->Release();
    m_VertexShader->Release();
    m_PixelShader->Release();
}

void PraticleSystem::Update()
{
    while (m_NumPraticle < m_Emitter.MaxParticles)
    {
        AddPraticle();
    }

    while (m_NumPraticle > m_Emitter.MaxParticles)
    {
        if (!DeletPraticle())break;
    }

    if (m_Emitter.EmissionRate <= m_Timer.GetElapsedTimeInSeconds() && !m_StackParticle.empty())
    {
        InitParticle(m_StackParticle.top());
        m_ParticleList.push_back(m_StackParticle.top());
        m_StackParticle.pop();
        m_Timer.Reset();
    }

    auto rmvIter = std::remove_if(m_ParticleList.begin(), m_ParticleList.end(), [=](PARTICLE* par)
        {
            par->Position += par->Velocity + m_Emitter.AddVelocity;
            if (m_Emitter.IsFade)
            {
                if (par->Timer.GetElapsedTimeInSeconds() >= m_Emitter.StartFadeTime)
                {
                    par->Color.a *= m_Emitter.DeclineFade;
                }
            }

            if (par->TimeToLife < par->Timer.GetElapsedTimeInSeconds())
            {
                m_StackParticle.push(par);
                return true;
            }
            return false;
        });

    m_ParticleList.erase(rmvIter, m_ParticleList.end());
}

void PraticleSystem::Draw()
{
    
    Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

    //入力レイアウト設定 
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

    //シェーダー設定  
    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

    for(auto& praticle : m_ParticleList)
    {
        if (m_Camera)
            D3DXMATRIX view = m_Camera->GetViewMatrix();

        ////ビューの逆行列 
        //D3DXMATRIX invView;
        //D3DXMatrixInverse(&invView, NULL, &view);
        //invView._41 = 0.0f;
        //invView._42 = 0.0f;
        //invView._43 = 0.0f;

        //マトリクス設定 
        D3DXMATRIX world, scale, rot, trans;
        D3DXVECTOR3 Scale = m_GameObject->m_Transform->GetScale().dx();
        D3DXVECTOR3 Position = praticle->Position.dx();

        D3DXMatrixScaling(&scale, Scale.x, Scale.y, Scale.z);
        D3DXMatrixTranslation(&trans, Position.x, Position.y, Position.z);
        world = scale /** invView */* trans;
        Renderer::SetWorldMatrix(&world);

        //頂点バッファ設定 
        UINT stride = sizeof(VERTEX_3D);
        UINT offset = 0;
        Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

        //マテリアル設定 
        MATERIAL material;
        ZeroMemory(&material, sizeof(material));
        material.Diffuse = praticle->Color;
        material.TextureEnable = true;
        Renderer::SetMaterial(material);

        PRATICLE pra;
        ZeroMemory(&pra, sizeof(pra));
        pra.Color[0] = m_Emitter.GradationColor[0];
        pra.Color[1] = m_Emitter.GradationColor[1];
        pra.Color[2] = m_Emitter.GradationColor[2];
        pra.IsGradation = m_Emitter.IsGradation;
        pra.Statposition = m_GameObject->m_Transform->GetPosition().dx();
        pra.Size = m_Emitter.GradationLength;
        Renderer::SetPraticle(pra);

        //テクスチャ設定 
        Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, TextureManager::GetTexture(m_TexNum));

        //プリミティブトポロジ設定 
        Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

        Renderer::SetATCEnable(true);
        //ポリゴン描画 
        Renderer::GetDeviceContext()->Draw(4, 0);

        Renderer::SetATCEnable(false);
    }
}


void PraticleSystem::SetTexture(const char* texname)
{
    m_Emitter.TexName = texname;
    std::string name = "asset\\texture\\" + m_Emitter.TexName;

    m_TexNum = TextureManager::LoadTexture(name.c_str());
}

void PraticleSystem::InitParticle(PARTICLE* partiale)
{
    partiale->Position = m_GameObject->m_Transform->GetPosition();
    partiale->Color = m_Emitter.StartColor;
    Vector3 vec;
    vec.x = randm.GenerateFloat(-1.0f, 1.0f);
    vec.y = randm.GenerateFloat(-1.0f, 1.0f);
    vec.z = randm.GenerateFloat(-1.0f, 1.0f);
    vec.NormalizThis();
    vec += m_Emitter.AddStateVelocity;
    vec.NormalizThis();
    partiale->Velocity = vec * m_Emitter.Speed;
    partiale->TimeToLife = m_Emitter.StartLifetime;
    partiale->Rotation = randm.GenerateFloat(0.0f,D3DX_PI * 2.0f);
    partiale->Timer.Reset();
}

void PraticleSystem::AddPraticle()
{
    m_NumPraticle++;
    PARTICLE* par = new PARTICLE;
    InitParticle(par);
    m_StackParticle.push(par);
}

bool PraticleSystem::DeletPraticle()
{
    if (!m_StackParticle.empty())
    {
        m_StackParticle.pop();
        m_NumPraticle--;
        return true;
    }
    return false;

}

