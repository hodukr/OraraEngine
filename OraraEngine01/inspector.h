#pragma once
#include "singleton.h"

class Inspector
{
private:
    Singleton(Inspector);
    class GameObject* m_GameObject;

    class Component* m_PopupComponent;
public:
    void Init();
    void Uninit();
    void Update();
    void Draw();
    void SetGameObejct(GameObject* obj){m_GameObject = obj;}
    const GameObject* GetGameObject(){return m_GameObject;}
private:
    void DrawComponent(class Component* component);
};
