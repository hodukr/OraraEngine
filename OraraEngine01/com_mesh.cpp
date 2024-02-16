#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "com_mesh.h"
#include "com_transform.h"
#include "modelManager.h"
#include "shaderManager.h"
#include "pass_depthShadow.h"

void Mesh::SetModel(std::string pas)
{
    m_Modelpas = pas;
    std::string name = "asset\\model\\" + m_Modelpas;
    m_ModelNum = ModelManager::Load(name.c_str());

}

void Mesh::Init()
{
    SetModel(m_Modelpas);
}

void Mesh::Uninit()
{
}

void Mesh::Update()
{
    if (m_ModelFolder.IsSet)
    {
        SetModel(m_ModelFolder.Date);
        m_ModelFolder.IsSet = false;
    }
}

void Mesh::Draw()
{

    //シャドウバッファテクスチャを1番へセット
    // テクスチャ設定
    DepthShadow* shadow = ShaderManager::Instance().GetPass<DepthShadow>(SHADER_SHADOW);

    Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, shadow->GetDepthShadowTexture());

    Renderer::SetWorldMatrix(m_GameObject->m_Transform->GetMatrixPtr());

    ModelManager::GetModel(m_ModelNum)->Draw();

}

