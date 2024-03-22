#pragma once
#include "pass.h"


class DepthShadow :public Pass
{
private:
    unordered_map<int, string> m_SelectDrawObj{};
    int m_DrawObjNum{};
    
    void BeginDepth(void);
public:
    void CreatePass() override;
    void Uninit()override;
    void Draw()override;
    const Shader GetPassId()override { return SHADER_SHADOW; }
};
