#include "main.h"
#include"renderer.h"
#include "textureManager.h"

std::unordered_map<int, TexData> TextureManager::m_Date;
int TextureManager::m_Index;

const int TextureManager::LoadTexture(const char* file)
{
	for (int i = 0; i < m_Index; i++)
	{
		if (strcmp(file, m_Date[i].file) == 0)
		{
			return i;
		}
	}

	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		file,
		NULL,
		NULL,
		&m_Date[m_Index].texture,
		NULL);
	assert(&m_Date[m_Index].texture);

	m_Date[m_Index].file = file;

	return m_Index++;
}

ID3D11ShaderResourceView** TextureManager::GetTexture(int index)
{
	return &m_Date[index].texture;
}

void TextureManager::Uninit()
{
	for (int i = 0; i < m_Index; i++)
	{
		m_Date[i].texture->Release();
	}
	m_Date.clear();
	m_Index = 0;
}
