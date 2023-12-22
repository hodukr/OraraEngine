#pragma once

#include "imgui/imgui.h"
#include "singleton.h"
#include <set>
#include <list>
#include <unordered_map>
#include <filesystem>  // C++17の標準ライブラリに含まれているヘッダ
namespace fs = std::filesystem;
#define MAX_TEXT_NUM 256

class AccessFolder
{
private:
	Singleton(AccessFolder);
	bool m_CreateFolder = false;
	char m_CreatedFolderName[MAX_TEXT_NUM];
	char m_FolderName[MAX_TEXT_NUM];                     //最大文字数256
	std::set<std::string> m_ProjectFolders;              //assetフォルダの中身のフォルダ名
	std::string m_ProjectFolderName;                     //今開いているフォルダの名前を格納
	std::unordered_map<std::string,std::string> m_Path; //名前をKeyにパスを格納
	float m_ImageSize = 20;

	void DrawFolderIconAndName(const char* name, ImVec2 size, ImVec2 uv);
public:
	void DrawProjectAssets();
	void ChangeImageSize();
	void CreateFolder();
	std::string GetNowFolder() { return m_ProjectFolderName; }
};