#pragma once

#include <list>
#include <Vector>
#include <cereal/cereal.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/memory.hpp>  
#include <string>
#include "main.h"
#include "com_common.h"
#include "material.h"
#include "reflection.h"
#include "pass.h"

class GameObject
{
private:
    std::string m_ObjctName;
    std::string m_Tag;
    DrawLayer m_DrawLayer{ GAME_OBJECT_DRAW_LAYER_NONE };
    bool m_Destroy = false;
    int m_RenderingPass{};
    std::list<std::unique_ptr<Component>> m_Component;
    int m_Version = 0;
    int m_UseShaderNum = -1;//使用するシェーダー番号
    std::unique_ptr<Material> m_Material{};
public:
    Transform* m_Transform = nullptr;
    GameObject() {
        m_ObjctName = "GameObject";
        m_Tag = "NoneTag";
        m_UseShaderNum = -1;
        m_DrawLayer = GAME_OBJECT_DRAW_LAYER_NONE;
        m_RenderingPass = SHADER_NONE;
    }
    void SetDestroy() { m_Destroy = true; }

    bool Destroy()
    {
        if (m_Destroy)
        {
            Uninit();
            //delete this;
            return true;
        }
        else
        {
            m_Component.remove_if([](const std::unique_ptr<Component>& component) {return component->Destroy(); });//ラムダ式
            return false;
        }
    }

    void Init() {
        m_Transform = GetComponent<Transform>();
        if (!m_Transform)
        {
            std::unique_ptr<Transform> transform = std::make_unique<Transform>();
            transform.get()->SetGameObejct(this);
            m_Transform = transform.get();
            m_Component.push_back(std::move(transform));
        }

        if (!m_Material)
        {
            m_Material = std::make_unique<Material>();
        }
            m_Material->Init();

        for (auto& component : m_Component)
        {
            component.get()->SetGameObejct(this);
            component.get()->Init();
        }

    };

    void Uninit()
    {
        for (const auto& component : m_Component)
        {
            component->Uninit();
        }

        m_Component.clear();
    };

    void EditorUpdate()
    {
        for (const auto& component : m_Component)
        {
            component->EditorUpdate();
        }
    }


    void Update()
    {
        for (const auto& component : m_Component)
        {
            component->Update();
        }

    };

    void Draw()
    {
        if (m_Material)m_Material->Draw();

        for (const auto& component : m_Component)
        {
            component->Draw();
        }
    };

    void SetName(std::string name);

    std::string GetName()
    {
        return m_ObjctName;
    }

    void SetTag(std::string tag)
    {
        m_ObjctName = tag;
    }

    const std::string GetTag()
    {
        return m_Tag;
    }

    void SetDrawLayer(DrawLayer pram)
    {
        m_DrawLayer = pram;
    }

    DrawLayer GetDrawLayer()
    {
        return m_DrawLayer;
    }

    template<typename T>
    T* AddComponent()
    {
        Component* component = new T;
        AddComponent(component);

        return dynamic_cast<T*>(m_Component.back().get());
    }

    Component* AddComponent(void* component)
    {
        std::unique_ptr<Component> com(static_cast<Component*>(component));
        if (m_DrawLayer == GAME_OBJECT_DRAW_LAYER_NONE)
        {
            if (com->GetDrawLayer() != GAME_OBJECT_DRAW_LAYER_NONE)
            {
                m_DrawLayer = com->GetDrawLayer();
            }
        }
        com->SetObjectName(m_ObjctName);
        com->SetGameObejct(this);
        com->Init();
        m_Component.push_back(std::move(com));

        return m_Component.back().get();

    }

    template<typename T>//テンプレート関数
    T* GetComponent()
    {
        for (auto& componet : m_Component)
        {
            //メモリをくうのであまり使わないほうがいい
            if (typeid(*componet) == typeid(T))//型を調べる(RTTI動的型情報)
            {
                return dynamic_cast<T*>(componet.get());
            }
        }

        return nullptr;
    }

    std::list<std::unique_ptr<Component>>* GetList()
    {
        return &m_Component;
    }

    void SetVersion(int version)
    {
        m_Version = version;
    }

    int GetVersion() { return m_Version; }

    void SetPass(int shader) { m_RenderingPass = shader; }
    int GetPass() { return m_RenderingPass; }

    Material* GetMaterial() { return m_Material.get(); }
    void SetMaterial(std::unique_ptr<Material> material)
    {
        m_Material = std::move(material);
    }

    template<class Archive>
    void serialize(Archive& archive)
    {
        try
        {
            archive(
                CEREAL_NVP(m_ObjctName),
                CEREAL_NVP(m_Tag),
                CEREAL_NVP(m_Component),
                CEREAL_NVP(m_Material),
                CEREAL_NVP(m_RenderingPass),
                CEREAL_NVP(m_DrawLayer)
            );
        }
        catch (const std::exception&)
        {

        }
        
    }
};
