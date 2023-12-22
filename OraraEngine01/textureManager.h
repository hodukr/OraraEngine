
#pragma once
#include <unordered_map>

struct TexData
{
	ID3D11ShaderResourceView* texture;
	const char* file;
};

class TextureManager
{
private:
	static std::unordered_map<int, TexData> m_Date;
	static int m_Index;
public:
	static const  int LoadTexture(const char* file);
	static ID3D11ShaderResourceView** GetTexture(int index);
	static void Uninit();
};