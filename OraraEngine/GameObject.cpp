#include "main.h"
#include "GameObject.h"
#include "modelManager.h"
#include "rendererManager.h"

void GameObject::Init()
{
    m_ModelIndex = ModelManager::Instance().Load((char*)"asset\\model\\torus.obj");
    m_Scale = Vector3(1.0f, 1.0f, 1.0f);
}

void GameObject::Uninit()
{

}

void GameObject::Update()
{

}

void GameObject::Draw()
{
    D3DXMATRIX world, scale, rot, trans;
    D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
    D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
    D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
    world = scale * rot * trans;
    RendererManager::Instance().SetWorldMatrix(&world);

    // テクスチャ設定
    //Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &m_TextureNormalMap);
    //ID3D11ShaderResourceView* bgTexture = Renderer::GetTexture(0);

    ModelManager::Instance().GetModel(m_ModelIndex)->Draw();
}
