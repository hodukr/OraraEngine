#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "com_mesh.h"
#include "com_transform.h"
#include "model.h"
#include "com_material.h"
#include "shaderManager.h"
#include "pass_depthShadow.h"

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

    //シャドウバッファテクスチャを1番へセット
    // テクスチャ設定
    DepthShadow* shadow = ShaderManager::Instance().GetPass<DepthShadow>(SHADER_SHADOW);

    Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, shadow->GetDepthShadowTexture());

    Renderer::SetWorldMatrix(m_GameObject->m_Transform->GetMatrixPtr());

    m_Model->Draw();

}

