#pragma once
#include "pass.h"
#include <string>
#include <unordered_map>
#include <list>

class DepthShadow :public Pass
{
private:
  /*  ID3D11DepthStencilView* m_DepthShadowDepthStencilView;
    ID3D11ShaderResourceView* m_DepthShadowShaderResourceView;*/

    //std::unordered_map<int, class GameObject*> m_ShadowDrawObj{};
    //std::list<class GameObject*> m_ShadowDrawObj{};
    std::unordered_map<int, std::string> m_SelectDrawObj{};
    int m_DrawObjNum{};
    
    void BeginDepth(void);
public:
    void CreatePass() override;
    void Init()override;
    void Uninit()override;
    void Draw()override;
    void Update() override;
    const Shader GetPassId()override { return SHADER_SHADOW; }

    //ID3D11ShaderResourceView** GetDepthShadowTexture() { return &m_DepthShadowShaderResourceView; }
};
