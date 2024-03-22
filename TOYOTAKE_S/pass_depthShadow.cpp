#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "gameObject.h"
#include "pass_depthShadow.h"
#include "imgui/imgui.h"

void DepthShadow::CreatePass()
{
    //シャドウバッファ作成
    ID3D11Texture2D* depthTexture = NULL;
    D3D11_TEXTURE2D_DESC td;
    ZeroMemory(&td, sizeof(td));
    td.Width = SCREEN_WIDTH;
    td.Height = SCREEN_HEIGHT;
    td.MipLevels = 1;
    td.ArraySize = 1;
    td.Format = DXGI_FORMAT_R32_TYPELESS;//32bitの自由な形式のデータとする
    td.SampleDesc = Renderer::GetSwapChainDesc().SampleDesc;
    td.Usage = D3D11_USAGE_DEFAULT; // ↓デプス＆ステンシルバッファとして作成
    td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    td.CPUAccessFlags = 0;
    td.MiscFlags = 0;
    Renderer::GetDevice()->CreateTexture2D(&td, NULL, &depthTexture);

    //デプスステンシルターゲットビュー作成
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
    ZeroMemory(&dsvd, sizeof(dsvd));
    dsvd.Format = DXGI_FORMAT_D32_FLOAT;//ピクセルフォーマットは32BitFLOAT型
    dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    Renderer::GetDevice()->CreateDepthStencilView(depthTexture, &dsvd,
        &m_DepthStencilView);

    //シェーダーリソースビュー作成
    D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
    ZeroMemory(&srvd, sizeof(srvd));
    srvd.Format = DXGI_FORMAT_R32_FLOAT;//ピクセルフォーマットは32BitFLOAT型
    srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvd.Texture2D.MipLevels = 1;
    Renderer::GetDevice()->CreateShaderResourceView(depthTexture, &srvd,
        &m_ShaderResourceView);
    depthTexture->Release();


}

void DepthShadow::Uninit()
{
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

    //ライトカメラのビュー行列を作成
    D3DXVECTOR3 lightPos = D3DXVECTOR3(-30.0f, 140.0f, -20.0f);
    D3DXVECTOR3 lightTarget = D3DXVECTOR3(-30.0f, 0.0f, -50.0f);
    D3DXVECTOR3 lightUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    D3DXMatrixLookAtLH(&light.ViewMatrix, &lightPos, &lightTarget, &lightUp);

    //ライトカメラのプロジェクション行列を作成
    D3DXMatrixPerspectiveFovLH(&light.ProjectionMatrix, 1.0f,
        (float)SCREEN_WIDTH / SCREEN_HEIGHT, 5.0f, 300.0f);

    //ライト情報をセット
    Renderer::SetLight(light);

    //シャドウバッファの作成
    BeginDepth();

    //ライトカメラの行列をセット
    Renderer::SetViewMatrix(&light.ViewMatrix);
    Renderer::SetProjectionMatrix(&light.ProjectionMatrix);

    //影を落としたいオブジェクトを描画(一応地面も)
    Scene* scene = Manager::GetScene();
    for (const auto& obj : scene->GetList()[1])
    {
        if(obj->GetPass() &SHADER_SHADOW)
            obj->Draw();
    }
}

void DepthShadow::BeginDepth(void)
{
    //シャドウバッファを深度バッファに設定し、内容を1で塗りつぶしておく
    Renderer::GetDeviceContext()->OMSetRenderTargets(0, NULL, m_DepthStencilView);
    Renderer::GetDeviceContext()->ClearDepthStencilView(m_DepthStencilView,
        D3D11_CLEAR_DEPTH, 1.0f, 0);
}

