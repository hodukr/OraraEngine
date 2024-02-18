#pragma once
//新たにパスクラスをを追加したらここにIDを追加する
enum Shader
{
    SHADER_NONE = 0,
    SHADER_POST,
    SHADER_ENVIRONMENTMAPPING,
    SHADER_SHADOW,
};

class Pass
{
public:
    virtual void CreatePass() {};
    virtual void Init() {};
    virtual void Uninit() {};
    virtual void Draw() {};
    virtual void Update() {};
    virtual const Shader GetPassId() = 0;
};
