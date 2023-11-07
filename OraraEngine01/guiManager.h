#pragma once
#include "singleton.h"
class GuiManager
{
private:
    Singleton(GuiManager);
public:
    void Init();
    void Uninit();
    void Update();
    void Draw();
};
