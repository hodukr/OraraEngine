﻿#pragma once
#include "imGuiWindow.h"

class Menu:public GuiWindowBase
{
private:
    class Scene* m_Scene;

public:
    void Init()override;
    void Uninit()override;
    void Update()override;
    void SetWinodwConfig()override;
    void Draw()override;

};

