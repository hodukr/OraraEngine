#pragma once
#include "com_common.h"
#include "material.h"
#include "reflection.h"
#include "pass.h"

class GameObject
{
private:
    string m_ObjctName;
    string m_Tag;
    DrawLayer m_DrawLayer{ GAME_OBJECT_DRAW_LAYER_NONE };
    bool m_Destroy = false;
    int m_RenderingPass{};
    list<unique_ptr<Component>> m_Component;
    int m_Version = 0;
    int m_UseShaderNum = -1;//使用するシェーダー番号
    unique_ptr<Material> m_Material{};
    class ElapsedTimeTracker* m_Timer =nullptr;
    double m_DeletTime;
public:
    Transform* m_Transform = nullptr;
    GameObject() {
        m_ObjctName = "GameObject";
        m_Tag = "Untagged";
        m_UseShaderNum = -1;
        m_DrawLayer = GAME_OBJECT_DRAW_LAYER_NONE;
        m_RenderingPass = SHADER_NONE;
    }
    void SetDestroy(float time =0.0f);

    bool Destroy();
   

    void Init() {
        m_Transform = GetComponent<Transform>();
        if (!m_Transform)
        {
            unique_ptr<Transform> transform = make_unique<Transform>();
            transform.get()->SetGameObejct(this);
            m_Transform = transform.get();
            m_Component.push_back(move(transform));
        }
        if (!m_Material)
        {
            m_Material = make_unique<Material>();
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
            if (component->GetEnable())component->EditorUpdate();
        }
    }


    void Update()
    {
        for (const auto& component : m_Component)
        {
            if (component->GetEnable())component->Update();
        }

    };

    void EditorDraw()
    {
        if (m_Material)m_Material->Draw();

        for (const auto& component : m_Component)
        {
            if (component->GetEnable())component->EditorDraw();
        }
    };

    void Draw()
    {
        if (m_Material)m_Material->Draw();

        for (const auto& component : m_Component)
        {
            if (component->GetEnable())component->Draw();
        }
    };

    void SetName(string name);

    string GetName()
    {
        return m_ObjctName;
    }

    void SetTag(string tag)
    {
        m_Tag = tag;
    }

    const string GetTag()
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
        unique_ptr<Component> com(static_cast<Component*>(component));
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
        m_Component.push_back(move(com));

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

    list<unique_ptr<Component>>* GetList()
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
    void SetMaterial(unique_ptr<Material> material)
    {
        m_Material = move(material);
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
        catch (const exception&)
        {

        }
        
    }
};
