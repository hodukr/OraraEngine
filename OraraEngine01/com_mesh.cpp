#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "com_mesh.h"
#include "com_transform.h"
#include "modelManager.h"
#include "shaderManager.h"
#include "pass_depthShadow.h"

void Mesh::SetModel(string file)
{
    m_Modelpas = file;
    string pass = "asset\\model\\" + m_Modelpas;
    m_ModelNum = ModelManager::Load(pass.c_str());

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

}

void Mesh::Draw()
{

    //シャドウバッファテクスチャを1番へセット
    // テクスチャ設定
    DepthShadow* shadow = ShaderManager::Instance().GetPass<DepthShadow>(SHADER_SHADOW);

    Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, shadow->GetTexture());

    Renderer::SetWorldMatrix(m_GameObject->m_Transform->GetMatrixPtr());

    ModelManager::GetModel(m_ModelNum)->Draw();

}

