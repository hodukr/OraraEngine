#pragma once
//新たにパスクラスをを追加したらここにIDを追加する
enum Shader
{
    SHADER_NONE                     = 0,
    SHADER_POST                     = 1 << 0,
    SHADER_ENVIRONMENTMAPPING       = 1 << 1,
    SHADER_SHADOW                   = 1 << 2,
    SHADER_CREATETEXTURE            = 1 << 3,
}; 

class Pass
{
protected:
    ID3D11DepthStencilView* m_DepthStencilView;
    ID3D11ShaderResourceView* m_ShaderResourceView;
public:
    virtual void CreatePass() {};
    virtual void Init() {};
    virtual void Uninit() {};
    virtual void Draw() {};
    virtual void Update() {};
    virtual const Shader GetPassId() = 0;

    ID3D11ShaderResourceView** GetTexture() { return &m_ShaderResourceView; }
    const static std::string GetName(Shader shader)
    {
        GetName(shader);
    }
    const static std::string GetName(int shader)
    {
        switch (shader)
        {
        case SHADER_NONE:
            break;
        case SHADER_ENVIRONMENTMAPPING:
            return "EnvironmentMapping";
            break;
        case SHADER_SHADOW:
            return "Shadow";
            break;
        default:
            return "";
            break;
        }
        return "";
    }
};
