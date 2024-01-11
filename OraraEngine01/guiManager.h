#pragma once
#include "singleton.h"
#include <list>
#include <cereal/cereal.hpp>
#include <cereal/types/list.hpp>
#include <filesystem>
class GuiManager
{
private:
    Singleton(GuiManager);
    std::list<class ImGuiWindow*> m_Windows;
public:
    void SetUp();
    void Init();
    void Uninit();
    void Update();
    void Draw();
    //static void SetText(std::string text) { m_Text = text; }

    template<class T>
    T* AddWindow()
    {
        T* window = new T;

        m_Windows.push_back(window);

        return window;
    }

    template<class T>
    T* GetGuiWindow()
    {
        for (auto window : m_Windows)
        {
            if (typeid(*window) == typeid(T))
            {
                return dynamic_cast<T*>(window);
            }
        }
        return nullptr;
    }

    std::list<ImGuiWindow*> GetList() { return m_Windows; }

    template<class Archive>
    void serialize(Archive& archive)
    {
        //archive(CEREAL_NVP(m_Windows));
    }
};
