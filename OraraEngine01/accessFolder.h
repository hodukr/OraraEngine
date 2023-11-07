#pragma once

#include "imgui/imgui.h"
#include <set>
#include <filesystem>  // C++17�̕W�����C�u�����Ɋ܂܂�Ă���w�b�_
namespace fs = std::filesystem;
#define MAX_TEXT_NUM 256

class AccessFolder
{
private:
	bool m_CreateFolder = false;
	char m_CreatedFolderName[MAX_TEXT_NUM];
	char m_FolderName[MAX_TEXT_NUM];           //�ő啶����256
	std::set<std::string> m_ProjectFolders;    //asset�t�H���_�̒��g�̃t�H���_��
	std::string m_ProjectFolderName;           //���J���Ă���t�H���_�̖��O���i�[
	
	void DrawFolderIconAndName(const char* name, ImVec2 size, ImVec2 uv);
public:
	void DrawProjectAssets();
	void ListAssetContents();
	void CreateFolder();
	std::string GetNowFolder() { return m_ProjectFolderName; }
};