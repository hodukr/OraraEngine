#pragma once

#include "imgui/imgui.h"
#include <filesystem>  // C++17�̕W�����C�u�����Ɋ܂܂�Ă���w�b�_
namespace fs = std::filesystem;
#define MAX_TEXT_NUM 256

struct FolderInfo
{
	std::string Name;
	ImVec2 IconSize;
	ImVec2 IconUV;
	bool IsFolder;
};

class AccessFolder
{
private:
	bool m_CreateFolder = false;
	char m_CreatedFolderName[MAX_TEXT_NUM];
	char m_FolderName[MAX_TEXT_NUM];//�ő啶����256
	std::vector<FolderInfo> m_ProjectFolders; // �v���W�F�N�g�̃t�H���_���

	void DrawFolderIconAndName(const FolderInfo& folder);
public:
	void DrawProjectAssets();
	void ListAssetContents();
	void CreateFolder();
};