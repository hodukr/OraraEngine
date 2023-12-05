#include "manager.h"
#include "scene.h"
#include "menu.h"
#include "imgui/imgui.h"

#include <fstream>
#include <cereal/archives/json.hpp>

void Menu::Init()
{
    m_Scene = Manager::GetScene();
}

void Menu::Uninit()
{

}

void Menu::Update()
{

}

void Menu::Draw()
{
    ImGui::Begin("Menu",0);
    if (ImGui::Button("save"))
    {
        std::string filename = "asset/scene/" + m_Scene->GetName() + ".json";
        std::ofstream outputFile(filename);
        cereal::JSONOutputArchive o_archive(outputFile);

        o_archive(cereal::make_nvp("secne", *m_Scene));
    }
    if (ImGui::Button("load"))
    {
        std::string filename = "asset/scene/NewScene.json";
        std::ifstream inputFile(filename);
        cereal::JSONInputArchive archive(inputFile);
        Scene* inscene = new Scene();
        archive(*inscene);
        Manager::SetScene(inscene);
    }
    ImGui::End();
}
