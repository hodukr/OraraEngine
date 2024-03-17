#include "main.h"
#include "manager.h"
#include "scene.h"
#include "guiw_menu.h"
#include "imgui/imgui.h"
#include <cereal/archives/json.hpp>

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
    m_IsShowWindow = true;
}

void Menu::SetWindowConfig()
{
    ImGui::SetNextWindowPos(ImVec2(0.0f,0.0f));
    ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH * 0.25f, 100.0f));
}

void Menu::Draw()
{

    ImGui::Begin("Menu",0,ImGuiWindowFlags_NoMove| ImGuiWindowFlags_NoResize| ImGuiWindowFlags_NoCollapse);
    if (ImGui::Button("save"))
    {
        SaveScene();
    }
    ImGui::SameLine();
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
                {//シーンfileの情報ロード
                    Manager::SetScene(sceneName);
                    break;
                }
            }
        }
        ImGui::EndPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("CreateScene"))
    {
        ImGui::OpenPopup("CreateScene");

    }
    if(ImGui::BeginPopup("CreateScene"))
    {
        static char buffer[128];
        ImGui::InputText("CreateScene", buffer, sizeof(buffer));
        std::string name = buffer;
        if (ImGui::Button("Create") && name != "")
        {
            ImGui::CloseCurrentPopup();
            int result = MessageBox(NULL, "現在のシーンを保存しますか？", "警告", MB_YESNO | MB_ICONQUESTION);
            if (result == IDYES)
            {
                SaveScene();
            }
            // フォルダのパスを指定 
            std::string folderPath = "asset\\scene";

            //シーン生成
            bool isScene = true;
            try {
                // 指定されたフォルダ内のファイルをイテレート 
                for (const auto& entry : fs::directory_iterator(folderPath)) {
                    // ファイル名をベクターに追加 
                    if (entry.path().filename().string() == name + ".json")//フォルダ内に同じ名前のシーンがないかを調べる
                    {
                        isScene = false;
                        break;
                    }
                }
            }
            catch (const std::filesystem::filesystem_error& ex) {
                std::cerr << "Error: " << ex.what() << std::endl;
            }
            if (isScene)
            {
                Manager::SetScene(name);
            }
            else
            {
                MessageBox(NULL, "すでにある名前のSceneを生成できません", "警告", MB_OK);
            }
        }
        ImGui::EndPopup();

    }

    if (ImGui::Selectable("PopWindow"))
    {
        ImGui::OpenPopup("WindowList");
    }
    if (ImGui::BeginPopup("WindowList"))
    {
        for (auto window : GuiManager::Instance().GetList())
        {
            std::string name = typeid(*window).name();
            name = name.substr(name.find(" ") + 1);
            if (ImGui::Selectable(name.c_str()))
            {
                window->SetShowWindow(true);
            }
        }
        

        ImGui::EndPopup();
    }


    ImGui::End();
}


bool Menu::SaveScene()
{
    try
    {
        std::string filename = "asset/scene/" + m_Scene->GetName() + ".json";
        std::ofstream outputFile(filename);
        cereal::JSONOutputArchive o_archive(outputFile);

        o_archive(cereal::make_nvp("secne", *m_Scene));
        return true;
    }
    catch (const std::exception&)
    {
        MessageBox(NULL, "正常に保存ができませんでした", "警告", MB_OK);
    }
    return false;
}

