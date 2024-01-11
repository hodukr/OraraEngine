#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "gameObject.h"
#include "environmentMapping.h"
#include "imgui/imgui.h"

void EnvironmentMapping::CreatePass()
{
    D3D11_TEXTURE2D_DESC td;
    ZeroMemory(&td, sizeof(td));
    td.ArraySize = 1;
    td.Width = 512;
    td.Height = 512;
    td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    td.Usage = D3D11_USAGE_DEFAULT;
    td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    td.SampleDesc = Renderer::GetSwapChainDesc().SampleDesc;
    td.MiscFlags = 0;
    td.MipLevels = 1;
    Renderer::GetDevice()->CreateTexture2D(&td, NULL, &m_ReflectTexture);

    D3D11_RENDER_TARGET_VIEW_DESC rtvd;
    ZeroMemory(&rtvd, sizeof(rtvd));
    rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    Renderer::GetDevice()->CreateRenderTargetView(m_ReflectTexture, &rtvd, &m_ReflectRenderTargetView);

    ID3D11Texture2D* depthTexture = NULL;
    ZeroMemory(&td, sizeof(td));
    td.Width = 512;
    td.Height = 512;
    td.MipLevels = 1;
    td.ArraySize = 1;
    td.Format = DXGI_FORMAT_D32_FLOAT;
    td.SampleDesc.Count = 1;
    td.SampleDesc.Quality = 0;
    td.Usage = D3D11_USAGE_DEFAULT;
    td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    td.CPUAccessFlags = 0;
    td.MiscFlags = 0;
    Renderer::GetDevice()->CreateTexture2D(&td, NULL, &depthTexture);

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
    ZeroMemory(&dsvd, sizeof(dsvd));
    dsvd.Format = DXGI_FORMAT_D32_FLOAT;
    dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    Renderer::GetDevice()->CreateDepthStencilView(depthTexture, &dsvd, &m_ReflectDepthStencilView);

    depthTexture->Release();

    ZeroMemory(&td, sizeof(td));
    td.ArraySize = 6;
    td.Width = 512;
    td.Height = 512;
    td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    td.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;
    td.MipLevels = 1;
    td.Usage = D3D11_USAGE_DEFAULT;
    td.SampleDesc = Renderer::GetSwapChainDesc().SampleDesc;
    Renderer::GetDevice()->CreateTexture2D(&td, NULL, &m_CubeReflectTexture);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
    ZeroMemory(&srvd, sizeof(srvd));
    srvd.Format = td.Format;
    srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    srvd.TextureCube.MipLevels = td.MipLevels;
    srvd.TextureCube.MostDetailedMip = 0;
    Renderer::GetDevice()->CreateShaderResourceView(m_CubeReflectTexture, &srvd, &m_CubeReflectShaderResourceView);

    m_DrawObjNum = 0;
}

void EnvironmentMapping::Uninit()
{
    m_ReflectTexture->Release();
    m_ReflectRenderTargetView->Release();
    m_ReflectDepthStencilView->Release();
    m_CubeReflectTexture->Release();
    m_CubeReflectShaderResourceView->Release();

    m_DrawObj.clear();
    m_SelectDrawObj.clear();
    m_SelectPosObj.clear();
}

void EnvironmentMapping::Draw()
{
    LIGHT light;
    light.Enable = true;
    light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);
    D3DXVec4Normalize(&light.Direction, &light.Direction);
    light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
    light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    Renderer::SetLight(light);

    D3DXVECTOR3 lookatOffset[6] = {
        { 1.0f, 0.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        { 0.0f, 1.0f, 0.0f},
        { 0.0f,-1.0f, 0.0f},
        { 0.0f, 0.0f, 1.0f},
        { 0.0f, 0.0f,-1.0f},
    };

    D3DXVECTOR3 upOffset[6] = {
        { 0.0f, 1.0f, 0.0f},
        { 0.0f, 1.0f, 0.0f},
        { 0.0f, 0.0f,-1.0f},
        { 0.0f, 0.0f, 1.0f},
        { 0.0f, 1.0f, 0.0f},
        { 0.0f, 1.0f, 0.0f},
    };

    D3DXVECTOR3 eye;
    D3DXVECTOR3 lookat;
    D3DXVECTOR3 up;
    D3DXMATRIX viewMatrixArray[6];

    for (int i = 0; i < 6; i++)
    {
        eye = m_EnvMapObjPos;
        lookat = m_EnvMapObjPos + lookatOffset[i];
        up = upOffset[i];
        D3DXMatrixLookAtLH(&viewMatrixArray[i], &eye, &lookat, &up);
    }

    D3DXMATRIX projectionMatrix;
    D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DX_PI / 2, 1.0f, 0.1f, 500.0f);
    Renderer::SetProjectionMatrix(&projectionMatrix);

    SetReflectViewport();

    for (int i = 0; i < 6; i++)
    {
        BeginCube();
        Renderer::SetViewMatrix(&viewMatrixArray[i]);

        for (const auto& obj : m_DrawObj)
        {
            obj.second->Draw();
        }
       
        Renderer::GetDeviceContext()->CopySubresourceRegion(
            GetCubeReflectTexture(),
            D3D11CalcSubresource(0, i, 1),
            0, 0, 0, GetReflectTexture(), 0, nullptr
        );
    }
}

void EnvironmentMapping::Update()
{
    Scene* scene = Manager::GetScene();

    ImGui::Begin("Shader", 0);

    if (ImGui::TreeNode("EnvironmentMapping"))
    {
        //ゲームオブジェクト一覧
        if (ImGui::BeginCombo("EnvMapObjPos", m_SelectPosObj.c_str()))
        {
            for (int i = 0; i < 3; i++)
            {
                if (scene->GetList()[i].empty())
                    continue;
                for (auto& gameobject : scene->GetList()[i])
                {
                    if (ImGui::Selectable(gameobject->GetName().c_str()))
                    {
                        m_SelectPosObj = gameobject->GetName();
                        m_EnvMapObjPos = gameobject.get()->m_Transform->GetPosition().dx();
                    }
                }
            }
            ImGui::EndCombo();
        }

        if (ImGui::TreeNode("DrawObj"))
        {
            for (int i = 0; i < m_DrawObjNum; i++)
            {
                //ゲームオブジェクト一覧
                if (ImGui::BeginCombo(std::to_string(i).c_str(), m_SelectDrawObj[i].c_str()))
                {
                    for (int j = 0; j < 3; j++)
                    {
                        if (scene->GetList()[j].empty())
                            continue;

                        for (auto& gameobject : scene->GetList()[j])
                        {
                            if (ImGui::Selectable(gameobject->GetName().c_str()))
                            {
                                m_SelectDrawObj[i] = gameobject->GetName();
                                m_DrawObj[i] = gameobject.get();
                            }
                        }
                    }

                    ImGui::EndCombo();
                }
            }

            ImGui::Separator();

            if (ImGui::Button("Add List"))
            {
                m_DrawObjNum++;
            }

            ImGui::SameLine();  // 同じ行に次の要素を配置する

            if (ImGui::Button("Erase List"))
            {
                if (m_DrawObjNum > 0)
                {
                    m_SelectDrawObj.erase(m_DrawObjNum - 1);
                    m_DrawObj.erase(m_DrawObjNum - 1);
                    m_DrawObjNum--;
                }
            }

            ImGui::TreePop();
        }

    
        ImGui::TreePop();
    }
    
    ImGui::End();
}

void EnvironmentMapping::BeginCube(void)
{
    //レンダリングターゲットを環境マップレンダリング用バッファに設定する 
    Renderer::GetDeviceContext()->OMSetRenderTargets(1, &m_ReflectRenderTargetView, m_ReflectDepthStencilView);

    //　バックバッファクリア 
    float ClearColor[4] = { 0.0f,0.0f,0.0f,1.0f };
    Renderer::GetDeviceContext()->ClearRenderTargetView(m_ReflectRenderTargetView, ClearColor);
    Renderer::GetDeviceContext()->ClearDepthStencilView(m_ReflectDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void EnvironmentMapping::SetReflectViewport(void)
{
    D3D11_VIEWPORT vp;
    vp.Width = 512;
    vp.Height = 512;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    Renderer::GetDeviceContext()->RSSetViewports(1, &vp);
}
