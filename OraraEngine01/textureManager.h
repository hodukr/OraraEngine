
#pragma once
struct TexData
{
	ID3D11ShaderResourceView* texture;
	std::string file;
};

class TextureManager
{
private:
	static std::unordered_map<int, TexData> m_Date;
	static int m_Index;
public:
	static const  int LoadTexture(std::string file);
	static ID3D11ShaderResourceView** GetTexture(int index);
	static void Uninit();
};