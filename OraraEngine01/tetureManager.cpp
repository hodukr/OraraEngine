#include "main.h"
#include"renderer.h"
#include "textureManager.h"

std::unordered_map<int, TexData> TextuereManager::m_Date;
int TextuereManager::m_Index;

const int TextuereManager::LoadTexture(const char* file, const char* name)
{
	for (int i = 0; i < m_Index; i++)
	{
		if (name == m_Date[i].file)
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
	assert(m_Date[m_Index].texture);

	m_Date[m_Index].name = name;
	m_Date[m_Index].file = file;


	return m_Index++;
}

ID3D11ShaderResourceView** TextuereManager::GetTexture(int index)
{
	return &m_Date[index].texture;
}

void TextuereManager::Uninit()
{
	for (int i = 0; i < m_Index; i++)
	{
		m_Date[i].texture->Release();
	}
	m_Date.clear();
	m_Index = 0;
}
