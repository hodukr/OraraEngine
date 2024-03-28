#pragma once
#include "singleton.h"
#include "imGuiWindow.h"
#include "guiw_common.h"
#include "imgui/ImGuizmo.h"

class GuiManager
{
private:
    Singleton(GuiManager);
    list<unique_ptr<GuiWindowBase>> m_Windows;
public:
    void SetUp();
    void Init();
    void Uninit();
    void Update();
    void Draw();
    //static void SetText(string text) { m_Text = text;

    template<class T>
    T* AddWindow()
    {
        unique_ptr<T> window = make_unique<T>();
        
        m_Windows.push_back(move(window));

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

    list<GuiWindowBase*> GetList() {
        list<GuiWindowBase*> windowlist;
        for (auto& window : m_Windows)
        {
            windowlist.push_back(window.get());
        }
        return windowlist; 
    }

};
