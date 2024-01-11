#pragma once
#include <string>
#include"component.h"
#include <cereal/types/string.hpp>
class Material: public Component
{
private:
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	std::string m_NameVS;
	std::string m_NamePS;
	D3DXCOLOR m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
public:
    Material() : m_NameVS("vertexLightingVS.cso"),m_NamePS("vertexLightingPS.cso"){}
    void Init();
    void Init(const char* VS, const char* PS);
	void Uninit();
	void Update();
	void Draw();

	void SetColor(D3DXCOLOR Color) { m_Color = Color; }

    void SetShaderVS(std::string Shader);
    void SetShaderPS(std::string Shader);
    std::string GetShaderVSName() { return m_NameVS; }
    std::string GetShaderPSName() { return m_NamePS; }

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(CEREAL_NVP(m_NameVS), CEREAL_NVP(m_NamePS));
    }
};
