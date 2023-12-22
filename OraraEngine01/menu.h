#pragma once
#include "singleton.h"

class Menu
{
private:
    Singleton(Menu);

    class Scene* m_Scene;

public:
    void Init();
    void Uninit();
    void Update();
    void Draw();

};

