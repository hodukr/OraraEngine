#pragma once
#include "singleton.h"
#include "imGuiWindow.h"

class Inspector:public ImGuiWindow
{
private:
    class GameObject* m_GameObject;

    class Component* m_PopupComponent{};
    bool m_KeyTransform[3]{};
public:
    void Init();
    void Uninit();
    void Update();
    void Draw();
    void SetGameObejct(GameObject* obj){
        m_GameObject = obj;
        m_IsShowWindow = true;
    }
    const GameObject* GetGameObject(){return m_GameObject;}
private:
    void DrawComponent(class Component* component);
};
