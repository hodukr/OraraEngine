#pragma once

#include "singleton.h"

struct VERTEX_3D
{
    D3DXVECTOR3 Position;
    D3DXVECTOR3 Normal;
    D3DXVECTOR4 Diffuse;
    D3DXVECTOR2 TexCoord;
};

struct LIGHT
{
    BOOL		Enable;
    BOOL		Dummy[3];
    D3DXVECTOR4	Direction;
    D3DXCOLOR	Diffuse;
    D3DXCOLOR	Ambient;
};

class RendererManager final
{
private:
    D3D_FEATURE_LEVEL       m_FeatureLevel;

    ID3D11Device* m_Device;
    ID3D11DeviceContext* m_DeviceContext;
    IDXGISwapChain* m_SwapChain;
    ID3D11RenderTargetView* m_RenderTargetView;
    ID3D11DepthStencilView* m_DepthStencilView;

    ID3D11Buffer* m_WorldBuffer;
    ID3D11Buffer* m_ViewBuffer;
    ID3D11Buffer* m_ProjectionBuffer;
    ID3D11Buffer* m_MaterialBuffer;
    ID3D11Buffer* m_LightBuffer;


    ID3D11DepthStencilState* m_DepthStateEnable;
    ID3D11DepthStencilState* m_DepthStateDisable;

    ID3D11BlendState* m_BlendState;
    ID3D11BlendState* m_BlendStateATC;

    Singleton(RendererManager);
public:

    void Init();
    void Uninit();
    void Begin();
    void End();
    void SetDepthEnable(bool enable);
    void SetATCEnable(bool enable);
    void SetWorldViewProjection2D();
    void SetWorldMatrix(D3DXMATRIX* worldMatrix);
    void SetViewMatrix(D3DXMATRIX* viewMatrix);
    void SetProjectionMatrix(D3DXMATRIX* projectionMatrix);
    //void SetMaterial(MATERIAL Material);
    void SetLight(LIGHT Light);
    ID3D11Device* GetDevice(void) { return m_Device; }
    ID3D11DeviceContext* GetDeviceContext(void) { return m_DeviceContext; }
    ID3D11Buffer* GetMaterialBuffer() { return m_MaterialBuffer; }
    void CreateVertexShader(ID3D11VertexShader** vertexShader, ID3D11InputLayout** vertexLayout, const char* fileName);
    void CreatePixelShader(ID3D11PixelShader** pixelShader, const char* fileName);
};
