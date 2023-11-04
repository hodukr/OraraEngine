#pragma once
#include "singleton.h"
class UIManager
{
private:
    Singleton(UIManager);
public:
    void Init();
    void Uninit();
    void Update();
};
