﻿#pragma once
#include "component.h"
#include "com_material.h"

class Mesh :public Component
{
private:
    int m_ModelNum{};
    std::string m_Modelpas;
    Material m_Material;

public:
    Mesh(std::string pas = "box.obj") :m_Modelpas(pas) {}
    void SetModel(std::string pas);
    
    void Init() override;
    void Uninit() override;
    void Update() override;
    void Draw()override;

    Material* GetMaterial() { return &m_Material; }
    const std::string GetModelpas() { return m_Modelpas; }
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(CEREAL_NVP(m_Modelpas), CEREAL_NVP(m_Material));
    }

};


