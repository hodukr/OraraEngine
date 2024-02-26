#pragma once
#include "pass.h"
#include <string>
#include <unordered_map>

class EnvironmentMapping :public Pass
{
private:
    ID3D11Texture2D* m_ReflectTexture{};
    ID3D11RenderTargetView* m_ReflectRenderTargetView{};
    ID3D11Texture2D* m_CubeReflectTexture{};
    D3DXVECTOR3 m_EnvMapObjPos{};
    std::string m_SelectPosObj{};
    std::unordered_map<int, std::string> m_SelectDrawObj{};
    int m_DrawObjNum{};

    void BeginCube(void);
    void SetReflectViewport(void);
public:
    void CreatePass() override;
    void Uninit()override;
    void Draw()override;
    const Shader GetPassId()override { return SHADER_ENVIRONMENTMAPPING; }


    ID3D11Texture2D* GetReflectTexture(void) { return m_ReflectTexture; }
    ID3D11Texture2D* GetCubeReflectTexture(void) { return m_CubeReflectTexture; }
  
    void SetEnvMapObjPos(D3DXVECTOR3 position) { m_EnvMapObjPos = position; }
};
