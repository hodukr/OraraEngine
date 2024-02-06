#pragma once
#include "imGuiWindow.h"

class GameManagerGui:public GuiWindowBase
{
private:

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};