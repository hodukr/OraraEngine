#include "manager.h"
#include "scene.h"
#include "menu.h"
#include "imgui/imgui.h"

#include <fstream>
#include <cereal/archives/json.hpp>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;
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
        ImGui::OpenPopup("LoadScene");

    }
    if (ImGui::BeginPopup("LoadScene"))
    {
        // フォルダのパスを指定 
        std::string folderPath = "asset\\scene";

        // ファイル名を格納するためのベクターを作成 
        std::vector<std::string> fileNames;

        try {
            // 指定されたフォルダ内のファイルをイテレート 
            for (const auto& entry : fs::directory_iterator(folderPath)) {
                // ファイル名をベクターに追加 
                fileNames.push_back(entry.path().filename().string());
            }
        }
        catch (const std::filesystem::filesystem_error& ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
        }

        // ファイル名を出力 
        for (const auto& fileName : fileNames) {
            std::string extension = fileName.substr(fileName.find_last_of(".") + 1);
            std::string sceneName = fileName.substr(0,fileName.find_last_of("."));

            if (extension == "json")
            {
                if (ImGui::Selectable(sceneName.c_str()))
                {
                    std::string filename = "asset/scene/" + sceneName + ".json";
                    std::ifstream inputFile(filename);
                    cereal::JSONInputArchive archive(inputFile);
                    Scene* inscene = new Scene();
                    archive(*inscene);
                    Manager::SetScene(inscene);
                }
            }
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}
