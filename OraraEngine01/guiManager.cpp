#include "guiManager.h"
#include "main.h"
#include "renderer.h"
#include "accessFolder.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

std::string GuiManager::m_Text{};

void GuiManager::Init()
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.DisplaySize.x = SCREEN_WIDTH;
    io.DisplaySize.y = SCREEN_HEIGHT;
    // フォントの読み込みと設定 
    ImGui_ImplWin32_Init(GetWindow());
    ImGui_ImplDX11_Init(Renderer::GetDevice(), Renderer::GetDeviceContext());
}

void GuiManager::Uninit()
{

}

void GuiManager::Update()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    AccessFolder::Instance().CreateFolder();
    AccessFolder::Instance().DrawProjectAssets();
    AccessFolder::Instance().ChangeImageSize();
}

void GuiManager::Draw()
{

    ImGui::SetNextWindowSize(ImVec2(320, 100));
    ImGui::Begin("debug", 0, ImGuiWindowFlags_NoScrollbar);

    ImGui::Text(m_Text.c_str());
    ImGui::End();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
