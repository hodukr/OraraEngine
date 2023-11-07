
#include "accessFolder.h"

void AccessFolder::DrawFolderIconAndName(const FolderInfo& folder)
{
    // フォルダアイコンを表示
    ImGui::Image(nullptr, folder.IconSize, folder.IconUV);

    // フォルダ名を表示
    ImGui::SameLine();
    ImGui::Text("%s", folder.Name.c_str());
}

void AccessFolder::DrawProjectAssets()
{
    for (const auto& entry : fs::directory_iterator("asset"))
    {
        const std::string& itemPath = entry.path().string();

        // サブフォルダ名を抽出
        fs::path folderPath(itemPath);
        std::string folderName = folderPath.filename().string();

        // サブフォルダの情報を設定
        m_ProjectFolders.push_back({ folderName.c_str(), ImVec2(16, 16), ImVec2(0.0f, 0.0f), true });
    }

    // フォルダとファイルを表示
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

    // フォルダ名を入力させるテキストボックスを表示
    ImGui::InputText("Folder Name", m_FolderName, ImGuiInputTextFlags_EnterReturnsTrue);

    // フォルダを作成するボタンを表示
    if (ImGui::Button("Create"))
    {
        if (m_FolderName[0] != '\0')
        { 
            std::string assetFolderPath = "asset/" + std::string(m_FolderName);
            // フォルダを作成
            if (fs::create_directory(assetFolderPath))
            {
                strcpy(m_CreatedFolderName, m_FolderName);
                m_CreateFolder = true;
            }
        }
    }

    // フォルダが作成されたら確認メッセージを表示
    if (m_CreateFolder)
    {
        ImGui::Text("Asset folder created: %s", m_CreatedFolderName);
    }

    ImGui::End();
}
