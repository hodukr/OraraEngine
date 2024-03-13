#include "main.h"
#include "manager.h"
#include "guiManager.h"
#include "scene.h"
#include "gameObject.h"
#include "guiw_hierarchy.h"
#include "guiw_inspector.h"
#include "imgui/imgui.h"
#include "input.h"
#include <cereal/archives/json.hpp>
#include <fstream>
namespace fs = std::filesystem;

void Hierarchy::Init()
{
    m_Scene = Manager::GetScene();
    m_OpenTree = true;
    m_SelectGameObject = nullptr;
    m_CopyGameObject = nullptr;
}


void Hierarchy::Uninit()
{
    m_Scene  = nullptr;
    m_SelectGameObject = nullptr;
    m_ConfigGameObject = nullptr;
    m_CopyGameObject = nullptr;
}

void Hierarchy::Update()
{

}

void Hierarchy::SetWindowConfig()
{
    ImGui::SetNextWindowPos(ImVec2(0.0f, 100.0f));
    ImGui::SetNextWindowSize(ImVec2(SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT - 100.0f));
}

void Hierarchy::Draw()
{
    Inspector* inspector = GuiManager::Instance().GetGuiWindow<Inspector>();
    ImGui::Begin("Hierarchy", &m_IsShowWindow, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    
    if(m_OpenTree)ImGui::SetNextItemOpen(true);

    bool opentree = ImGui::TreeNode(m_Scene->GetName().c_str());
    if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
    {
        ImGui::OpenPopup("SceneConfig");
    }
    if (ImGui::BeginPopup("SceneConfig")) {
        char name[128];
        strncpy_s(name, (char*)m_Scene->GetName().c_str(), sizeof(name));
        ImGui::InputText("Name", name, sizeof(name));
        std::string rename = name;
        if ("" != rename && Input::Instance().GetKeyPress(VK_RETURN))
        {
            m_Scene->SetName(rename);
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();

    }
    if (opentree)
    {
        m_OpenTree = false;
        for (int i = 0; i < 3; i++)
        {
            bool push = false;
            for (auto& gameobject : m_Scene->GetList()[i])
            {
                //ゲームオブジェクト一覧表示
                ImGui::PushID(gameobject.get());
                if (m_SelectGameObject == gameobject.get())push = true;
                else push = false;
                if (ImGui::Selectable(gameobject.get()->GetName().c_str(), push))
                {
                    m_SelectGameObject = gameobject.get();
                    inspector->SetGameObejct(gameobject.get());
                }
                ImGui::PopID();
                if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
                {
                    ImGui::OpenPopup("GameObjectConfig"); 
                    m_ConfigGameObject = gameobject.get();
                }
            }
        }

        if (ImGui::BeginPopup("GameObjectConfig")) {

            if (ImGui::Selectable("削除"))
            {
                if (m_ConfigGameObject == m_SelectGameObject)
                    m_SelectGameObject = nullptr;
                if(m_ConfigGameObject == inspector->GetGameObject())
                    inspector->SetGameObejct(nullptr);
                m_ConfigGameObject->SetDestroy();

            }
            ImGui::EndPopup();
        }

        ImGui::Separator();
        ImGui::TreePop();
    }
    DrawCreateObject();
    CleatePrefab();

    ImGui::End();
}



void Hierarchy::DrawCreateObject()
{
    if (ImGui::Button("オブジェクト追加"))
    {
        ImGui::OpenPopup("CreateGameObject");
    }

    if (ImGui::BeginPopup("CreateGameObject"))
    {
        if (ImGui::Button("GameObject"))
        {
            GameObject* gameobjct = m_Scene->AddGameObject(1);
            gameobjct->SetName(gameobjct->GetName());
        }
        if (ImGui::Button("Prefab"))
        {
            ImGui::OpenPopup("CreatePrefab");
        }

        if (ImGui::BeginPopup("CreatePrefab"))
        {
            // フォルダのパスを指定 
            std::string folderPath = "asset/prefab/";

            // ファイル名を格納するためのベクターを作成 
            std::vector<std::string> fileNames;

            try {
                // 指定されたフォルダ内のファイルをイテレート 
                for (const auto& entry : fs::directory_iterator(folderPath)) {
                    std::string name = entry.path().filename().string();
                    fileNames.push_back(name);
                }
            }
            catch (const std::filesystem::filesystem_error& ex) {
                std::cerr << "Error: " << ex.what() << std::endl;
            }
            for (auto& file : fileNames)
            {
                std::string name = file.substr(0, file.find("."));
                if(ImGui::Selectable(name.c_str()))
                {
                    try
                    {
                        std::string filename = "asset/prefab/" + file;
                        std::ifstream inputFile(filename);
                        cereal::JSONInputArchive archive(inputFile);
                        std::unique_ptr<GameObject> obj = std::make_unique<GameObject>();
                        archive(*obj);
                        m_Scene->SetGameObject(std::move(obj));
                    }
                    catch (const std::exception&)
                    {

                    }
                }
            }


            ImGui::EndPopup();
        }


        ImGui::EndPopup();
    }
    
}

void Hierarchy::CleatePrefab()
{
    if (m_SelectGameObject)
    { 
        if (ImGui::Button("CreatePrefab"))
        {
            try
            {
                std::string filename = "asset/prefab/" + m_SelectGameObject->GetName() + ".json.prefab";
                std::ofstream outputFile(filename);
                cereal::JSONOutputArchive o_archive(outputFile);

                o_archive(cereal::make_nvp(m_SelectGameObject->GetName().c_str(), *m_SelectGameObject));
            }
            catch (const std::exception&)
            {
                //MessageBox(NULL, "正常に保存ができませんでした", "警告", MB_OK);
            }
        }
    }
}

