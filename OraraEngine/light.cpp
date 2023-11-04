#include "main.h"
#include "light.h"
#include "rendererManager.h"


void Light::Init()
{
    Enable = true;
    Direction = D3DXVECTOR4(0.5f, -1.0f, 0.8f, 0.0f);
    D3DXVec4Normalize(&Direction, &Direction);
    Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
    Diffuse = D3DXCOLOR(1.5f, 1.5f, 1.5f, 1.0f);
}

void Light::SetLight(Light light)
{
    ID3D11DeviceContext* deviceContext = RendererManager::Instance().GetDeviceContext();
    ID3D11Buffer* buffer = RendererManager::Instance().GetLightBuffer();

    deviceContext->UpdateSubresource(buffer, 0, NULL, &light, 0, 0);
}
