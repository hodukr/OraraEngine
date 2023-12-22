#include "reflection.h"
#include "Inspector.h"
#include "gameObject.h"
#include "imgui/imgui.h"
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

void Inspector::Init()
{
    m_PopupComponent = nullptr;
    for (int i = 0; i < 3; i++)
    {
        m_KeyTransform[i] = false;
    }

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

                ImGui::Checkbox("##lock", &m_KeyTransform[0]);
                ImGui::SameLine();
                ImGui::Text("Position");
                ImGui::SameLine();

                float x = transfrom->GetPosition().x;
                float y = transfrom->GetPosition().y;
                float z = transfrom->GetPosition().z;

                ImGui::SetNextItemWidth(70.0f);
                if (ImGui::InputFloat("##positionX", &x) && m_KeyTransform[0])
                {
                    y = x;
                    z = x;
                }

                ImGui::SameLine();

                ImGui::SetNextItemWidth(70.0f);
                if (ImGui::InputFloat("##positionY", &y) && m_KeyTransform[0])
                {
                    x = y;
                    z = y;
                }
                ImGui::SameLine();

                ImGui::SetNextItemWidth(70.0f);
                if (ImGui::InputFloat("##positionZ", &z) && m_KeyTransform[0])
                {
                    x = z;
                    y = z;
                }

                Vector3 pos(x, y, z);
                transfrom->SetPosition(pos);
            }



            {//Rotation 
                ImGui::Checkbox("##lock1", &m_KeyTransform[1]);
                ImGui::SameLine();
                ImGui::Text("Rotation");
                ImGui::SameLine();
                float x = (transfrom->GetRotation().x / D3DX_PI) * 180.0f;
                float y = (transfrom->GetRotation().y / D3DX_PI) * 180.0f;
                float z = (transfrom->GetRotation().z / D3DX_PI) * 180.0f;

                ImGui::SetNextItemWidth(70.0f);
                if (ImGui::InputFloat("##rotationX", &x) && m_KeyTransform[1])
                {
                    y = x;
                    z = x;
                }
                ImGui::SameLine();

                ImGui::SetNextItemWidth(70.0f);
                if (ImGui::InputFloat("##rotationY", &y) && m_KeyTransform[1])
                {
                    x = y;
                    z = y;
                }
                ImGui::SameLine();

                ImGui::SetNextItemWidth(70.0f);
                if (ImGui::InputFloat("##rotationZ", &z) && m_KeyTransform[1])
                {
                    x = z;
                    y = z;
                }
                x = (x / 180.0f) * D3DX_PI;
                y = (y / 180.0f) * D3DX_PI;
                z = (z / 180.0f) * D3DX_PI;

                Vector3 rot(x, y, z);

                transfrom->SetRotation(rot);
            }


            {//Scale 
                ImGui::Checkbox("##lock2", &m_KeyTransform[2]);
                ImGui::SameLine();

                ImGui::Text("Scale   ");
                ImGui::SameLine();

                float x = transfrom->GetScale().x;
                float y = transfrom->GetScale().y;
                float z = transfrom->GetScale().z;

                ImGui::SetNextItemWidth(70.0f);
                if(ImGui::InputFloat("##scaleX", &x) && m_KeyTransform[2])
                {
                    y = x;
                    z = x;
                }
                ImGui::SameLine();

                ImGui::SetNextItemWidth(70.0f);
                if(ImGui::InputFloat("##scaleY", &y) && m_KeyTransform[2])
                {
                    x = y;
                    z = y;
                }
                ImGui::SameLine();

                ImGui::SetNextItemWidth(70.0f);
                if (ImGui::InputFloat("##scaleZ", &z) && m_KeyTransform[2])
                {
                    x = z;
                    y = z;
                }
                Vector3 sca(x, y, z);
                transfrom->SetScale(sca);
            }
        }

        if (typeid(PraticleSystem).name() == comname)
        {
            PraticleSystem* com = dynamic_cast<PraticleSystem*>(component);
            if (ImGui::TreeNode(com->GetEmitter()->TexName.c_str()))
            {
                // フォルダのパスを指定 
                std::string folderPath = "asset\\texture";

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
                    if (ImGui::Selectable(fileName.c_str()))
                    {
                        com->SetTexture(fileName.c_str());
                    }
                }
                ImGui::TreePop();
            }

            ImGui::ColorEdit4("Color", com->GetEmitter()->StartColor);
            ImGui::InputFloat("Lifetime", &com->GetEmitter()->StartLifetime);
            ImGui::InputFloat("EmissionRate", &com->GetEmitter()->EmissionRate);
            ImGui::InputFloat("Rotation", &com->GetEmitter()->StartRotation);
            ImGui::InputFloat("Speed",&com->GetEmitter()->Speed);
            ImGui::InputInt("MaxParticles", &com->GetEmitter()->MaxParticles);

            ImGui::InputFloat3("AddStateVelocity", com->GetEmitter()->AddStateVelocity);
            ImGui::InputFloat3("AddVelocity", com->GetEmitter()->AddVelocity);

            ImGui::Checkbox("IsGradation", &com->GetEmitter()->IsGradation);

            if (com->GetEmitter()->IsGradation)
            {
                ImGui::SliderFloat("GradationSize", &com->GetEmitter()->GradationLength,1.0f,5.0f);
                ImGui::ColorEdit4("Color1", com->GetEmitter()->GradationColor[0]);
                ImGui::ColorEdit4("Color2", com->GetEmitter()->GradationColor[1]);
                ImGui::ColorEdit4("Color3", com->GetEmitter()->GradationColor[2]);
            }
            ImGui::Checkbox("IsFade", &com->GetEmitter()->IsFade);

            if (com->GetEmitter()->IsFade)
            {
                ImGui::InputFloat("StartFadeTime", &com->GetEmitter()->StartFadeTime);
                ImGui::SliderFloat("DeclineFade", &com->GetEmitter()->DeclineFade,0.0f,1.0f);

            }
        }

        if (typeid(Mesh).name() == comname)
        {
            Mesh* com = dynamic_cast<Mesh*>(component);

            if (ImGui::TreeNode(com->GetModelpas().c_str()))
            {
                // フォルダのパスを指定 
                std::string folderPath = "asset\\model";

                // ファイル名を格納するためのベクターを作成 
                std::vector<std::string> fileNames;

                try { 
                    // 指定されたフォルダ内のファイルをイテレート 
                    for (const auto& entry : fs::directory_iterator(folderPath)) {
                        std::string filename = entry.path().filename().string();
                        std::string extension = filename.substr(filename.find_last_of(".") +1);
                        if (extension == "obj")
                        {
                            // ファイル名をベクターに追加 
                            fileNames.push_back(entry.path().filename().string());
                        }
                    }
                }
                catch (const std::filesystem::filesystem_error& ex) {
                    std::cerr << "Error: " << ex.what() << std::endl;
                }

                // ファイル名を出力 
                for (const auto& fileName : fileNames) {
                    if (ImGui::Selectable(fileName.c_str()))
                    {
                        com->SetModel(fileName.c_str());
                    }
                }
                ImGui::TreePop();
            }
            Material* material = com->GetMaterial();
            std::string comname = typeid(*material).name();
            std::string name = comname.substr(6);
            if (ImGui::TreeNode(name.c_str())) {
                // フォルダのパスを指定 
                std::string folderPath = "shader";

                // ファイル名を格納するためのベクターを作成 
                std::vector<std::string> fileNamesVS;
                std::vector<std::string> fileNamesPS;

                try {
                    // 指定されたフォルダ内のファイルをイテレート 
                    for (const auto& entry : fs::directory_iterator(folderPath)) {

                        std::string filename = entry.path().filename().string();
                        std::string extension = filename.substr(filename.find_last_of(".") - 2);
                        if (extension == "VS.cso")
                        {
                            // ファイル名をベクターに追加 
                            fileNamesVS.push_back(entry.path().filename().string());
                        }
                        if (extension == "PS.cso")
                        {
                            fileNamesPS.push_back(entry.path().filename().string());
                        }
                    }
                }
                catch (const std::filesystem::filesystem_error& ex) {
                    std::cerr << "Error: " << ex.what() << std::endl;
                }
                if (ImGui::TreeNode(material->GetShaderVSName().c_str()))
                {
                    // ファイル名を出力 
                    for (const auto& fileName : fileNamesVS) {
                        if (ImGui::Selectable(fileName.c_str()))
                        {
                            material->SetShaderVS(fileName.c_str());
                        }
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode(material->GetShaderPSName().c_str()))
                {
                    // ファイル名を出力 
                    for (const auto& fileName : fileNamesPS) {
                        if (ImGui::Selectable(fileName.c_str()))
                        {
                            material->SetShaderPS(fileName.c_str());
                        }
                    }
                    ImGui::TreePop();
                }
                ImGui::TreePop();

            }

        }

        ImGui::TreePop();

    }
}
