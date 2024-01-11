#pragma once
#include "imGuiWindow.h"

class GameManagerGui:public ImGuiWindow
{
private:

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};