#pragma once
enum Shader
{
    SHADER_NONE = 0,
    SHADER_POST,
    SHADER_ENVIRONMENTMAPPING,
};

class Pass
{
public:
    virtual void CreatePass(DXGI_SWAP_CHAIN_DESC swapChainDesc, ID3D11Device* device) {};
    virtual void Uninit() {};
    virtual void Draw() {};
    virtual const Shader GetPassId() = 0;
};
