
#include "accessFolder.h"

void AccessFolder::DrawFolderIconAndName(const FolderInfo& folder)
{
    // �t�H���_�A�C�R����\��
    ImGui::Image(nullptr, folder.IconSize, folder.IconUV);

    // �t�H���_����\��
    ImGui::SameLine();
    ImGui::Text("%s", folder.Name.c_str());
}

void AccessFolder::DrawProjectAssets()
{
    for (const auto& entry : fs::directory_iterator("asset"))
    {
        const std::string& itemPath = entry.path().string();

        // �T�u�t�H���_���𒊏o
        fs::path folderPath(itemPath);
        std::string folderName = folderPath.filename().string();

        // �T�u�t�H���_�̏���ݒ�
        m_ProjectFolders.push_back({ folderName.c_str(), ImVec2(16, 16), ImVec2(0.0f, 0.0f), true });
    }

    // �t�H���_�ƃt�@�C����\��
    for (const auto& folder : m_ProjectFolders)
    {
        if (folder.IsFolder)
        {
            if (ImGui::TreeNodeEx(folder.Name.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen))
            {
                DrawFolderIconAndName(folder);
                ImGui::TreePop();
            }
        }
        else
        {
            DrawFolderIconAndName(folder);
        }
    }
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
    ImGui::Begin("Create Asset Folder");

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
