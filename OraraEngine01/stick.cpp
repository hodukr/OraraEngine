#include "main.h"
#include "renderer.h"
#include "boxCollision.h"
#include "sphereCollision.h"
#include "stick.h"


void Stick::Init()
{
    m_Model = new Model();
    m_Model->Load("asset\\model\\Stick.obj");

    
    m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_Scale = D3DXVECTOR3(0.8f, 0.8f, 0.8f);


    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "shader\\vertexLightingVS.cso");

    Renderer::CreatePixelShader(&m_PixelShader,
        "shader\\vertexLightingPS.cso");

    //m_Collision->SetOffset(D3DXVECTOR3(0.0f, 3.0f, 0.0f));
}

void Stick::Uninit()
{
    m_Model->Unload();
    delete m_Model;

    m_VertexLayout->Release();
    m_VertexShader->Release();
    m_PixelShader->Release();

}
void Stick::Update()
{
    //static  D3DXVECTOR3 rot = D3DXVECTOR3(0.0f,0.0f,0.0f);

    //m_Rotation = D3DXVECTOR3(rot.x, rot.y, rot.z);
    //rot.x += 0.1f;
    //rot.y += 0.1f;
 //   rot.z += 0.1f;

    GameObject::Update();
}

void Stick::Draw()
{
    GameObject::Draw();
    //入力レイアウト設定

    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

    //シェーダー設定
    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

    //マトリクス設定
    D3DXMATRIX world, scale, rot, trans;

    D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
    D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y + D3DX_PI, m_Rotation.x, m_Rotation.z);
    D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
    world = scale * rot * trans;

    Renderer::SetWorldMatrix(&world);

    PARAMETER parm;
    parm.pos = D3DXVECTOR4(m_Position, 0.0f);
    Renderer::SetParameter(parm);

    m_Model->Draw();

}
