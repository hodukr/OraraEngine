#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "postPass.h"

void PostPass::CreatePass(DXGI_SWAP_CHAIN_DESC swapChainDesc, ID3D11Device* device)
{
    //テクスチャー作成 
    ID3D11Texture2D* ppTexture = NULL;
    D3D11_TEXTURE2D_DESC td;
    ZeroMemory(&td, sizeof(td));

    td.Width = swapChainDesc.BufferDesc.Width;
    td.Height = swapChainDesc.BufferDesc.Height;

    td.MipLevels = 1;
    td.ArraySize = 1;

    td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    td.SampleDesc = swapChainDesc.SampleDesc;
    td.Usage = D3D11_USAGE_DEFAULT;

    //使用法のフラグを設定 
    td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    td.CPUAccessFlags = 0;
    td.MiscFlags = 0;


    device->CreateTexture2D(&td, NULL, &ppTexture);

    //レンダーターゲットビューの作成 
    D3D11_RENDER_TARGET_VIEW_DESC rtvd;
    ZeroMemory(&rtvd, sizeof(rtvd));
    rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    device->CreateRenderTargetView(ppTexture, &rtvd, &m_PPRenderTargetView);

    //シェーダーリソースビュー作成 
    D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
    ZeroMemory(&srvd, sizeof(srvd));
    srvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvd.Texture2D.MipLevels = 1;
    device->CreateShaderResourceView(ppTexture, &srvd, &m_PPShaderResourceView);

    ppTexture->Release();
}

void PostPass::Uninit()
{
    m_PPRenderTargetView->Release();
    m_PPShaderResourceView->Release();
}

void PostPass::BeginPP()
{
    Renderer::GetDeviceContext()->OMSetRenderTargets(1, &m_PPRenderTargetView, Renderer::GetDepthStencilView());

    float clearColor[4] = { 0.0f,0.5f,0.0f,1.0f };
    Renderer::GetDeviceContext()->ClearRenderTargetView(m_PPRenderTargetView, clearColor);
    Renderer::GetDeviceContext()->ClearDepthStencilView(Renderer::GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}
