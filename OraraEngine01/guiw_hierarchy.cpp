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
            ImGui::InputText("Name", name,sizeof(name));
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

        if (ImGui::Button("オブジェクト追加"))
        {
            GameObject* gameobjct =  m_Scene->AddGameObject(1);
            gameobjct->SetName(gameobjct->GetName());
        }
        ImGui::TreePop();
    }
    ImGui::End();
}
