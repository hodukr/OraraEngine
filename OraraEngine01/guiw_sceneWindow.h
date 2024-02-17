#pragma once

#include "imgui/imgui.h"
#include "imGuiWindow.h"
#include "imgui/ImGuizmo.h"

class SceneWindow :public GuiWindowBase
{
private:
    ImGuizmo::OPERATION m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
    ID3D11ShaderResourceView** m_SceneTexture = nullptr;

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
    void EditTransform(float* cameraView, float* cameraProjection, float* matrix);
    void SetSceneTexture(ID3D11ShaderResourceView** texture) { m_SceneTexture = texture; }
    //ID3D11ShaderResourceView** GetSceneTexture() { return m_SceneTexture; }
    bool GetChildWindowFocused() { return m_IsChildWindowFocused; }
    bool GetMouseHoveringChildWindow() { return m_IsMouseHoveringChildWindow; }
};