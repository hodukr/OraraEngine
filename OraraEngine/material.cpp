#include "main.h"
#include "material.h"
#include "rendererManager.h"


void Material::Init()
{
    // マテリアル初期化
    Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

void Material::SetMaterial(Material material)
{
    ID3D11DeviceContext* deviceContext = RendererManager::Instance().GetDeviceContext();
    ID3D11Buffer* buffer = RendererManager::Instance().GetMaterialBuffer();

    deviceContext->UpdateSubresource(buffer, 0, NULL, &material, 0, 0);
}
