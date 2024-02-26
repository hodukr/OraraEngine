#pragma once


class Post
{
private:
    bool m_IsPost = false;
    int  m_DefaultShader{};
    int  m_PostShader{};

    ID3D11Buffer* m_VertexBuffer = NULL;

    WATER m_Water{};
    float m_RandNum{};
    
public:
    void Init();
    void Uninit();
    void Update();
    void Draw();

    void SetIsPost(bool flag) { m_IsPost = flag; }
    void SetPostShader(int shaderNum) { m_PostShader = shaderNum; }
};
