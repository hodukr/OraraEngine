#include "manager.h"
#include "scene.h"
#include "hierarchy.h"
#include "imgui/imgui.h"

void Hierarchy::Init()
{

}


void Hierarchy::Uninit()
{

}

void Hierarchy::Update()
{
    Scene* scene = Manager::GetScene();
}

void Hierarchy::Draw()
{
    ImGui::SetNextWindowSize(ImVec2(320, 100));
    ImGui::Begin("Hierarchy", 0, ImGuiWindowFlags_NoScrollbar);

    ImGui::Text("fugafuga");
    ImGui::End();
}
