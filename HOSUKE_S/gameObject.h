﻿#pragma once

#include <list>
#include <Vector>
#include <cereal/cereal.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/memory.hpp>  
#include <string>
#include "main.h"
#include "com_common.h"
class GameObject
{
private:
    std::string m_ObjctName;
    std::string m_Tag;
    bool m_Destroy = false;
    std::list<std::unique_ptr<Component>> m_Component;

public:
    Transform* m_Transform = nullptr;
    GameObject() {
        m_ObjctName = "GameObject";
        m_Tag = "NoneTag";
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
            return false;
        }
    }

    virtual void Init() {
        m_Transform = GetComponent<Transform>();
        if (!m_Transform)
        {
            std::unique_ptr<Transform> transform = std::make_unique<Transform>();
            transform.get()->SetGameObejct(this);
            m_Transform = transform.get();
            m_Component.push_back(std::move(transform));
        }


        for (auto& component : m_Component)
        {
            component.get()->SetGameObejct(this);
            component.get()->Init();
        }

    };

    virtual void Uninit()
    {
        for (const auto& component : m_Component)
        {
            component->Uninit();
        }

        m_Component.clear();
    };

    virtual void Update()
    {
        for (const auto& component : m_Component)
        {
            component->Update();
            component.get_deleter();
        }
        m_Component.remove_if([](const std::unique_ptr<Component>& component) {return component->Destroy(); });//ラムダ式

    };

    virtual void Draw()
    {
        for (const auto& component : m_Component)
        {
            component->Draw();
        }
    };

    void SetName(std::string name)
    {
        m_ObjctName = name;
    }

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

    template<typename T>
    T* AddComponent()
    {
        std::unique_ptr<Component> component = std::make_unique<T>();
        component->SetObjectName(m_ObjctName);
        component->SetGameObejct(this);
        component->Init();
        m_Component.push_back(std::move(component));

        return dynamic_cast<T*>(m_Component.back().get());
    }

    void AddComponent(void* component)
    {
        std::unique_ptr<Component> com(static_cast<Component*>(component));
        com->SetObjectName(m_ObjctName);
        com->SetGameObejct(this);
        com->Init();
        m_Component.push_back(std::move(com));

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

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(CEREAL_NVP(m_ObjctName), CEREAL_NVP(m_Tag), CEREAL_NVP(m_Component));
    }
};

