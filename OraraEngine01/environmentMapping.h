#pragma once
#include "pass.h"

class EnvironmentMapping :public Pass
{
private:
    ID3D11Texture2D* m_ReflectTexture{};
    ID3D11RenderTargetView* m_ReflectRenderTargetView{};
    ID3D11DepthStencilView* m_ReflectDepthStencilView{};
    ID3D11Texture2D* m_CubeReflectTexture{};
    ID3D11ShaderResourceView* m_CubeReflectShaderResourceView{};

    std::list<class GameObject*> m_DrawObj;
    D3DXVECTOR3 m_EnvMapObjPos{};
public:
    void CreatePass(DXGI_SWAP_CHAIN_DESC swapChainDesc, ID3D11Device* device) override;
    void Uninit()override;
    void Draw()override;
    const Shader GetPassId()override { return SHADER_ENVIRONMENTMAPPING; }

    void BeginCube(void);
    void SetReflectViewport(void);

    ID3D11Texture2D* GetReflectTexture(void) { return m_ReflectTexture; }
    ID3D11Texture2D* GetCubeReflectTexture(void) { return m_CubeReflectTexture; }
    ID3D11ShaderResourceView** GetCubeReflectShaderResourceView(void) { return &m_CubeReflectShaderResourceView; }

    void SetDrawObj(GameObject* drawObj) { m_DrawObj.push_back(drawObj); }
    void SetEnvMapObjPos(D3DXVECTOR3 position) { m_EnvMapObjPos = position; }
};
