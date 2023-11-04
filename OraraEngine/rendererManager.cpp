#include "main.h"
#include "rendererManager.h"
#include "material.h"
#include "light.h"
#include <io.h>


void RendererManager::Init()
{
    HRESULT hr = S_OK;

    // デバイス、スワップチェーン作成
    DXGI_SWAP_CHAIN_DESC swapChainDesc{};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = SCREEN_WIDTH;
    swapChainDesc.BufferDesc.Height = SCREEN_HEIGHT;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = GetWindow();
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;

    hr = D3D11CreateDeviceAndSwapChain(NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        0,
        NULL,
        0,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &m_SwapChain,
        &m_Device,
        &m_FeatureLevel,
        &m_DeviceContext);

    // レンダーターゲットビュー作成
    ID3D11Texture2D* renderTarget{};
    m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&renderTarget);
    m_Device->CreateRenderTargetView(renderTarget, NULL, &m_RenderTargetView);
    renderTarget->Release();


    // デプスステンシルバッファ作成
    ID3D11Texture2D* depthStencile{};
    D3D11_TEXTURE2D_DESC textureDesc{};
    textureDesc.Width = swapChainDesc.BufferDesc.Width;
    textureDesc.Height = swapChainDesc.BufferDesc.Height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_D16_UNORM;
    textureDesc.SampleDesc = swapChainDesc.SampleDesc;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;
    m_Device->CreateTexture2D(&textureDesc, NULL, &depthStencile);

    // デプスステンシルビュー作成
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
    depthStencilViewDesc.Format = textureDesc.Format;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Flags = 0;
    m_Device->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &m_DepthStencilView);
    depthStencile->Release();


    m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);


    // ビューポート設定
    D3D11_VIEWPORT viewport;
    viewport.Width = (FLOAT)SCREEN_WIDTH;
    viewport.Height = (FLOAT)SCREEN_HEIGHT;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    m_DeviceContext->RSSetViewports(1, &viewport);



    // ラスタライザステート設定
    D3D11_RASTERIZER_DESC rasterizerDesc{};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.DepthClipEnable = TRUE;
    rasterizerDesc.MultisampleEnable = FALSE;

    ID3D11RasterizerState* rs;
    m_Device->CreateRasterizerState(&rasterizerDesc, &rs);

    m_DeviceContext->RSSetState(rs);


    // ブレンドステート設定
    D3D11_BLEND_DESC blendDesc{};
    blendDesc.AlphaToCoverageEnable = FALSE;
    blendDesc.IndependentBlendEnable = FALSE;
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    m_Device->CreateBlendState(&blendDesc, &m_BlendState);

    blendDesc.AlphaToCoverageEnable = TRUE;
    m_Device->CreateBlendState(&blendDesc, &m_BlendStateATC);

    float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    m_DeviceContext->OMSetBlendState(m_BlendState, blendFactor, 0xffffffff);


    // デプスステンシルステート設定
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    depthStencilDesc.StencilEnable = FALSE;

    m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateEnable);//深度有効ステート

    //depthStencilDesc.DepthEnable = FALSE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateDisable);//深度無効ステート

    m_DeviceContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);


    // サンプラーステート設定
    D3D11_SAMPLER_DESC samplerDesc{};
    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MaxAnisotropy = 4;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    ID3D11SamplerState* samplerState{};
    m_Device->CreateSamplerState(&samplerDesc, &samplerState);

    m_DeviceContext->PSSetSamplers(0, 1, &samplerState);

    // 定数バッファ生成
    D3D11_BUFFER_DESC bufferDesc{};
    bufferDesc.ByteWidth = sizeof(D3DXMATRIX);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = sizeof(float);

    m_Device->CreateBuffer(&bufferDesc, NULL, &m_WorldBuffer);
    m_DeviceContext->VSSetConstantBuffers(0, 1, &m_WorldBuffer);

    m_Device->CreateBuffer(&bufferDesc, NULL, &m_ViewBuffer);
    m_DeviceContext->VSSetConstantBuffers(1, 1, &m_ViewBuffer);

    m_Device->CreateBuffer(&bufferDesc, NULL, &m_ProjectionBuffer);
    m_DeviceContext->VSSetConstantBuffers(2, 1, &m_ProjectionBuffer);


    bufferDesc.ByteWidth = sizeof(Material);

    m_Device->CreateBuffer(&bufferDesc, NULL, &m_MaterialBuffer);
    m_DeviceContext->VSSetConstantBuffers(3, 1, &m_MaterialBuffer);
    m_DeviceContext->PSSetConstantBuffers(3, 1, &m_MaterialBuffer);


    bufferDesc.ByteWidth = sizeof(Light);

    m_Device->CreateBuffer(&bufferDesc, NULL, &m_LightBuffer);
    m_DeviceContext->VSSetConstantBuffers(4, 1, &m_LightBuffer);
    m_DeviceContext->PSSetConstantBuffers(4, 1, &m_LightBuffer);


    // ライト初期化
    Light light{};
    light.Init();
    light.SetLight(light);

    Material material{};
    material.Init();
    material.SetMaterial(material);
}

void RendererManager::Uninit()
{
    m_WorldBuffer->Release();
    m_ViewBuffer->Release();
    m_ProjectionBuffer->Release();
    m_LightBuffer->Release();
    m_MaterialBuffer->Release();


    m_DeviceContext->ClearState();
    m_RenderTargetView->Release();
    m_SwapChain->Release();
    m_DeviceContext->Release();
    m_Device->Release();

}




void RendererManager::Begin()
{
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, clearColor);
    m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}



void RendererManager::End()
{
    m_SwapChain->Present(1, 0);
}




void RendererManager::SetDepthEnable(bool enable)
{
    if (enable)
        m_DeviceContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);
    else
        m_DeviceContext->OMSetDepthStencilState(m_DepthStateDisable, NULL);

}



void RendererManager::SetATCEnable(bool enable)
{
    float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

    if (enable)
        m_DeviceContext->OMSetBlendState(m_BlendStateATC, blendFactor, 0xffffffff);
    else
        m_DeviceContext->OMSetBlendState(m_BlendState, blendFactor, 0xffffffff);

}

void RendererManager::SetWorldViewProjection2D()
{
    D3DXMATRIX world;
    D3DXMatrixIdentity(&world);
    D3DXMatrixTranspose(&world, &world);

    m_DeviceContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &world, 0, 0);

    D3DXMATRIX view;
    D3DXMatrixIdentity(&view);
    D3DXMatrixTranspose(&view, &view);
    m_DeviceContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &view, 0, 0);

    D3DXMATRIX projection;
    D3DXMatrixOrthoOffCenterLH(&projection, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
    D3DXMatrixTranspose(&projection, &projection);
    m_DeviceContext->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &projection, 0, 0);

}

void RendererManager::SetWorldMatrix(D3DXMATRIX* worldMatrix)
{
    D3DXMATRIX world;
    D3DXMatrixTranspose(&world, worldMatrix);
    m_DeviceContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &world, 0, 0);
}

void RendererManager::SetViewMatrix(D3DXMATRIX* viewMatrix)
{
    D3DXMATRIX view;
    D3DXMatrixTranspose(&view, viewMatrix);
    m_DeviceContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &view, 0, 0);
}

void RendererManager::SetProjectionMatrix(D3DXMATRIX* projectionMatrix)
{
    D3DXMATRIX projection;
    D3DXMatrixTranspose(&projection, projectionMatrix);
    m_DeviceContext->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &projection, 0, 0);
}


//ここから分ける
//void RendererManager::SetMaterial(MATERIAL Material)
//{
//    m_DeviceContext->UpdateSubresource(m_MaterialBuffer, 0, NULL, &Material, 0, 0);
//}

//void RendererManager::SetLight(LIGHT light)
//{
//    m_DeviceContext->UpdateSubresource(m_LightBuffer, 0, NULL, &light, 0, 0);
//}


void RendererManager::CreateVertexShader(ID3D11VertexShader** vertexShader, ID3D11InputLayout** vertexLayout, const char* fileName)
{

    FILE* file;
    long int fsize;

    file = fopen(fileName, "rb");
    assert(file);

    fsize = _filelength(_fileno(file));
    unsigned char* buffer = new unsigned char[fsize];
    fread(buffer, fsize, 1, file);
    fclose(file);

    m_Device->CreateVertexShader(buffer, fsize, NULL, vertexShader);


    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    UINT numElements = ARRAYSIZE(layout);

    m_Device->CreateInputLayout(layout,
        numElements,
        buffer,
        fsize,
        vertexLayout);

    delete[] buffer;
}


void RendererManager::CreatePixelShader(ID3D11PixelShader** pixelShader, const char* fileName)
{
    FILE* file;
    long int fsize;

    file = fopen(fileName, "rb");
    assert(file);

    fsize = _filelength(_fileno(file));
    unsigned char* buffer = new unsigned char[fsize];
    fread(buffer, fsize, 1, file);
    fclose(file);

    m_Device->CreatePixelShader(buffer, fsize, NULL, pixelShader);

    delete[] buffer;
}


