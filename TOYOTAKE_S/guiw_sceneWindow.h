#pragma once

#include "imgui/imgui.h"
#include "imGuiWindow.h"

class SceneWindow :public ImGuiWindow
{
private:

public:
	void Draw()override;
};