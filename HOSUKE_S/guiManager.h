#pragma once
#include "singleton.h"
#include <list>
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>  
#include <cereal/types/list.hpp>
#include <filesystem>
#include "imGuiWindow.h"
#include "guiw_common.h"

class GuiManager
{
private:
    Singleton(GuiManager);
    std::list<std::unique_ptr<ImGuiWindow>> m_Windows;
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
        std::unique_ptr<T> window = std::make_unique<T>();
        
        m_Windows.push_back(std::move(window));

        return window.get();
    }

    template<class T>
    T* GetGuiWindow()
    {
        for (auto& window : m_Windows)
        {
            if (typeid(*window.get()) == typeid(T))
            {
                return dynamic_cast<T*>(window.get());
            }
        }
        return nullptr;
    }

    std::list<ImGuiWindow*> GetList() {
        std::list<ImGuiWindow*> windowlist;
        for (auto& window : m_Windows)
        {
            windowlist.push_back(window.get());
        }
        return windowlist; 
    }

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(CEREAL_NVP(m_Windows));
    }
};
