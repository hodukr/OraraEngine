#pragma once


class Post
{
private:
    bool m_IsPost = false;
    int  m_DefaultShader{};
    int  m_PostShader{};
    int  m_TextureNum = -1;
    bool m_IsWipe = false;
    bool m_Debug = false;

    ID3D11Buffer* m_VertexBuffer = NULL;

    WATER m_Water{};
    PARAMETER m_Param{};
    float m_WipeSpeed = 0.01f;
    float m_RandNum{};
    
public:
    void Init();
    void Uninit();
    void Update();
    void Draw();

    void SetIsPost(bool flag) { m_IsPost = flag; }
    void SetDebug(bool flag) { m_Debug = flag; }
    void SetPostShader(int shaderNum) { m_PostShader = shaderNum; }
    void SetTextureNum(int num) { m_TextureNum = num; }
    void SetWipeSpeed(float speed) { m_WipeSpeed = speed; }
    void SetIsWipe(bool flag) { m_IsWipe = flag; }
    void SetThreshold(int value) { m_Param.dissolveThreshold = value; }
    bool GetIsWipe() { return m_IsWipe; }

};
