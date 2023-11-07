
#pragma once
#include <unordered_map>

struct TexData
{
	ID3D11ShaderResourceView* texture;
	const char* file;
	const char* name;
};

class TextuereManager
{
private:
	static std::unordered_map<int, TexData> m_Date;
	static int m_Index;
public:
	static const  int LoadTexture(const char* file, const char* name);
	static ID3D11ShaderResourceView** GetTexture(int index);
	static void Uninit();
};