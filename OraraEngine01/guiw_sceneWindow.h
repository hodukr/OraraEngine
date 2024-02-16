#pragma once

#include "imgui/imgui.h"
#include "imGuiWindow.h"
#include "imgui/ImGuizmo.h"

class SceneWindow :public GuiWindowBase
{
private:
    bool m_UseWindow = true;
    int  m_GizmoCount = 1;
    float m_CamDistance = 8.0f;
    ImGuizmo::OPERATION m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
    ID3D11ShaderResourceView** m_SceneTexture = nullptr;

    bool m_IsChildWindowFocused = false;
    bool m_IsMouseHoveringChildWindow = false;
    bool one{};
public:
    void Update()override;
	void Draw()override;
    void EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition);
    void SetSceneTexture(ID3D11ShaderResourceView** texture) { m_SceneTexture = texture; }
    //ID3D11ShaderResourceView** GetSceneTexture() { return m_SceneTexture; }
    bool GetChildWindowFocused() { return m_IsChildWindowFocused; }
    bool GetMouseHoveringChildWindow() { return m_IsMouseHoveringChildWindow; }
};