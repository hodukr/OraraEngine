#include "reflection.h"
#include "Inspector.h"
#include "gameObject.h"
#include "imgui/imgui.h"




void Inspector::Init()
{
    m_PopupComponent = nullptr;

}

void Inspector::Uninit()
{
    m_GameObject = nullptr;
}

void Inspector::Update()
{

}

void Inspector::Draw()
{
    ImGui::Begin("Inspector", 0, ImGuiWindowFlags_NoScrollbar);

    if (m_GameObject)
    {
        //オブジェクト名表示
        char buffer[256];
        strncpy(buffer, m_GameObject->GetName().c_str(), sizeof(buffer));
        ImGui::InputText("##GameObjectName", buffer, sizeof(buffer));
        m_GameObject->SetName(buffer);



        //コンポーネントの表示
        for (auto& obj : *m_GameObject->GetList())
        {
            DrawComponent(obj.get());
            
        }


       

        if (ImGui::Button("AddComponent")) {

            ImGui::OpenPopup("ComponentList");
        }
        // ポップアップメニュー
        if (ImGui::BeginPopup("ComponentList")) {
            for (auto it : ReflectionList().GetNameList())
            {
                if (ImGui::MenuItem(it.c_str())) {
                    m_GameObject->AddComponent(ReflectionList().CreateInstans(it.c_str()));

                }

            }
            ImGui::EndPopup();

        }

    }

    ImGui::End();
}

void Inspector::DrawComponent(Component* component)
{
    std::string comname = typeid(*component).name();
    std::string name = comname.substr(6);

    bool isTreenode = ImGui::TreeNode(name.c_str());

    if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
    {
        ImGui::OpenPopup("ComponentConfig");
        m_PopupComponent = component;
    }


    if (m_PopupComponent == component)
    {
        if (ImGui::BeginPopup("ComponentConfig")) {

            if (ImGui::Selectable("Delet"))
            {
                m_PopupComponent->SetDestroy();

            }
            ImGui::EndPopup();
        }
    }

    if (isTreenode)
    {

        if (typeid(Transform).name() == comname)
        {
            Transform* transfrom = component->GetGameObejct()->m_Transform;
            {//position
                ImGui::Text("Position");
                ImGui::SameLine();

                float x = transfrom->GetPosition().x;
                ImGui::SetNextItemWidth(70.0f);
                ImGui::InputFloat("##positionX", &x);
                ImGui::SameLine();

                float y = transfrom->GetPosition().y;
                ImGui::SetNextItemWidth(70.0f);
                ImGui::InputFloat("##positionY", &y);
                ImGui::SameLine();

                float z = transfrom->GetPosition().z;
                ImGui::SetNextItemWidth(70.0f);
                ImGui::InputFloat("##positionZ", &z);

                Vector3 pos(x, y, z);
                transfrom->SetPosition(pos);
            }



            {//Rotation
                ImGui::Text("Rotation");
                ImGui::SameLine();

                float x = transfrom->GetRotation().x;
                ImGui::SetNextItemWidth(70.0f);
                ImGui::InputFloat("##rotationX", &x);
                ImGui::SameLine();

                float y = transfrom->GetRotation().y;
                ImGui::SetNextItemWidth(70.0f);
                ImGui::InputFloat("##rotationY", &y);
                ImGui::SameLine();

                float z = transfrom->GetRotation().z;
                ImGui::SetNextItemWidth(70.0f);
                ImGui::InputFloat("##rotationZ", &z);

                Vector3 rot(x, y, z);
                transfrom->SetRotation(rot);
            }


            {//Scale
                ImGui::Text("Scale   ");
                ImGui::SameLine();

                float x = transfrom->GetScale().x;
                ImGui::SetNextItemWidth(70.0f);
                ImGui::InputFloat("##scaleX", &x);
                ImGui::SameLine();

                float y = transfrom->GetScale().y;
                ImGui::SetNextItemWidth(70.0f);
                ImGui::InputFloat("##scaleY", &y);
                ImGui::SameLine();

                float z = transfrom->GetScale().z;
                ImGui::SetNextItemWidth(70.0f);
                ImGui::InputFloat("##scaleZ", &z);

                Vector3 sca(x, y, z);
                transfrom->SetScale(sca);
            }
        }

        if (typeid(Material).name() == comname)
        {
            if (ImGui::Selectable(component->GetGameObejct()->GetComponent<Material>()->GetShader().c_str()))
            {
            }
        }

        ImGui::TreePop();

    }
}
