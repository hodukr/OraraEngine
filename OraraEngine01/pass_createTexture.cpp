#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "pass_createTexture.h"

void CreateTexture::CreatePass()
{
    //�e�N�X�`���[�쐬 
    ID3D11Texture2D* ppTexture = NULL;
    D3D11_TEXTURE2D_DESC td;
    ZeroMemory(&td, sizeof(td));

    td.Width = Renderer::GetSwapChainDesc().BufferDesc.Width;
    td.Height = Renderer::GetSwapChainDesc().BufferDesc.Height;

    td.MipLevels = 1;
    td.ArraySize = 1;

    td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    td.SampleDesc = Renderer::GetSwapChainDesc().SampleDesc;
    td.Usage = D3D11_USAGE_DEFAULT;

    //�g�p�@�̃t���O��ݒ� 
    td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    td.CPUAccessFlags = 0;
    td.MiscFlags = 0;

    Renderer::GetDevice()->CreateTexture2D(&td, NULL, &ppTexture);

    //�����_�[�^�[�Q�b�g�r���[�̍쐬 
    D3D11_RENDER_TARGET_VIEW_DESC rtvd;
    ZeroMemory(&rtvd, sizeof(rtvd));
    rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    Renderer::GetDevice()->CreateRenderTargetView(ppTexture, &rtvd, &m_RenderTargetView);

    //�V�F�[�_�[���\�[�X�r���[�쐬 
    D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
    ZeroMemory(&srvd, sizeof(srvd));
    srvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvd.Texture2D.MipLevels = 1;
    Renderer::GetDevice()->CreateShaderResourceView(ppTexture, &srvd, &m_ShaderResourceView);

    ppTexture->Release();
}

void CreateTexture::Uninit()
{
    m_RenderTargetView->Release();
}

void CreateTexture::BeginCT()
{
    Renderer::GetDeviceContext()->OMSetRenderTargets(1, &m_RenderTargetView, Renderer::GetDepthStencilView());

    float clearColor[4] = { 0.0f,0.5f,0.0f,1.0f };
    Renderer::GetDeviceContext()->ClearRenderTargetView(m_RenderTargetView, clearColor);
    Renderer::GetDeviceContext()->ClearDepthStencilView(Renderer::GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}
