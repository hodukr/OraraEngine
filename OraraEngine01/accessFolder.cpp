#include "main.h"
#include "renderer.h"
#include "textureManager.h"
#include "accessFolder.h"

#include <algorithm>

void AccessFolder::DrawFolderIconAndName(const char* name, ImVec2 size, ImVec2 uv)
{
    // �t�@�C���g���q���擾
    std::string filename = name;
    std::string extension = filename.substr(filename.find_last_of(".") + 1);

    // �g���q�� .png �܂��� .jpg �̏ꍇ�ɂ̂݉摜��\��
    if (extension == "png" || extension == "jpg")
    {
        //�p�X������Ă����Ȃ���const char*�̏ꏊ���ς��TextureManager�œ����摜���肶��Ȃ��Ȃ�o�O��
        auto it = m_Path.find(name);
        if (it == m_Path.end())
        {
            std::string path = "asset\\" + m_ProjectFolderName + "\\" + name;
            m_Path[name] = path;
            it = m_Path.find(name);  // �v�f��ǉ���A�ēx�������ăC�e���[�^�Œ��g�����o���Ȃ��ƃo�O��
        }

        int textureNum = TextureManager::LoadTexture(it->second.c_str());
        ID3D11ShaderResourceView* texture = *TextureManager::GetTexture(textureNum);

        // �t�H���_�A�C�R����\��
        ImGui::Image((ImTextureID)texture, size, uv);
    }
    else
    {
        // �摜�łȂ��ꍇ�̓e�L�X�g�Ƃ��ĕ\��
        ImGui::Image(nullptr, size, uv);
    }

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

                DrawFolderIconAndName(folderName.c_str(), ImVec2(m_ImageSize, m_ImageSize), ImVec2(0.0f, 0.0f));
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

void AccessFolder::ChangeImageSize()
{
    ImGui::Begin("Image Size");

    ImGui::SliderFloat("Image Width", &m_ImageSize, 20.0f, 200.0f);
 
    ImGui::End();
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
