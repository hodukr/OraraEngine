#pragma once
#include "imGuiWindow.h"

class Menu:public GuiWindowBase
{
private:
    class Scene* m_Scene = nullptr;
    string m_StateScene{};
public:
    void Init()override;
    void Uninit()override;
    void Update()override;
    void SetWindowConfig()override;
    void Draw()override;


private:
    bool SaveScene();

};

