#pragma once

#include "imgui/imgui.h"
#include "imGuiWindow.h"
#include <set>
namespace fs = filesystem;
#define MAX_TEXT_NUM 256

class AccessFolder:public GuiWindowBase
{
private:
	bool m_CreateFolder = false;
	bool m_IsWindw = false;
	char m_CreatedFolderName[MAX_TEXT_NUM]{};
	char m_FolderName[MAX_TEXT_NUM]{};                     //最大文字数256
	set<string> m_ProjectFolders{};              //assetフォルダの中身のフォルダ名
	string m_ProjectFolderName{};                     //今開いているフォルダの名前を格納
	unordered_map<string, string> m_Path{}; //名前をKeyにパスを格納
	float m_ImageSize = 30.0f;
	string m_DragName{};   //ドラッグされたものの名前


	void CreateFolder();
	void DrawFolderIconAndName(const char* name, ImVec2 size, ImVec2 uv);
public:
	void SetWindowConfig();
	void Draw()override;
	string GetNowFolder() { return m_ProjectFolderName; }
	string GetDragName() { return m_DragName; }
};