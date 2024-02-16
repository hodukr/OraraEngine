#pragma once
#include <string>
#include <cereal/types/string.hpp>
class Material
{
private:
    int m_ShaderNum;
    std::string m_ShaderName;
	D3DXCOLOR m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
public:
    Material() : m_ShaderName("vertexLighting"){}
    void Init();

	void Draw();

	void SetColor(D3DXCOLOR Color) { m_Color = Color; }
    std::string GetFileName() { return m_ShaderName; }
    void SetShader(std::string file);
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(CEREAL_NVP(m_ShaderName));
    }
};
