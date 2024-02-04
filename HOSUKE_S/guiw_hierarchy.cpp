#include "manager.h"
#include "guiManager.h"
#include "scene.h"
#include "guiw_hierarchy.h"
#include "guiw_inspector.h"
#include "imgui/imgui.h"
#include "input.h"

void Hierarchy::Init()
{
    m_Scene = Manager::GetScene();
    m_OpenTree = true;
    m_SelectGameObject = nullptr;
}


void Hierarchy::Uninit()
{
    m_Scene  =nullptr;
    m_SelectGameObject = nullptr;
    m_ConfigGameObject = nullptr;
}

void Hierarchy::Update()
{
    //Scene* scene = Manager::GetScene();
}

void Hierarchy::Draw()
{
    Inspector* inspector = GuiManager::Instance().GetGuiWindow<Inspector>();
    ImGui::Begin("Hierarchy", &m_IsShowWindow, ImGuiWindowFlags_NoScrollbar);
    
    if(m_OpenTree)ImGui::SetNextItemOpen(true);

    bool opentree = ImGui::TreeNode(m_Scene->GetName().c_str());
    if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
    {
        ImGui::OpenPopup("SceneConfig");
    }
    if (ImGui::BeginPopup("SceneConfig")) {

            char* name = (char*)m_Scene->GetName().c_str();
            ImGui::InputText("Name", name,20);
            std::string rename = name;
            if ("" != rename && ImGui::GetKeyName(ImGuiKey_Enter))
            {
                m_Scene->SetName(rename);
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

            //ゲームオブジェクト名がかぶってないかを調べる 
            bool flg = true;
            int num = 1;
            std::string name = gameobjct->GetName();
            while (flg) {
                flg = false;
                for (auto& obj : m_Scene->GetList()[1])
                {
                    std::string objnum = "(" + std::to_string(num) + ")";
                    if (name == obj->GetName() && obj.get() != gameobjct)
                    {
                        name = gameobjct->GetName() + "(" + std::to_string(num) + ")";
                        flg = true;
                        num++;
                        break;
                    }
                }
            };
            gameobjct->SetName(name);
            
        }
        ImGui::TreePop();
    }
    ImGui::End();
}
