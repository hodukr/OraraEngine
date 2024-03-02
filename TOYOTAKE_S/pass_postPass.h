#pragma once
#include "pass.h"

class PostPass :public Pass
{
private:
    ID3D11RenderTargetView* m_PPRenderTargetView{};
public:
    void CreatePass() override;
    void Uninit()override;
    const Shader GetPassId()override { return SHADER_POST; }

    void BeginPP();
};
