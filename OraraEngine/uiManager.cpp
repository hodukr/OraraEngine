#include "uiManager.h"
#include "main.h"
#include "rendererManager.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

void UIManager::Init()
{
    m_RenderManager = &RendererManager::Instance();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.DisplaySize.x = SCREEN_WIDTH;
    io.DisplaySize.y = SCREEN_HEIGHT;
    // フォントの読み込みと設定
    ImGui_ImplWin32_Init(GetWindow());
    ImGui_ImplDX11_Init(m_RenderManager->GetDevice(), m_RenderManager->GetDeviceContext());
}

void UIManager::Uninit()
{

}

void UIManager::Update()
{

}

void UIManager::Draw()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowSize(ImVec2(320, 100));
    ImGui::Begin("debug", 0, ImGuiWindowFlags_NoScrollbar);

    ImGui::Text("fugafuga");
    ImGui::End();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
