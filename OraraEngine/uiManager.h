#pragma once
#include "singleton.h"
class UIManager
{
private:
    Singleton(UIManager);
    class RendererManager* m_RenderManager;
public:
    void Init();
    void Uninit();
    void Update();
    void Draw();
};
