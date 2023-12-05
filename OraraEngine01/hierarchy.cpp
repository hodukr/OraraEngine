#include "manager.h"
#include "scene.h"
#include "hierarchy.h"
#include "inspector.h"
#include "imgui/imgui.h"

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
    ImGui::Begin("Hierarchy", 0, ImGuiWindowFlags_NoScrollbar);
    if(m_OpenTree)ImGui::SetNextItemOpen(true);
    if (ImGui::TreeNode(m_Scene->GetName().c_str()))
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
                    Inspector::Instance().SetGameObejct(gameobject.get());
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

            if (ImGui::Selectable("Delet"))
            {
                if(m_ConfigGameObject == Inspector::Instance().GetGameObject())
                Inspector::Instance().SetGameObejct(nullptr);
                m_ConfigGameObject->SetDestroy();

            }
            ImGui::EndPopup();

        }


        ImGui::Separator();

        if (ImGui::Button("AddGameObject"))
        {
            m_Scene->AddGameObject<GameObject>(1);
        }
        ImGui::TreePop();

    }
    ImGui::End();
}
