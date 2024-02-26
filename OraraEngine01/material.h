#pragma once
#include <string>
#include <cereal/types/string.hpp>
class Material
{
private:
    int m_ShaderNum;
    std::string m_ShaderName;
    int m_TextureNum = -1;
    std::string m_TextureName;
	D3DXCOLOR m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
public:
    Material(std::string passname = "vertexLighting") : m_ShaderName(passname), m_TextureName("") {}
    void Init();
	void Draw();

    void SetColor(D3DXCOLOR Color) { m_Color = Color; }
    D3DXCOLOR GetColor() { return m_Color; }
    std::string GetFileName() { return m_ShaderName; }
    int  GetShaderNum() { return m_ShaderNum; }
    void SetShader(std::string file);
    void SetTexture(std::string name);
    std::string GetTexture() { return m_TextureName; }
    template<class Archive>
    void serialize(Archive& archive)
    {
        try
        {
            archive(CEREAL_NVP(m_ShaderName), CEREAL_NVP(m_Color), CEREAL_NVP(m_TextureName));
        }
        catch (const std::exception&)
        {

        }
    }
};
