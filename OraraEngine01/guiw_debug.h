#pragma once
#include "imgui/imgui.h"
#include "imGuiWindow.h"

class Debug :public GuiWindowBase
{
private:
   
public:
    void SetWindowConfig()override;
    void Draw()override;
};