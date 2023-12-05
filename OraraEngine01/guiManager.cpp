#include "guiManager.h"
#include "main.h"
#include "renderer.h"
#include "accessFolder.h"
#include "menu.h"
#include "hierarchy.h"
#include  "Inspector.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"


void GuiManager::SetUp()
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

void GuiManager::Init()
{
    Menu::Instance().Init();
    Hierarchy::Instance().Init();
    Inspector::Instance().Init();

}

void GuiManager::Uninit()
{
    Inspector::Instance().Uninit();
    Hierarchy::Instance().Uninit();
    Menu::Instance().Uninit();

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
    Menu::Instance().Draw();
    Hierarchy::Instance().Draw();
    Inspector::Instance().Draw();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
