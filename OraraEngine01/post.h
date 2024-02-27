#pragma once


class Post
{
private:
    bool m_IsPost = false;
    int  m_DefaultShader{};
    int  m_PostShader{};
    int  m_TextureNum = -1;

    ID3D11Buffer* m_VertexBuffer = NULL;

    WATER m_Water{};
    PARAMETER m_Param{};
    float m_Delta = 0.01f;
    float m_RandNum{};
    
public:
    void Init();
    void Uninit();
    void Update();
    void Draw();

    void SetIsPost(bool flag) { m_IsPost = flag; }
    void SetPostShader(int shaderNum) { m_PostShader = shaderNum; }
    void SetTextureNum(int num) { m_TextureNum = num; }
};
