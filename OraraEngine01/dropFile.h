#pragma once
#include <list>
#include <filesystem> //これを使う場合プロパティのC++言語標準をC++17に設定しなきゃいけない

namespace fs = std::filesystem;

class DropFile
{
private:
	static HDROP m_Drop;
	static std::list<fs::path> m_fullPathNames;
	static void CopyFileToProjectFolder(fs::path filePath);
public:
	static void SetDrop(HDROP hDROP) { m_Drop = hDROP; }
	static void DropFileData();
};