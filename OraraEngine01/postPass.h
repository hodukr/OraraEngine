#pragma once
#include "pass.h"

class PostPass :public Pass
{
private:
    ID3D11RenderTargetView* m_PPRenderTargetView{};
    ID3D11ShaderResourceView* m_PPShaderResourceView{};
public:
    void CreatePass() override;
    void Uninit()override;
    const Shader GetPassId()override { return SHADER_POST; }

    void BeginPP();
  
    ID3D11ShaderResourceView** GetPPTexture() { return &m_PPShaderResourceView; }//レンダリングテクスチャのポインタを取得  
};
