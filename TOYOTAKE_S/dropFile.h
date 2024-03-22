#pragma once

namespace fs = filesystem;

class DropFile
{
private:
	static HDROP m_Drop;
	static list<fs::path> m_fullPathNames;
	static void CopyFileToProjectFolder(fs::path filePath);
public:
	static void SetDrop(HDROP hDROP) { m_Drop = hDROP; }
	static void DropFileData();
};