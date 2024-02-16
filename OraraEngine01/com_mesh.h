#pragma once
#include "component.h"

class Mesh :public Component
{
private:
    int m_ModelNum{};
    std::string m_Modelpas;
    FolderPass m_ModelFolder;//デバッグでアクセスするフォルダパスの指定

public:
    Mesh(std::string pas = "box.obj") :m_Modelpas(pas)
    {
        m_ModelFolder.Date = m_Modelpas;
        m_ModelFolder.Pass = "asset\\model";
        m_ModelFolder.Extension = ".obj";//表示する拡張子
        SETDATE(m_ModelFolder);
    }
    void SetModel(std::string pas);
    
    void Init() override;
    void Uninit() override;
    void Update() override;
    void Draw()override;

    const std::string GetModelpas() { return m_Modelpas; }
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(CEREAL_NVP(m_Modelpas));
    }

};


