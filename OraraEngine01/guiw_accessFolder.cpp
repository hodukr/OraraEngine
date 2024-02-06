#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h" 
#include "textureManager.h"
#include "guiw_accessFolder.h"

#include <algorithm>

void AccessFolder::DrawFolderIconAndName(const char* name, ImVec2 size, ImVec2 uv)
{
    // ファイル拡張子を取得
    std::string filename = name;
    std::string extension = filename.substr(filename.find_last_of(".") + 1);

    ID3D11ShaderResourceView* texture = nullptr;
    int textureNum = 0;
   
    // 拡張子が .png または .jpg の場合にのみ画像を表示
    if (extension == "png" || extension == "jpg" || extension == "dds")
    {
        //パスを取っておかないとconst char*の場所が変わりTextureManagerで同じ画像判定じゃなくなりバグる
        auto it = m_Path.find(name);
        if (it == m_Path.end())
        {
            std::string path = "asset\\" + m_ProjectFolderName + "\\" + name;
            m_Path[name] = path;
            it = m_Path.find(name);  // 要素を追加後、再度検索してイテレータで中身を取り出さないとバグる
        }

        textureNum = TextureManager::LoadTexture(it->second.c_str());
        texture = *TextureManager::GetTexture(textureNum);
     
        // ドラッグ元の要素の描画
        ImGui::Image((ImTextureID)texture, size, uv);

       
    }
    else if (extension == "wav")
    {
        textureNum = TextureManager::LoadTexture("asset\\Music.png");
        texture = *TextureManager::GetTexture(textureNum);

        ImGui::Image((ImTextureID)texture, ImVec2(m_ImageSize, m_ImageSize), ImVec2(0.0f, 0.0f));
    }
    else if (extension == "json")
    {
        textureNum = TextureManager::LoadTexture("asset\\Memo.png");
        texture = *TextureManager::GetTexture(textureNum);

        ImGui::Image((ImTextureID)texture, ImVec2(m_ImageSize, m_ImageSize), ImVec2(0.0f, 0.0f));
    }
    else if (extension == "obj" || extension == "fbx")
    {
        textureNum = TextureManager::LoadTexture("asset\\3DModel.png");
        texture = *TextureManager::GetTexture(textureNum);

        ImGui::Image((ImTextureID)texture, ImVec2(m_ImageSize, m_ImageSize), ImVec2(0.0f, 0.0f));
    }
    else
    {
        ImGui::Image(nullptr, size, uv);
    }

    // フォルダ名を表示
    ImGui::SameLine();
    ImGui::Button(name);

    // ドラッグソースを設定
    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("MY_PAYLOAD_TYPE", &name, sizeof(const char*));

        // ドラッグ元の要素の描画
        ImGui::Image((ImTextureID)texture, size, uv);
        std::string path = "asset\\" + m_ProjectFolderName + "\\" + name;

        m_DragName = path;

        ImGui::EndDragDropSource();
    }

    //右クリックでポップアップを出す 
    if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
    {
        ImGui::OpenPopup(name);
    }
    if (ImGui::BeginPopup(name))
    {
        //ファイル消去
        if (ImGui::Selectable("Delet"))
        {
            fs::path filePathToDelete = "asset\\" + m_ProjectFolderName + "\\" + name;

            // ファイルを削除
            fs::remove(filePathToDelete);
        }

        ImGui::EndPopup();
    }
}

void AccessFolder::CreateFolder()
{
    ImGui::Begin("Create Folder in asset", &m_IsWindw);

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

void AccessFolder::Draw()
{
    ImGui::Begin("Asset", &m_IsShowWindow);

    if (ImGui::Button("Create Folder"))
        m_IsWindw = true;
    if (m_IsWindw)
        CreateFolder();

    ImGui::SameLine();

    ImGui::SliderFloat("Icon Size", &m_ImageSize, 20.0f, 200.0f);

    ImGui::Separator();

    for (const auto& entry : fs::directory_iterator("asset"))
    {
        const std::string& itemPath = entry.path().filename().string();

        // フォルダ名が重複しない場合に追加
        if (itemPath.find('.') == std::string::npos && m_ProjectFolders.insert(itemPath).second);
    }

    // フォルダとファイルを表示
    for (const auto& folder : m_ProjectFolders)
    {
        if (!m_ProjectFolderName.empty() && m_ProjectFolderName != folder)
        {
            continue;
        }
        else
        {
            // フォルダの中身を取得
            bool hasContents = false;
            for (const auto& entry : fs::directory_iterator("asset/" + folder))
            {
                hasContents = true;
                break;
            }

            if (hasContents)
            {
                int textureNum = TextureManager::LoadTexture("asset\\InFolder.png");
                ID3D11ShaderResourceView* texture = *TextureManager::GetTexture(textureNum);

                // フォルダアイコンを表示
                ImGui::Image((ImTextureID)texture, ImVec2(m_ImageSize, m_ImageSize), ImVec2(0.0f, 0.0f));
            }
            else
            {
                int textureNum = TextureManager::LoadTexture("asset\\NonFolder.png");
                ID3D11ShaderResourceView* texture = *TextureManager::GetTexture(textureNum);

                // フォルダアイコンを表示
                ImGui::Image((ImTextureID)texture, ImVec2(m_ImageSize, m_ImageSize), ImVec2(0.0f, 0.0f));
            }

            ImGui::SameLine();
        }
        if (ImGui::TreeNode(folder.c_str()))
        {
            m_ProjectFolderName = folder;
            for (const auto& entry : fs::directory_iterator("asset/" + folder))
            {
                const std::string& itemPath = entry.path().filename().string();

                std::string extension = itemPath.substr(itemPath.find_last_of(".") + 1);
                if (extension != "mtl")
                    DrawFolderIconAndName(itemPath.c_str(), ImVec2(m_ImageSize, m_ImageSize), ImVec2(0.0f, 0.0f));
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

        //右クリックでポップアップを出す 
        if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
        {
            ImGui::OpenPopup(folder.c_str());
        }
        if (ImGui::BeginPopup(folder.c_str()))
        {
            //ファイル消去
            if (ImGui::Selectable("Delet"))
            {
                int result = MessageBox(NULL, ("本当に" + folder + "を消しますか?").c_str(), "警告", MB_YESNO | MB_ICONQUESTION);

                if (result == IDYES)
                {
                    // Yesが選択された場合の処理
                    fs::path filePathToDelete = "asset\\" + folder;

                    for (const auto& entry : fs::directory_iterator(filePathToDelete))
                    {
                        // ファイルを削除
                        fs::remove(entry.path());
                    }

                    // ファイルを削除
                    if (fs::remove(filePathToDelete))
                    {
                        m_ProjectFolders.erase(folder);
                        m_ProjectFolderName = "";
                    }
                    else
                    {
                        std::cerr << "Error deleting file: " << filePathToDelete << std::endl;
                    }
                }
                ImGui::EndPopup();
                break;
            }

            ImGui::EndPopup();
        }
    }

   
   
   

    ImGui::End();
}
