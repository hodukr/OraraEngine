#include "accessFolder.h"


void AccessFolder::DrawFolderIconAndName(const char* name, ImVec2 size, ImVec2 uv)
{
    // フォルダアイコンを表示
    ImGui::Image(nullptr, size, uv);

    // フォルダ名を表示
    ImGui::SameLine();
    ImGui::Button(name);
}

void AccessFolder::DrawProjectAssets()
{
    ImGui::Begin("Asset");

    for (const auto& entry : fs::directory_iterator("asset"))
    {
        const std::string& itemPath = entry.path().string();

        // サブフォルダ名を抽出
        fs::path folderPath(itemPath);
        std::string folderName = folderPath.filename().string();

        // フォルダ名が重複しない場合に追加
        if (m_ProjectFolders.insert(folderName).second);
    }

    // フォルダとファイルを表示
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

                // サブフォルダ名を抽出
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
