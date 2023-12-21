#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "mesh.h"
#include "transform.h"
#include "model.h"
#include "material.h"
void Mesh::SetModel(std::string pas)
{
    m_Modelpas = pas;
    if (m_Model)
    {
        m_Model->Unload();
        delete m_Model;
    }

    m_Model = new Model;

    std::string name = "asset\\model\\" + m_Modelpas;
    m_Model->Load(name.c_str());

}

void Mesh::Init()
{
    m_Model = new Model;
    std::string name = "asset\\model\\" + m_Modelpas;
    m_Model->Load(name.c_str());
    m_Material.Init();
}

void Mesh::Uninit()
{
    m_Material.Uninit();
    m_Model->Unload();
    delete m_Model;
}

void Mesh::Update()
{
    m_Material.Update();

}

void Mesh::Draw()
{
    m_Material.Draw();
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
