#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "gameObject.h"
#include "pass_depthShadow.h"
#include "imgui/imgui.h"

void DepthShadow::CreatePass()
{
    //�V���h�E�o�b�t�@�쐬
    ID3D11Texture2D* depthTexture = NULL;
    D3D11_TEXTURE2D_DESC td;
    ZeroMemory(&td, sizeof(td));
    td.Width = SCREEN_WIDTH;
    td.Height = SCREEN_HEIGHT;
    td.MipLevels = 1;
    td.ArraySize = 1;
    td.Format = DXGI_FORMAT_R32_TYPELESS;//32bit�̎��R�Ȍ`���̃f�[�^�Ƃ���
    td.SampleDesc = Renderer::GetSwapChainDesc().SampleDesc;
    td.Usage = D3D11_USAGE_DEFAULT; // ���f�v�X���X�e���V���o�b�t�@�Ƃ��č쐬
    td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    td.CPUAccessFlags = 0;
    td.MiscFlags = 0;
    Renderer::GetDevice()->CreateTexture2D(&td, NULL, &depthTexture);

    //�f�v�X�X�e���V���^�[�Q�b�g�r���[�쐬
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
    ZeroMemory(&dsvd, sizeof(dsvd));
    dsvd.Format = DXGI_FORMAT_D32_FLOAT;//�s�N�Z���t�H�[�}�b�g��32BitFLOAT�^
    dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    Renderer::GetDevice()->CreateDepthStencilView(depthTexture, &dsvd,
        &m_DepthStencilView);

    //�V�F�[�_�[���\�[�X�r���[�쐬
    D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
    ZeroMemory(&srvd, sizeof(srvd));
    srvd.Format = DXGI_FORMAT_R32_FLOAT;//�s�N�Z���t�H�[�}�b�g��32BitFLOAT�^
    srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvd.Texture2D.MipLevels = 1;
    Renderer::GetDevice()->CreateShaderResourceView(depthTexture, &srvd,
        &m_ShaderResourceView);
    depthTexture->Release();


}

void DepthShadow::Init()
{
    //Scene* scene = Manager::GetScene();
    //if (scene)
    //{
    //    for (auto& objlist : scene->GetList()[1])
    //    {
    //        if (objlist->GetPass() & SHADER_SHADOW)m_ShadowDrawObj.push_back(objlist.get());
    //    }
    //}
}

void DepthShadow::Uninit()
{
    //m_ShadowDrawObj.clear();
    m_DepthStencilView->Release();
    m_ShaderResourceView->Release();
  
}

void DepthShadow::Draw()
{
    LIGHT light;
    light.Enable = true;
    light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);
    D3DXVec4Normalize(&light.Direction, &light.Direction);
    light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
    light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    //���C�g�J�����̃r���[�s����쐬
    D3DXVECTOR3 lightPos = D3DXVECTOR3(-20.0f, 20.0f, -20.0f);
    D3DXVECTOR3 lightTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 lightUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    D3DXMatrixLookAtLH(&light.ViewMatrix, &lightPos, &lightTarget, &lightUp);

    //���C�g�J�����̃v���W�F�N�V�����s����쐬
    D3DXMatrixPerspectiveFovLH(&light.ProjectionMatrix, 1.0f,
        (float)SCREEN_WIDTH / SCREEN_HEIGHT, 5.0f, 100.0f);

    //���C�g�����Z�b�g
    Renderer::SetLight(light);

    //�V���h�E�o�b�t�@�̍쐬
    BeginDepth();

    //���C�g�J�����̍s����Z�b�g
    Renderer::SetViewMatrix(&light.ViewMatrix);
    Renderer::SetProjectionMatrix(&light.ProjectionMatrix);

    //�e�𗎂Ƃ������I�u�W�F�N�g��`��(�ꉞ�n�ʂ�)
    Scene* scene = Manager::GetScene();
    for (const auto& obj : scene->GetList()[1])
    {
        if(obj->GetPass() &SHADER_SHADOW)
            obj->Draw();
    }

    //Scene* scene = Manager::GetScene();
    //scene->Draw();
}

void DepthShadow::Update()
{
    //Scene* scene = Manager::GetScene();

    //ImGui::Begin("Shader", 0);

  /*  if (ImGui::TreeNode("Shadow"))
    {*/
    //    //�Q�[���I�u�W�F�N�g�ꗗ
    //    if (ImGui::BeginCombo("EnvMapObjPos", m_SelectPosObj.c_str()))
    //    {
    //        for (int i = 0; i < 3; i++)
    //        {
    //            if (scene->GetList()[i].empty())
    //                continue;
    //            for (auto& gameobject : scene->GetList()[i])
    //            {
    //                if (ImGui::Selectable(gameobject->GetName().c_str()))
    //                {
    //                    m_SelectPosObj = gameobject->GetName();
    //                    m_EnvMapObjPos = gameobject.get()->m_Transform->GetPosition().dx();
    //                }
    //            }
    //        }
    //        ImGui::EndCombo();
    //    }

    //    if (ImGui::TreeNode("DrawObj"))
    //    {
    //        for (int i = 0; i < m_DrawObjNum; i++)
    //        {
    //            //�Q�[���I�u�W�F�N�g�ꗗ
    //            if (ImGui::BeginCombo(std::to_string(i).c_str(), m_SelectDrawObj[i].c_str()))
    //            {
    //                for (int j = 0; j < 3; j++)
    //                {
    //                    if (scene->GetList()[j].empty())continue;

    //                    for (auto& gameobject : scene->GetList()[j])
    //                    {
    //                        if (ImGui::Selectable(gameobject->GetName().c_str()))
    //                        {
    //                            m_SelectDrawObj[i] = gameobject->GetName();
    //                            m_ShadowDrawObj[i] = gameobject.get();
    //                        }
    //                    }
    //                }
    //                ImGui::EndCombo();
    //            }
    //        }

    //        ImGui::Separator();

    //        if (ImGui::Button("Add Object"))
    //        {
    //            m_DrawObjNum++;
    //        }

    //        ImGui::SameLine();  // �����s�Ɏ��̗v�f��z�u����

    //        if (ImGui::Button("Erase List"))
    //        {
    //            if (m_DrawObjNum > 0)
    //            {
    //                m_SelectDrawObj.erase(m_DrawObjNum - 1);
    //                m_ShadowDrawObj.erase(m_DrawObjNum - 1);
    //                m_DrawObjNum--;
    //            }
    //        }

    //        ImGui::TreePop();
    //    }


    //    ImGui::TreePop();
    //}

    //ImGui::End();
}

void DepthShadow::BeginDepth(void)
{
    //�V���h�E�o�b�t�@��[�x�o�b�t�@�ɐݒ肵�A���e��1�œh��Ԃ��Ă���
    Renderer::GetDeviceContext()->OMSetRenderTargets(0, NULL, m_DepthStencilView);
    Renderer::GetDeviceContext()->ClearDepthStencilView(m_DepthStencilView,
        D3D11_CLEAR_DEPTH, 1.0f, 0);
}

