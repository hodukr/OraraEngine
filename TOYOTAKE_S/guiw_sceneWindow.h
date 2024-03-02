#pragma once

#include "imgui/imgui.h"
#include "imGuiWindow.h"
#include "imgui/ImGuizmo.h"

class SceneWindow :public GuiWindowBase
{
private:
    ImGuizmo::OPERATION m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;

    bool m_IsChildWindowFocused = false;
    bool m_IsMouseHoveringChildWindow = false;

    const float m_IdentityMatrix[16] =
    {
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f
    };

public:
    void Update()override;
    void SetWindowConfig()override;
	void Draw()override;
    bool GetChildWindowFocused() { return m_IsChildWindowFocused; }
    bool GetMouseHoveringChildWindow() { return m_IsMouseHoveringChildWindow; }
};