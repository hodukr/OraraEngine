#pragma once

#include "shaderResource.h"
#include "pass.h"

class Renderer
{
private:

	static D3D_FEATURE_LEVEL       m_FeatureLevel;

	static ID3D11Device*           m_Device;
	static ID3D11DeviceContext*    m_DeviceContext;
	static IDXGISwapChain*         m_SwapChain;
	static ID3D11RenderTargetView* m_RenderTargetView;
	static ID3D11DepthStencilView* m_DepthStencilView;

	static ID3D11Buffer*			m_WorldBuffer;
	static ID3D11Buffer*			m_ViewBuffer;
	static ID3D11Buffer*			m_ProjectionBuffer;
	static ID3D11Buffer*			m_MaterialBuffer;
	static ID3D11Buffer*			m_LightBuffer;
	static ID3D11Buffer*            m_ParameterBuffer;
	static ID3D11Buffer*            m_PraticleBuffer;
	static ID3D11Buffer*            m_CameraBuffer;
	static ID3D11Buffer*            m_WaterBuffer;


	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;

	static ID3D11BlendState*		m_BlendState;
	static ID3D11BlendState*		m_BlendStateATC;

	static DXGI_SWAP_CHAIN_DESC m_SwapChainDesc;
public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void End();

	static void SetDepthEnable(bool Enable);
	static void SetATCEnable(bool Enable);
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(D3DXMATRIX* WorldMatrix);
	static void SetViewMatrix(D3DXMATRIX* ViewMatrix);
	static void SetProjectionMatrix(D3DXMATRIX* ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
    static void SetLight(LIGHT Light);

	static void SetParameter(PARAMETER param);
    static void SetPraticle(PRATICLE Praticle);
    static void SetCameraPosition(D3DXVECTOR3 cameraPosition);
    static void SetWater(WATER water);

	static ID3D11Device* GetDevice( void ){ return m_Device; }
	static ID3D11DeviceContext* GetDeviceContext( void ){ return m_DeviceContext; }
	static IDXGISwapChain* GetSwapChain() { return m_SwapChain; }
	static ID3D11DepthStencilView* GetDepthStencilView() { return m_DepthStencilView; }
	static DXGI_SWAP_CHAIN_DESC GetSwapChainDesc() { return m_SwapChainDesc; }
	

	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);

	static void SetDefaultViewport(void);

	
};
