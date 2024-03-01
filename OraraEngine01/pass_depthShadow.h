#pragma once
#include "pass.h"
#include <string>
#include <unordered_map>
#include <list>

class DepthShadow :public Pass
{
private:
    std::unordered_map<int, std::string> m_SelectDrawObj{};
    int m_DrawObjNum{};
    
    void BeginDepth(void);
public:
    void CreatePass() override;
    void Uninit()override;
    void Draw()override;
    const Shader GetPassId()override { return SHADER_SHADOW; }
};
