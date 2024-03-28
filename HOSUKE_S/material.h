#pragma once
class Material
{
private:
    int m_ShaderNum{};
    string m_ShaderName{};
    int m_TextureNum = -1;
    string m_TextureName{};
    string m_Extension{};
	D3DXCOLOR m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
public:
    Material(string passname = "vertexLighting") : m_ShaderName(passname), m_TextureName(""), m_Extension("") {}
    void Init();
	void Draw();

    void SetColor(D3DXCOLOR Color) { m_Color = Color; }
    D3DXCOLOR GetColor() { return m_Color; }
    string GetFileName() { return m_ShaderName; }
    int  GetShaderNum() { return m_ShaderNum; }
    void SetShader(string file);
    void SetTexture(string name, string extension);
    string GetTextureName() { return m_TextureName; }
    int GetTextureNum(){ return m_TextureNum; }
    template<class Archive>
    void serialize(Archive& archive)
    {
        try
        {
            archive(CEREAL_NVP(m_ShaderName), CEREAL_NVP(m_Color), CEREAL_NVP(m_TextureName), CEREAL_NVP(m_Extension));
        }
        catch (const exception&)
        {

        }
    }
};
