#pragma once
#include "pass.h"
#include <string>
#include <unordered_map>

class DepthShadow :public Pass
{
private:
    ID3D11DepthStencilView* m_DepthShadowDepthStencilView;
    ID3D11ShaderResourceView* m_DepthShadowShaderResourceView;

    std::unordered_map<int, class GameObject*> m_ShadowDrawObj{};
    std::unordered_map<int, std::string> m_SelectDrawObj{};
    int m_DrawObjNum{};
   /* D3DXVECTOR3 m_EnvMapObjPos{}; 
    std::string m_SelectPosObj{};*/
    
    void BeginDepth(void);
public:
    void CreatePass() override;
    void Uninit()override;
    void Draw()override;
    void Update() override;
    const Shader GetPassId()override { return SHADER_SHADOW; }

    ID3D11ShaderResourceView** GetDepthShadowTexture() { return &m_DepthShadowShaderResourceView; }

    //void SetDrawObj(GameObject* drawObj) { m_DrawObj.push_back(drawObj); }
    //void SetEnvMapObjPos(D3DXVECTOR3 position) { m_EnvMapObjPos = position; }
};
