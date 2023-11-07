#include "accessFolder.h"


void AccessFolder::DrawFolderIconAndName(const char* name, ImVec2 size, ImVec2 uv)
{
    // �t�H���_�A�C�R����\��
    ImGui::Image(nullptr, size, uv);

    // �t�H���_����\��
    ImGui::SameLine();
    ImGui::Button(name);
}

void AccessFolder::DrawProjectAssets()
{
    ImGui::Begin("Asset");

    for (const auto& entry : fs::directory_iterator("asset"))
    {
        const std::string& itemPath = entry.path().string();

        // �T�u�t�H���_���𒊏o
        fs::path folderPath(itemPath);
        std::string folderName = folderPath.filename().string();

        // �t�H���_�����d�����Ȃ��ꍇ�ɒǉ�
        if (m_ProjectFolders.insert(folderName).second);
    }

    // �t�H���_�ƃt�@�C����\��
    for (const auto& folder : m_ProjectFolders)
    {
        if (!m_ProjectFolderName.empty() && m_ProjectFolderName != folder)
        {
            continue;
        }
        if (ImGui::TreeNode(folder.c_str()))
        {
            m_ProjectFolderName = folder;
            for (const auto& entry : fs::directory_iterator("asset/" + folder))
            {
                const std::string& itemPath = entry.path().string();

                // �T�u�t�H���_���𒊏o
                fs::path folderPath(itemPath);
                std::string folderName = folderPath.filename().string();

                DrawFolderIconAndName(folderName.c_str(), ImVec2(16, 16), ImVec2(0.0f, 0.0f));
            }

            ImGui::TreePop();
        }
        else
        {
            if (!m_ProjectFolderName.empty())
            {
                m_ProjectFolderName = "";
            }
        }
    }

  
    ImGui::End();
}

void AccessFolder::ListAssetContents()
{
    ImGui::Text("Asset Folder Contents:");

    for (const auto& entry : fs::directory_iterator("asset"))
    {
        const std::string& itemPath = entry.path().string();
        ImGui::Text("%s", itemPath.c_str());
    }
}

void AccessFolder::CreateFolder()
{
    ImGui::Begin("Create Folder in asset");

    // �t�H���_������͂�����e�L�X�g�{�b�N�X��\��
    ImGui::InputText("Folder Name", m_FolderName, ImGuiInputTextFlags_EnterReturnsTrue);

    // �t�H���_���쐬����{�^����\��
    if (ImGui::Button("Create"))
    {
        if (m_FolderName[0] != '\0')
        { 
            std::string assetFolderPath = "asset/" + std::string(m_FolderName);
            // �t�H���_���쐬
            if (fs::create_directory(assetFolderPath))
            {
                strcpy(m_CreatedFolderName, m_FolderName);
                m_CreateFolder = true;
            }
        }
    }

    // �t�H���_���쐬���ꂽ��m�F���b�Z�[�W��\��
    if (m_CreateFolder)
    {
        ImGui::Text("Asset folder created: %s", m_CreatedFolderName);
    }

    ImGui::End();
}
