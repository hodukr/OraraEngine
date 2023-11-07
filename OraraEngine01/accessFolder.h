#pragma once

#include "imgui/imgui.h"
#include <filesystem>  // C++17の標準ライブラリに含まれているヘッダ
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
	char m_FolderName[MAX_TEXT_NUM];//最大文字数256
	std::vector<FolderInfo> m_ProjectFolders; // プロジェクトのフォルダ情報

	void DrawFolderIconAndName(const FolderInfo& folder);
public:
	void DrawProjectAssets();
	void ListAssetContents();
	void CreateFolder();
};