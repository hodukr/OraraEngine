#pragma once
#include "imGuiWindow.h"

class Menu:public ImGuiWindow
{
private:
    class Scene* m_Scene;

public:
    void Init()override;
    void Uninit()override;
    void Update()override;
    void Draw()override;

};

