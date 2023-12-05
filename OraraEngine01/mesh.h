#pragma once
#include "component.h"



class Mesh :public Component
{
private:
    D3DMATRIX m_Matrix;
    class Model* m_Model;
    std::string m_Modelpas;

public:
    Mesh(std::string pas = "asset\\model\\box.obj") :m_Modelpas(pas) {}
    void SetModelPas(std::string pas)
    {
        m_Modelpas = pas;
    }

    void Init() override;
    void Uninit() override;
    void Update() override;
    void Draw()override;

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(CEREAL_NVP(m_Modelpas));
    }

};

//CEREAL_REGISTER_DYNAMIC_INIT(Mesh)
CEREAL_REGISTER_TYPE(Mesh)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Mesh)

