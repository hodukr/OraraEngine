#pragma once
#include <string>
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>

class Component
{
protected:
    std::string m_ObjectName;
    class GameObject* m_GameObject;
    bool m_Destroy = false;
public:
    Component();
	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};
    void SetObjectName(std::string name) { m_ObjectName = name; };
    void SetGameObejct(GameObject* gameobj) { m_GameObject = gameobj; }
    GameObject* GetGameObejct() {return m_GameObject; }
    std::string GettName() { return m_ObjectName; }

    void SetDestroy() { m_Destroy = true; }

    bool Destroy()
    {
        if (m_Destroy)
        {
            Uninit();
            return true;
        }
        else
        {
            return false;
        }
    }

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(CEREAL_NVP(m_ObjectName));
    }
};
