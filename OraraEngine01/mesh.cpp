#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "mesh.h"
#include "transform.h"
#include "model.h"

void Mesh::Init()
{
    m_Model = new Model;
    m_Model->Load(m_Modelpas.c_str());

}

void Mesh::Uninit()
{
    m_Model->Unload();
    delete m_Model;
}

void Mesh::Update()
{

}

void Mesh::Draw()
{
    //マトリクス設定 
    D3DXMATRIX world, scale, rot, trans;
    D3DXVECTOR3 Scale = m_GameObject->m_Transform->GetScale().dx();
    D3DXVECTOR3 Rotation = m_GameObject->m_Transform->GetRotation().dx();
    D3DXVECTOR3 Position = m_GameObject->m_Transform->GetPosition().dx();

    D3DXMatrixScaling(&scale, Scale.x, Scale.y, Scale.z);
    D3DXMatrixRotationYawPitchRoll(&rot, Rotation.y, Rotation.x, Rotation.z);
    D3DXMatrixTranslation(&trans, Position.x, Position.y, Position.z);
    world = scale * rot * trans;
    m_Matrix = world;
    Renderer::SetWorldMatrix(&world);
    m_Model->Draw();

}
//CEREAL_REGISTER_TYPE(Mesh);
//CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Mesh)
