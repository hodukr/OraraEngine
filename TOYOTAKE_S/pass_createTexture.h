#pragma once
#include "pass.h"

class CreateTexture :public Pass
{
private:
    ID3D11RenderTargetView* m_RenderTargetView{};
public:
    void CreatePass() override;
    void Uninit()override;
    const Shader GetPassId()override { return SHADER_CREATETEXTURE; }

    void BeginCT();
};
